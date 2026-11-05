import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import struct
import subprocess
import sys
import tempfile
import zipfile

EPOCH = 315532800
NAMES = ("venerad.exe", "venera-rpc.exe", "venera-local-cli.exe")
SYSTEM_DLL = re.compile(
    r"(?i)(?:(?:api-ms-|ext-ms-)[a-z0-9_.-]+|"
    r"kernel32|kernelbase|ntdll|ucrtbase|msvcrt|advapi32|bcrypt|"
    r"bcryptprimitives|crypt32|cryptbase|dnsapi|gdi32|iphlpapi|mswsock|"
    r"ncrypt|normaliz|ole32|oleaut32|powrprof|psapi|rpcrt4|secur32|"
    r"setupapi|shell32|shlwapi|user32|userenv|version|winhttp|wininet|"
    r"winmm|winspool|ws2_32|wtsapi32|wldap32|wintrust|dbghelp|comctl32|"
    r"dwmapi|netapi32|userenv|cfgmgr32|imm32|comdlg32|wsock32)\.dll"
)
PATH_PATTERN = re.compile(
    r"(?i)(?:[a-z]:[\\/][a-z0-9_. -]+[\\/]|"
    r"(?<![\w./\\-])/(?:[\w.-]+/)*(?:home|Users|root|workspace|build|builds|mingw|msys64)/)"
)

class BuildError(Exception):
    pass


def need(condition, message):
    if not condition:
        raise BuildError(message)


class PE:
    def __init__(self, data):
        self.data = bytearray(data)
        need(len(data) >= 64 and data[:2] == b"MZ", "Not a Windows executable")
        self.pe = self.u32(60)
        need(self.take(self.pe, 4) == b"PE\0\0", "Invalid PE signature")
        self.coff = self.pe + 4
        need(self.u16(self.coff) == 0x8664, "Expected a Windows x64 executable")
        self.opt = self.coff + 20
        need(self.u16(self.opt) == 0x20B, "Expected PE32+")
        optional_size = self.u16(self.coff + 16)
        count = self.u32(self.opt + 108)
        need(optional_size >= 112 + 8 * count and count <= 16, "Invalid PE directories")
        self.dirs = [struct.unpack("<II", self.take(self.opt + 112 + i * 8, 8)) for i in range(count)]
        self.sections = []
        table = self.opt + optional_size
        for i in range(self.u16(self.coff + 2)):
            row = self.take(table + i * 40, 40)
            name = row[:8].split(b"\0", 1)[0].decode("ascii", "replace")
            virtual_size, address, size, offset = struct.unpack_from("<IIII", row, 8)
            need(offset + size <= len(self.data), "Section exceeds executable size")
            self.sections.append((name, address, virtual_size, offset, size))

    def take(self, offset, size):
        need(0 <= offset <= len(self.data) and size >= 0 and offset + size <= len(self.data), "Invalid PE offset")
        return self.data[offset:offset + size]

    def u16(self, offset):
        return struct.unpack("<H", self.take(offset, 2))[0]

    def u32(self, offset):
        return struct.unpack("<I", self.take(offset, 4))[0]

    def directory(self, index):
        return self.dirs[index] if index < len(self.dirs) else (0, 0)

    def offset(self, address, size=1):
        for _, start, _, offset, raw_size in self.sections:
            if start <= address and address + size <= start + raw_size:
                return offset + address - start
        if address + size <= self.u32(self.opt + 60):
            self.take(address, size)
            return address
        raise BuildError("PE address has no file-backed section")

    def string(self, address):
        offset = self.offset(address)
        end = self.data.find(0, offset, min(offset + 512, len(self.data)))
        need(end != -1, "Invalid PE import name")
        return self.data[offset:end].decode("ascii")

    def timestamp_fields(self):
        fields = [self.coff + 4]
        address, size = self.directory(0)
        if address:
            fields.append(self.offset(address, 40) + 4)
        address, size = self.directory(1)
        if address:
            terminated = False
            for relative in range(0, size - 19, 20):
                offset = self.offset(address + relative, 20)
                if not any(self.take(offset, 20)):
                    terminated = True
                    break
                fields.append(offset + 4)
            need(terminated, "Invalid import directory")
        address, size = self.directory(13)
        if address:
            terminated = False
            for relative in range(0, size - 31, 32):
                offset = self.offset(address + relative, 32)
                if not any(self.take(offset, 32)):
                    terminated = True
                    break
                fields.append(offset + 28)
            need(terminated, "Invalid delay import directory")
        address, size = self.directory(2)
        if address:
            visited = set()

            def visit(relative, depth):
                need(depth <= 16 and relative not in visited and relative + 16 <= size, "Invalid PE resource tree")
                visited.add(relative)
                offset = self.offset(address + relative, 16)
                fields.append(offset + 4)
                entries = self.u16(offset + 12) + self.u16(offset + 14)
                need(relative + 16 + entries * 8 <= size, "Invalid PE resources")
                for i in range(entries):
                    entry = self.offset(address + relative + 16 + i * 8, 8)
                    child = self.u32(entry + 4)
                    if child & 0x80000000:
                        visit(child & 0x7FFFFFFF, depth + 1)

            visit(0, 0)
        return fields

    def imports(self):
        names = set()
        for directory, width, name_offset in ((1, 20, 12), (13, 32, 4)):
            address, size = self.directory(directory)
            if not address:
                continue
            for relative in range(0, size - width + 1, width):
                offset = self.offset(address + relative, width)
                if not any(self.take(offset, width)):
                    break
                if directory == 13:
                    need(self.u32(offset) & 1, "Unsupported delay import format")
                names.add(self.string(self.u32(offset + name_offset)).lower())
        return sorted(names)



