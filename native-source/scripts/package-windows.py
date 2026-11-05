import hashlib
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import zipfile

root=Path(__file__).resolve().parents[1]
source=Path(sys.argv[1]).resolve()
output=Path(sys.argv[2]).resolve()
prefix_bin=Path(sys.argv[3]).resolve()
mode=sys.argv[4]
if output.exists():
    raise SystemExit('Distribution folder already exists; rename/remove it before packaging again: '+str(output))
output.mkdir(parents=True)
known={p.name.lower():p for p in prefix_bin.glob('*.dll')}
system=re.compile(r'(?i)^(?:api-ms-|ext-ms-).+\.dll$|^(?:kernel32|kernelbase|ntdll|ucrtbase|msvcrt|advapi32|bcrypt|bcryptprimitives|crypt32|cryptbase|dnsapi|gdi32|iphlpapi|mswsock|ncrypt|normaliz|ole32|oleaut32|powrprof|psapi|rpcrt4|secur32|setupapi|shell32|shlwapi|user32|userenv|version|winhttp|wininet|winmm|winspool|ws2_32|wtsapi32|wldap32|wintrust|dbghelp|comctl32|dwmapi)\.dll$')
queue=[]
for name in ('venerad.exe','venera-rpc.exe','venera-local-cli.exe'):
    target=output/name
    shutil.copy2(source/name,target)
    queue.append(target)
seen=set()
packages=set()
while queue:
    path=queue.pop()
    if path.name.lower() in seen:continue
    seen.add(path.name.lower())
    result=subprocess.run(['objdump','-p',str(path)],check=True,capture_output=True,text=True)
    for name in re.findall(r'DLL Name:\s*(\S+)',result.stdout):
        lower=name.lower()
        if lower in {'msys-2.0.dll','cygwin1.dll'}:
            raise SystemExit('Non-native Windows runtime dependency: '+name)
        if system.fullmatch(lower):continue
        if mode=='static':raise SystemExit('Unexpected non-system DLL in static build: '+name)
        if lower not in known:raise SystemExit('Unresolved runtime DLL: '+name)
        target=output/known[lower].name
        if not target.exists():
            shutil.copy2(known[lower],target)
            original=str(known[lower])
            native=subprocess.run(['cygpath','-u',original],check=True,capture_output=True,text=True).stdout.strip()
            owner=subprocess.run(['pacman','-Qoq','--',native],check=True,capture_output=True,text=True).stdout.strip()
            packages.update(owner.splitlines())
        queue.append(target)
licenses=output/'licenses'
shutil.copytree(root/'daemon',licenses/'bundled',ignore=lambda d,n:[x for x in n if (Path(d)/x).is_file() and not (x.lower().startswith(('license','copying','copyright')))])
 
package_licenses=prefix_bin.parent/'share/licenses'
if package_licenses.exists():
    for package in sorted(packages):
        listing=subprocess.run(['pacman','-Qql','--',package],check=True,capture_output=True,text=True).stdout.splitlines()
        for item in listing:
            if '/share/licenses/' not in item.replace('\\','/'):continue
            normalized=item.replace('\\','/')
            relative=normalized.split('/share/licenses/',1)[1]
            if not relative or '..' in Path(relative).parts:continue
            original=Path(item)
            if os.name=='nt':original=Path(subprocess.run(['cygpath','-m',item],check=True,capture_output=True,text=True).stdout.strip())
            if original.is_file():
                target=licenses/'msys2'/relative
                target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(original,target)
        short=re.sub(r'^mingw-w64-ucrt-x86_64-','',package)
        for name in {package,short}:
            notice=package_licenses/name
            if notice.is_dir():shutil.copytree(notice,licenses/'msys2'/name,dirs_exist_ok=True)
shutil.copy2(root/'LICENSE',licenses/'Venera-LICENSE')
boost_license=root/'build/windows-deps-src/boost_1_78_0/LICENSE_1_0.txt'
if boost_license.exists():shutil.copy2(boost_license,licenses/'Boost-LICENSE_1_0.txt')
for directory in sorted(licenses.rglob('*'),reverse=True):
    if directory.is_dir() and not any(directory.iterdir()):directory.rmdir()
files=sorted(p for p in output.rglob('*') if p.is_file())
(output/'SHA256SUMS.txt').write_text(''.join(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+p.relative_to(output).as_posix()+'\n' for p in files),encoding='utf-8')
with zipfile.ZipFile(str(output)+'.zip','w',zipfile.ZIP_DEFLATED,compresslevel=9) as archive:
    for path in sorted(output.rglob('*')):
        if path.is_file():
            info = zipfile.ZipInfo(output.name + '/' + path.relative_to(output).as_posix(), (1980, 1, 1, 0, 0, 0))
            info.create_system = 0
            info.external_attr = 0x20
            info.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(info, path.read_bytes(), compresslevel=9)
print('Windows package:',str(output)+'.zip')
