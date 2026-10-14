import json,pathlib
rows=json.loads(pathlib.Path('build/skin-model-catalog.json').read_text())
mapping={}
for r in rows:
 if r.get('paint_index') is None: continue
 w=int(r['weapon']['weapon_id']); p=int(r['paint_index']); legacy=r.get('legacy_model')
 if w>=5027 or not isinstance(legacy,bool): continue
 key=(w,p); mask=1 if legacy else 2
 if key in mapping and mapping[key]!=mask: raise ValueError(key)
 mapping[key]=mask
lines=['#pragma once','#include <cstdint>','#include <algorithm>','// Generated 2026-10-07 from ByMykel/CSGO-API public/api/en/skins.json.','// Lookup is per weapon and paint kit; 0 means metadata is unavailable.','namespace skin_models {','struct Entry { int weapon; int paint; uint64_t mask; };','inline constexpr Entry entries[] = {']
lines += [f'    {{{w}, {p}, {m}}},' for (w,p),m in sorted(mapping.items())]
lines += ['};','inline uint64_t Mask(int weapon, int paint) {','    if (paint == 0) return 2;','    const auto it = std::lower_bound(std::begin(entries), std::end(entries), Entry{weapon, paint, 0},','        [](const Entry& a, const Entry& b) { return a.weapon < b.weapon || (a.weapon == b.weapon && a.paint < b.paint); });','    return it != std::end(entries) && it->weapon == weapon && it->paint == paint ? it->mask : 0;','}','}']
pathlib.Path('manager/core/skin_models.h').write_text('\n'.join(lines)+'\n')
print('Mapped',len(mapping),'weapon/paint pairs; legacy',sum(v==1 for v in mapping.values()))
print([(r['name'],r['weapon']['weapon_id'],r['paint_index'],r['legacy_model']) for r in rows if r['name'] in ['AK-47 | Redline','AK-47 | Slate']])
