"""Read-only verification of entity lifecycle signatures in a supplied PE64 file."""
import argparse
import hashlib
from pathlib import Path
import re
import struct

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("client", type=Path)
args = parser.parse_args()
data = args.client.read_bytes()
if data[:2] != b"MZ":
    raise SystemExit("Not a PE file")
pe = struct.unpack_from("<I", data, 60)[0]
if data[pe:pe + 4] != b"PE\0\0":
    raise SystemExit("Invalid PE signature")
count = struct.unpack_from("<H", data, pe + 6)[0]
optional_size = struct.unpack_from("<H", data, pe + 20)[0]
sections = [struct.unpack_from("<8sIIII", data, pe + 24 + optional_size + i * 40)
            for i in range(count)]
header = Path(__file__).resolve().parents[1] / "manager/core/constants/hooks_signatures.h"
patterns = header.read_text()
print("SHA-256:", hashlib.sha256(data).hexdigest())
failed = False
for name in ("entity_added", "entity_removed"):
    pattern = re.search(name + r'\[\] = "([^"]+)"', patterns)[1]
    regex = b"".join(b"." if "?" in token else re.escape(bytes.fromhex(token))
                     for token in pattern.split())
    matches = []
    for index, (_, _, rva, size, offset) in enumerate(sections):
        characteristics = struct.unpack_from("<I", data, pe + 24 + optional_size + index * 40 + 36)[0]
        if not characteristics & 0x20000000:
            continue
        matches.extend(rva + match.start() for match in re.finditer(regex, data[offset:offset + size], re.S))
    print(name + ":", len(matches), "executable matches", ", ".join(hex(x) for x in matches))
    failed |= len(matches) != 1
raise SystemExit(1 if failed else 0)
