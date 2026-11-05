(() => {
  'use strict';

  const WALLET_VERSION = 3;

  async function checkVersion() {
    if (!location.pathname.endsWith('/wallet.html') || window.__veneraVersionCheckStarted) return;
    window.__veneraVersionCheckStarted = true;
    let banner = document.getElementById('update-banner');
    if (!banner) {
      banner = document.createElement('div');
      banner.id = 'update-banner';
      document.body.append(banner);
    }
    banner.classList.add('venera-version-notice');
    banner.classList.remove('opacity-0', '-translate-y-32');
    banner.setAttribute('role', 'status');
    banner.setAttribute('aria-live', 'polite');
    banner.setAttribute('aria-atomic', 'true');
    let kind = 'unavailable';
    let message = 'Unable to check for wallet updates';
    try {
      const data = await window.veneraAPI.call('public.version');
      if (!Number.isSafeInteger(data?.version) || data.version < 1) throw new Error('Invalid wallet version');
      const outdated = WALLET_VERSION < data.version;
      kind = outdated ? 'outdated' : 'current';
      message = outdated ? 'Outdated wallet. New version available' : 'Wallet is up to date';
    } catch {}
    banner.dataset.kind = kind;
    banner.textContent = message;
    banner.dataset.visible = 'true';
    setTimeout(() => {banner.dataset.visible = 'false';}, kind === 'outdated' ? 12000 : 7000);
  }

  if (document.readyState === 'loading') document.addEventListener('DOMContentLoaded', checkVersion, {once: true});
  else void checkVersion();
})();
