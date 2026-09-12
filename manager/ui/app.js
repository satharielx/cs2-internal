'use strict';
const KNIVES = new Map([[500,'Bayonet'],[503,'Classic Knife'],[505,'Flip Knife'],[506,'Gut Knife'],[507,'Karambit'],[508,'M9 Bayonet'],[509,'Huntsman Knife'],[512,'Falchion Knife'],[514,'Bowie Knife'],[515,'Butterfly Knife'],[516,'Shadow Daggers'],[517,'Paracord Knife'],[518,'Survival Knife'],[519,'Ursus Knife'],[520,'Navaja Knife'],[521,'Nomad Knife'],[522,'Stiletto'],[523,'Talon Knife'],[525,'Skeleton Knife'],[526,'Kukri Knife']]);
const integer = (n, min, max) => Number.isInteger(n) && n >= min && n <= max;
function validateCatalog(data) {
    if (!data || data.version !== 1 || !Array.isArray(data.skins) || !Array.isArray(data.gloves) || !Array.isArray(data.loadout) || data.skins.length > 50000 || data.loadout.length > 128 || !KNIVES.has(data.knife) || !integer(data.glove,0,100000)) throw Error('This is not a supported catalog. Export a new catalog from the native menu.');
    for (const s of data.skins) if (!integer(s.id,1,65535) || !integer(s.paint,1,100000) || typeof s.name !== 'string' || typeof s.weapon !== 'string' || typeof s.stattrak !== 'boolean') throw Error('Catalog contains an invalid finish.');
    for (const g of data.gloves) if (!integer(g.paint,1,100000) || typeof g.name !== 'string') throw Error('Catalog contains an invalid glove.');
    if (data.glove && !data.gloves.some(g => g.paint === data.glove)) throw Error('Unknown glove in catalog.');
    const seen = new Set();
    for (const c of data.loadout) {
        const finish = data.skins.find(s => s.id === c.id && s.paint === c.paint);
        if (seen.has(c.id) || !integer(c.id,1,65535) || !integer(c.paint,0,100000) || (c.paint && !finish) || (!c.paint && !KNIVES.has(c.id) && !data.skins.some(s => s.id === c.id)) || !integer(c.seed,0,1000) || !Number.isFinite(c.wear) || c.wear < 0 || c.wear > 1 || typeof c.stattrak !== 'boolean' || (c.stattrak && !finish?.stattrak) || !integer(c.count,0,99999) || typeof c.name !== 'string' || new TextEncoder().encode(c.name).length > 31 || /[\x00-\x1f]/.test(c.name)) throw Error('Catalog contains an invalid loadout setting.');
        seen.add(c.id);
    }
    return data;
}
function serializeLoadout(data) {
    validateCatalog(data);
    return [`NEPHILIM_LOADOUT 1 ${data.knife} ${data.glove}`, ...data.loadout.map(c => {
        const hex = Array.from(new TextEncoder().encode(c.name), b => b.toString(16).padStart(2,'0')).join('') || '-';
        return `${c.id} ${c.paint} ${c.seed} ${c.wear.toFixed(4)} ${Number(c.stattrak)} ${c.count} ${hex}`;
    })].join('\n') + '\n';
}
if (typeof module !== 'undefined') module.exports = {validateCatalog,serializeLoadout};
if (typeof document !== 'undefined') {
    const $ = id => document.getElementById(id);
    let data = {version:1,knife:500,glove:0,skins:[],gloves:[],loadout:[]}, category = 'all', selected = null, loaded = false, dirty = false, toastTimer;
    const notify = message => { $('status').textContent = message; clearTimeout(toastTimer); toastTimer = setTimeout(() => $('status').textContent = '',6000); };
    const save = () => { try { localStorage.setItem('nephilim-studio-v1',JSON.stringify(data)); } catch { notify('Browser storage unavailable. Export your loadout to keep it.'); } };
    const configured = s => s.glove ? data.glove === s.paint : data.loadout.some(c => c.id === s.id && c.paint === s.paint);
    const items = () => {
        const unique = new Map(data.skins.map(s => [`${s.id}:${s.paint}`,s]));
        if (loaded) for (const [id,weapon] of KNIVES) unique.set(`${id}:0`,{id,weapon,paint:0,name:'Default finish',stattrak:false});
        return [...unique.values(),...data.gloves.map(g => ({...g,id:0,weapon:'Gloves',glove:true}))];
    };
    const element = (tag,text,className) => { const el = document.createElement(tag); el.textContent = text; if (className) el.className = className; return el; };
    function renderOptions() {
        const previous = $('weapon').value;
        $('weapon').replaceChildren(new Option('All equipment','all'));
        const weapons = new Map(items().filter(s => !s.glove && (category !== 'knives' || KNIVES.has(s.id)) && (category !== 'weapons' || !KNIVES.has(s.id))).map(s => [s.id,s.weapon]));
        for (const [id,name] of [...weapons].sort((a,b) => a[1].localeCompare(b[1]))) $('weapon').add(new Option(name,id));
        $('weapon').value = [...$('weapon').options].some(o => o.value === previous) ? previous : 'all';
        $('weapon').disabled = category === 'gloves';
    }
    function render() {
        const query = $('search').value.trim().toLowerCase();
        const list = items().filter(s => (category !== 'knives' || KNIVES.has(s.id)) && (category !== 'weapons' || (!s.glove && !KNIVES.has(s.id))) && (category !== 'gloves' || s.glove) && (category !== 'saved' || configured(s)) && (category === 'gloves' || $('weapon').value === 'all' || Number($('weapon').value) === s.id) && `${s.name} ${s.weapon} ${s.paint}`.toLowerCase().includes(query));
        const fragment = document.createDocumentFragment();
        for (const s of list) {
            const card = element('button','','card'); card.type = 'button';
            const active = selected && selected.id === s.id && selected.paint === s.paint && !!selected.glove === !!s.glove;
            card.classList.toggle('selected',!!active); card.setAttribute('aria-pressed',String(!!active));
            const swatch = element('div',s.glove ? '◇ ◇' : KNIVES.has(s.id) ? '╱' : '━','swatch'); swatch.style.setProperty('--hue',String((s.paint * 47 + s.id) % 360)); swatch.setAttribute('aria-hidden','true');
            const info = element('div','','card-info'); info.append(element('small',s.weapon),element('strong',s.name),element('small',configured(s) ? '✓ In your loadout' : `PAINTKIT ${s.paint}`,configured(s) ? 'badge' : ''));
            card.append(swatch,info); card.onclick = () => select(s); fragment.append(card);
        }
        $('cards').replaceChildren(fragment); $('count').textContent = `${list.length} finishes`;
        $('empty').hidden = list.length > 0; $('empty').querySelector('h3').textContent = loaded ? 'No finishes found.' : 'A fresh canvas.';
        $('empty').querySelector('p').textContent = loaded ? 'Try another search or equipment filter. Save an item to build your loadout.' : 'Open a catalog to explore weapon and knife paintkits.';
        $('welcome').hidden = loaded; $('export').disabled = !loaded;
        $('saved-count').textContent = `${data.loadout.length + Number(!!data.glove)} configured items`;
    }
    function select(s) {
        if (dirty && !confirm('Discard the unsaved changes to this item?')) return;
        dirty = false; selected = s;
        const c = data.loadout.find(c => c.id === s.id) || {seed:0,wear:0.01,stattrak:false,count:0,name:''};
        $('item-title').textContent = s.name; $('item-subtitle').textContent = `${s.weapon} · Paintkit ${s.paint}`;
        $('preview').textContent = s.glove ? '◇ ◇' : KNIVES.has(s.id) ? '╱' : '━';
        $('settings').hidden = false;
        for (const el of $('settings').querySelectorAll('label,.presets')) el.hidden = !!s.glove;
        $('paint').value = s.paint; $('seed').value = c.seed; $('wear').value = c.wear; $('wear-value').value = Number(c.wear).toFixed(4);
        $('stattrak').checked = !!(c.stattrak && s.stattrak); $('stattrak').disabled = !s.stattrak;
        $('kills').value = c.count; $('name').value = c.name;
        $('count-label').hidden = !!s.glove || !$('stattrak').checked;
        $('remove').disabled = s.glove ? !data.glove : !data.loadout.some(c => c.id === s.id);
        render();
    }
    $('catalog').onchange = async event => {
        const file = event.target.files[0]; if (!file) return;
        try {
            if (file.size > 20 * 1024 * 1024) throw Error('Catalog is too large.');
            const next = validateCatalog(JSON.parse(await file.text()));
            if (loaded && !confirm('Replace this browser draft with the loadout from the catalog?')) return;
            data = next; loaded = true; dirty = false; selected = null; $('settings').hidden = true;
            $('item-title').textContent = 'Choose your finish'; $('item-subtitle').textContent = 'Select an item to customize it.';
            renderOptions(); render(); save(); notify('Catalog opened. Your workspace is ready.');
        } catch (error) { notify(error.message); }
        finally { event.target.value = ''; }
    };
    for (const button of document.querySelectorAll('[data-category]')) button.onclick = () => {
        category = button.dataset.category;
        document.querySelectorAll('[data-category]').forEach(b => { b.classList.toggle('active',b === button); b.setAttribute('aria-pressed',String(b === button)); });
        $('collection-title').textContent = button.textContent.replace('↗','').trim(); $('weapon').value = 'all'; renderOptions(); render();
    };
    $('search').oninput = render; $('weapon').onchange = render;
    $('settings').oninput = () => { dirty = true; $('wear-value').value = Number($('wear').value).toFixed(4); $('count-label').hidden = !$('stattrak').checked; };
    document.querySelectorAll('[data-wear]').forEach(b => b.onclick = () => { $('wear').value = b.dataset.wear; $('wear').dispatchEvent(new Event('input',{bubbles:true})); });
    $('settings').onsubmit = event => {
        event.preventDefault(); if (!selected) return;
        const next = JSON.parse(JSON.stringify(data));
        if (selected.glove) next.glove = selected.paint;
        else {
            const c = {id:selected.id,paint:selected.paint,seed:Number($('seed').value),wear:Number($('wear').value),stattrak:$('stattrak').checked,count:Number($('kills').value),name:$('name').value};
            next.loadout = next.loadout.filter(item => item.id !== c.id); next.loadout.push(c);
            if (KNIVES.has(c.id)) next.knife = c.id;
        }
        try { validateCatalog(next); data = next; dirty = false; save(); select(selected); notify('Saved to your loadout. Export to use it in the native menu.'); }
        catch { notify('Check your settings. Name tags must fit in 31 UTF-8 bytes; seed is 0–1000 and kills 0–99999.'); }
    };
    $('remove').onclick = () => { if (!selected) return; if (selected.glove) data.glove = 0; else data.loadout = data.loadout.filter(c => c.id !== selected.id); dirty = false; save(); select(selected); notify('Item removed from the draft.'); };
    $('export').onclick = () => {
        if (dirty) { notify('Save your item changes before exporting.'); return; }
        try {
            const url = URL.createObjectURL(new Blob([serializeLoadout(data)],{type:'text/plain;charset=utf-8'}));
            const link = document.createElement('a'); link.href = url; link.download = 'nephilim.loadout'; link.click(); setTimeout(() => URL.revokeObjectURL(url),1000);
            notify('Loadout exported. Choose Import Loadout in the native menu.');
        } catch(error) { notify(error.message); }
    };
    window.addEventListener('beforeunload',e => { if (dirty) { e.preventDefault(); e.returnValue = ''; } });
    try { const stored = localStorage.getItem('nephilim-studio-v1'); if (stored) { data = validateCatalog(JSON.parse(stored)); loaded = true; } } catch { notify('Saved draft could not be restored. Open a catalog to continue.'); }
    renderOptions(); render();
}
