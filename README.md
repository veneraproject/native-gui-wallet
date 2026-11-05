# Venera Desktop Wallet

Venera Desktop Wallet combines an Electron interface with native Venera wallet software. Create or restore a wallet, manage VNR and supported BNB Smart Chain assets, and access Venera rewards, staking, burns and games from one application.

Private-key operations and transaction signing run locally. Community services such as rewards, games and burn-yield claims communicate with the Venera Rewards central system.

## Features

- **Create and restore wallets:** create a new wallet, restore a 25-word Venera seed, or open a compatible encrypted CLI `.keys` file.
- **VNR transfers:** send and receive VNR through the native wallet RPC, with balances and transaction history obtained from the daemon and wallet scan.
- **BNB Smart Chain assets:** manage the locally derived BSC address and supported BNB, USDT and USDC balances and transfers. BSC token transfers require BNB for network fees.
- **Receive addresses and QR codes:** display and copy addresses and use QR codes for supported receive and export flows.
- **Wallet backups:** export the wallet backup and access password-protected seed/private-key export functions.
- **Rewards and games:** access crate rewards, spins and other supported games.
- **Staking:** submit a VNR transfer to the staking destination configured in the local module and view backend-tracked staking information.
- **Burns and yield:** submit burns, view burn statistics and claim available yield through the backend.

The GUI uses the native wallet implementation for local wallet operations.

## How it works

| Component | Purpose |
| --- | --- |
| Electron GUI | Wallet screens, user interaction and communication with the native helper |
| `venera-local-cli.exe` | Native wallet operations, local signing and the desktop service protocol |
| `venera-rpc.exe` | Venera wallet scanning and transaction operations |
| `venerad.exe` | Venera network connectivity and blockchain synchronization |
| Venera backend | Games, crate claims, reward payouts, staking records, burn statistics and yield claims |

The public daemon build synchronizes and verifies the network.

For users migrating from the web wallet, the restored VNR address remains the identity used to associate existing records. The locally derived ERC-20 address can differ from the old address due to differences in key derivation algorithms.

## Encryption and wallet security

### Encrypted key files

The native wallet's `VNRKEY02` file format encrypts the wallet seed using **AES-256-GCM**. The encryption key is derived from the wallet password with **PBKDF2-HMAC-SHA256 using 600,000 iterations**. The format uses a 16-byte salt, a 12-byte GCM nonce and a 16-byte authentication tag.

Authenticated encryption protects the stored seed and detects tampering or an incorrect password. The GUI and CLI use the same compatible key-file format. A `.keys` extension alone does not establish compatibility with files produced by unrelated wallet software.

Keep the recovery seed and encrypted backup in a safe location. Anyone with the recovery seed or an exported private key can control the corresponding funds. Password encryption protects data at rest; unlocked wallet secrets must be available in memory while the wallet operates.

### Local operation and desktop isolation

Signing keys are handled by the native wallet software. Normal backend requests use public wallet information and authorization proofs rather than uploading the recovery seed or private keys.

The memory-only desktop protocol avoids the desktop metadata sidecar files used by older versions. Runtime state is reconstructed from the wallet, daemon and backend after reopening. This does not eliminate blockchain storage or files created by Electron and the operating system. Allow the wallet to finish synchronizing before relying on a displayed balance.

## Build for Windows x64

Use the complete cleaned source package containing `scripts/build-windows-release.sh`. The commands below are run on Windows in **MSYS2 UCRT64**.

### 1. Install the build tools

Install:

- [MSYS2](https://www.msys2.org/), normally under `C:\msys64`.
- [Node.js for Windows](https://nodejs.org/), version 22 or a newer compatible LTS version, including npm. Enable its PATH option during installation.

Open **MSYS2 UCRT64** from the Start menu and update it:

```bash
pacman -Syu
```

If the update asks you to close the terminal, close it, reopen UCRT64 and run the command again to finish updating.

Install the native dependencies:

```bash
pacman -S --needed base-devel python curl tar bzip2 \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-pkgconf \
  mingw-w64-ucrt-x86_64-openssl \
  mingw-w64-ucrt-x86_64-libsodium \
  mingw-w64-ucrt-x86_64-unbound \
  mingw-w64-ucrt-x86_64-zeromq \
  mingw-w64-ucrt-x86_64-curl \
  mingw-w64-ucrt-x86_64-json-c
```

Check the environment:

```bash
echo "$MSYSTEM"
g++ -dumpmachine
node --version
npm.cmd --version
```

The first two commands should report `UCRT64` and `x86_64-w64-mingw32`. If Node or npm is missing, reopen the terminal after installing Node.js and ensure its installation directory is on PATH.

### 2. Extract the source

Use a short path without spaces or non-ASCII characters, for example:

```text
C:\venera\venera-native-gui
```

In UCRT64, enter the project root containing `package.json`, `native-source/` and `scripts/`:

```bash
cd /c/venera/venera-native-gui
```

Use a fresh source extraction when moving between different source releases or toolchains. Keep the root `build/` folder: it contains the GUI packaging icon. Keep all native source and bundled dependency files, including `instructions_portable.cpp` and `verification_context.h`.

### 3. Build the release

Run:

```bash
bash scripts/build-windows-release.sh
```

The script builds the native daemon, RPC and CLI, verifies and packages the static native executables, copies them into `native/bin/`, installs the locked npm dependencies, builds the GUI assets and creates the Windows portable application.

Boost 1.78 is downloaded and built automatically. Internet access is required for dependency downloads. The first build takes longer; subsequent builds can reuse the build cache.

The default native build uses two parallel jobs. To use four:

```bash
JOBS=4 bash scripts/build-windows-release.sh
```

Use the same release command after source changes. No package-only environment variable, manual binary copying or separate patch scripts are required.

### 4. Find the output

| Location | Output |
| --- | --- |
| `dist/` | Packaged Windows GUI portable executable |
| `native/bin/` | Native executables copied into the GUI build |
| `native-source/dist/venera-windows-x64-private-static/` | Verified native distribution folder |
| `native-source/dist/venera-windows-x64-private-static.zip` | Native distribution archive |

The native executables statically link application dependencies while retaining normal Windows system DLL dependencies. Electron still packages its own runtime and resources.

## Troubleshooting

- **Wrong compiler or missing `cl.exe`:** open MSYS2 UCRT64 and use the final release script. The native build uses GCC.
- **Build stopped:** resolve the first actual error above the failure message, then rerun the release script. CMake deprecation warnings alone are not build failures.
- **Balance not visible:** check daemon connectivity and synchronization, then allow the wallet scan to catch up. A daemon at height 1 with zero outgoing connections has not synchronized.

## Source and licenses

Retain the project's license files and third-party notices when publishing or redistributing the software. Dependency licenses are also included in the native distribution.

MSYS2 setup references: [updating](https://www.msys2.org/docs/updating/) and [environments](https://www.msys2.org/docs/environments/).
