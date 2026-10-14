import pathlib,struct,re
b=pathlib.Path(r'C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Global Offensive/game/csgo/bin/win64/client.dll').read_bytes()
pe=struct.unpack_from('<I',b,60)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+opt+i*40) for i in range(n)]
def rva(o): return next(s[2]+o-s[4] for s in secs if s[4]<=o<s[4]+s[3])
def off(r): return next(s[4]+r-s[2] for s in secs if s[2]<=r<s[2]+s[3])
td=rva(b.index(b'.?AVClientModeCSNormal@@')-16)
for m in re.finditer(re.escape(struct.pack('<I',td)),b):
 o=m.start()-12
 if o<0: continue
 sig,offset,cd,t,cl,self=struct.unpack_from('<6I',b,o)
 if sig!=1 or self!=rva(o): continue
 print('COL',hex(rva(o)),offset)
 for v in re.finditer(re.escape(struct.pack('<Q',0x180000000+rva(o))),b):
  print('vtable',hex(rva(v.start()+8)))
  print([(i,hex(struct.unpack_from('<Q',b,v.start()+8+i*8)[0])) for i in range(10,21)])
