"""Prepare a bounded static agent preview from a local CS2 VPK using official VRF CLI."""
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct
import subprocess

def convert(source, budget=10000):
    data=source.read_bytes()
    magic,version,length=struct.unpack_from('<III',data)
    assert magic==0x46546c67 and version==2 and length==len(data)
    json_length,kind=struct.unpack_from('<II',data,12)
    assert kind==0x4e4f534a
    doc=json.loads(data[20:20+json_length])
    offset=20+json_length
    binary_length,kind=struct.unpack_from('<II',data,offset)
    assert kind==0x004e4942
    binary=data[offset+8:offset+8+binary_length]
    def accessor(index):
        a=doc['accessors'][index];v=doc['bufferViews'][a['bufferView']]
        assert v.get('buffer',0)==0 and 'sparse' not in a
        component={5121:'B',5123:'H',5125:'I',5126:'f'}[a['componentType']]
        count={'SCALAR':1,'VEC3':3}[a['type']]
        fmt='<'+component*count;size=struct.calcsize(fmt)
        start=v.get('byteOffset',0)+a.get('byteOffset',0);stride=v.get('byteStride',size)
        assert start+(a['count']-1)*stride+size<=len(binary)
        return [struct.unpack_from(fmt,binary,start+i*stride) for i in range(a['count'])]
    vertices=[];faces=[];groups=[];positions={}
    for mesh in doc['meshes']:
        name=mesh.get('name','')
        if not any(name.endswith('.'+part) for part in ('thirdperson_body','thirdperson_default_gloves')): continue
        groups.append(name)
        for primitive in mesh['primitives']:
            assert primitive.get('mode',4)==4
            index=primitive['attributes']['POSITION']
            if index not in positions:
                points=accessor(index);positions[index]=len(vertices)
                vertices.extend((x,-z,y) for x,y,z in points) # glTF Y-up to inspector Z-up.
            base=positions[index];indices=[x[0] for x in accessor(primitive['indices'])]
            assert len(indices)%3==0
            faces.extend(tuple(base+v for v in indices[i:i+3]) for i in range(0,len(indices),3))
    assert len(vertices)>1000 and len(faces)>1000,'Reject dummy/placeholder models'
    assert all(all(math.isfinite(v) for v in p) for p in vertices)
    assert all(0<=i<len(vertices) for f in faces for i in f)
    low=[min(p[i] for p in vertices) for i in range(3)]
    extent=max(max(p[i] for p in vertices)-low[i] for i in range(3))
    def cluster(resolution):
        buckets={};remap=[];sums=[];counts=[]
        for p in vertices:
            key=tuple(int((p[i]-low[i])/extent*resolution) for i in range(3))
            if key not in buckets: buckets[key]=len(sums);sums.append([0.,0.,0.]);counts.append(0)
            index=buckets[key];remap.append(index);counts[index]+=1
            for i in range(3): sums[index][i]+=p[i]
        points=[tuple(v/counts[n] for v in p) for n,p in enumerate(sums)]
        triangles=[];seen=set()
        for face in faces:
            f=tuple(remap[i] for i in face);key=tuple(sorted(f))
            if len(set(f))==3 and key not in seen: triangles.append(f);seen.add(key)
        return points,triangles
    # Offline reduction retains contiguous surfaces instead of skipping random
    # faces every frame. The original exported GLB remains available for review.
    left,right=4,512;best=None
    while left<=right:
        middle=(left+right)//2;result=cluster(middle)
        if len(result[1])<=budget: best=result;left=middle+1
        else: right=middle-1
    assert best and len(best[0])<=20000 and len(best[1])<=budget
    return best,dict(source_vertices=len(vertices),source_triangles=len(faces),mesh_groups=groups)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cli',type=Path,required=True)
    p.add_argument('--vpk',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    args=p.parse_args()
    args.output.mkdir(parents=True,exist_ok=True)
    resource='agents/models/ctm_st6/ctm_st6_variante.vmdl_c'
    glb=args.output/'seal_team_6.glb'
    subprocess.run([str(args.cli.resolve()),'-i',str(args.vpk.resolve()),'-f',resource,
                    '-o',str(glb.resolve()),'--gltf_export_format','glb'],check=True)
    (vertices,faces),metadata=convert(glb)
    obj=args.output/'seal_team_6.obj'
    obj.write_text('# Locally exported CS2 Seal Team 6, variant E. Static geometry preview.\n'+
        ''.join('v '+' '.join(f'{v:.7g}' for v in p)+'\n' for p in vertices)+
        ''.join('f '+' '.join(str(v+1) for v in f)+'\n' for f in faces),encoding='utf-8')
    metadata.update(resource=resource,vertices=len(vertices),triangles=len(faces),
                    obj_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),
                    mode='static bind-pose geometry; no game shaders, textures or animation')
    (args.output/'seal_team_6.json').write_text(json.dumps(metadata,indent=2)+'\n')
    print(json.dumps(metadata,indent=2))

if __name__=='__main__': main()
