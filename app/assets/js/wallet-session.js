(() => {
 'use strict';
 if(!window.veneraAPI || window.parent!==window)return;
 document.addEventListener('DOMContentLoaded',()=>{
  const entry=location.pathname.endsWith('/wallet.html');
  if(!entry){const badge=document.createElement('button');badge.id='native-sync-status';badge.title='Refresh local node and wallet status';document.body.append(badge);let wasScanning=false;const update=async()=>{try{const s=await window.veneraAPI.call('status');if(!s.unlocked&&!location.pathname.endsWith('/explorer.html')){location.href='wallet.html';return;}if(wasScanning&&!s.scanning)window.dispatchEvent(new Event('venera:ready'));wasScanning=s.scanning;badge.textContent=s.scanning?'Scanning wallet…':`Node ${s.chain.height.toLocaleString()} / ${s.chain.target.toLocaleString()} · Wallet ${s.walletHeight.toLocaleString()}`;if(s.scanError)badge.textContent='Wallet scan failed · click to retry';if(s.evmError)badge.textContent+=' · BSC RPC unavailable';if(s.registrationError)badge.title='Reward registration pending: '+s.registrationError;}catch(e){badge.textContent=e.message;}};badge.onclick=async()=>{await window.veneraAPI.call('wallet.refresh');update();};update();setInterval(update,5000);}
  document.getElementById('sign-out-btn')?.addEventListener('click',async e=>{e.preventDefault();e.stopImmediatePropagation();await window.veneraAPI.call('wallet.lock');localStorage.removeItem('jwt_token');location.href='wallet.html';},true);
 });
})();
