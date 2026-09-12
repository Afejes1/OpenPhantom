# Original compiler access and fingerprinting

The calibrated code generator is Visual C++ 5.0 RTM, cl.exe 11.00.7022. The
observed linker is 5.00.7022. This agrees with the compiler identified by
[OpenPhantom's compilation research](../engine-identification.md#1a-resolved-the-toolchain-by-byte-for-byte-compilation).
All four functions and the synthetic behavioral fixture have been compiled
with those original tools inside Docker/wibo. Compiler execution on the Windows
host was not used for this calibration.

## Acquired toolchain

At the contributor's request on 2026-09-12, a private copy was obtained from the
[archaic-msvc/msvc500 community preservation repository](https://github.com/archaic-msvc/msvc500),
commit `8abf95ce980161ad87b0b02402269cce76988953`.
The commit-addressed ZIP has SHA-256
`5f7728d1b3b8b8c61bb35e42a7680211019d290b30376c1a0a8faaa6d166cd1f`.
Archive paths, symlinks, total expanded size and ZIP CRCs were checked before
extraction. The private acquisition record retains the exact URL and provenance.

This is a community archive, not authenticated original Microsoft installation
media. Matching compiler output is evidence of code generation, not of archive
redistribution rights or complete historical toolchain provenance. Compiler
files remain private. Original licensed VC5 / Visual Studio 97 media remains a
route for independently checking these fingerprints.

The old search of Microsoft's public subscription catalog did not find an exact
VC5 or VS97 listing. It was not proof that every subscriber's authenticated
catalog lacked those products. No purchase, subscription or maintainer contact
was made during this setup.

## Reproduce the container runtime on Windows

The runtime image contains Debian and wibo only. VC5 is mounted read-only from
its separate private directory; it is not embedded in the image. The controller
currently runs on Windows with Docker Desktop's Linux engine. No legacy guest
Windows installation or Hyper-V VM was needed for the successful calibration.

1. Start Docker Desktop and obtain your private VC5 tree. Retain its acquisition
   provenance. The required native files include bin/cl.exe, bin/link.exe,
   bin/c1.dll, **bin/c2.exe**, include/, and lib/libcmt.lib.
2. From the repository root, download the pinned open-source wibo runtime:
   ```powershell
   New-Item -ItemType Directory -Force engine/private/container-runtime | Out-Null
   Invoke-WebRequest -Uri 'https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686' -OutFile engine/private/container-runtime/wibo-i686
   $runtimeHash = (Get-FileHash engine/private/container-runtime/wibo-i686 -Algorithm SHA256).Hash.ToLowerInvariant()
   if ($runtimeHash -ne '2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304') { throw 'wibo digest mismatch' }
   docker build --platform linux/amd64 -t openphantom-vc5-wibo:1.2.0 -f engine/docker/wibo/Dockerfile engine/private/container-runtime
   docker image inspect --format '{{.Id}}' openphantom-vc5-wibo:1.2.0
   ```
   The Dockerfile pins the Debian image digest and checks wibo's published
   release digest again. Only the runtime file belongs in this build context.
3. Copy `engine/toolchain.docker.example.json` to
   `engine/private/toolchain.json`. Set real absolute compiler/search paths,
   provenance, the Docker CLI path, and the printed immutable image ID. Keep
   all compiler paths beneath runner.toolchain_root. Do not use modern SDKs.
4. Run `configure`, `preflight`, `build`, `test`, `accept`, and a second fresh
   build followed by `test`/`check` as described in [the README](../README.md).
   A new image or compiler inventory requires an explicitly named new lock and
   reviewed baseline. Existing locks and baselines are never overwritten.

Each invocation has no network, runs as an unprivileged container user with
capabilities dropped, uses a read-only root filesystem, and mounts only the
compiler tree, src/, tests/, and that invocation's build directory. Only the
last mount is writable. The game reference and Docker socket are not mounted.
The verified newly built console fixture also runs in this container. Tool
banners, exact arguments, source hashes, tool/library hashes, and runtime image
identity are retained in private reports. Timed-out task containers are removed
by their unique names.

## Remaining limits

VC5 /Gy- is not supported; the original scaffold's assumption was corrected
from the real compiler warning. /W4 /WX remains enabled, and the driver also
rejects command-line warnings which VC5 /WX does not turn into failure.

The complete include/library search inventory is fingerprinted. The RTM LIBCMT
here successfully links the synthetic fixture, but this does not establish the
retail startup/heap library revision. Upstream reports possible SP1/SP2 code.
Keep that question open until compared; no copied startup bytes are used.

The native Windows configuration remains available for independently authorized
installations. It was not exercised with this community archive. The verified
setup is the container profile described above.
