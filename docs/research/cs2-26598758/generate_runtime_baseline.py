"""Generate the app's identity-only catalog from the saved PE inventory."""
import json
from pathlib import Path

here = Path(__file__).resolve().parent
workspace = here.parents[2]
inventory = json.loads((here/'inventory.json').read_text(encoding='utf-8'))
roles = {
    'cs2.exe':'Launcher', 'client.dll':'Client gameplay', 'server.dll':'Server gameplay',
    'engine2.dll':'Engine services', 'tier0.dll':'Foundation / KV3 / memory',
    'schemasystem.dll':'Type reflection', 'resourcesystem.dll':'Resource loading',
    'filesystem_stdio.dll':'Filesystem', 'host.dll':'Game hosting', 'matchmaking.dll':'Matchmaking',
    'scenesystem.dll':'Scene objects', 'worldrenderer.dll':'World rendering',
    'scenefilecache.dll':'Scene cache', 'materialsystem2.dll':'Materials', 'meshsystem.dll':'Meshes',
    'rendersystemdx11.dll':'D3D11 backend', 'rendersystemvulkan.dll':'Vulkan backend',
    'rendersystemempty.dll':'Empty renderer', 'animationsystem.dll':'Animation',
    'particles.dll':'Particles', 'pulse_system.dll':'Pulse systems', 'vphysics2.dll':'Physics',
    'navsystem.dll':'Navigation', 'networksystem.dll':'Engine networking',
    'steamnetworkingsockets.dll':'Network transport', 'steam_api64.dll':'Steam integration',
    'panorama.dll':'Panorama UI', 'panoramauiclient.dll':'UI client',
    'panorama_text_pango.dll':'UI text', 'inputsystem.dll':'Input', 'SDL3.dll':'Windowing / input',
    'localize.dll':'Localization', 'imemanager.dll':'Input methods', 'soundsystem.dll':'Audio',
    'steamaudio.dll':'Spatial audio', 'vscript.dll':'Scripting', 'v8system.dll':'V8 embedding',
    'assetpreview.dll':'Asset tools', 'resourcecompiler.dll':'Resource compiler',
    'physicsbuilder.dll':'Physics tools', 'visbuilder.dll':'Visibility tools',
    'propertyeditor.dll':'Property editor', 'toolframework2.dll':'Tool framework',
    'vconsole2.exe':'Console tools', 'vconcomm.dll':'Console transport',
    'csgo_legacy_app.exe':'Legacy x86 launcher', 'dbghelp.dll':'Debug support', 'symsrv.dll':'Symbol support',
}
rows = []
for module in inventory['modules']:
    name = Path(module['path']).name
    role = roles.get(name)
    if not role:
        if name.startswith(('Qt5','qt')): role = 'Qt tools / plugins'
        elif name.startswith('v8'): role = 'V8 / ICU runtime'
        elif '/subtools/' in module['path']: role = 'Diagnostic subtool'
        elif name.startswith(('libav','libsw','libmpg','valve_','video')): role = 'Media / codecs'
        else: role = 'Bundled support library'
    identity = module['pdb'][0]['symbol_key'] if module['pdb'] else ''
    rows.append((name,role,int(module['machine'],16),identity))
rows.append(('gameoverlayrenderer64.dll','Steam overlay integration',0x8664,''))
assert len({row[0].lower() for row in rows}) == len(rows), 'Catalog names must be unique'
text = '// Generated from Steam build 26598758 inventory; compare PDB identity, not live compatibility.\n'
for name,role,machine,identity in sorted(rows,key=lambda row:row[0].lower()):
    text += '    {'+', '.join([json.dumps(name),json.dumps(role),hex(machine),json.dumps(identity)])+'},\n'
target = workspace/'manager/core/runtime_research_baseline.inc'
target.write_text(text,encoding='utf-8')
print(f'Generated {len(rows)} module identities: {target}')
