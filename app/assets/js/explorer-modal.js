(() => {
    let previousFocus;
    function closeExplorer() {
        const old=document.getElementById('lookupOverlay');
        if(old){old.removeAttribute('id');old.classList.add('is-closing');setTimeout(()=>old.remove(),180);}
        previousFocus?.focus?.();
    }
    window.openExplorerModal = function (query) {
        if (!/^(?:\d+|[a-fA-F0-9]{64})$/.test(String(query))) return;
        closeExplorer(); previousFocus = document.activeElement;
        const overlay = document.createElement('div');
        overlay.id = 'lookupOverlay'; overlay.className = 'explorer-overlay';
        const panel = document.createElement('section');
        panel.id = 'lookupModal'; panel.className = 'explorer-panel';
        panel.setAttribute('role', 'dialog'); panel.setAttribute('aria-modal', 'true');
        panel.setAttribute('aria-label', 'Venera transaction explorer');
        const header = document.createElement('div'); header.className = 'explorer-modal-header';
        const title = document.createElement('strong'); title.textContent = 'Venera Explorer';
        const close = document.createElement('button'); close.type = 'button';
        close.innerHTML = '<span class="material-symbols-outlined">close</span>';  close.setAttribute('aria-label', 'Close explorer'); close.onclick = closeExplorer;
        header.append(title, close);
        const frame = document.createElement('iframe'); frame.title = 'Transaction and burn details';
        frame.src = `explorer.html?embed=1&q=${encodeURIComponent(query)}`;
        panel.append(header, frame); overlay.append(panel); document.body.append(overlay);
        overlay.addEventListener('click', e => { if (e.target === overlay) closeExplorer(); });
        close.focus();
        frame.addEventListener('load', () => {
            try { frame.contentDocument.addEventListener('keydown', e => { if (e.key === 'Escape') closeExplorer(); }); } catch (_) {}
        });
    };
    document.addEventListener('keydown', e => { if (e.key === 'Escape' && document.getElementById('lookupOverlay')) {e.preventDefault();e.stopImmediatePropagation();closeExplorer();} });
})();
