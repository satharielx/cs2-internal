"""Inspect RVAs in the saved read-only research project without executing targets."""
import argparse
from pathlib import Path
import subprocess

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('module')
p.add_argument('output', type=Path)
p.add_argument('rvas', nargs='+')
args = p.parse_args()
w = Path(__file__).resolve().parents[1]
b = w/'build/cs2-research'
g = Path(r'C:\Users\demon\Downloads\ghidra_12.2_PUBLIC_20261016\ghidra_12.2_PUBLIC')
r = w/'docs/research/cs2-26598758'
cmd = [str(next((b/'tools/jdk25').glob('*/bin/java.exe'))), '-Xmx4G', '-Djava.awt.headless=true',
       '-Djava.system.class.loader=ghidra.GhidraClassLoader', '-Duser.home='+str(b/'user'),
       '-Dapplication.cachedir='+str(b/'cache'), '-Dapplication.settingsdir='+str(b/'settings-final'),
       '-cp', str(g/'Ghidra/Framework/Utility/lib/Utility.jar'), 'ghidra.Ghidra',
       'ghidra.app.util.headless.AnalyzeHeadless', str(b/'projects'), 'CS2_26598758_Inventory/binaries',
       '-process', args.module, '-noanalysis', '-readOnly', '-scriptPath', str(r),
       '-postScript', 'InspectFunctions.java', str(args.output.resolve()), *args.rvas]
with args.output.with_suffix('.log').open('w') as log:
    result = subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT)
raise SystemExit(result.returncode)
