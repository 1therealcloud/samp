# SA-MP 0.3.7-R5

This branch archives the SA-MP 0.3.7-R5 client reconstructed from the original binary. Fixes and improvements are developed on `master`.

The 0.2x sources serve as reference material for names, organization and some code\
(`source/archive`, `tools/arctool2`, `tools/rcon`).

## Dependencies

RakNet, zlib, LZMA, and TTMath are pinned Git submodules under `third-party/`.\
BASS, Detours, and the modified DXUT sources are included directly.\
The DirectX SDK supplies D3DX9.\
The MSVC runtime is linked statically.

| Dependency | Match status |
| --- | --- |
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

Open `sa-mp.sln` in Visual Studio .NET 2003 and build `Release`. The client DLL is written to `build/binaries/Release/samp.dll`.

The solution includes `rcon`, `arctool2` and `launch3`.

To build the server browser, open `tools/exgui/samp.dpr` in Delphi 7 and build the project.

## Formatting

Run `format.cmd` to format `source/` and `tools/` using the included `.clang-format`.
