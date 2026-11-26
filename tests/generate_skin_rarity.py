"""Extract rarity metadata from the installed game's VPK; never load game code."""
import argparse
from pathlib import Path
import re
import struct

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('vpk',type=Path)
p.add_argument('--output',type=Path,required=True)
args=p.parse_args()
data=args.vpk.read_bytes()
magic,version,size=struct.unpack_from('<III',data)
assert magic==0x55aa1234 and version in (1,2)
header=28 if version==2 else 12
pos=header
def string():
    global pos
    end=data.index(b'\0',pos);value=data[pos:end].decode();pos=end+1;return value
content=None
while (extension:=string()):
    while (folder:=string()):
        while (name:=string()):
            crc,preload,archive,offset,length,end=struct.unpack_from('<IHHIIH',data,pos);pos+=18
            assert end==0xffff
            prefix=data[pos:pos+preload];pos+=preload
            if f'{folder}/{name}.{extension}'=='scripts/items/items_game.txt':
                if archive==0x7fff: payload=data[header+size+offset:header+size+offset+length]
                else:
                    archive_path=args.vpk.with_name(args.vpk.name.replace('_dir.vpk',f'_{archive:03d}.vpk'))
                    with archive_path.open('rb') as source: source.seek(offset);payload=source.read(length)
                content=(prefix+payload).decode('utf-8-sig')
assert content is not None,'items_game.txt missing'
tokens=iter(re.findall(r'"((?:\\.|[^"\\])*)"|([{}])',re.sub(r'//[^\r\n]*','',content)))
def block():
    result={}
    for quoted,brace in tokens:
        if brace=='}': return result
        key=quoted
        value,opening=next(tokens)
        parsed=block() if opening=='{' else value
        if isinstance(parsed,dict) and isinstance(result.get(key),dict): result[key].update(parsed)
        else: result[key]=parsed
    return result
root=block()['items_game']
rarities={name:int(value['value']) for name,value in root['rarities'].items()}
paint_names=root['paint_kits_rarity']
rows=[]
for identifier,kit in root['paint_kits'].items():
    if not identifier.isdigit(): continue
    name=kit.get('name','');rarity=rarities.get(paint_names.get(name,''))
    if rarity is not None: rows.append((int(identifier),rarity))
rows.sort()
def item_properties(item,depth=0):
    assert depth<32
    result={}
    for prefab in item.get('prefab','').split(): result.update(item_properties(root['prefabs'][prefab],depth+1))
    result.update(item)
    return result
base_rows=[]
for identifier,item in root['items'].items():
    if not identifier.isdigit(): continue
    definition=int(identifier)
    if not (0<definition<=1000 or 5027<=definition<=5035): continue
    rarity=rarities.get(item_properties(item).get('item_rarity','default'),0)
    base_rows.append((definition,rarity))
base_rows.sort()
args.output.write_text('// Generated from installed scripts/items/items_game.txt; build 26598758.\n'
    +'// Native rarity values (1 consumer .. 6 covert, 7 contraband).\n'
    +'inline constexpr RarityEntry paint_rarities[]{\n'
    +''.join(f'    {{{identifier}, {rarity}}},\n' for identifier,rarity in rows)+'};\n'
    +'inline constexpr RarityEntry item_rarities[]{\n'
    +''.join(f'    {{{identifier}, {rarity}}},\n' for identifier,rarity in base_rows)+'};\n')
print('rarities:',rarities,'paint kits:',len(rows),'examples:',[row for row in rows if row[0] in (282,309,344,803,962,37)])
