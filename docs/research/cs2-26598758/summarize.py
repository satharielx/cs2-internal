"""Produce readable per-module evidence and static dependency edges."""
import collections
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
data = json.loads((HERE/'inventory.json').read_text(encoding='utf-8'))
modules = data['modules']
names = {Path(m['path']).name.lower() for m in modules}
lines = ['# Per-module evidence', '', 'Names and RVAs below come from PE tables and strings. Roles are discussed in overview.md.',
         'Interface candidates are strings only. RTTI names identify types, not named function bodies.', '']
edges = []
for m in modules:
    name = Path(m['path']).name
    lines.extend([f'## {name}', '', f"Path: `{m['path']}`  ",
                  f"SHA-256: `{m['sha256']}`  ",
                  f"Architecture: {m['format']} {m['architecture']}; preferred base `{m['image_base']}`; entry RVA `{m['entry_rva']}`.  ",
                  f"Exports: {len(m['exports'])}; RTTI types: {len(m['rtti'])}; x64 unwind entries: {m['runtime_function_entries']}.", ''])
    for p in m['pdb']:
        lines.extend([f"PDB reference: `{p['path']}`; GUID `{p['guid']}`; age {p['age']}; lookup key `{p['symbol_key']}`.", ''])
    dependencies = [i['module'] for i in m['imports']]
    lines.extend(['Normal import modules: '+', '.join(f'`{n}`' for n in dependencies)+'.', ''])
    if m['delay_import_modules']:
        lines.extend(['Delay import modules: '+', '.join(m['delay_import_modules'])+'.', ''])
    for dep in dependencies+m['delay_import_modules']:
        if dep.lower() in names:
            edges.append((name,dep))
    if m['exports']:
        lines.extend(['Export sample (first 30 entries; complete table in inventory.json):', '', '| Name | RVA | Forwarder |','|---|---|---|'])
        for e in m['exports'][:30]:
            label = ', '.join(e['names']) or '#'+str(e['ordinal'])
            lines.append(f"| `{label}` | `{e['rva']}` | {e['forwarder'] or ''} |")
        lines.append('')
    if m['coff_symbols']:
        lines.extend(['COFF symbol names: '+', '.join('`'+s['name']+'`' for s in m['coff_symbols'])+'.',''])
    if m['rtti']:
        lines.extend(['RTTI sample: '+', '.join('`'+s+'`' for s in m['rtti'][:15])+'.', ''])
    if m['interface_candidates']:
        lines.extend(['Interface-like string sample (unconfirmed; shared constants and false positives included): '+', '.join('`'+s+'`' for s in m['interface_candidates'][:12])+'.', ''])
(HERE/'module-details.md').write_text('\n'.join(lines),encoding='utf-8')
(HERE/'dependencies.tsv').write_text('module\timported_module\n'+''.join(a+'\t'+b+'\n' for a,b in sorted(set(edges))),encoding='utf-8')
stats = dict(modules=len(modules), bytes=sum(m['size'] for m in modules),
             architectures=dict(collections.Counter(m['architecture'] for m in modules)),
             exports=sum(len(m['exports']) for m in modules),
             named_exports=sum(bool(e['names']) for m in modules for e in m['exports']),
             rtti_occurrences=sum(len(m['rtti']) for m in modules),
             distinct_rtti=len({s for m in modules for s in m['rtti']}),
             pdb_references=sum(bool(m['pdb']) for m in modules),
             coff_symbols=sum(len(m['coff_symbols']) for m in modules),
             create_interface_modules=sum(any('CreateInterface' in e['names'] for e in m['exports']) for m in modules),
             installed_dependency_edges=len(set(edges)))
(HERE/'statistics.json').write_text(json.dumps(stats,indent=2)+'\n',encoding='utf-8')
print(json.dumps(stats,indent=2))
