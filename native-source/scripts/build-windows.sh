#!/usr/bin/env bash
set -Eeuo pipefail
trap 'venera_build_status=$?; printf "Build failed (exit %s) at line %s: %s\n" "$venera_build_status" "$LINENO" "$BASH_COMMAND" >&2; exit "$venera_build_status"' ERR
cd "$(dirname "$0")/.."
[[ ${MSYSTEM:-} == UCRT64 ]] || { echo 'Run in MSYS2 UCRT64 on Windows x64.' >&2; exit 1; }
mode=${1:-bundle}
case "$mode" in bundle|static) ;; *) echo 'Usage: bash scripts/build-windows.sh [bundle|static]' >&2; exit 2;; esac
for tool in cmake ninja g++ gcc pkg-config python3 objdump curl tar; do command -v "$tool" >/dev/null; done
root=$PWD
root_native=$(cygpath -m "$root")
prefix=$(cygpath -m "$MINGW_PREFIX")
boost="$root/build/windows-boost-prefix"
boost_native=$(cygpath -m "$boost")
mkdir -p build/downloads build/windows-deps-src
boost_archive=build/downloads/boost_1_78_0.tar.bz2
if [[ ! -s $boost_archive ]]; then
    curl --fail --location --retry 3 --proto '=https' https://archives.boost.io/release/1.78.0/source/boost_1_78_0.tar.bz2 -o "$boost_archive.part"
    mv "$boost_archive.part" "$boost_archive"
fi
printf '%s  %s\n' 8681f175d4bdb26c52222665793eef08490d7758529330f98d3b29dd0735bccc "$boost_archive" | sha256sum -c -
if [[ ! -f $boost/.venera-boost-178-complete ]]; then
    tar -xf "$boost_archive" -C build/windows-deps-src
    (
        cd build/windows-deps-src/boost_1_78_0
                                                                          
        engine_cxx="$MINGW_PREFIX/bin/g++.exe"
        [[ -x "$engine_cxx" ]] || { echo "Missing UCRT64 compiler: $engine_cxx" >&2; exit 1; }
        [[ $("$engine_cxx" -dumpmachine) == x86_64-w64-mingw32 ]] || {
            echo 'Boost engine requires the native Windows x64 GCC toolchain.' >&2; exit 1;
        }
        (
            cd tools/build/src/engine
            "$engine_cxx" -O2 -s -std=c++11 -DNDEBUG -pthread \
                -o ../../../../b2.exe \
                builtins.cpp class.cpp command.cpp compile.cpp constants.cpp cwd.cpp \
                debug.cpp debugger.cpp execcmd.cpp execnt.cpp execunix.cpp filent.cpp \
                filesys.cpp fileunix.cpp frames.cpp function.cpp glob.cpp hash.cpp \
                hcache.cpp hdrmacro.cpp headers.cpp jam.cpp jamgram.cpp lists.cpp \
                make.cpp make1.cpp md5.cpp mem.cpp modules.cpp native.cpp \
                object.cpp option.cpp output.cpp parse.cpp pathnt.cpp pathsys.cpp \
                pathunix.cpp regexp.cpp rules.cpp scan.cpp search.cpp jam_strings.cpp \
                startup.cpp subst.cpp sysinfo.cpp timestamp.cpp variable.cpp w32_getreg.cpp \
                modules/order.cpp modules/path.cpp modules/property-set.cpp modules/regex.cpp modules/sequence.cpp modules/set.cpp \
                -lkernel32 -ladvapi32 -luser32
        )
        printf 'using gcc ;\n' > project-config.jam
        [[ -f b2.exe ]] || { echo 'Boost bootstrap did not produce b2.exe.' >&2; exit 1; }
        echo 'Building Boost libraries with UCRT64 GCC...'
        ./b2.exe -j"${JOBS:-2}" toolset=gcc target-os=windows architecture=x86 address-model=64 \
            variant=release link=static runtime-link=shared threading=multi threadapi=win32 \
            --layout=system --prefix="$boost_native" \
            cxxflags='-std=c++14 -O2 -march=x86-64 -mtune=generic' \
            --with-system --with-filesystem --with-thread --with-date_time --with-chrono \
            --with-regex --with-serialization --with-program_options --with-locale \
            boost.locale.icu=off boost.locale.iconv=off install
    )
    touch "$boost/.venera-boost-178-complete"
fi
build="build/windows-$mode"
static=OFF
suffix=.dll.a
link_flags=''
cli_defines=()
platform_defines=''
if [[ $mode == static ]]; then
    static=ON;suffix=.a;link_flags=-static
    cli_defines=(-DCURL_STATICLIB -DJSON_C_STATIC)
    platform_defines=-DSODIUM_STATIC
