const JWT_TOKEN = localStorage.getItem("jwt_token");
let depositAddresses = {
    vnr: null,
    evm: null
};
document.getElementById('sign-out-btn').addEventListener('click', async () => {
    const token = localStorage.getItem('jwt_token');

    try {
        if (token) {
            await fetch('https://wallet.veneralabs.org/dinvalidate', {
                method: 'POST',
                headers: {
                    'Authorization': `Bearer ${token}`
                }
            });
        }
    } catch (err) {
        console.error('Logout request failed');
    } finally {
                             
        localStorage.removeItem('jwt_token');

                                  
        window.location.href = 'wallet.html';
    }
});

function formatTimeUntil(unixTimestamp) {
    const now = Math.floor(Date.now() / 1000);
    let diff = unixTimestamp - now;

    if (diff <= 0) return "0m";

    const days = Math.floor(diff / 86400);
    diff %= 86400;
    const hours = Math.floor(diff / 3600);
    diff %= 3600;
    const minutes = Math.floor(diff / 60);

    if (days > 0) return `${days}d`;
    if (hours > 0) return `${hours}h`;
    return `${minutes}m`;
}
async function updateBalances() {
    const indicator = document.getElementById('sync-indicator');

    try {
        
         if (indicator) {
            indicator.classList.remove('opacity-0');
            indicator.classList.add('opacity-100');
        }
        const response = await fetch(
            "https://wallet.veneralabs.org/totalbalanced",
            {
                method: "POST",

                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${JWT_TOKEN}`
                }
            }
        );

        const data = await response.json();

        if (data.error) {
            console.error(data.error);
            return;
        }

                                 
        depositAddresses.vnr = data.wallet_address;
        depositAddresses.evm = data.usdt_address;

                         
        const usdt = parseFloat(data.usdt_balance || 0);
        const usdc = parseFloat(data.usdc_balance || 0);
        const bnb = parseFloat(data.bnb_balance || 0);
        const vnr = parseFloat(data.vnr_balance || 0);

                 
        const VNR_PRICE = 0.0035;
        const BNB_PRICE = 582;

                     
        const usdtValue = usdt;
        const usdcValue = usdc;
        const bnbValue = bnb * BNB_PRICE;
        const vnrValue = vnr * VNR_PRICE;

                          
        document.getElementById("usdt-balance").textContent =
            usdt.toFixed(2);

        document.getElementById("usdc-balance").textContent =
            usdc.toFixed(2);

        document.getElementById("bnb-balance").textContent =
            bnb.toFixed(4);

        document.getElementById("vnr-balance").textContent =
            vnr.toFixed(2);

                            
        document.getElementById("usdt-value").textContent =
            `$${usdtValue.toFixed(2)}`;

        document.getElementById("usdc-value").textContent =
            `$${usdcValue.toFixed(2)}`;

        document.getElementById("bnb-value").textContent =
            `$${bnbValue.toFixed(2)}`;

        document.getElementById("vnr-value").textContent =
            `$${vnrValue.toFixed(2)}`;

                       
        ["usdt", "usdc", "bnb", "vnr"].forEach(asset => {

            document
                .getElementById(`${asset}-loader`)
                ?.classList.add("hidden");

            document
                .getElementById(`${asset}-content`)
                ?.classList.remove("hidden");
        });

        if(data.evm_error){for(const coin of ['usdt','usdc','bnb']){const node=document.getElementById(coin+'-balance');if(node){node.textContent='—';node.title=data.evm_error;}}}
        if(data.scanning||data.scan_error){const node=document.getElementById('vnr-balance');if(node&&data.vnr_balance==='0'){node.textContent='—';node.title=data.scan_error||'Wallet scanning is in progress';}}
                                
        const total =
            usdtValue +
            usdcValue +
            bnbValue +
            vnrValue;

        const totalElem =
            document.getElementById("totalvalue");

        if (totalElem) {
            totalElem.textContent =
                `$${total.toFixed(2)}`;
        }

    } catch (err) {

        console.error(
            "Failed to update balances"
        );
    } finally {
        if (indicator) {
            indicator.classList.remove('opacity-100');

            setTimeout(() => {
                indicator.classList.add('opacity-0');
            }, 150);
        }
    }
}
document.addEventListener("DOMContentLoaded", () => {

    updateBalances();
    loadStakes();
    setInterval(updateBalances, 15000);
    
    setInterval(dRefresh, 15000);

});
async function loadStakes() {

    try {

   

        const res = await fetch(
            "https://wallet.veneralabs.org/dstakes",
            {
                method: "GET",

                headers: {
                    "Authorization": `Bearer ${JWT_TOKEN}`
                }
            }
        );

        const data = await res.json();

        const stakes = data.stakes || [];

        let totalRewards = 0;

        let totalAmount = 0;

        let nextUnlock = null;

        stakes.forEach(s => {

            const amount =
                parseFloat(s.amount || 0);

            totalRewards += Math.max(0, Number(s.amount_out ?? amount) - amount);

            totalAmount += amount;

                          
            if (
                s.timestamp_y > Date.now() / 1000 && (!nextUnlock ||
                s.timestamp_y < nextUnlock.timestamp_y
            )) {
                nextUnlock = s;
            }
        });

        const avgTermReturn =
            totalAmount > 0
                ? (totalRewards / totalAmount * 100).toFixed(2)
                : "0.00";

        document.querySelector(
            ".text-4xl.font-bold.text-on-surface"
        ).innerHTML =
            `${avgTermReturn}% <span class="text-xl font-normal text-slate-500">Term return</span>`;

                      
        if (nextUnlock) {

            const timeUntil =
                formatTimeUntil(nextUnlock.timestamp_y);

            const yieldAmount = Math.max(0, Number(nextUnlock.amount_out ?? nextUnlock.amount) - Number(nextUnlock.amount)).toFixed(3);

            document.querySelector(
                ".flex.justify-between.mt-3.text-xs.font-medium.text-slate-500 span:last-child"
            ).textContent =
                `+${yieldAmount} VNR`;

            document.querySelector(
                ".flex.justify-between.mt-3.text-xs.font-medium.text-slate-500 span:first-child"
            ).textContent =
                `Next Unlock: ${timeUntil}`;
        }

    } catch (err) {

        console.error(
            "Error fetching stakes:"
        );
    }
}
async function dRefresh() {
    try {
        const res = await fetch("https://wallet.veneralabs.org/drefresh", {
            method: "POST",
            headers: {
                "Authorization": `Bearer ${JWT_TOKEN}`
            }
        });

        const data = await res.json();

        if (data?.error === "Token expired") {
            window.location.href = "wallet.html";
            return;
        }

        if (!res.ok) {
            console.error("drefresh failed");
        }

    } catch (err) {
        console.error("drefresh failed");
    }
}
async function refreshCore() {

    try {

        await fetch(
            "https://wallet.veneralabs.org/dcore",
            {
                method: "POST",

                headers: {
                    "Authorization": `Bearer ${JWT_TOKEN}`
                }
            }
        );

    } catch (err) {

        console.error(
            "refreshCore failed"
        );
    }
}

                          
document.addEventListener("DOMContentLoaded", () => {

                      
    refreshCore();

                               
    setInterval(refreshCore, 15000);

});
                                                                    
let miningInterval = null;

async function updateMiningStatus() {
    const statusText = document.getElementById("statusText");
    const hashrateEl = document.getElementById("hashrateEl");

    try {
        const res = await fetch("https://pool.veneralabs.org/status", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                wallet_address: depositAddresses.vnr
            })
        });

        const data = await res.json();

        const rawHashrate = Number(data.hashrate || 0);
        const hashrateKH = (rawHashrate / 1000).toFixed(2);

        if (rawHashrate > 0) {
            statusText.textContent = "Mining at ";
            statusText.style.color = "#C48EF8";

            hashrateEl.textContent = ` ${hashrateKH} kH/s`;
        } else {
            statusText.textContent = "Not mining";
            statusText.style.color = "#aaa";

            hashrateEl.textContent = "0 kH/s";
        }

    } catch (err) {
        statusText.textContent = "Not mining";
        statusText.style.color = "#aaa";
        hashrateEl.textContent = "0 kH/s";
        console.error("Mining status error");
    }
}

                  
updateMiningStatus();

                        
if (!miningInterval) {
    miningInterval = setInterval(updateMiningStatus, 5000);
}