def _is_absolute_metadata_path(value, binary, offset):
    unit = 2 if binary[offset + 1:offset + 2] == b"\0" else 1
    end = offset + len(value) * unit
    if binary[end:end + unit] != b"\0" * unit:
        return False

    def complete_path(text):
        text = text.strip()
        if len(text) >= 2 and text[0] == text[-1] and text[0] in "\"'":
            text = text[1:-1]
        if re.fullmatch(r"(?i)\\\\\.\\pipe\\[^\x00-\x1f]+", text):
            return False
        if text.startswith(("\\\\?\\", "\\\\.\\")):
            remainder = text[4:]
            if remainder.casefold().startswith("unc\\"):
                text = "\\\\" + remainder[4:]
            elif re.match(r"[A-Za-z]:[\\/]", remainder):
                text = remainder
        if re.fullmatch(r"[A-Za-z]:[\\/][^<>:\"|?*\x00-\x1f]+", text):
            return True
        if re.fullmatch(r"\\\\[A-Za-z0-9_.-]+\\[^<>:\"|?*\x00-\x1f]+", text):
            return True
        if re.fullmatch(r"/[\w./+ @%=-]+", text) and PATH_PATTERN.search(text):
            return True
        return False

    if complete_path(value):
        return True
    labelled = re.fullmatch(
        r"(?i)\s*(?:OPENSSLDIR|ENGINESDIR|MODULESDIR|source|file|filename|build directory|path)\s*[:=]\s*(.+?)\s*",
        value,
    )
    if labelled and complete_path(labelled[1]):
        return True
    compiler_path = re.compile(
        r"(?:^|\s)-(?:I|L|B|isystem\s+|iquote\s+|isysroot\s+|-sysroot=|f(?:file|debug|macro)-prefix-map=)"
        r"(\"[^\"]+\"|'[^']+'|[^\s]+)"
    )
    for match in compiler_path.finditer(value):
        token = match[1]
        if len(token) >= 2 and token[0] == token[-1] and token[0] in "\"'":
            token = token[1:-1]
        if complete_path(token.split("=", 1)[0]):
            return True
    return False

def audit(path, forbidden=()):
    image = PE(path.read_bytes())
    need(all(image.u32(offset) == 0 for offset in image.timestamp_fields()), "PE timestamps are not zero")
    need(image.u32(image.coff + 8) == 0 and image.u32(image.coff + 12) == 0, "COFF symbols remain")
    need(image.directory(6) == (0, 0), "PE debug/PDB data remains")
    need(image.directory(4) == (0, 0), "Audit unsigned executables before code signing")
    need(image.directory(11) == (0, 0), "Bound imports remain")
    for name, *_ in image.sections:
        need(not name.startswith((".debug", ".zdebug", ".stab")) and name not in (".comment", ".buildid", ".gnu_deb"), "Debug/compiler metadata section remains")
    imports = image.imports()
    need(imports, "No import table found; cannot verify dependencies")
    external = [name for name in imports if not SYSTEM_DLL.fullmatch(name)]
    need(not external, "Non-system DLL dependencies: " + ", ".join(external))
    binary = bytes(image.data)
    tokens = []
    for token in forbidden:
        if len(token) >= 4:
            tokens.extend({token, token.replace("\\", "/"), token.replace("/", "\\")})
    tokens = [token.casefold() for token in tokens]
    streams = ((m.group().decode("ascii"), m.start()) for m in re.finditer(rb"[\x20-\x7e]{5,}", binary))
    wide = ((m.group().decode("utf-16le"), m.start()) for m in re.finditer(rb"(?:[\x20-\x7e]\x00){5,}", binary))
    for stream in (streams, wide):
        for value, offset in stream:
            need(not _is_absolute_metadata_path(value, binary, offset), f"Absolute machine/build path in {path.name} at byte {offset}: {value!r}")
            need(not any(token in value.casefold() for token in tokens), "Known machine/build identifier found at byte " + str(offset))
    return {"file": path.name, "pe_timestamps": 0, "debug_directory": False, "coff_symbols": False, "imports": imports, "machine_path_scan": "passed"}



