const {test} = require('node:test');
const assert = require('node:assert/strict');
const {validateCatalog,serializeLoadout} = require('../manager/ui/app.js');
const catalog = () => ({version:1,knife:507,glove:0,gloves:[],skins:[{id:507,paint:38,name:'Fade',weapon:'Karambit',stattrak:true}],loadout:[{id:507,paint:38,seed:321,wear:0.08,stattrak:true,count:42,name:'My knife'}]});
test('knife settings serialize to native format',() => assert.equal(serializeLoadout(catalog()),'NEPHILIM_LOADOUT 1 507 0\n507 38 321 0.0800 1 42 4d79206b6e696665\n'));
test('reject incompatible knife paintkits',() => {const c=catalog(); c.loadout[0].id=500; assert.throws(() => validateCatalog(c));});
test('reject duplicate items and invalid ranges',() => {
    for (const [key,value] of [['seed',1001],['wear',NaN],['wear',1.1],['count',-1],['name','é'.repeat(16)]]) {const c=catalog(); c.loadout[0][key]=value; assert.throws(() => validateCatalog(c));}
    const c=catalog(); c.loadout.push({...c.loadout[0]}); assert.throws(() => validateCatalog(c));
});
test('default knife finish and empty name round trip format',() => {const c=catalog(); Object.assign(c.loadout[0],{paint:0,stattrak:false,name:''}); assert.match(serializeLoadout(c),/507 0 321 0.0800 0 42 -\n$/);});
test('reject unsupported StatTrak and unknown gloves',() => {let c=catalog(); c.skins[0].stattrak=false; assert.throws(() => validateCatalog(c)); c=catalog(); c.glove=12345; assert.throws(() => validateCatalog(c));});
