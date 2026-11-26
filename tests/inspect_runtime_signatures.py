"""Read-only executable-section signature audit for an installed CS2 build.

Checks current hook/interface/resolver patterns and corroborates the frame
callback against CSource2Client RTTI. Does not execute or patch target binaries.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
from inspect_chams_binary import Image

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('installation', type=Path)
parser.add_argument('--overlay', type=Path)
parser.add_argument('--output', type=Path)
args = parser.parse_args()
workspace = Path(__file__).resolve().parents[1]
cache, patterns, records = {}, {}, []
for subsystem in ['hooks', 'interfaces', 'pattern_resolver']:
    text = (workspace/f'manager/core/constants/{subsystem}_signatures.h').read_text(encoding='utf-8')
    for name, pattern in re.findall(r'char (\w+)\[\] = "([^"]+)"', text):
        if not name.endswith('_legacy'):
            patterns[subsystem+'.'+name] = pattern
    for name, owner, target in re.findall(r'auto& (\w+) = runtime_patterns::(\w+)::(\w+)', text):
        patterns[subsystem+'.'+name] = patterns[owner+'.'+target]
for identifier, pattern in patterns.items():
    name = identifier.split('.')[1]
    module = 'client.dll'
    if name in ['present', 'resize_buffers', 'create_swap_chain']:
        module = 'gameoverlayrenderer64.dll'
    elif name in ['sky_draw', 'base_draw_array', 'light_update']:
        module = 'scenesystem.dll'
    elif name == 'network_game_client':
        module = 'engine2.dll'
    if module not in cache:
        path = (args.overlay or args.installation.parents[2]/module) if module.startswith('gameoverlay') else next(args.installation.rglob(module))
        image = Image(path)
        pe = struct.unpack_from('<I', image.data, 60)[0]
        opt = struct.unpack_from('<H', image.data, pe+20)[0]
        code = []
        for i, (_, virtual_size, rva, raw_size, offset) in enumerate(image.sections):
            flags = struct.unpack_from('<I', image.data, pe+24+opt+i*40+36)[0]
            if flags & 0x20000000:
                code.append((rva, image.data[offset:offset+min(virtual_size or raw_size, raw_size)]))
        cache[module] = (image, code)
    image, code = cache[module]
    regex = b''.join(b'.' if '?' in token else re.escape(bytes.fromhex(token)) for token in pattern.split())
    matches = [rva+m.start() for rva, data in code for m in re.finditer(b'(?='+regex+b')', data, re.S)]
    records.append(dict(pattern=identifier, module=module, matches=[hex(x) for x in matches]))
    print(identifier, len(matches), ', '.join(hex(x) for x in matches))
client = cache['client.dll'][0]
tables = list(client.tables('.?AVCSource2Client@@'))
target = client.slots(tables[0], [36])[36] if len(tables) == 1 else None
frame = next(r for r in records if r['pattern'] == 'interfaces.frame_stage_notify')
failures = [r['pattern'] for r in records if len(r['matches']) != 1]
if target is None or frame['matches'] != [hex(target)]:
    failures.append('Frame callback does not match CSource2Client primary vtable slot 36')
result = dict(modules={name: dict(path=str(image.path), sha256=hashlib.sha256(image.data).hexdigest())
                            for name, (image, _) in cache.items()},
              patterns=records, frame_vtable_rva=hex(tables[0]) if len(tables) == 1 else None,
              frame_slot=36, frame_callback_rva=hex(target) if target is not None else None,
              failures=failures)
if args.output:
    args.output.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
print('FAILURES:', failures)
raise SystemExit(bool(failures))
