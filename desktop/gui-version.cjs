'use strict';

async function fetchGuiVersion(fetchImpl, origin) {
  const response = await fetchImpl(origin + '/api/v2/version', {
    method: 'GET',
    redirect: 'error',
    credentials: 'omit',
    cache: 'no-store',
    headers: {Accept: 'application/json', 'Cache-Control': 'no-cache'},
    signal: AbortSignal.timeout(8000)
  });
  if (!response.ok) throw new Error('Version endpoint HTTP ' + response.status);
  const data = await response.json();
  const version = data?.version ?? data?.latest_version;
  if (!Number.isSafeInteger(version) || version < 1) throw new Error('Invalid backend wallet version');
  return {version};
}

module.exports = {fetchGuiVersion};
