                                                                                 
(() => {
    document.addEventListener('DOMContentLoaded', () => {
        const receipt = document.getElementById('collect-yield-status');
        const overlay = document.getElementById('collect-success-overlay');
        if (!receipt) return;
        let revealed = false;
        const update = () => {
            if (receipt.hidden || receipt.classList.contains('hidden')) { revealed = false; return; }
            if (overlay && !overlay.hidden && !overlay.classList.contains('hidden')) return;
            if (revealed) return;
            revealed = true;
            requestAnimationFrame(() => {
                if (receipt.hidden || receipt.classList.contains('hidden')) return;
                const reduced = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
                receipt.scrollIntoView({block: 'nearest', behavior: reduced ? 'auto' : 'smooth'});
            });
        };
        const observer = new MutationObserver(update);
        observer.observe(receipt, {attributes: true, attributeFilter: ['class', 'hidden']});
        if (overlay) observer.observe(overlay, {attributes: true, attributeFilter: ['class', 'hidden']});
        update();
    });
})();
