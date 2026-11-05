(() => {

    const api = 'https://wallet.veneralabs.org';

    const $ = id => document.getElementById(id);

    const cells = [...document.querySelectorAll('#otp-modal .otp-cell')];

    let selectedAsset = null, busy = false, pending = null, resetTimer = null;

    const btn = $('confirm-send'), status = $('send-status');

    async function request(path, body) {

        const response = await fetch(api + path, {method:'POST',headers:{

            'Content-Type':'application/json',Authorization:`Bearer ${JWT_TOKEN}`

        }, ...(body === undefined ? {} : {body:JSON.stringify(body)})});

        const data = await response.json();

        if (response.status===401 || ['Token expired','Invalid token'].includes(data.error)) {

            localStorage.removeItem('jwt_token'); location.href='wallet.html'; throw new Error('Session expired');

        }

        if (!response.ok || data.error) throw new Error(data.error || 'Request failed');

        return data;

    }

    const hideTimers = new Map();

    function openModal(modalId, backdropId, contentId) {

        clearTimeout(hideTimers.get(modalId));

        $(modalId).classList.remove('hidden');

        requestAnimationFrame(() => requestAnimationFrame(() => {

            $(backdropId)?.classList.remove('opacity-0');

            $(contentId).classList.remove('opacity-0','scale-95');

        }));

        $(contentId).querySelector('.modal-dismiss')?.addEventListener('click', () => closeModal(modalId,backdropId,contentId), {once:true});

        if ($(backdropId)) $(backdropId).onclick = () => closeModal(modalId,backdropId,contentId);

    }

    function closeModal(modalId, backdropId, contentId) {

        $(backdropId)?.classList.add('opacity-0');

        $(contentId).classList.add('opacity-0','scale-95');

        hideTimers.set(modalId,setTimeout(() => $(modalId).classList.add('hidden'),180));

    }

    function clearSend() {

        clearTimeout(resetTimer);resetTimer=null; pending=null;

        btn.classList.remove('transfer-success');

        btn.textContent='Confirm Transfer';btn.disabled=false;

        status.replaceChildren();status.className='mt-4 text-center text-sm hidden';

        $('recipient-box').classList.remove('input-error');$('amount-box').classList.remove('input-error');

        $('send-recipient').value='';$('send-amount').value='';

    }

    function openOtp() {

        $('otp-error').classList.add('hidden');cells.forEach(c=>c.value='');

        clearTimeout(hideTimers.get('otp-modal'));

        $('otp-modal').classList.remove('hidden','opacity-0');

        $('otp-modal-content').classList.remove('opacity-0','scale-95');

        $('otp-modal-content').classList.add('opacity-100','scale-100');cells[0].focus();

    }

    function closeOtp(cancel=true) {

        if (cancel && $('verify-otp-btn').disabled) return;

        $('otp-modal').classList.add('opacity-0');

        hideTimers.set('otp-modal',setTimeout(()=>$('otp-modal').classList.add('hidden'),180));

        $('otp-modal-content').classList.remove('opacity-100','scale-100');

        $('otp-modal-content').classList.add('opacity-0','scale-95');

        cells.forEach(c=>c.value='');

        if (cancel) {pending=null;btn.disabled=false;btn.textContent='Confirm Transfer';}

    }

    function showError(message) {

        btn.disabled=false;btn.textContent='Confirm Transfer';

        status.className='mt-4 text-center text-sm text-red-300';status.textContent=safeWalletError(message, 'Unable to confirm the transfer. Refresh your balance and history before trying again.');

    }

    async function sendTransaction(transfer) {

        if (busy) return;

        busy=true;btn.disabled=true;btn.textContent='Sending…';

        try {

            const data=await request(transfer.endpoint,transfer.body);

            if (!data.tx_hash) throw new Error('Transfer did not return a transaction hash');

            btn.textContent='Transfer Successful';btn.classList.add('transfer-success');

            status.className='transfer-receipt';

            const hash=document.createElement('button'); hash.type='button';

            hash.className='transfer-hash';

            const text=document.createElement('span');text.textContent=data.tx_hash;

            const icon=document.createElement('span');icon.className='material-symbols-outlined';

            icon.textContent=transfer.asset==='vnr'?'open_in_new':'content_copy';

            hash.setAttribute('aria-label',transfer.asset==='vnr'?'View transaction':'Copy transaction hash');

            hash.append(text,icon);

            hash.onclick=transfer.asset==='vnr'?()=>window.openExplorerModal(data.tx_hash):async()=>{

                try {await navigator.clipboard.writeText(data.tx_hash);icon.textContent='check';setTimeout(()=>icon.textContent='content_copy',1600);}

                catch (_) {icon.textContent='error';}

            };

            status.replaceChildren(hash);

            updateBalances();

            resetTimer=setTimeout(()=>{clearSend();closeModal('sendFormModal','sendFormBackdrop','sendFormContent');},7000);

        } catch(error) {showError(error.message);} finally {busy=false;pending=null;}

    }

    $('send-btn').onclick=()=>{

        if (busy || pending || resetTimer) {openModal('sendFormModal','sendFormBackdrop','sendFormContent');return;}

        openModal('sendSelectModal','sendSelectBackdrop','sendSelectContent');

    };

    $('closeSendFormModal')?.addEventListener('click',()=>closeModal('sendFormModal','sendFormBackdrop','sendFormContent'));

    document.querySelectorAll('.send-asset-btn').forEach(button=>button.onclick=()=>{

        if (busy || pending) return;

        selectedAsset=button.dataset.asset;clearSend();

        $('send-title').textContent=`Send ${selectedAsset.toUpperCase()}`;

        const networkNote=$('send-network-note');
        if (networkNote) networkNote.textContent=selectedAsset==='vnr'?'Venera network':selectedAsset==='bnb'?'BNB Smart Chain network':'BNB Smart Chain · BNB is required for network fees';

        $('send-recipient').placeholder=selectedAsset==='vnr'?'VENeRa...':'0x...';

        closeModal('sendSelectModal','sendSelectBackdrop','sendSelectContent');

        setTimeout(()=>openModal('sendFormModal','sendFormBackdrop','sendFormContent'),180);

    });

    $('send-recipient').addEventListener('input',()=>$('recipient-box').classList.remove('input-error'));

    $('send-amount').addEventListener('input',()=>$('amount-box').classList.remove('input-error'));

    btn.onclick=async()=>{

        if (busy || pending || !selectedAsset) return;

        const recipient=$('send-recipient').value.trim(),amount=Number($('send-amount').value);

        const validAddress=selectedAsset==='vnr'?recipient.startsWith('VENeRa') && recipient.length>10:/^0x[a-fA-F0-9]{40}$/.test(recipient);

        $('recipient-box').classList.toggle('input-error',!validAddress);

        $('amount-box').classList.toggle('input-error',!Number.isFinite(amount)||amount<=0);

        if (!validAddress || !Number.isFinite(amount) || amount<=0) return;

        const transfer={asset:selectedAsset,endpoint:({vnr:'/dtransfer',usdt:'/dsendusdt',usdc:'/dsendusdc',bnb:'/dsendbnb'})[selectedAsset],

            body:selectedAsset==='vnr'?{sendaddress:recipient,sendamount:$('send-amount').value.trim()}:{recipient,amount:$('send-amount').value.trim()}};

        busy=true;btn.disabled=true;btn.textContent='Checking security…';status.classList.add('hidden');

        try {

                                                                              
                                                                              
            busy=false;await sendTransaction(transfer);

        } catch(error) {showError(error.message);} finally {busy=false;}

    };

    $('verify-otp-btn').onclick=async()=>{

        if (!pending || busy) return;

        const code=cells.map(c=>c.value).join('');

        if (!/^\d{6}$/.test(code)) { $('otp-error').textContent='Enter all six digits';$('otp-error').classList.remove('hidden');return; }

        const transfer=pending;busy=true;$('verify-otp-btn').disabled=true;$('verify-otp-btn').textContent='Verifying…';

        try {

            const data=await request('/d2fa/verify',{code});

            if (!data.success) throw new Error(data.error || 'Invalid code');

            closeOtp(false);busy=false;await sendTransaction(transfer);

        } catch(error) {$('otp-error').textContent=safeWalletError(error, 'Unable to verify the code. Please try again.');$('otp-error').classList.remove('hidden');}

        finally {busy=false;$('verify-otp-btn').disabled=false;$('verify-otp-btn').textContent='Verify Code';}

    };

    $('otp-modal').onclick=e=>{if(!$('otp-modal-content').contains(e.target))closeOtp();};

    cells.forEach((cell,index)=>{

        cell.oninput=()=>{cell.value=cell.value.replace(/\D/g,'').slice(-1);if(cell.value&&index<5)cells[index+1].focus();};

        cell.onkeydown=e=>{if(e.key==='Backspace'&&!cell.value&&index>0)cells[index-1].focus();if(e.key==='Enter')$('verify-otp-btn').click();};

        cell.onpaste=e=>{e.preventDefault();const digits=(e.clipboardData.getData('text')||'').replace(/\D/g,'').slice(0,6);digits.split('').forEach((d,i)=>{if(cells[index+i])cells[index+i].value=d;});cells[Math.min(index+digits.length,5)].focus();};

    });

    $('receive-btn').onclick=()=>openModal('receiveModal','receiveBackdrop','receiveContent');

    document.querySelectorAll('.coin-btn').forEach(button=>button.onclick=async()=>{

        const coin=button.dataset.coin;if(!['vnr','usdt','usdc','bnb'].includes(coin))return;

        button.disabled=true;

        try {

            if (!(coin==='vnr'?depositAddresses.vnr:depositAddresses.evm))await updateBalances();

            const address=coin==='vnr'?depositAddresses.vnr:depositAddresses.evm;

            if(!address)throw new Error('Deposit address is not available. Refresh your balances and try again.');

            $('deposit-title').textContent=`Receive ${coin.toUpperCase()}`;

            $('deposit-network').textContent=coin==='vnr'?'Venera network':'BNB Smart Chain (BSC / BEP-20)';

            $('deposit-address').textContent=address;

            QRCode.toCanvas($('qr-code'),address,{width:180,margin:4,color:{dark:'#211a2b',light:'#ffffff'}},error=>{if(error)console.error('QR generation failed:',error.message);});

            closeModal('receiveModal','receiveBackdrop','receiveContent');

            openModal('addressModal','addressBackdrop','addressContent');

        } catch(error){alert(safeWalletError(error, 'Deposit details are temporarily unavailable.'));}finally{button.disabled=false;}

    });

    $('copy-address-btn').onclick=async()=>{

        try {await navigator.clipboard.writeText($('deposit-address').textContent);$('copy-icon').textContent='check';setTimeout(()=>$('copy-icon').textContent='content_copy',1500);}

        catch (_){$('copy-icon').textContent='error';}

    };

    $('addressBackdrop').onclick=()=>closeModal('addressModal','addressBackdrop','addressContent');

    document.addEventListener('keydown',e=>{

        if(e.key!=='Escape')return;

        if(!$('otp-modal').classList.contains('hidden')){closeOtp();return;}

        for(const [m,b,c] of [['receiveModal','receiveBackdrop','receiveContent'],['addressModal','addressBackdrop','addressContent'],['sendSelectModal','sendSelectBackdrop','sendSelectContent'],['sendFormModal','sendFormBackdrop','sendFormContent']]){

            if(!$(m).classList.contains('hidden')&&!busy)closeModal(m,b,c);

        }

    });

})();