"""Check whole-installation Ghidra coverage against the independent PE inventory."""
import json
from pathlib import Path
import re

here = Path(__file__).resolve().parent
inventory = json.loads((here/'inventory.json').read_text(encoding='utf-8'))
failures = list(inventory['errors'])
discovery = json.loads((here/'discovery.json').read_text(encoding='utf-8'))
if set(discovery['native_magic_candidates']) != {m['path'] for m in inventory['modules']} or discovery['errors']:
    failures.append('Header discovery does not match module inventory')
console = (here.parents[2]/'build/cs2-research/headless-console.log').read_text(encoding='utf-8')
completed = re.findall(r'RESEARCH_COMPLETE (\S+)', console)
if set(completed) != {Path(m['path']).name for m in inventory['modules']}:
    failures.append('Final Ghidra run completion log does not cover the inventory')
if re.search(r'^ERROR', console, re.M):
    failures.append('Final Ghidra console contains errors')
functions = []
for module in inventory['modules']:
    name = Path(module['path']).name
    output = here/'ghidra'/(name+'.txt')
    if not output.exists():
        failures.append(name+': missing Ghidra output')
        continue
    text = output.read_text(encoding='utf-8')
    if 'SHA256: '+module['sha256'] not in text:
        failures.append(name+': SHA256 mismatch')
    expected = 'Language: x86:LE:'+('64' if module['architecture']=='x86-64' else '32')+':default'
    if expected not in text:
        failures.append(name+': language mismatch')
    if 'Function count after targeted disassembly:' not in text:
        failures.append(name+': incomplete script output')
    for symbol, address, rva in re.findall(r'^FUNCTION (\S+) (\S+) RVA=(\S+)$',text,re.M):
        functions.append((name,symbol,rva))
lines = ['# Ghidra inspected export index', '', 'Selected exports disassembled and submitted to the decompiler. See each module report for pseudocode or decompiler warnings.', '',
         '| Module | Export | RVA |', '|---|---|---|']
lines.extend(f'| {m} | `{s}` | `0x{r.upper()}` |' for m,s,r in sorted(functions))
(here/'functions.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
result = dict(expected_modules=len(inventory['modules']),
              ghidra_reports=len(list((here/'ghidra').glob('*.txt'))),
              ghidra_completion_records=len(completed),
              inspected_exports=len(functions), failures=failures)
(here/'verification.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps(result,indent=2))
raise SystemExit(bool(failures))
