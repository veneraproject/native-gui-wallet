                                                                                           
(() => {
    let navigating=false;
    document.addEventListener('click',event=>{
        const link=event.target.closest('#sidebar a[href]');
        if(!link || event.defaultPrevented || event.button!==0 || event.ctrlKey || event.metaKey || event.shiftKey || event.altKey || link.target || link.hasAttribute('download')) return;
        const href=link.getAttribute('href');
        if(!/^[\w-]+\.html$/.test(href) || matchMedia('(prefers-reduced-motion: reduce)').matches) return;
        event.preventDefault();if(navigating)return;navigating=true;
        document.querySelector('main')?.classList.add('wallet-leaving');
        setTimeout(()=>location.href=href,120);
    });
})();

                                                                    
(() => {
    const selector='[id$="Modal"], #trust-modal, #otp-modal, #reg-modal, #lookupOverlay';
    function mark(root){
        const overlays=[];
        if(root.matches?.(selector))overlays.push(root);
        overlays.push(...(root.querySelectorAll?.(selector)||[]));
        for(const overlay of overlays){
            overlay.classList.add('wallet-modal-overlay');
            for(const panel of overlay.querySelectorAll('.glass-modal, #stakeContent, #lookupModal, #otp-modal-content'))panel.classList.add('wallet-modal-panel');
        }
    }
    document.addEventListener('DOMContentLoaded',()=>{
        mark(document);
        new MutationObserver(records=>{for(const record of records)for(const node of record.addedNodes)if(node.nodeType===1)mark(node);}).observe(document.body,{childList:true,subtree:true});
    });
})();
