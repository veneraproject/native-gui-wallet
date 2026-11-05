const JWT_TOKEN = localStorage.getItem('jwt_token');
const API_BASE = 'https://wallet.veneralabs.org';
let historyData = [], currentPage = 0, currentFilter = 'all', checkingBurns = false;
const PAGE_SIZE = 6;
const txHashOf = tx => String(tx.tx_hash || tx.hash || tx.txid || '');
function parsedTx(tx) {
    const value = tx.parsed_json ?? tx.as_json ?? {};
    if (typeof value === 'object' && value) return value;
    try { return JSON.parse(value); } catch (_) { return {}; }
}
function burnAtomic(tx) {
    const parsed = parsedTx(tx);
    const value = tx.amount_burnt ?? tx.amount_burned ?? tx.burned_amount_atomic ?? parsed.amount_burnt ?? parsed.amount_burned ?? 0;
    try { return BigInt(String(value)); } catch (_) { return 0n; }
}
function isBurn(tx) {
    const p = parsedTx(tx);
    return tx._isBurn === true || ['burn','burned'].includes(String(tx.type).toLowerCase()) ||
        tx.is_burn === true || tx.is_burn === 1 || tx.is_burn === 'true' || tx.is_burn === '1' ||
        Number(tx.tx_type ?? p.type ?? tx.type) === 5 || burnAtomic(tx) > 0n;
}
function filteredHistory() {
    return historyData.filter(tx => currentFilter === 'burn' ? isBurn(tx) :
        currentFilter === 'send' ? !isBurn(tx) && ['out','pending','pool_out'].includes(tx.type) :
        currentFilter === 'receive' ? !isBurn(tx) && ['in','pool','block'].includes(tx.type) : true);
}
function escapeHistory(value) {
    return String(value ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
}
function formatAmount(atomic) {
    try {
        const n = BigInt(String(atomic || 0)), whole = n / 1000000000n;
        const fraction = (n % 1000000000n).toString().padStart(9,'0').replace(/0+$/,'');
        return `${whole.toLocaleString('en-US')}${fraction ? '.' + fraction : ''}`;
    } catch (_) { return '0'; }
}
async function loadHistory() {
    try {
        const response = await fetch(`${API_BASE}/dhistory`, {method:'POST',headers:{Authorization:`Bearer ${JWT_TOKEN}`}});
        const data = await response.json();
        if (response.status === 401 || data.error === 'Token expired' || data.error === 'Invalid token') {
            localStorage.removeItem('jwt_token'); location.href = 'wallet.html'; return;
        }
        if (!response.ok || data.error) throw new Error(data.error || 'Unable to load history');
        historyData = Array.isArray(data) ? data : (data.transfers || data.history || []);
        currentPage = 0; renderHistory();
    } catch (error) {
        document.getElementById('pagination-info').textContent = 'History is temporarily unavailable. Try refreshing.';
        console.error('History load failed:', error.message);
        return;
    } finally { checkingBurns = false; }
    renderHistory();
}
function formatDate(timestamp) {
    const date = new Date(Number(timestamp || 0) * 1000);
    if (!Number.isFinite(date.getTime())) {
        return { dateStr: "Unknown", timeStr: "" };
    }
    return {
        dateStr: date.toLocaleDateString("en-US", {
            month: "short",
            day: "numeric",
            year: "numeric"
        }),
        timeStr: date.toLocaleTimeString("en-US", {
            hour: "2-digit",
            minute: "2-digit"
        })
    };
}

function renderHistory(emptyMessage = "No transactions found") {
    const table = document.getElementById("history-table");
    if (!table) return;

    table.innerHTML = "";
    ['all','send','receive','burn'].forEach(filter => {
        const button = document.getElementById('filter-'+filter);
        button?.classList.toggle('history-filter-active', currentFilter===filter);
        button?.setAttribute('aria-pressed',String(currentFilter===filter));
    });

    const filtered = filteredHistory();
    const maxPage = Math.max(0, Math.ceil(filtered.length / PAGE_SIZE) - 1);
    currentPage = Math.min(currentPage, maxPage);

    const start = currentPage * PAGE_SIZE;
    const pageItems = filtered.slice(start, start + PAGE_SIZE);

    if (!pageItems.length) {
        table.innerHTML = `
            <tr>
                <td class="py-14 text-center">
                    <div class="mx-auto w-14 h-14 rounded-2xl bg-primary/10 border border-primary/15 flex items-center justify-center text-primary mb-4">
                        <span class="material-symbols-outlined text-[28px]">history</span>
                    </div>
                    <div class="font-semibold text-on-surface">${escapeHistory(emptyMessage)}</div>
                    <div class="text-sm text-on-surface-variant mt-1">Wallet activity will appear here.</div>
                </td>
            </tr>
        `;
        updatePagination(filtered.length);
        return;
    }

    pageItems.forEach(tx => {
        const type = (isBurn(tx) ? 'burn' : ['out','pending','pool_out'].includes(tx.type) ? 'out' : 'in');
        const burned = type === "burn";
        const isSend = type === "out";
        const atomic = burned ? (tx._burnAtomic && tx._burnAtomic !== "0" ? tx._burnAtomic : burnAtomic(tx) > 0n ? burnAtomic(tx) : tx.amount) : tx.amount;
        const amount = Number(atomic || 0) / 1e9;
        const usd = amount * 0.0035;
        const { dateStr, timeStr } = formatDate(tx.timestamp);
        const status = Number(tx.confirmations || 0) < 10 ? "Processing" : "Confirmed";
        const txHash = tx.tx_hash || tx.hash || tx.txid || "";

        let label = "Receive";
        let icon = "south_west";
        let iconClass = "bg-primary/10 text-[#d8b8ff] border-primary/15";
        let amountClass = "text-[#d8b8ff]";
        let sign = "+";

        if (isSend) {
            label = "Send";
            icon = "north_east";
            iconClass = "bg-primary/10 text-primary border-primary/15";
            amountClass = "text-primary";
            sign = "-";
        } else if (burned) {
            label = "Burn";
            icon = "local_fire_department";
            iconClass = "bg-orange-500/10 text-orange-300 border-orange-400/15";
            amountClass = "text-orange-300";
            sign = "-";
        }

        const row = `
            <tr class="history-row group">
                <td class="py-2">
                    <div class="relative overflow-hidden rounded-[22px] border ${burned ? "border-orange-400/10" : "border-white/[0.055]"} bg-white/[0.025] px-4 sm:px-5 py-4 transition-all duration-300 hover:bg-white/[0.045] hover:border-primary/15">
                        <div class="absolute inset-y-0 left-0 w-px ${burned ? "bg-gradient-to-b from-transparent via-orange-300/70 to-transparent" : "bg-gradient-to-b from-transparent via-primary/45 to-transparent"}"></div>
                        ${burned ? '<div class="absolute -right-8 -top-10 w-32 h-32 rounded-full bg-orange-500/[0.055] blur-[45px] pointer-events-none"></div>' : ''}

                        <div class="relative flex flex-col sm:flex-row sm:items-center sm:justify-between gap-4">
                            <div class="flex items-start gap-3.5 min-w-0">
                                <div class="shrink-0 w-11 h-11 rounded-[15px] border ${iconClass} flex items-center justify-center shadow-[inset_0_1px_0_rgba(255,255,255,0.04)]">
                                    <span class="material-symbols-outlined text-[20px]">${icon}</span>
                                </div>

                                <div class="min-w-0">
                                    <div class="flex items-center gap-2 flex-wrap">
                                        <span class="font-semibold text-[15px] text-on-surface">${label}</span>
                                        ${burned ? '<span class="px-2 py-0.5 rounded-full bg-orange-400/10 border border-orange-300/10 text-[9px] font-bold uppercase tracking-[0.13em] text-orange-300">Burned</span>' : ''}
                                    </div>

                                    <div class="mt-1 font-bold text-[15px] ${amountClass}">${sign}${escapeHistory(formatAmount(atomic))} VNR</div>


                                    ${txHash ? `
                                        <button class="tx-hash mt-2.5 inline-flex items-center gap-1.5 text-[10px] sm:text-[11px] font-mono text-primary/90 hover:text-primary transition-colors max-w-full" data-hash="${escapeHistory(txHash)}" data-tx-type="${type}" data-amount-atomic="${escapeHistory(tx.amount || 0)}">
                                            <span class="truncate">${escapeHistory(txHash.slice(0, 13))}...${escapeHistory(txHash.slice(-10))}</span>
                                            <span class="material-symbols-outlined text-[14px] opacity-60">open_in_new</span>
                                        </button>
                                    ` : ""}
                                </div>
                            </div>

                            <div class="sm:text-right sm:pl-5 shrink-0 flex sm:flex-col items-center sm:items-end justify-between gap-2 sm:gap-1.5 border-t sm:border-t-0 border-white/5 pt-3 sm:pt-0">
                                <div class="text-[11px] text-on-surface-variant">${dateStr} <span class="opacity-40">•</span> ${timeStr}</div>
                                <span class="inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full text-[9px] font-bold uppercase tracking-[0.1em] ${
                                    status === "Confirmed"
                                        ? "bg-primary/10 text-[#d9bbff] border border-primary/15"
                                        : "bg-white/[0.04] text-on-surface-variant border border-white/[0.06]"
                                }">
                                    <span class="w-1.5 h-1.5 rounded-full ${status === "Confirmed" ? "bg-primary shadow-[0_0_7px_rgba(204,151,255,.7)]" : "bg-on-surface-variant/50"}"></span>
                                    ${status}
                                </span>
                                <div class="text-[10px] text-on-surface-variant/60">${Number(tx.confirmations || 0)} confirmations · ~$${usd.toFixed(2)}</div>
                            </div>
                        </div>
                    </div>
                </td>
            </tr>
        `;

        table.insertAdjacentHTML("beforeend", row);
    });

    updatePagination(filtered.length);
    document.querySelectorAll('.tx-hash').forEach(el => el.onclick = () => window.openExplorerModal(el.dataset.hash));
    ['all','send','receive','burn'].forEach(filter => {
        const button = document.getElementById('filter-'+filter);
        button?.classList.toggle('history-filter-active', currentFilter===filter);
        button?.setAttribute('aria-pressed',String(currentFilter===filter));
    });
}

function updatePagination() {
    const total = filteredHistory().length;
    document.getElementById('pagination-info').textContent = `Showing ${total ? currentPage*PAGE_SIZE+1 : 0}–${Math.min((currentPage+1)*PAGE_SIZE,total)} of ${total} transactions${checkingBurns ? ' · checking burn types…' : ''}`;
    document.getElementById('prev-page').disabled = currentPage===0;
    document.getElementById('next-page').disabled = (currentPage+1)*PAGE_SIZE>=total;
}
document.getElementById('refresh-history').onclick = loadHistory;
['all','send','receive','burn'].forEach(filter => document.getElementById('filter-'+filter).onclick = () => { currentFilter=filter; currentPage=0; renderHistory(); });
document.getElementById('prev-page').onclick = () => { if (currentPage>0) {currentPage--;renderHistory();} };
document.getElementById('next-page').onclick = () => { if((currentPage+1)*PAGE_SIZE<filteredHistory().length){currentPage++;renderHistory();} };
document.getElementById('sign-out-btn').onclick = async () => {
    try {await fetch(`${API_BASE}/dinvalidate`,{method:'POST',headers:{Authorization:`Bearer ${JWT_TOKEN}`}});} catch (_) {}
    localStorage.removeItem('jwt_token'); location.href='wallet.html';
};
document.addEventListener('DOMContentLoaded', loadHistory);


