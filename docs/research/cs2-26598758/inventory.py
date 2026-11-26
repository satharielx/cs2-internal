"""Read-only, standard-library PE inventory; does not load target libraries."""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import struct
import uuid


def inspect(path, root):
    b = path.read_bytes()
    def u(fmt, off):
        return struct.unpack_from('<' + fmt, b, off)
    pe = u('I', 60)[0]
    if b[:2] != b'MZ' or b[pe:pe+4] != b'PE\0\0':
        raise ValueError('Not PE')
    machine, ns, timestamp, symbols, symbol_count, optsize, flags = u('HHIIIHH', pe+4)
    op = pe+24
    is64 = u('H', op)[0] == 0x20b
    base = u('Q' if is64 else 'I', op+(24 if is64 else 28))[0]
    sections = []
    for i in range(ns):
        s = op+optsize+40*i
        name, vs, va, size, raw = u('8sIIII', s)
        sections.append(dict(name=name.rstrip(b'\0').decode('ascii', 'replace'), rva=va,
                             virtual_size=vs, raw_size=size, raw_offset=raw,
                             flags=hex(u('I', s+36)[0])))
    def off(rva):
        for s in sections:
            if s['rva'] <= rva < s['rva']+s['raw_size']:
                return s['raw_offset']+rva-s['rva']
        if rva < u('I', op+60)[0]:
            return rva
        raise ValueError(f'Unmapped RVA {rva:x}')
    def string(o):
        return b[o:b.index(b'\0', o)].decode('ascii', 'replace')
    dd = op+(112 if is64 else 96)
    def directory(n):
        return u('II', dd+8*n)
    exports = []
    erva, esize = directory(0)
    if erva:
        e = off(erva)
        ordinal_base, nf, nn, funcs, names, ords = u('IIIIII', e+16)
        labels = collections.defaultdict(list)
        for i in range(nn):
            labels[u('H', off(ords)+i*2)[0]].append(string(off(u('I', off(names)+i*4)[0])))
        for i in range(nf):
            target = u('I', off(funcs)+i*4)[0]
            if target:
                exports.append(dict(ordinal=ordinal_base+i, names=labels[i], rva=hex(target),
                                    forwarder=string(off(target)) if erva <= target < erva+esize else None))
    imports = []
    irva, _ = directory(1)
    if irva:
        p = off(irva)
        while any(u('IIIII', p)):
            original, _, _, name, first = u('IIIII', p)
            thunk = off(original or first)
            entries = []
            step = 8 if is64 else 4
            while True:
                value = u('Q' if is64 else 'I', thunk)[0]
                if not value:
                    break
                entries.append('#'+str(value & 0xffff) if value & (1 << (step*8-1)) else string(off(value)+2))
                thunk += step
            imports.append(dict(module=string(off(name)), symbols=entries))
            p += 20
    delay_imports = []
    drva, dsize = directory(13)
    if drva:
        for p in range(off(drva), off(drva)+dsize, 32):
            fields = u('IIIIIIII', p)
            if not any(fields):
                break
            delay_imports.append(string(off(fields[1] if fields[0] & 1 else fields[1]-base)))
    pdb = []
    debug_rva, debug_size = directory(6)
    if debug_rva:
        for i in range(debug_size//28):
            d = off(debug_rva)+i*28
            kind, size, _, pointer = u('IIII', d+12)
            if kind == 2 and b[pointer:pointer+4] == b'RSDS':
                guid = str(uuid.UUID(bytes_le=b[pointer+4:pointer+20]))
                age = u('I', pointer+20)[0]
                pdb.append(dict(path=string(pointer+24), guid=guid, age=age,
                                symbol_key=guid.replace('-', '').upper()+format(age, 'X')))
    coff_symbols = []
    if symbols and symbol_count:
        table_end = symbols+18*symbol_count
        i = 0
        while i < symbol_count:
            p = symbols+18*i
            rawname, value, section, kind, storage, aux = u('8sIhHBB', p)
            if rawname[:4] == b'\0'*4:
                name = string(table_end+struct.unpack('<I', rawname[4:])[0])
            else:
                name = rawname.rstrip(b'\0').decode('ascii', 'replace')
            coff_symbols.append(dict(name=name, value=hex(value), section=section, type=kind, storage=storage))
            i += 1+aux
    strings = [m.group().decode('ascii') for m in re.finditer(rb'[\x20-\x7e]{5,}', b)]
    rtti = sorted(set(s for s in strings if s.startswith(('.?AV', '.?AU')) and s.endswith('@@')))
    interfaces = sorted(set(s for s in strings if re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]{3,85}0[0-9]{2}', s)))
    interesting = sorted(set(s for s in strings if len(s) < 220 and re.search(
        r'CreateInterface|SchemaSystem|Source2Client|Source2Engine|GameResourceService|NetworkSystem|SceneSystem|ResourceSystem|RenderSystem|PanoramaUIEngine|CGameEntitySystem|CEntitySystem|C_CSPlayerPawn|CCSPlayerController|CGameSceneNode|CBaseSceneObject|CAnimatableSceneObject|Register.*Schema', s)))
    return dict(path=path.relative_to(root).as_posix(), size=len(b), sha256=hashlib.sha256(b).hexdigest(),
                machine=hex(machine), architecture={0x8664:'x86-64',0x14c:'x86',0xaa64:'ARM64'}.get(machine,'unknown'),
                format='PE32+' if is64 else 'PE32', image_base=hex(base), entry_rva=hex(u('I',op+16)[0]),
                coff_timestamp=timestamp, coff_symbol_count=symbol_count, coff_symbols=coff_symbols, sections=sections,
                exports=exports, imports=imports, delay_import_modules=delay_imports, pdb=pdb,
                rtti=rtti, interface_candidates=interfaces, interesting_strings=interesting,
                runtime_function_entries=directory(3)[1]//12 if is64 else None)


def main():
    p = argparse.ArgumentParser()
    p.add_argument('root', type=Path)
    p.add_argument('output', type=Path)
    args = p.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    files = sorted(x for x in args.root.rglob('*') if x.is_file() and x.suffix.lower() in {'.dll','.exe','.sys','.so','.dylib','.elf'})
    records, errors = [], []
    for path in files:
        try:
            record = inspect(path,args.root)
            records.append(record)
            print(record['path'], record['architecture'], len(record['exports']), flush=True)
        except Exception as ex:
            errors.append(dict(path=str(path), error=str(ex)))
    result = dict(root=str(args.root), modules=records, errors=errors,
                  loose_pdb_files=[str(x.relative_to(args.root)) for x in args.root.rglob('*.pdb')])
    (args.output/'inventory.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
    lines = ['# Complete installed binary inventory', '', '| Module | Architecture | MiB | Exports | Imports | RTTI types | PDB reference |', '|---|---|---:|---:|---:|---:|---|']
    for r in records:
        lines.append(f"| {r['path']} | {r['architecture']} | {r['size']/1048576:.2f} | {len(r['exports'])} | {sum(len(i['symbols']) for i in r['imports'])} | {len(r['rtti'])} | {', '.join(Path(d['path']).name for d in r['pdb'])} |")
    (args.output/'modules.md').write_text('\n'.join(lines)+'\n', encoding='utf-8')
    print('TOTAL',len(records),'ERRORS',errors)


if __name__ == '__main__':
    main()
