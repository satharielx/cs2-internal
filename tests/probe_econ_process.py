"""Read-only econ layout probe. No remote calls, allocation, writes, or injection."""
import argparse
import ctypes as c
from ctypes import wintypes as w
import json
import struct
from pathlib import Path

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('pid',type=int)
p.add_argument('--output',type=Path,required=True)
args=p.parse_args()
k=c.WinDLL('kernel32',use_last_error=True)
k.OpenProcess.argtypes=[w.DWORD,w.BOOL,w.DWORD];k.OpenProcess.restype=w.HANDLE
k.CloseHandle.argtypes=[w.HANDLE]
k.ReadProcessMemory.argtypes=[w.HANDLE,c.c_void_p,c.c_void_p,c.c_size_t,c.POINTER(c.c_size_t)]
k.K32EnumProcessModulesEx.argtypes=[w.HANDLE,c.POINTER(w.HMODULE),w.DWORD,c.POINTER(w.DWORD),w.DWORD]
k.K32GetModuleFileNameExW.argtypes=[w.HANDLE,w.HMODULE,w.LPWSTR,w.DWORD]
handle=k.OpenProcess(0x410,False,args.pid)
if not handle: raise c.WinError(c.get_last_error())
try:
    def read(address,size):
        buf=c.create_string_buffer(size);actual=c.c_size_t()
        if not k.ReadProcessMemory(handle,address,buf,size,c.byref(actual)) or actual.value!=size:
            raise OSError(f'Unreadable metadata at {address:#x}')
        return buf.raw
    def value(address,fmt='Q'): return struct.unpack('<'+fmt,read(address,struct.calcsize(fmt)))[0]
    modules=(w.HMODULE*2048)();needed=w.DWORD()
    if not k.K32EnumProcessModulesEx(handle,modules,c.sizeof(modules),c.byref(needed),3): raise c.WinError(c.get_last_error())
    client=0
    for module in modules[:min(len(modules),needed.value//c.sizeof(w.HMODULE))]:
        name=c.create_unicode_buffer(32768)
        if k.K32GetModuleFileNameExW(handle,module,name,len(name)) and Path(name.value).name.lower()=='client.dll': client=int(module)
    if not client: raise RuntimeError('client.dll is not loaded')
    manager=client+0x24c6f40
    inventory=value(manager+0x528d0)
    cache=value(inventory+0x68) if inventory else 0
    owner=read(inventory+0x10,16) if inventory else bytes(16)
    result=dict(client_base=hex(client),manager_vtable_rva=hex(value(manager)-client),
        inventory_present=bool(inventory),inventory_vtable_rva=hex(value(inventory)-client) if inventory else None,
        owner_nonzero=bool(int.from_bytes(owner[:8],'little')),owner_type=int.from_bytes(owner[8:12],'little'),
        cache_present=bool(cache),cache_vtable_rva=hex(value(cache)-client) if cache else None,
        inventory_vector_words=read(inventory+0x20,24).hex(' ') if inventory else None)
    if inventory:
        count=value(inventory+0x20,'I');data=value(inventory+0x28)
        result['view_count']=count;result['view_samples']=[];view_ids=[]
        if count<=100000 and data:
            for n in range(count):
                view=value(data+n*8)
                if not view: continue
                identity=value(view+0x1c8);view_ids.append(identity)
                if n<3 or value(view+0x1ba,'H')==7:
                    result['view_samples'].append(dict(vtable_rva=hex(value(view)-client),definition=value(view+0x1ba,'H'),
                        id_high_nibble=identity>>60,position=value(view+0x1dc,'I')))
        result['default_marker_view_ids']=sum(bool(i>>60==15) for i in view_ids)
        result['maximum_view_id_high_nibble']=max(view_ids,default=0)>>60
    if cache:
        count=value(cache+0x10,'I');data=value(cache+0x18)
        result['type_cache_count']=count
        result['type_caches']=[]
        if count<=64 and data:
            for n in range(count):
                pointer=value(data+n*8)
                try:
                    row=dict(vtable_rva=hex(value(pointer)-client),count=value(pointer+8,'I'),
                        type_candidate=value(pointer+0x20,'I'))
                    objects=value(pointer+0x10)
                    row['econ_samples']=[]
                    if row['count']<=100000 and objects:
                        for j in range(min(row['count'],8)):
                            item=value(objects+j*8)
                            if value(item)-client!=0x1d07d00: continue
                            blob=value(item+0x20)
                            sample=dict(definition=value(item+0x30,'H'),attribute_blob_present=bool(blob))
                            if blob:
                                total=value(blob+2,'B');sample['attribute_count']=total;sample['attributes']=[]
                                for a in range(min(total,12)):
                                    index=value(blob+4+a*16,'H');bits=value(blob+12+a*16,'I')
                                    sample['attributes'].append(dict(index=index,bits=hex(bits),float_value=struct.unpack('<f',struct.pack('<I',bits))[0]))
                            row['econ_samples'].append(sample)
                    result['type_caches'].append(row)
                except OSError: result['type_caches'].append(dict(unreadable=True))
    getter=client+0x1189b30
    code=read(getter,11)
    if code[:7]==bytes.fromhex('4883ec28488b05'):
        system=value(getter+11+struct.unpack_from('<i',code,7)[0])
        schema=value(system+8) if system else 0
        result['schema_present']=bool(schema);result['attribute_types']=[]
        if schema:
            kit_count=value(schema+0x2f0,'I');kit_nodes=value(schema+0x2f8)
            result['paint_kit_rarity_samples']=[]
            if 0<kit_count<=65536 and kit_nodes:
                for i in range(kit_count):
                    kit=value(kit_nodes+i*0x20+0x18)
                    if kit and value(kit,'I') in [282,344,309,803,962,37]:
                        result['paint_kit_rarity_samples'].append(dict(id=value(kit,'I'),words_60_80=read(kit+0x60,0x20).hex(' ')))
            count=value(schema+0x150,'I');definitions=value(schema+0x158)
            for index in [6,7,8,80,81,111]:
                if index>=count: continue
                definition=value(definitions+index*8);attribute_type=value(definition+0x18)
                table=value(attribute_type)
                result['attribute_types'].append(dict(index=index,type_vtable_rva=hex(table-client),
                    set_from_string_rva=hex(value(table+0x30)-client)))
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
finally: k.CloseHandle(handle)