def _normalize_vendor_source_paths(image):
    prefixes = ("D:/W/B/src/", "C:/M/B/src/")
    source_path = re.compile(r"(?:D:/W/B/src/|C:/M/B/src/)[A-Za-z0-9_./+ -]+\.(?:c|h|cc|cpp|cxx|hpp)")
    builtin_psl = re.compile(r"(?:D:/W/B/src/|C:/M/B/src/)libpsl-0\.21\.5/list/public_suffix_list\.dat")
    new_prefix = "./vendor__/"
    if any(len(prefix) != len(new_prefix) for prefix in prefixes):
        raise ValueError("Vendor prefix lengths differ")
    table = image.opt + image.u16(image.coff + 16)
    changed = 0
    for index, (name, address, virtual_size, offset, size) in enumerate(image.sections):
        flags = image.u32(table + index * 40 + 36)
        if name != ".rdata" or not flags & 0x40000000 or flags & 0xA0000000:
            continue
        payload = bytes(image.data[offset:offset + size])
        for pattern, encoding, unit in (
            (rb"[\x20-\x7e]{5,}", "ascii", 1),
            (rb"(?:[\x20-\x7e]\x00){5,}", "utf-16le", 2),
        ):
            for match in re.finditer(pattern, payload):
                value = match.group().decode(encoding)
                if not (source_path.fullmatch(value) or builtin_psl.fullmatch(value)):
                    continue
                if payload[match.end():match.end() + unit] != b"\0" * unit:
                    continue
                old_prefix = next(prefix for prefix in prefixes if value.startswith(prefix))
                before = old_prefix.encode(encoding)
                after = new_prefix.encode(encoding)
                position = offset + match.start()
                if image.data[position:position + len(before)] != before:
                    raise ValueError("Vendor path bytes changed unexpectedly")
                image.data[position:position + len(before)] = after
                changed += 1
    if changed:
        print("Normalized vendor source and built-in PSL filename prefixes:", changed)
    return changed

def sanitize(path, forbidden):
    need(PE(path.read_bytes()).directory(4) == (0, 0), "Refusing to modify a signed executable")
    subprocess.run(["strip", "--strip-all", "--remove-section=.comment", "--remove-section=.GCC.command.line", "--remove-section=.gnu_debuglink", "--remove-section=.gnu_debugaltlink", str(path)], check=True)
    image = PE(path.read_bytes())
    _normalize_vendor_source_paths(image)
    for offset in image.timestamp_fields():
        struct.pack_into("<I", image.data, offset, 0)
    struct.pack_into("<I", image.data, image.opt + 64, 0)
    path.write_bytes(image.data)
    os.utime(path, (EPOCH, EPOCH))
    return audit(path, forbidden)


def replace_one(source, old, new, label):
    need(source.count(old) == 1, "Cannot locate " + label + " in your build script; no original files changed")
    return source.replace(old, new, 1)


