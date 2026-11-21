from pathlib import Path
import json
import os
import struct
import sys
import pefile

root = Path(__file__).parent
manifest = root / (sys.argv[2] if len(sys.argv) > 2 else 'imports.json')
wanted = json.loads(manifest.read_text())
candidate_path = Path(sys.argv[1])
raw = candidate_path.read_bytes()
candidate = pefile.PE(data=raw)
image = bytearray(raw)
base = candidate.OPTIONAL_HEADER.ImageBase

def key(symbol):
    return symbol.name.decode("ascii") if symbol.name is not None else symbol.ordinal

def rva_offset(rva):
    offset = candidate.get_offset_from_rva(rva)
    if offset is None or offset+4 > len(image):
        raise ValueError(f'Unmapped RVA {rva:#x}')
    return offset

def read(rva):
    return struct.unpack_from('<I', image, rva_offset(rva))[0]

def write(rva, value):
    struct.pack_into('<I', image, rva_offset(rva), value)

def descriptors(pe):
    return {entry.dll.decode("ascii").lower(): entry for entry in pe.DIRECTORY_ENTRY_IMPORT}

assert candidate.FILE_HEADER.Machine == 0x14c
assert candidate.OPTIONAL_HEADER.Magic == 0x10b
assert not candidate.OPTIONAL_HEADER.DATA_DIRECTORY[11].Size
assert not candidate.OPTIONAL_HEADER.DATA_DIRECTORY[13].Size
assert not candidate.OPTIONAL_HEADER.DATA_DIRECTORY[4].Size

actual = descriptors(candidate)
if list(wanted) != list(actual):
    raise ValueError(f"DLL order: expected {list(wanted)}, got {list(actual)}")
slot_map = {}
for dll, entry in actual.items():
    original_symbols = [key(symbol) for symbol in entry.imports]
    wanted_symbols = wanted[dll]
    assert len(set(original_symbols)) == len(original_symbols)
    if set(original_symbols) != set(wanted_symbols):
        missing = sorted(set(wanted_symbols) - set(original_symbols), key=str)
        extra = sorted(set(original_symbols) - set(wanted_symbols), key=str)
        raise ValueError(f"{dll}: missing {missing}, extra {extra}")
    lookup = entry.struct.OriginalFirstThunk
    iat = entry.struct.FirstThunk
    assert lookup and read(lookup+4*len(original_symbols)) == read(iat+4*len(original_symbols)) == 0
    values = {}
    for index, symbol in enumerate(original_symbols):
        assert read(lookup+4*index) == read(iat+4*index)
        values[symbol] = (read(lookup+4*index), base+iat+4*index)
    for index, symbol in enumerate(wanted_symbols):
        value, old_slot = values[symbol]
        write(lookup+4*index, value)
        write(iat+4*index, value)
        slot_map[old_slot] = base+iat+4*index

relocation_rvas = {
    e.rva for block in candidate.DIRECTORY_ENTRY_BASERELOC
    for e in block.entries if e.type == pefile.RELOCATION_TYPE['IMAGE_REL_BASED_HIGHLOW']
}
patched = {}
for rva in relocation_rvas:
    old = candidate.get_dword_at_rva(rva)
    new = slot_map.get(old)
    if new is not None:
        patched[rva] = (old, new)
        write(rva,new)
assert {old for old, new in patched.values()} == set(slot_map)

# Find any other raw occurrences of an absolute slot address in mapped sections.
# Separate these from relocations and report them for inspection.
unknown = []
for section in candidate.sections:
    start = section.PointerToRawData
    end = start+min(section.SizeOfRawData,section.Misc_VirtualSize)
    for old in slot_map:
        needle = struct.pack('<I',old)
        offset = raw.find(needle,start,end)
        while offset >= 0:
            rva = section.VirtualAddress+offset-start
            if rva not in patched:
                unknown.append((section.Name.rstrip(b'\0').decode(),hex(rva),hex(old)))
            offset = raw.find(needle,offset+1,end)
if unknown:
    raise ValueError(f"Absolute IAT slot values outside HIGHLOW relocations: {unknown}")

if candidate.OPTIONAL_HEADER.CheckSum:
    p = pefile.PE(data=bytes(image))
    struct.pack_into('<I', image, candidate.OPTIONAL_HEADER.get_field_absolute_offset('CheckSum'), p.generate_checksum())

verified = pefile.PE(data=bytes(image))
def order(pe):
    return [(dll.dll.decode("ascii").lower(), [key(symbol) for symbol in dll.imports]) for dll in pe.DIRECTORY_ENTRY_IMPORT]

assert order(verified) == list(wanted.items())
def slots(pe):
    return {symbol.address:(dll.dll.decode("ascii").lower(),key(symbol)) for dll in pe.DIRECTORY_ENTRY_IMPORT for symbol in dll.imports}

before_slots, after_slots = slots(candidate), slots(verified)
assert all(before_slots[old] == after_slots[verified.get_dword_at_rva(rva)] for rva,(old,new) in patched.items())
assert len(image) == len(raw)
temporary = candidate_path.with_suffix('.iat.tmp')
temporary.write_bytes(image)
os.replace(temporary, candidate_path)
print(f'Verified {len(wanted)} DLLs, {len(slot_map)} ordered imports and {len(patched)} IAT references.')
