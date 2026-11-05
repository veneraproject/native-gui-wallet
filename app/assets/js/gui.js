                                                                                
const content=document.getElementById('panel-content');
const createBtn=document.getElementById('createBtn');
const restoreBtn=document.getElementById('restoreBtn');
let selection=null;
const loginUI = `
<div class="entry-restore-form">
    <div class="entry-form-heading"><h2>Restore your wallet</h2><p>Enter your seed phrase or import an encrypted wallet.</p></div>
    <div id="seedRestorePanel"><label class="entry-label" for="restoreInput">25-word seed phrase</label><textarea id="restoreInput" class="entry-input entry-seed" placeholder="Enter your seed phrase…" autocomplete="off" spellcheck="false"></textarea></div>
    <input id="walletFileInput" type="file" accept=".keys,application/octet-stream" hidden/>
    <div id="fileRestorePanel" hidden class="entry-file-card"><span class="material-symbols-outlined">lock</span><div><span class="entry-label">Encrypted wallet selected</span><p id="walletFileName"></p></div><button id="clearWalletFileBtn" type="button" aria-label="Remove wallet file" class="entry-icon-button"><span class="material-symbols-outlined">close</span></button></div>
    <button id="chooseWalletFileBtn" type="button" class="entry-import"><span class="material-symbols-outlined">upload_file</span><span id="importWalletLabel">Import wallet file</span><span class="entry-file-extension">.keys</span></button>
    <p id="restoreFileStatus" class="hidden entry-status" role="status"></p>
    <button id="restoreSubmit" type="button" class="entry-primary">Restore Wallet <span class="material-symbols-outlined">arrow_forward</span></button>
</div>
`;
const createUI = `
<div class="entry-restore-form">
    <div class="entry-form-heading"><h2>Create your wallet</h2><p>Generate a new seed phrase and keep a secure backup.</p></div>
    <div><label class="entry-label" for="seedOutput">Generated seed phrase</label><div class="entry-seed-wrap"><textarea id="seedOutput" readonly class="entry-input entry-seed" placeholder="Generate your seed phrase…"></textarea><div id="actionSlot" class="entry-copy-slot"><div id="loader" class="hidden entry-mini-spinner"></div><button id="copyIcon" type="button" class="material-symbols-outlined hidden" aria-label="Copy seed phrase">content_copy</button><span id="checkIcon" class="material-symbols-outlined hidden">check</span></div></div></div>
    <button id="generateBtn" type="button" class="entry-primary">Generate Seed Phrase <span class="material-symbols-outlined">auto_awesome</span></button>
    <div id="encryptedSaveBox" class="hidden"><button id="saveEncryptedWalletBtn" type="button" class="entry-import"><span class="material-symbols-outlined">encrypted</span>Save encrypted wallet file<span class="entry-file-extension">.keys</span></button></div>
    <button id="enterCreatedWallet" type="button" class="entry-primary" hidden>Open wallet</button><p id="saveWalletStatus" class="hidden entry-status" role="status"></p>
</div>
`;
function message(id,value,error=false){const node=document.getElementById(id);if(!node)return;node.textContent=value;node.classList.remove('hidden');node.classList.toggle('text-red-300',error);node.classList.toggle('text-green-300',!error);}
async function newPassword(){const one=await nativeDialog('Encrypt local wallet','Choose a wallet password. Use printable ASCII characters for CLI compatibility.',{password:true,confirm:'Continue'});if(one===null)return null;const two=await nativeDialog('Confirm wallet password','Enter the same password again.',{password:true,confirm:'Create encrypted wallet'});if(two===null)return null;if(one!==two)throw new Error('Passwords do not match');if(!/^[\x20-\x7e]+$/.test(one))throw new Error('Use printable ASCII characters for CLI compatibility');return one;}
function attachEvents(){
 const generate=document.getElementById('generateBtn');
 if(generate){generate.onclick=async()=>{if(generate.disabled)return;generate.disabled=true;const loader=document.getElementById('loader');loader.classList.remove('hidden');try{const password=await newPassword();if(password===null)return;message('saveWalletStatus','Starting local daemon and wallet service…');const result=await veneraAPI.call('wallet.create',{password});if(!result)return;document.getElementById('seedOutput').value=result.seed;document.getElementById('copyIcon').classList.remove('hidden');document.getElementById('encryptedSaveBox').classList.remove('hidden');document.getElementById('enterCreatedWallet').hidden=false;message('saveWalletStatus','Encrypted .keys wallet saved. Back up the file, password and seed.');}catch(e){message('saveWalletStatus',e.message,true);}finally{generate.disabled=false;loader.classList.add('hidden');}};
  document.getElementById('copyIcon').onclick=async()=>{await navigator.clipboard.writeText(document.getElementById('seedOutput').value);document.getElementById('checkIcon').classList.remove('hidden');setTimeout(()=>document.getElementById('checkIcon').classList.add('hidden'),1500);};
  document.getElementById('saveEncryptedWalletBtn').onclick=async()=>{try{const password=await nativeDialog('Export wallet backup','Enter your wallet password to save another CLI-compatible .keys backup.',{password:true});if(password!==null)await veneraAPI.call('wallet.backup',{password});}catch(e){message('saveWalletStatus',e.message,true);}};
  document.getElementById('enterCreatedWallet').onclick=()=>{document.getElementById('seedOutput').value='';location.href='account.html';};return;
 }
 const choose=document.getElementById('chooseWalletFileBtn');
 choose.onclick=async()=>{selection=await veneraAPI.call('wallet.pick');if(!selection)return;document.getElementById('seedRestorePanel').hidden=true;document.getElementById('fileRestorePanel').hidden=false;document.getElementById('walletFileName').textContent=selection.name;};
 document.getElementById('clearWalletFileBtn').onclick=()=>{selection=null;document.getElementById('seedRestorePanel').hidden=false;document.getElementById('fileRestorePanel').hidden=true;};
 const restore=document.getElementById('restoreSubmit');restore.onclick=async()=>{if(restore.disabled)return;restore.disabled=true;try{let result;
   if(selection){const password=await nativeDialog('Unlock local wallet',selection.name,{password:true,confirm:'Unlock and scan'});if(password===null)return;message('restoreFileStatus','Starting local wallet service…');result=await veneraAPI.call('wallet.open',{selection:selection.id,password});}
   else{let seed=document.getElementById('restoreInput').value.trim().replace(/\s+/g,' ');if(seed.split(' ').length!==25)throw new Error('Enter a 25-word VNR seed');const password=await newPassword();if(password===null)return;message('restoreFileStatus','Restoring locally and saving an encrypted .keys wallet…');result=await veneraAPI.call('wallet.import',{seed,password});seed='';document.getElementById('restoreInput').value='';}
   if(result)location.href='account.html';
  }catch(e){message('restoreFileStatus',e.message,true);}finally{restore.disabled=false;}};
}
function select(create){selection=null;content.innerHTML=create?createUI:loginUI;createBtn.classList.toggle('entry-tab-active',create);restoreBtn.classList.toggle('entry-tab-active',!create);attachEvents();}
createBtn.onclick=()=>select(true);restoreBtn.onclick=()=>select(false);select(false);
