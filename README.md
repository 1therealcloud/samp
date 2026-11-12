# SA-MP 0.3.DL-R1

This project aims to reimplement SA-MP 0.3.DL-R1 as closely as possible to its original source code through reverse engineering and matching decompilation.

The 0.3.DL-R1 client is reconstructed from its binary. The 0.2x sources serve as reference material for names and organization, and some legacy components, including the archive code, are reused.

## Status

The client is fully functional and playable, with no known issues observed during testing. All SA-MP functionality is implemented. Matching the compiled code to the original binary remains a work in progress, with substantial work still ahead.

Reference binary: **SA-MP 0.3.DL-R1 `samp.dll`**.

SHA-256:

```text
bccdb297464bd382625635be25585df07a8fa6668bc0015650708e3eb4ffcd4b
```

The current binary inventory contains **9,095 native entries**: **9,012 function and compiler-helper entries**, plus **83 import thunks**. These counts describe native entry addresses, rather than unique C++ methods or byte-matched functions.

| Component | Entries |
| --- | ---: |
| SA-MP code | 2,407 |
| RakNet | 1,785 |
| zlib | 43 |
| libcurl | 839 |
| LZMA | 135 |
| ttmath | 215 |
| BASS imports | 17 |
| Detours | 51 |
| DXUT | 1,038 |
| DirectX SDK helpers | 34 |
| D3DX9 | 53 |
| CRT / STL | 864 |
| MSVC-generated code / exception handling | 1,333 |
| Shared code / uncertain ownership | 215 |
| Other import thunks | 66 |
| **Total** | **9,095** |

BASS is an external DLL; its entries above are import thunks. The other 66 import thunks cover D3DX9 and Windows APIs. Uncertain ownership is recorded separately from functionality and matching progress.

## Dependencies

RakNet, zlib, libcurl, LZMA and ttmath are pinned Git submodules under `third-party/`. BASS, Detours and the modified DXUT sources are included directly in that directory. D3DX9 is supplied by the DirectX SDK. The MSVC runtime is linked statically.

## Building

Build requirements:

- Git.
- Visual Studio .NET 2003 (Service Pack 1 optional).
- DirectX 9.0 SDK, April 2005.
- Delphi 7 (optional, for the server-browser launcher).

Initialize the dependencies:

```console
git submodule update --init --recursive
```

Open `sa-mp.sln` in Visual Studio .NET 2003 and build the solution. The output is placed in `build/binaries/`.

To build the server-browser launcher, open `tools/exgui/samp.dpr` in Delphi 7 and build the project to produce `samp.exe`.

## Formatting

Run `format.cmd` to format `source/` using the included `.clang-format`.

Inline assembly is enclosed in `// clang-format off` and `// clang-format on` guards.