fi
for stem in ssl crypto unbound zmq sodium; do
    [[ -f "$MINGW_PREFIX/lib/lib$stem$suffix" ]] || { echo "Missing $MINGW_PREFIX/lib/lib$stem$suffix" >&2; exit 1; }
done
pkg-config --atleast-version=8.13 libcurl
unbound="$prefix/lib/libunbound$suffix"
zmq="$prefix/lib/libzmq$suffix"
if [[ $mode == static ]]; then
    read -r -a unbound_extra <<< "$(pkg-config --libs --static libunbound)"
    for item in "${unbound_extra[@]}"; do
        case "$item" in -lunbound|-L*) ;; *) unbound+=";$item";; esac
    done
    read -r -a zmq_extra <<< "$(pkg-config --libs --static libzmq)"
    for item in "${zmq_extra[@]}"; do
        case "$item" in -lzmq|-L*) ;; *) zmq+=";$item";; esac
    done
fi
cmake -S daemon -B "$build" -G Ninja \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_POLICY_DEFAULT_CMP0167=OLD \
    -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DSTATIC="$static" \
    -DARCH=default -DARCH_ID=x86_64 -DNO_AES=ON -DUSE_LTO=OFF -DSTACK_TRACE=OFF -DSTRIP_TARGETS=ON -DUSE_READLINE=OFF \
    -DCMAKE_C_FLAGS="-march=x86-64 -mtune=generic $platform_defines" \
    -DCMAKE_CXX_FLAGS="-march=x86-64 -mtune=generic -Wa,-mbig-obj $platform_defines" \
    -DBOOST_ROOT:PATH="$boost_native" -DBOOST_INCLUDEDIR:PATH="$boost_native/include" \
    -DBOOST_LIBRARYDIR:PATH="$boost_native/lib" -DBoost_NO_SYSTEM_PATHS=ON \
    -DBoost_USE_STATIC_LIBS=ON -DBoost_USE_STATIC_RUNTIME=OFF \
    -DOPENSSL_INCLUDE_DIR:PATH="$prefix/include" \
    -DOPENSSL_SSL_LIBRARY:FILEPATH="$prefix/lib/libssl$suffix" \
    -DOPENSSL_CRYPTO_LIBRARY:FILEPATH="$prefix/lib/libcrypto$suffix" \
    -DUNBOUND_INCLUDE_DIR:PATH="$prefix/include" -DUNBOUND_LIBRARIES:STRING="$unbound" \
    -DZMQ_INCLUDE_PATH:PATH="$prefix/include" -DZMQ_LIB:STRING="$zmq" \
    -DMINIUPNP_INCLUDE_DIR:PATH="$root_native/daemon/external/miniupnp/miniupnpc" \
    -DMINIUPNP_LIBRARY:STRING=libminiupnpc-static \
    -DSODIUM_LIBRARY:FILEPATH="$prefix/lib/libsodium$suffix" \
    -DGSSAPI_LIBRARY:FILEPATH='' -DPGM_LIBRARY:FILEPATH='' -DNORM_LIBRARY:FILEPATH='' \
    -DPROTOLIB_LIBRARY:FILEPATH='' -DPROTOKIT_LIBRARY:FILEPATH='' -DBSD_LIBRARY:FILEPATH='' -DMD_LIBRARY:FILEPATH=''
cmake --build "$build" --target daemon wallet_rpc_server -j"${JOBS:-2}"
read -r -a cli_headers <<< "$(pkg-config --cflags libcurl json-c openssl)"
if [[ $mode == static ]]; then
    read -r -a cli_libraries <<< "$(pkg-config --libs --static libcurl json-c openssl)"
else
    read -r -a cli_libraries <<< "$(pkg-config --libs libcurl json-c openssl)"
fi
g++ -O2 -s -std=c++17 -march=x86-64 -mtune=generic -pthread -Wa,-mbig-obj \
    -DOPENSSL_SUPPRESS_DEPRECATED -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0602 \
    "${cli_defines[@]}" "${cli_headers[@]}" ${link_flags:+"$link_flags"} \
    cli/venera_cli_local.cpp -o "$build/bin/venera-local-cli.exe" \
    "${cli_libraries[@]}" -ladvapi32 -lws2_32
"$build/bin/venera-local-cli.exe" --self-test
"$build/bin/venerad.exe" --version
"$build/bin/venera-rpc.exe" --version
echo 'Packaging native executables and DLLs...'
python3 scripts/package-windows.py "$build/bin" "dist/venera-windows-x64-$mode" "$MINGW_PREFIX/bin" "$mode"
