(() => {
 'use strict';
 const embeddedExplorer = !window.veneraAPI && window.parent !== window && location.pathname.endsWith('/explorer.html');
 if(!window.veneraAPI && !embeddedExplorer)return;
                                                                              
 const publicOperations = new Set(['public.lookup','public.network']);
 const pendingPublic = new Map();let publicSequence=0;
 window.addEventListener('message',async event=>{
  const data=event.data;
  if(!data || typeof data!=='object')return;
  if(embeddedExplorer && event.source===window.parent && data.type==='venera-public-result'){
   const pending=pendingPublic.get(data.id);if(!pending)return;
   clearTimeout(pending.timer);pendingPublic.delete(data.id);
   if(data.ok)pending.resolve(data.value);else pending.reject(new Error(data.error||'Public lookup failed'));
  }else if(window.veneraAPI && data.type==='venera-public-request'){
   const frame=Array.from(document.querySelectorAll('iframe')).find(item=>item.contentWindow===event.source);
   if(!frame)return;const url=new URL(frame.src,location.href);
   if(url.protocol!==location.protocol || url.host!==location.host || !url.pathname.endsWith('/explorer.html'))return;
   if(typeof data.id!=='string' || data.id.length>80 || !publicOperations.has(data.operation))return;
   try{const value=await window.veneraAPI.call(data.operation,data.params||{});event.source.postMessage({type:'venera-public-result',id:data.id,ok:true,value},'*');}
   catch(error){event.source.postMessage({type:'venera-public-result',id:data.id,ok:false,error:error.message},'*');}
  }
 });
 const call=(operation,p={})=>{
  if(window.veneraAPI)return window.veneraAPI.call(operation,p);
  if(!publicOperations.has(operation))return Promise.reject(new Error('Only public explorer operations are available in this frame'));
  return new Promise((resolve,reject)=>{const id='explorer-'+(++publicSequence);const timer=setTimeout(()=>{pendingPublic.delete(id);reject(new Error('Public explorer request timed out'));},20000);pendingPublic.set(id,{resolve,reject,timer});window.parent.postMessage({type:'venera-public-request',id,operation,params:p},'*');});
 };
                                                                         
 if(window.veneraAPI)localStorage.setItem('jwt_token','native-local-session');
 const originalFetch=window.fetch.bind(window);
 window.nativeDialog=async function(title,description,{password=false,code=false,textInput=false,confirm='Continue',size='normal'}={}){
  const dialog=document.createElement('dialog');dialog.className='native-dialog'+(size==='large'?' native-dialog-large':'');
  dialog.setAttribute('aria-label',title);
  const form=document.createElement('form');form.method='dialog';const heading=document.createElement('h2');heading.textContent=title;const text=document.createElement('p');text.textContent=description;
  const input=document.createElement('input');input.type=password?'password':'text';input.autocomplete=password?'current-password':'one-time-code';input.placeholder=password?'Wallet password':'6-digit code';input.required=password||code||textInput;if(textInput)input.placeholder='0x transaction hash';if(code){input.pattern='[0-9]{6}';input.inputMode='numeric';input.maxLength=6;}
  const buttons=document.createElement('div');buttons.className='native-dialog-actions';const cancel=document.createElement('button');cancel.textContent='Cancel';cancel.type='button';const ok=document.createElement('button');ok.textContent=confirm;ok.className='native-primary';ok.type='submit';buttons.append(cancel,ok);form.append(heading,text);if(password||code||textInput)form.append(input);form.append(buttons);dialog.append(form);document.body.append(dialog);
  return new Promise(resolve=>{let done=false;const finish=value=>{if(done)return;done=true;input.value='';dialog.classList.add('is-closing');const duration=window.matchMedia?.('(prefers-reduced-motion: reduce)')?.matches?0:220;setTimeout(()=>{dialog.close();dialog.remove();resolve(value);},duration);};cancel.onclick=()=>finish(null);dialog.oncancel=e=>{e.preventDefault();finish(null);};form.onsubmit=e=>{e.preventDefault();if(form.reportValidity())finish(password||code||textInput?input.value:true);};dialog.showModal();if(password||code||textInput)input.focus();});
 };
 async function transfer(action,asset,body){
  const prepared=await call('transfer.prepare',{action,asset,recipient:body.sendaddress||body.recipient||'',amount:String(body.sendamount??body.amount??''),lock_days:Number(body.lock)||0});
  const details=`${action==='burn'?'Irreversibly burn':action==='stake'?'Stake':'Send'} ${prepared.amount} ${prepared.asset.toUpperCase()}${prepared.recipient?'\nTo: '+prepared.recipient:''}\nFee: ${prepared.fee} ${prepared.fee_asset||'VNR'}\n${asset==='vnr'?'Venera mainnet':'BNB Smart Chain · chain 56'}`;
  const password=await window.nativeDialog('Confirm transaction',details,{password:true,confirm:'Sign and broadcast'});
  if(password===null){await call('transfer.cancel');throw new Error('Transaction cancelled; nothing broadcast');}
  let result;try{result=await call('transfer.confirm',{id:prepared.id,password});}catch(e){await call('transfer.cancel').catch(()=>{});throw e;}
  if(result.status==='unknown')throw new Error('Broadcast result is uncertain. Check history before sending again. Transaction: '+result.tx_hash);
  return {...result,success:true};
 }
 function parseBody(options){if(!options?.body)return {};try{return JSON.parse(options.body);}catch{throw new Error('Invalid request body');}}
 async function route(url,options){
  const body=parseBody(options),p=url.pathname;
  if(url.hostname==='pool.veneralabs.org'&&p==='/status')return call('public.mining');
  if(url.hostname==='veneralabs.org'){
   if(p==='/api/lookup_hash')return call('public.lookup',{query:url.searchParams.get('q')||''});
   if(p==='/api/network_stats'||p==='/api/tokeninfo')return call('public.network',{resource:p.split('/').pop(),page:url.searchParams.get('page')});
   throw new Error('Unsupported public resource');
  }
  switch(p){
   case '/totalbalanced':return call('wallet.balances');case '/drefresh':case '/dcore':return call('wallet.refresh');case '/dhistory':return call('wallet.history');case '/dinvalidate':await call('wallet.lock');localStorage.removeItem('jwt_token');return {success:true};
   case '/dtransfer':return transfer('send','vnr',body);case '/dsendusdt':return transfer('send','usdt',body);case '/dsendusdc':return transfer('send','usdc',body);case '/dsendbnb':return transfer('send','bnb',body);
   case '/dtransfer/burn':return transfer('burn','vnr',body);case '/dstaked':return transfer('stake','vnr',body);
   case '/dstakes':{const d=await call('backend.stakes');return {...d,stakes:(d.stakes||[]).map(s=>({...s,timestamp_x:s.timestamp_in,timestamp_y:s.timestamp_out,amount:s.amount_s}))};}
   case '/dtransfers/burn_stats':{const d=await call('backend.burns');let units=0n;for(const x of d.burns||[])units+=BigInt(x.amount_burned_atomic||'0');return {...d,total_burned:(units/1000000000n).toString()+'.'+(units%1000000000n).toString().padStart(9,'0'),total_burned_atomic:units.toString(),burn_transactions:(d.burns||[]).length,current_points_per_vnr:(await call('backend.info')).burn_policy?.point_rate||0};}
   case '/dtransfer/collect_yield':{const d=await call('backend.claimYield');return {...d,success:d.status==='paid'};}
   case '/dtransfer/collect_yield_status':{const d=await call('backend.claimYieldStatus');return {...d,success:d.status==='paid'};}
   case '/dgold':return call('backend.crates');case '/dcrate':case '/dplatinum':case '/dtether':{const d=await call('backend.crate',{crate:p==='/dcrate'?'basic':p==='/dplatinum'?'platinum':'tether'});if(d.status!=='paid')throw new Error('Reward payment '+d.status+'. Retry checks the same request. Transaction: '+d.tx_hash);return d;}
   case '/drewards/me':return call('backend.crates');case '/drewards/spin':return call('backend.wheel');case '/drewards/coinsweeper':return call('backend.sweeperStart');case '/drewards/sweep':return call('backend.sweeperAction',body);
   case '/deha':case '/d2fa/validate':case '/2fa/dvalidate':return call('backend.settings');case '/dehc':case '/ehc':return call('backend.setting',{action:'enhanced',data:{enabled:true}});case '/deho':case '/eho':return call('backend.setting',{action:'enhanced',data:{enabled:false}});
   case '/d2fa/setup':return call('backend.setting',{action:'totp_setup'});case '/d2fa/confirmsetup':return call('backend.setting',{action:'totp_confirm',data:body});case '/d2fa/verify':case '/2fa/verifyd':return call('backend.setting',{action:'totp_verify',data:body});case '/dreguser':return call('backend.setting',{action:'username',data:body});
   case '/dimport/trusti':return {imported:false};case '/dimport/trustc':{const password=await window.nativeDialog('Export BSC private key','This QR code controls your local BNB, USDT and USDC account. Show it only to a trusted wallet.',{password:true,confirm:'Reveal key'});if(password===null)throw new Error('Export cancelled');const d=await call('wallet.exportEvm',{password});return {trust:d.private_key};}
   case '/checkupdate':return call('public.version');
   default:throw new Error('Unsupported wallet action: '+p);
  }
 }
 window.fetch=async function(input,options={}){
  const url=new URL(typeof input==='string'?input:input.url,location.href);
  const host=['wallet.veneralabs.org','veneralabs.org','pool.veneralabs.org'].includes(url.hostname);
  const localOld=url.protocol===location.protocol&&url.hostname===location.hostname&&['/ehc','/eho'].includes(url.pathname);
  if(!host&&!localOld)return originalFetch(input,options);
  try{const target=localOld?new URL('https://wallet.veneralabs.org'+url.pathname):url;let value;try{value=await route(target,options);}catch(e){if(e.message!=='twofa_required')throw e;const code=await nativeDialog('Verify backend operation','Enter your authenticator code.',{code:true,confirm:'Verify'});if(code===null)throw new Error('Verification cancelled');await call('backend.setting',{action:'totp_verify',data:{code}});value=await route(target,options);}return new Response(JSON.stringify(value??{}),{status:200,headers:{'Content-Type':'application/json'}});}catch(error){return new Response(JSON.stringify({error:error.message||'Operation failed'}),{status:400,headers:{'Content-Type':'application/json'}});}
 };
})();

window.addEventListener('venera:ready',()=>{for(const name of ['loadAvailableCrates','loadSpins','loadStakes','updateBalances','loadHistory'])if(typeof window[name]==='function')window[name]();});
