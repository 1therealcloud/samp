# SA-MP 0.3.7 / 0.3.DL

This project reimplements the SA-MP 0.3.7-R5 and 0.3.DL-R1 clients through reverse engineering and matching decompilation.

The clients are reconstructed from the original binaries.\
The 0.2x sources serve as reference material for names, organization and some code\
(`source/archive`, `tools/arctool2`, `tools/rcon`).

## Status

The client is fully functional and playable, with no known issues observed during testing.\
All SA-MP functionality is implemented.\
Approximately **75% of the SA-MP client code** matches the original machine code.\
The remaining **25%** follows the original logic.

The match percentage and inventory below use **SA-MP 0.3.DL-R1 `samp.dll`** as the reference.

SHA-256:

```text
bccdb297464bd382625635be25585df07a8fa6668bc0015650708e3eb4ffcd4b
```

The current binary inventory contains **9,093 native entries**.\
These counts describe native entry addresses, rather than unique C++ methods or byte-matched functions.

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
| Shared code / uncertain ownership | 213 |
| Other import thunks | 66 |
| **Total** | **9,093** |

## Dependencies

RakNet, zlib, libcurl, LZMA, and TTMath are pinned Git submodules under `third-party/`.\
BASS, Detours, and the modified DXUT sources are included directly.\
The DirectX SDK supplies D3DX9.\
The MSVC runtime is linked statically.

| Dependency | Match status |
| --- | --- |
| libcurl 7.57.0 | All verified function bodies match the original. |
| zlib 1.2.11 | All verified function bodies match the original. |
| LZMA 16.04 | All verified function bodies match the original. |
| BASS | The project uses [BASS 2.4.7](http://www.un4seen.com/stuff/bass247.zip), the same build shipped with the original client. |
| TTMath | Uses the [open.mp fork of TTMath 0.9.3](https://github.com/openmultiplayer/ttmath), which includes small source changes. |
| RakNet and DXUT | Closely match the client-specific versions in the original binary. Some implementations still differ. |
| Detours | The project uses the same Detours version as the original client (1.5 Build 46). |

## Utilities

The repository also includes the server browser and supporting tools:

| Tool | Source | Purpose |
| --- | --- | --- |
| `samp.exe` | [`tools/exgui`](tools/exgui) | Server browser, favorites, server queries and game launcher. |
| `rcon.exe` | [`tools/rcon`](tools/rcon) | Console client for remote server administration. |
| `arctool2.exe` | [`tools/arctool2`](tools/arctool2) | Builds SA-MP archives from a file list. |
| `launch3.exe` | [`tools/launch3`](tools/launch3) | Alternative game launcher with DLL injection. |
| `kdff.exe` | [`tools/dff/dfftool`](tools/dff/dfftool) | Inspects DFF files, generates and attaches COL collision data, and imports night vertex colors. |
| `kdff.dll` | [`tools/dff/DffFormatDll`](tools/dff/DffFormatDll) | DFF/COL processing and vertex-color operations used by the DFF GUI. |
| `kdffgui.exe` | [`tools/dff/Project1`](tools/dff/Project1) | Graphical DFF preview, collision generation and vertex-color editing. |

## Building

Build requirements:

- [Git](https://git-scm.com/install/windows).
- [Visual Studio .NET 2003 RTM](https://archive.org/details/microsoft-visual-studio-.-net-2003-professional-disc-1).
- [DirectX 9.0 SDK, April 2005](https://web.archive.org/web/20111118175754/https://download.microsoft.com/download/a/c/d/acdfb557-266f-4af4-8673-6ab8009b4ddd/dxsdk_apr2005.exe).
- [Delphi 7 (optional)](https://altd.embarcadero.com/download/delphi/d7/english/ent/delphi_7_ent_en.iso).

Initialize the dependencies:

```console
git submodule update --init --recursive
```

Open `sa-mp.sln` in Visual Studio .NET 2003 and build the configuration for your client version.

| Client | Configuration | Output |
| --- | --- | --- |
| 0.3.7-R5 | `Release` | `build/binaries/Release/` |
| 0.3.DL-R1 | `Release DL` | `build/binaries/Release DL/` |

The solution includes the C++ utilities: `rcon`, `arctool2`, `launch3`, `dfftool` and `DffFormatDll`.

To build the server browser, open `tools/exgui/samp.dpr` in Delphi 7 and build the project.

To build the DFF GUI, open `tools/dff/Project1/Project1.dpr` in Delphi 7 and build the project.

## Formatting

Run `format.cmd` to format `source/` and `tools/` using the included `.clang-format`.
