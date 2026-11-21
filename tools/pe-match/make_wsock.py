from pathlib import Path
import struct
import subprocess
import sys

definition = Path(__file__).with_name("wsock.def")
output = Path(sys.argv[1])
subprocess.run(
    [sys.argv[2], "/nologo", "/machine:x86", f"/def:{definition}", f"/out:{output}"],
    check=True,
)
archive = bytearray(output.read_bytes())
assert archive.startswith(b"!<arch>\n")
offset = 8
count = 0
while offset + 60 <= len(archive):
    size = int(archive[offset + 48:offset + 58])
    member = offset + 60
    offset = member + size + (size % 2)
    if archive[member:member + 4] == b"\0\0\xff\xff":
        # IMPORT_OBJECT_NAME_UNDECORATE keeps the x86 stdcall linker symbol
        # and strips its decoration from the name the Windows loader sees.
        flags = struct.unpack_from("<H", archive, member + 18)[0]
        struct.pack_into("<H", archive, member + 18, (flags & 3) | (3 << 2))
        count += 1
assert count == 25
output.write_bytes(archive)
