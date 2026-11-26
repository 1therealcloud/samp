# SA-MP 0.3.7 / 0.3.DL

This project reimplements the SA-MP 0.3.7-R5 and 0.3.DL-R1 clients through reverse engineering and matching decompilation.

The clients are reconstructed from the original binaries.\
The 0.2x sources serve as reference material for names, organization and some code\
(`source/archive`, `tools/arctool2`, `tools/rcon`).

## Branches

- `master` is the development branch for fixes and improvements.
- `0.3.7-R5` contains the reconstructed SA-MP 0.3.7-R5 client.
- `0.3.DL-R1` contains the reconstructed SA-MP 0.3.DL-R1 client.

## Dependencies

RakNet, zlib, libcurl, LZMA, and TTMath are pinned Git submodules under `third-party/`.\
DXSDK, BASS, Detours, and the DXUT are included directly.\
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
- [Visual Studio 2022](https://aka.ms/vs/17/release/vs_community.exe) with the MSVC v141 (VS 2017) - Windows XP support toolset.
- [Delphi 7 (optional)](https://altd.embarcadero.com/download/delphi/d7/english/ent/delphi_7_ent_en.iso).

Initialize the dependencies:

```console
git submodule update --init --recursive
```

Open `sa-mp.sln` in Visual Studio 2022 and build.

The solution includes the C++ utilities: `rcon`, `arctool2`, `launch3`, `dfftool` and `DffFormatDll`.

To build the server browser, open `tools/exgui/samp.dpr` in Delphi 7 and build the project.

To build the DFF GUI, open `tools/dff/Project1/Project1.dpr` in Delphi 7 and build the project.

## Formatting

Run `format.cmd` to format `source/` and `tools/` using the included `.clang-format`.