def transform(source, compile_flags):
    flags = " ".join(compile_flags)
    need(not re.search(r"[^\w./:=+\-]", "".join(flag.split("=", 1)[1] for flag in compile_flags if "prefix-map=" in flag)), "Use a project/toolchain path without spaces or shell punctuation")
    source = replace_one(source, 'build="build/windows-$mode"', 'build="build/windows-private-$mode"', "native build directory")
    source = replace_one(source, 'boost="$root/build/windows-boost-prefix"', 'boost="$root/build/windows-private-boost-prefix"', "Boost build prefix")
    for key in ("CMAKE_C_FLAGS", "CMAKE_CXX_FLAGS"):
        pattern = re.compile(r"(-D" + key + r"=)([\"'])([^\n]*?)\2")
        source, count = pattern.subn(lambda m: m[1] + '"' + m[3] + " " + flags + '"', source)
        need(count == 1, "Cannot locate " + key + "; no original files changed")
    source = replace_one(source, 'cmake -S daemon -B "$build" -G Ninja \\\n', 'cmake -S daemon -B "$build" -G Ninja \\\n    -DCMAKE_EXE_LINKER_FLAGS="-static -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp,--build-id=none,--strip-all" \\\n    -DCMAKE_SHARED_LINKER_FLAGS="-Wl,--no-insert-timestamp,--build-id=none" \\\n    -DCMAKE_MODULE_LINKER_FLAGS="-Wl,--no-insert-timestamp,--build-id=none" \\\n    -DCMAKE_C_COMPILER_LAUNCHER:STRING= -DCMAKE_CXX_COMPILER_LAUNCHER:STRING= \\\n', "CMake configure command")
    pattern = re.compile(r"cxxflags=([\"'])([^\n]*?)\1")
    source, count = pattern.subn(lambda m: "cxxflags=" + shlex.quote(m[2] + " " + flags), source)
    need(count == 1, "Cannot locate Boost compiler flags; no original files changed")
    source = source.replace("runtime-link=shared", "runtime-link=static")
    source = source.replace("-DBoost_USE_STATIC_RUNTIME=OFF", "-DBoost_USE_STATIC_RUNTIME=ON")
    source = source.replace("pkg-config --cflags libcurl json-c openssl", "pkg-config --cflags --static libcurl json-c openssl")
    blocks = re.findall(r"(?m)^g\+\+ (?:(?:[^\n]*\\\n)+[^\n]*|[^\n]*)", source)
    cli = [block for block in blocks if "cli/venera_cli_local.cpp" in block]
    need(len(cli) == 1, "Cannot locate CLI compiler command; no original files changed")
    source = source.replace(cli[0], cli[0].replace("g++ ", "g++ " + flags + " -static -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp,--build-id=none,--strip-all ", 1), 1)
    package = re.compile(r'(?m)^python3 scripts/package-windows\.py [^\n]+$')
    source, count = package.subn(":", source)
    need(count == 1, "Cannot locate package command; no original files changed")
    return source


def run_text(args):
    return subprocess.run(args, check=True, capture_output=True, text=True).stdout.strip()


def check_protocol(path):
    result = subprocess.run([str(path), "--desktop-service"], input='{"id":"private-check","method":"ping","params":{}}\n', capture_output=True, text=True, timeout=30, check=True)
    messages = [json.loads(line) for line in result.stdout.splitlines() if line.strip()]
    need(len(messages) == 1 and messages[0].get("result", {}).get("protocol") == "venera-desktop-2", "CLI is not the memory-only venera-desktop-2 build")


