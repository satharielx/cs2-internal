"""Stage copies of inventoried binaries and run Ghidra without executing them."""
import json
from pathlib import Path
import shutil
import subprocess

workspace = Path(__file__).resolve().parents[3]
research = Path(__file__).resolve().parent
build = workspace/'build/cs2-research'
inventory = json.loads((research/'inventory.json').read_text(encoding='utf-8'))
staging = build/'binaries'
staging.mkdir(parents=True, exist_ok=True)
for module in inventory['modules']:
    shutil.copy2(Path(inventory['root'])/module['path'], staging/Path(module['path']).name)
jdk = next((build/'tools/jdk25').glob('*/bin/java.exe'))
ghidra = Path(r'C:\Users\demon\Downloads\ghidra_12.2_PUBLIC_20261016\ghidra_12.2_PUBLIC')
utility = next(ghidra.glob('Ghidra/Framework/Utility/lib/Utility.jar'))
project = build/'projects'
project.mkdir(exist_ok=True)
for folder in ['user','cache','settings-final','tmp']:
    (build/folder).mkdir(exist_ok=True)
command = [str(jdk), '-Xmx4G', '-Djava.awt.headless=true', '-Djava.system.class.loader=ghidra.GhidraClassLoader',
           '-Duser.home='+str(build/'user'), '-Djava.io.tmpdir='+str(build/'tmp'),
           '-Dapplication.cachedir='+str(build/'cache'), '-Dapplication.settingsdir='+str(build/'settings-final'),
           '-cp',str(utility),'ghidra.Ghidra','ghidra.app.util.headless.AnalyzeHeadless',
           str(project),'CS2_26598758_Inventory','-import',str(staging),'-recursive','-noanalysis','-overwrite',
           '-loader-libraryLoadDepth','0',
           '-scriptPath',str(research),'-postScript','ResearchInventory.java',str(research/'ghidra'),
           '-max-cpu','4','-log',str(build/'ghidra.log'),'-scriptlog',str(build/'scripts.log')]
with (build/'headless-console.log').open('w',encoding='utf-8') as log:
    result = subprocess.run(command,stdout=log,stderr=subprocess.STDOUT)
print('Ghidra exit:',result.returncode,flush=True)
raise SystemExit(result.returncode)
