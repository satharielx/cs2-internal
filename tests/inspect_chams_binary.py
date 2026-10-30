"""Read-only PE64 inspection of the installed chams-related RTTI and exports.

Usage: python tests/inspect_chams_binary.py <path-to-client.dll>
Does not load the binaries, attach to the game, or patch any addresses.
"""
import argparse
import hashlib
from pathlib import Path
import re
import struct


class Image:
    def __init__(self, path):
        self.path = path
        self.data = path.read_bytes()
        if self.data[:2] != b"MZ":
            raise ValueError(f"Not a PE: {path}")
        pe = self.unpack("I", 60)[0]
        if self.data[pe:pe + 4] != b"PE\0\0" or self.unpack("H", pe + 24)[0] != 0x20B:
            raise ValueError(f"Not PE64: {path}")
        count = self.unpack("H", pe + 6)[0]
        optional_size = self.unpack("H", pe + 20)[0]
        self.base = self.unpack("Q", pe + 48)[0]
        self.directory = pe + 24 + 112
        self.sections = [self.unpack("8sIIII", pe + 24 + optional_size + i * 40)
                         for i in range(count)]

    def unpack(self, fmt, offset):
        return struct.unpack_from("<" + fmt, self.data, offset)

    def rva(self, offset):
        return next(s[2] + offset - s[4] for s in self.sections
                    if s[4] <= offset < s[4] + s[3])

    def offset(self, rva):
        return next(s[4] + rva - s[2] for s in self.sections
                    if s[2] <= rva < s[2] + s[3])

    def string(self, offset):
        return self.data[offset:self.data.index(b"\0", offset)].decode("ascii", "replace")

    def tables(self, name):
        for match in re.finditer(re.escape(name.encode() + b"\0"), self.data):
            descriptor = self.rva(match.start() - 16)
            for reference in re.finditer(re.escape(struct.pack("<I", descriptor)), self.data):
                pos = reference.start() - 12
                if pos < 0 or pos + 24 > len(self.data):
                    continue
                sig, displacement, _, _, _, self_rva = self.unpack("6I", pos)
                try:
                    valid = sig == 1 and self_rva == self.rva(pos)
                except StopIteration:
                    continue
                if not valid or displacement != 0:
                    continue
                for table in re.finditer(re.escape(struct.pack("<Q", self.base + self_rva)), self.data):
                    yield self.rva(table.start() + 8)

    def slots(self, table, indices):
        return {i: self.unpack("Q", self.offset(table) + i * 8)[0] - self.base for i in indices}

    def exports(self):
        directory, size = self.unpack("II", self.directory)
        if not directory or not size:
            return
        values = self.unpack("IIHHIIIIIII", self.offset(directory))
        count, functions, names, ordinals = values[7:11]
        for i in range(count):
            name = self.string(self.offset(self.unpack("I", self.offset(names) + i * 4)[0]))
            ordinal = self.unpack("H", self.offset(ordinals) + i * 2)[0]
            yield name, self.unpack("I", self.offset(functions) + ordinal * 4)[0]


def inspect(client):
    game = client.parents[3]
    images = [Image(client)] + [Image(game / "bin/win64" / name)
                               for name in ("scenesystem.dll", "materialsystem2.dll", "tier0.dll")]
    for image in images:
        print(image.path.name, "SHA-256", hashlib.sha256(image.data).hexdigest())
    scene, material, tier = images[1:]
    targets = []
    for name in (".?AVCAnimatableSceneObjectDesc@@", ".?AVCBaseSceneObjectDesc@@"):
        tables = list(scene.tables(name))
        if len(tables) != 1:
            raise ValueError(f"Expected one {name} primary vtable, found {tables}")
        slots = scene.slots(tables[0], [4])
        targets.append(slots[4])
        print(name, "vtable RVA", hex(tables[0]), "slot 4 RVA", hex(slots[4]))
    if targets[0] != targets[1]:
        raise ValueError("Descriptor slot 4 functions differ in this build")
    body = scene.data[scene.offset(targets[0]):scene.offset(targets[0]) + 160]
    for instruction in (bytes.fromhex("48 8B 82 A8 00 00 00"),
                        bytes.fromhex("4C 8B 80 E8 03 00 00"),
                        bytes.fromhex("8B 88 F0 03 00 00")):
        if instruction not in body:
            raise ValueError("Override field load not found at function entry: " + instruction.hex())
    print("Confirmed holder +0xA8, array +0x3E8, count +0x3F0 entry loads")
    tables = list(material.tables(".?AVCMaterialSystem2@@"))
    if len(tables) != 1:
        raise ValueError("Material system primary vtable is not unique")
    print("CMaterialSystem2 vtable RVA", hex(tables[0]),
          "slots", {i: hex(r) for i, r in material.slots(tables[0], [14, 29]).items()})
    header = (Path(__file__).resolve().parents[1] / "manager/core/constants/chams_signatures.h").read_text()
    for name in ("arena_construct", "arena_root", "arena_cleanup_call", "material_create"):
        pattern = re.search(name + r'\[\] = "([^"]+)"', header)[1]
        regex = b"".join(b"." if token == "?" else re.escape(bytes.fromhex(token))
                         for token in pattern.split())
        image = material if name == "material_create" else images[0]
        matches = [image.rva(m.start()) for m in re.finditer(regex, image.data, re.S)]
        if len(matches) != 1:
            raise ValueError(f"{name}: expected a unique match, found {matches}")
        if name == "material_create" and matches[0] != material.slots(tables[0], [29])[29]:
            raise ValueError("Material creation fingerprint differs from slot 29")
        print(name, "RVA", hex(matches[0]))
        if name == "arena_cleanup_call":
            offset = image.offset(matches[0])
            cleanup = matches[0] + 12 + image.unpack("i", offset + 8)[0]
            print("arena cleanup target RVA", hex(cleanup))
    for name, rva in tier.exports():
        if name.startswith("?LoadKV3@@") and "PEAVKeyValues3@@PEAVCUtlString@@PEBD" in name:
            print("Text KV3 export RVA", hex(rva), name)
    print("Static discovery only; material ownership and live hook behavior require validation.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("client", type=Path)
    inspect(parser.parse_args().client)