def write_zip(folder, archive_path):
    with zipfile.ZipFile(archive_path, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        for path in sorted(folder.rglob("*")):
            if path.is_file():
                info = zipfile.ZipInfo(folder.name + "/" + path.relative_to(folder).as_posix(), (1980, 1, 1, 0, 0, 0))
                info.create_system = 0
                info.external_attr = 0x20
                info.compress_type = zipfile.ZIP_DEFLATED
                archive.writestr(info, path.read_bytes())


def build():
    root = Path(__file__).resolve().parents[1]
    os.chdir(root)
    need(os.environ.get("MSYSTEM") == "UCRT64", "Run this in MSYS2 UCRT64")
    for tool in ("bash", "g++", "gcc", "strip", "objdump", "pkg-config", "cygpath"):
        need(shutil.which(tool), "Missing tool: " + tool)
    need("mingw" in run_text(["g++", "-dumpmachine"]), "Expected the native MinGW compiler")
    header = (root / "cli/desktop_service.h").read_text(encoding="utf-8")
    need('"venera-desktop-2"' in header, "Native source must support the memory-only venera-desktop-2 protocol")
    native_root = run_text(["cygpath", "-m", str(root)])
    msys_root = run_text(["cygpath", "-m", "/"])
    prefix = os.environ.get("MINGW_PREFIX", "")
    need(prefix, "MINGW_PREFIX is missing")
    native_prefix = run_text(["cygpath", "-m", prefix])
    mappings = ((str(root), "."), (native_root, "."), (prefix, "/toolchain"), (native_prefix, "/toolchain"), (msys_root.rstrip("/"), "/toolchain-root"))
    flags = ["-g0", "-fno-record-gcc-switches", "-fno-ident", "-Werror=date-time"]
    for before, after in sorted(mappings, key=lambda item: len(item[0])):
        before = before.replace("\\", "/")
        for name in ("file", "debug", "macro"):
            flags.append("-f" + name + "-prefix-map=" + before + "=" + after)
    source = transform((root / "scripts/build-windows.sh").read_text(encoding="utf-8"), flags)
    environment = os.environ.copy()
    environment.update(SOURCE_DATE_EPOCH=str(EPOCH), ZERO_AR_DATE="1", TZ="UTC", LC_ALL="C", CCACHE_DISABLE="1")
    exclusions = "-ffile-prefix-map=;-fdebug-prefix-map=;-fmacro-prefix-map=;-DCMAKE_C_FLAGS=;-DCMAKE_CXX_FLAGS=;cxxflags="
    environment["MSYS2_ARG_CONV_EXCL"] = environment.get("MSYS2_ARG_CONV_EXCL", "") + ";" + exclusions
    forbidden = [str(root), native_root, msys_root.rstrip("/"), os.environ.get("USERPROFILE", ""), os.environ.get("COMPUTERNAME", "")]
    forbidden = [token for token in forbidden if len(token) >= 4]
    fd, name = tempfile.mkstemp(prefix=".private-static-", suffix=".sh", dir=root / "scripts")
    try:
        with os.fdopen(fd, "w", encoding="utf-8", newline="\n") as generated:
            generated.write(source)
        subprocess.run(["bash", name, "static"], check=True, env=environment)
    finally:
        Path(name).unlink(missing_ok=True)
    binaries = root / "build/windows-private-static/bin"
    report = [sanitize(binaries / name, forbidden) for name in NAMES]
    check_protocol(binaries / "venera-local-cli.exe")
    for args in (("venera-local-cli.exe", "--self-test"), ("venerad.exe", "--version"), ("venera-rpc.exe", "--version")):
        subprocess.run([str(binaries / args[0]), *args[1:]], check=True, env=environment, timeout=120)
    dist = root / "dist"
    dist.mkdir(exist_ok=True)
    prefix_path = Path(native_prefix) if os.name == "nt" else Path(prefix)
    with tempfile.TemporaryDirectory(prefix=".private-stage-", dir=dist) as stage_name:
        stage = Path(stage_name)
        output = stage / "venera-windows-x64-private-static"
        subprocess.run([sys.executable, str(root / "scripts/package-windows.py"), str(binaries), str(output), str(prefix_path / "bin"), "static"], check=True, env=environment)
        licenses = prefix_path / "share/licenses"
        if licenses.is_dir():
            shutil.copytree(licenses, output / "licenses/msys2", dirs_exist_ok=True)
        for directory, label in ((root, "Native"), (root.parent, "GUI")):
            notices = directory / "SOURCE_NOTICES.txt"
            if notices.is_file():
                shutil.copy2(notices, output / "licenses" / (label + "-SOURCE_NOTICES.txt"))
        need(not list(output.rglob("*.dll")), "Static package unexpectedly contains DLLs")
        for name in NAMES:
            audit(output / name, forbidden)
        (output / "BUILD_CHECKS.json").write_text(json.dumps({"desktop_protocol": "venera-desktop-2", "executable_checks": report, "archive_timestamp": "1980-01-01T00:00:00Z", "scope": "Native Venera executables; Windows system DLLs remain required. The scan checks known paths and identifiers, not arbitrary unknown strings."}, indent=2) + "\n", encoding="utf-8")
        files = sorted(path for path in output.rglob("*") if path.is_file() and path.name != "SHA256SUMS.txt")
        (output / "SHA256SUMS.txt").write_text("".join(hashlib.sha256(path.read_bytes()).hexdigest() + "  " + path.relative_to(output).as_posix() + "\n" for path in files), encoding="utf-8")
        for path in output.rglob("*"):
            os.utime(path, (EPOCH, EPOCH))
        archive = stage / (output.name + ".zip")
        write_zip(output, archive)
        final = dist / output.name
        final_zip = dist / archive.name
        for target in (final, final_zip):
            if target.exists():
                previous = target.with_name(target.name + ".previous")
                if previous.is_dir():
                    shutil.rmtree(previous)
                elif previous.exists():
                    previous.unlink()
                target.rename(previous)
        shutil.move(str(output), final)
        shutil.move(str(archive), final_zip)
        os.utime(final_zip, (EPOCH, EPOCH))
    print("Verified static package:", final_zip)
    print("Native GUI copy source:", final)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("action", choices=("build", "audit"))
    parser.add_argument("files", nargs="*", type=Path)
    parser.add_argument("--forbid", action="append", default=[])
    args = parser.parse_args()
    if args.action == "build":
        need(not args.files, "build takes no file arguments")
        build()
    else:
        need(args.files, "Specify one or more EXEs to audit")
        for path in args.files:
            print(json.dumps(audit(path, args.forbid), indent=2))


if __name__ == "__main__":
    try:
        main()
    except (BuildError, OSError, ValueError, subprocess.SubprocessError) as error:
        print("Private static build failed:", error, file=sys.stderr)
        sys.exit(1)
