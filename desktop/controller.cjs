'use strict';
const fs=require('node:fs');const path=require('node:path');const crypto=require('node:crypto');const {spawn}=require('node:child_process');
const {NativeTransport}=require('./native-transport.cjs');
const ORIGIN='https://wallet.veneralabs.org';
const STAKING='VENeRa1zWhh2AWLi78SUg2HApA6SF69cKRTNsf7YrKxZSmXMCe1mSZyT41SPV8u1h2KJEeSCB6B4iUzHSVeAqwEkYYummpduFj218';
const ROUTES={info:'/api/v2/info',challenge:'/api/v2/wallet/challenge',profile:'/api/v2/wallet/session',logout:'/api/v2/wallet/logout',burn:'/api/v2/burns/register',burn_stats:'/api/v2/burns/stats',stake:'/api/v2/stakes/register',stake_stats:'/api/v2/stakes/stats',details:'/api/v2/stakes/details',claim_burn_yield:'/api/v2/yield/claim',claim_status:'/api/v2/yield/status'};
const delay=ms=>new Promise(r=>setTimeout(r,ms));
const assert=(v,message)=>{if(!v)throw new Error(message);};
const decimalKey=value=>{let [whole,fraction='']=value.split('.');whole=whole.replace(/^0+(?=\d)/,'');fraction=fraction.replace(/0+$/,'');return whole+(fraction?'.'+fraction:'');};
class Controller {
 constructor({nativeDir,dataDir,dialog,fetchImpl=globalThis.fetch,native,spawnImpl=spawn}){
  this.nativeDir=nativeDir;this.dataDir=dataDir;this.dialog=dialog;this.fetch=fetchImpl;this.spawn=spawnImpl;this.native=native;this.accounts=null;this.balance=null;this.lastEvmUpdate=0;this.evm={};this.backendToken='';this.backendExpiry=0;this.busy=false;this.walletScanning=false;this.selections=new Map();this.prepared=null;this.cache={};this.chain={};this.operationQueue=Promise.resolve();
  fs.mkdirSync(dataDir,{recursive:true,mode:0o700});this.memoryStorage = true;this.state={operations:{},unknownTransfers:{}};
 }
 persist(){}
 binary(name){const p=path.join(this.nativeDir,name+(process.platform==='win32'?'.exe':''));assert(fs.existsSync(p)&&fs.lstatSync(p).isFile(),`Missing ${name}. Build the native executables before starting the wallet.`);return p;}
 async daemon(method,params={},endpoint='/json_rpc'){
  const body=endpoint==='/json_rpc'?{jsonrpc:'2.0',id:'desktop',method,params}:params;
  const response=await this.fetch('http://127.0.0.1:29930'+endpoint,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body),signal:AbortSignal.timeout(5000)});
  assert(response.ok,'Local daemon is not responding');const data=await response.json();assert(!data.error,data.error?.message||'Daemon RPC failed');return endpoint==='/json_rpc'?data.result:data;
 }
 async ensureDaemon(){
  try{const info=await this.daemon('get_info');assert(!info.testnet&&!info.stagenet,'Daemon must be on Venera mainnet');this.chain=info;return;}catch(e){if(/mainnet/.test(e.message))throw e;}
  const nullLog=process.platform==='win32'?'NUL':'/dev/null';
  this.daemonChild=this.spawn(this.binary('venerad'),['--data-dir',path.join(this.dataDir,'chain'),'--rpc-bind-ip','127.0.0.1','--rpc-bind-port','29930','--no-igd','--hide-my-port','--non-interactive','--log-file',nullLog,'--max-log-file-size','0'],{windowsHide:true,shell:false,stdio:'ignore'});
  let launchError;this.daemonChild.on('error',e=>{launchError=e;});
  for(let i=0;i<40;i++){if(launchError)throw launchError;await delay(500);try{this.chain=await this.daemon('get_info');return;}catch{}}
  throw new Error('Local daemon did not start; check node configuration and connectivity.');
 }
 async start(){if(this.started)return;await this.ensureDaemon();if(!this.native)this.native=new NativeTransport(this.binary('venera-local-cli'));await this.native.start();this.started=true;this.timer=setInterval(()=>{this.maintenance().catch(()=>{});},15000);this.timer.unref();}
 async maintenance(){try{this.chain=await this.daemon('get_info');}catch{}if(!this.accounts||this.busy)return;await this.refresh();await this.syncRewards();}
 async pickWallet(){const r=await this.dialog.showOpenDialog({title:'Open CLI-compatible Venera wallet',filters:[{name:'Venera wallet',extensions:['keys']}],properties:['openFile']});if(r.canceled)return null;const id=crypto.randomBytes(16).toString('hex');this.selections.set(id,r.filePaths[0]);return {id,name:path.basename(r.filePaths[0])};}
 async open(mode,p){
  assert(!this.busy,'Wait for the current wallet operation');await this.start();let file;
  if(mode==='open'){file=this.selections.get(p.selection);assert(file,'Choose a wallet file first');}
  else{const r=await this.dialog.showSaveDialog({title:mode==='create'?'Save new encrypted Venera wallet':'Save restored encrypted Venera wallet',defaultPath:'venera-wallet.keys',filters:[{name:'Venera wallet',extensions:['keys']}]});if(r.canceled)return null;file=r.filePath.endsWith('.keys')?r.filePath:r.filePath+'.keys';assert(!fs.existsSync(file),'Choose a new file name; existing wallets are never overwritten');}
  this.busy=true;try{const out=await this.native.call(mode,{file,password:p.password,seed:p.seed||''});this.accounts={vnr:out.vnr,evm:out.evm,file};await this.recoverTransfers();this.balance=null;this.lastEvmUpdate=0;this.evm={};this.backendToken='';this.backendExpiry=0;this.cache={};setImmediate(()=>this.refresh().catch(()=>{}));return out;}finally{this.busy=false;}
 }
 async refresh(force=false){
  if(!this.accounts||this.busy)return this.balance;
  const needScan=!this.balance||this.walletScanning||Number(this.balance.height)<Number(this.chain.height||0);
  const needEvm=force||!this.lastEvmUpdate||Date.now()-this.lastEvmUpdate>60000;
  if(!needScan&&!needEvm)return this.balance;
  this.busy=true;
  try{
   if(needScan){this.balance=await this.native.call('refresh_async');this.walletScanning=!!this.balance.scanning;}
   if(needEvm){try{this.evm=await this.native.call('evm_balance');this.evmError='';}catch(e){this.evmError=e.message;}this.lastEvmUpdate=Date.now();}
   this.scanError='';return this.balance;
  }catch(e){this.scanError=e.message;throw e;}finally{this.busy=false;}
 }
 async balances(){assert(this.accounts,'Wallet is locked');if(!this.busy){try{this.balance=await this.native.call('balance');this.walletScanning=!!this.balance.scanning;}catch{}}
  const a=this.accounts,b=this.balance||{};return {wallet_address:a.vnr,usdt_address:a.evm,vnr_balance:b.balance||'0',unlocked_balance:b.unlocked||'0',bnb_balance:this.evm.bnb||'0',usdt_balance:this.evm.usdt||'0',usdc_balance:this.evm.usdc||'0',scanning:this.busy||this.walletScanning,height:b.height||0,evm_error:this.evmError||'',scan_error:this.scanError||''};}
 async backend(url,data={},token=''){
  assert(url.startsWith('/api/v2/'),'Invalid backend route');const response=await this.fetch(ORIGIN+url,{method:'POST',redirect:'error',headers:{'Content-Type':'application/json',...(token?{Authorization:'Bearer '+token}:{})},body:JSON.stringify(data),signal:AbortSignal.timeout(45000)});const value=await response.json();if(!response.ok||value.error){const e=new Error(value.error||`Backend HTTP ${response.status}`);e.status=response.status;throw e;}return value;
 }
 async signed(action,extra={}){
  assert(this.accounts,'Wallet is locked');assert(!this.busy&&!this.walletScanning,'Wallet is scanning; try again when scanning completes');
  if(action==='claim_burn_yield'||action==='claim_status')await this.connect();
  const payload={audience:ORIGIN,action,wallet_address:this.accounts.vnr,txid:'',lock_days:0,erc20_address:'',request_id:'',...extra};
  const challenge=await this.backend(ROUTES.challenge,payload);assert(typeof challenge.challenge_id==='string'&&challenge.challenge_id.length<=4096,'Invalid backend challenge');
  const fields={...payload,nonce:challenge.nonce,expires_at:challenge.expires_at,message:challenge.message,staking_address:STAKING};const proof=await this.native.call('authorize_reward',fields);return this.backend(ROUTES[action],{challenge_id:challenge.challenge_id,...proof},action==='claim_burn_yield'||action==='claim_status'?this.backendToken:'');
 }
 async connect(){if(this.backendToken&&this.backendExpiry>Date.now()/1000+60)return;const response=await this.signed('profile',{erc20_address:this.accounts.evm.toLowerCase()});assert(response.wallet_address===this.accounts.vnr&&response.erc20_address.toLowerCase()===this.accounts.evm.toLowerCase(),'Backend account mismatch');this.backendToken=response.token;this.backendExpiry=response.expires_at;}
 async authBackend(route,data={}){await this.connect();try{return await this.backend(route,data,this.backendToken);}catch(e){if(e.status!==401)throw e;this.backendToken='';await this.connect();return this.backend(route,data,this.backendToken);}}
 opId(key){const wallet=this.accounts.vnr;const full=crypto.createHash('sha256').update(wallet+'\n'+key).digest('hex');if(!this.state.operations[full]){this.state.operations[full]=crypto.randomBytes(16).toString('hex');this.persist();}return [full,this.state.operations[full]];}
 finishOp(key){delete this.state.operations[key];this.persist();}
 async prepare(p){
  assert(this.accounts&&!this.busy,'Wait for the current local wallet request');if(String(p.asset||'vnr').toLowerCase()==='vnr')assert(!this.walletScanning,'Wallet must finish scanning before VNR spending');assert(!this.prepared,'Confirm or cancel the previous transfer');
  let asset=String(p.asset||'vnr').toLowerCase(),action=p.action||'send',amount=String(p.amount||''),recipient=String(p.recipient||'');assert(/^[0-9]+(\.[0-9]+)?$/.test(amount)&&amount.length<120,'Enter a positive decimal amount');
  if(recipient.startsWith('@'))recipient=(await this.authBackend('/api/v2/usernames/resolve',{username:recipient})).address;
  if(action==='stake'||action==='burn'){const info=await this.backend(ROUTES.info);assert(info.network==='mainnet','Wrong rewards network');if(action==='burn')assert(info.burn_policy,'Burn reward policy is unavailable');if(action==='stake'){const details=await this.backend(ROUTES.details);const mask=crypto.createHash('sha256').update('Venera staking details obfuscation v1:'+details.nonce).digest();const bytes=Buffer.from(details.payload,'hex');for(let i=0;i<bytes.length;i++)bytes[i]^=mask[i%32];const d=JSON.parse(bytes.toString('utf8'));assert(d.network==='mainnet'&&d.address===STAKING&&d.expires_at>=Date.now()/1000&&d.lock_days.includes(Number(p.lock_days)),'Invalid staking details');recipient=STAKING;}}
  if(asset==='vnr'){const info=await this.daemon('get_info');assert(!info.target_height||info.height>=info.target_height,'Local daemon is still synchronizing');}
  const key=this.accounts.vnr+':'+JSON.stringify({asset,action,amount:decimalKey(amount),recipient:asset==='vnr'?recipient:recipient.toLowerCase()});const previous=this.state.unknownTransfers[key];assert(!previous,'A previous broadcast is uncertain. Check history for '+previous?.tx_hash+' before sending again.');
  this.prepared=await this.native.call('prepare',{asset,action,amount,recipient,lock_days:Number(p.lock_days)||0});this.preparedKey=key;return this.prepared;
 }
 async receiptFor(record){if(!record.file||record.file!==this.accounts?.file||!/^([a-f0-9]{32})$/.test(record.id||''))return null;try{return (await this.native.call('desktop_receipt',{id:record.id})).receipt||null;}catch{return null;}}
 async recoverTransfers(){for(const [key,record] of Object.entries(this.state.unknownTransfers)){if(!key.startsWith(this.accounts.vnr+':'))continue;const receipt=await this.receiptFor(record);const hash=receipt?.tx_hash||receipt?.hashes?.[0];if(hash)record.tx_hash=hash;}}
 async confirm(p){assert(this.prepared&&p.id===this.prepared.id,'Unknown prepared transaction');await this.native.call('verify_password',{password:p.password});const key=this.preparedKey,record={id:p.id,file:this.accounts.file,asset:this.prepared.asset,tx_hash:'confirmation pending',status:'unknown'};this.state.unknownTransfers[key]=record;this.persist();let result;try{result=await this.native.call('confirm',{id:p.id});}catch(e){const receipt=await this.receiptFor(record);if(e.nativeError&&!receipt)delete this.state.unknownTransfers[key];else{const hash=receipt?.tx_hash||receipt?.hashes?.[0];if(hash)record.tx_hash=hash;}this.persist();throw e;}if(result.status==='unknown')this.state.unknownTransfers[key]={...record,...result};else delete this.state.unknownTransfers[key];this.persist();this.prepared=null;this.preparedKey=null;this.syncRewards().catch(()=>{});setImmediate(()=>this.refresh(true).catch(()=>{}));return result;}
 async syncRewards(){if(!this.accounts||this.busy||this.walletScanning)return;this.registrationError='';let queue;try{queue=await this.native.call('rewards_queue');}catch{return;}for(const item of queue.pending||[]){try{const r=await this.signed(item.action,{txid:item.txid,lock_days:item.lock_days||0});if(r.status==='registered')await this.native.call('ack_reward',{txid:item.txid});else this.registrationError='Waiting for transaction confirmations';}catch(e){this.registrationError=e.message;}}}
 async history(){assert(this.accounts,'Wallet is locked');await this.recoverTransfers();if(this.busy||this.walletScanning)return this.cache.history||[];const data=await this.native.call('history');const list=[];for(const key of ['in','out','pending','failed','pool'])for(const item of data[key]||[])list.push(item);list.sort((a,b)=>(b.timestamp||0)-(a.timestamp||0));let changed=false;for(const [key,value] of Object.entries(this.state.unknownTransfers)){if(list.some(tx=>(tx.txid||tx.tx_hash)===value.tx_hash&&Number(tx.height)>0&&['out','burn'].includes(tx.type))){delete this.state.unknownTransfers[key];changed=true;}}if(changed)this.persist();this.cache.history=list;return list;}
 async stats(action){if(this.busy&&this.cache[action])return this.cache[action];const response=await this.signed(action);this.cache[action]=response.data;return response.data;}
 async crate(type){const [key,rid]=this.opId('crate:'+type);const value=await this.authBackend('/api/v2/crates/open',{crate:type,request_id:rid});if(['paid','failed'].includes(value.status))this.finishOp(key);return value;}
 async mutation(pathname,data,key){const [keyId,rid]=this.opId(key);const value=await this.authBackend(pathname,{...data,request_id:rid});this.finishOp(keyId);return value;}
 async claimYield(){const [key,rid]=this.opId('yield');let value;try{value=await this.signed('claim_burn_yield',{erc20_address:this.accounts.evm.toLowerCase(),request_id:rid});}catch(e){throw e;}if(['paid','failed'].includes(value.status))this.finishOp(key);return value;}
 async lock(){const oldToken=this.backendToken;if(oldToken)this.backend(ROUTES.logout,{},oldToken).catch(()=>{});this.backendToken='';this.backendExpiry=0;this.accounts=null;this.walletScanning=false;this.prepared=null;this.balance=null;this.evm={};if(this.native)await this.native.call('lock');}
 async backup(p){assert(this.accounts,'Wallet is locked');await this.native.call('verify_password',{password:p.password});const r=await this.dialog.showSaveDialog({title:'Export CLI-compatible wallet backup',defaultPath:'venera-wallet-backup.keys',filters:[{name:'Venera wallet',extensions:['keys']}]});if(r.canceled)return {cancelled:true};const target=r.filePath.endsWith('.keys')?r.filePath:r.filePath+'.keys';assert(!fs.existsSync(target),'Choose a new backup filename');fs.copyFileSync(this.accounts.file,target,fs.constants.COPYFILE_EXCL);fs.chmodSync(target,0o600);return {saved:true};}
 async handle(op,p={}){
  if(op==='public.version')return require('./gui-version.cjs').fetchGuiVersion(this.fetch,ORIGIN);

  assert(typeof op==='string'&&p&&typeof p==='object'&&JSON.stringify(p).length<=131072,'Invalid desktop request');
  switch(op){
   case 'status':try{this.chain=await this.daemon('get_info');}catch{}return {unlocked:!!this.accounts,scanning:this.busy||this.walletScanning,accounts:this.accounts?{vnr:this.accounts.vnr,evm:this.accounts.evm}:null,chain:{height:this.chain.height||0,target:this.chain.target_height||this.chain.height||0},walletHeight:this.balance?.height||0,registrationError:this.registrationError||'',scanError:this.scanError||'',evmError:this.evmError||''};
   case 'wallet.pick':return this.pickWallet();case 'wallet.create':return this.open('create',p);case 'wallet.import':return this.open('import',p);case 'wallet.open':return this.open('open',p);case 'wallet.lock':return this.lock();case 'wallet.backup':return this.backup(p);
   case 'wallet.refresh':this.refresh(true).catch(()=>{});return {success:true};case 'wallet.balances':return this.balances();case 'wallet.history':return this.history();
   case 'wallet.exportEvm':assert(!this.busy,'Wait for scanning');return this.native.call('export_evm',{password:p.password});case 'wallet.exportSeed':assert(!this.busy&&!this.walletScanning,'Wait for scanning');return this.native.call('export_seed',{password:p.password});
   case 'transfer.prepare':return this.prepare(p);case 'transfer.confirm':return this.confirm(p);case 'transfer.cancel':this.prepared=null;this.preparedKey=null;return this.native.call('cancel');
   case 'backend.info':return this.backend(ROUTES.info);case 'wallet.receipt':{const result=await this.native.call('receipt',{hash:p.hash});if(result.receipt?.blockNumber){for(const [key,value] of Object.entries(this.state.unknownTransfers))if(value.tx_hash.toLowerCase()===p.hash.toLowerCase())delete this.state.unknownTransfers[key];this.persist();}return result;}
   case 'backend.crates':return this.authBackend('/api/v2/crates');case 'backend.crate':return this.crate(p.crate);case 'backend.wheel':return this.mutation('/api/v2/games/wheel/spin',{},'wheel');
   case 'backend.sweeperStart':return this.mutation('/api/v2/games/coinsweeper/start',{},'sweeper:start');case 'backend.sweeperAction':return this.mutation('/api/v2/games/coinsweeper/action',p,'sweeper:'+JSON.stringify(p));
   case 'backend.stakes':return this.stats('stake_stats');case 'backend.burns':return this.stats('burn_stats');case 'backend.claimYield':return this.claimYield();
   case 'backend.claimYieldStatus':{assert(this.accounts,'Wallet is locked');const key=crypto.createHash('sha256').update(this.accounts.vnr+'\nyield').digest('hex');const rid=this.state.operations[key];if(!rid)return {status:'idle'};const result=await this.signed('claim_status',{request_id:rid});if(['paid','failed'].includes(result.status))this.finishOp(key);return result;}
   case 'backend.settings':return this.authBackend('/api/v2/settings/get');case 'backend.setting':assert(['enhanced','totp_setup','totp_confirm','totp_verify','username'].includes(p.action),'Invalid setting');return this.authBackend('/api/v2/settings/'+p.action,p.data||{});
   case 'public.lookup':{assert(typeof p.query==='string'&&/^([a-fA-F0-9]{64}|[0-9]{1,12})$/.test(p.query),'Invalid explorer query');const r=await this.fetch('https://veneralabs.org/api/lookup_hash?q='+encodeURIComponent(p.query),{signal:AbortSignal.timeout(15000)});return r.json();}
   case 'public.network':{assert(['network_stats','tokeninfo'].includes(p.resource),'Invalid public resource');const r=await this.fetch('https://veneralabs.org/api/'+p.resource+(p.resource==='network_stats'?'?page='+Math.max(1,Math.min(100000,Number(p.page)||1)):''),{signal:AbortSignal.timeout(15000)});return r.json();}
   case 'public.mining':{if(!this.accounts)return {hashrate:0};const r=await this.fetch('https://pool.veneralabs.org/status',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({wallet_address:this.accounts.vnr}),signal:AbortSignal.timeout(15000)});return r.json();}
   default:throw new Error('Unsupported desktop operation');
  }
 }
 async close(){clearInterval(this.timer);try{await this.native?.close();}catch{}if(this.daemonChild){try{await this.daemon('',{},'/stop_daemon');}catch{}                                    }}
}
module.exports={Controller,ORIGIN,ROUTES,STAKING};
