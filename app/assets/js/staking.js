const JWT_TOKEN = localStorage.getItem("jwt_token");
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
        console.error('Logout request failed:');
    } finally {
                             
        localStorage.removeItem('jwt_token');

                                  
        window.location.href = 'wallet.html';
    }
});
async function loadStakes() {
    try {
                                               
        const token = JWT_TOKEN;                                     

        const res = await fetch('https://wallet.veneralabs.org/dstakes', {
            method: 'GET',
            headers: {
                'Authorization': `Bearer ${token}`
            }
        });

        const data = await res.json();

        const stakes = data.stakes || [];

        let totalStaked = 0;
        let totalRewards = 0;

        let now = Math.floor(Date.now() / 1000);
        let nextUnlockTs = null;
        let nextUnlockReward = 0;

                                           
        stakes.forEach(stake => {
            const amount = parseFloat(stake.amount || 0);
            totalStaked += amount;
            const reward = Math.max(0, Number(stake.amount_out ?? amount) - amount);
            totalRewards += reward;

            if (stake.timestamp_y > now) {
                if (!nextUnlockTs || stake.timestamp_y < nextUnlockTs) {
                    nextUnlockTs = stake.timestamp_y;
                    nextUnlockReward = reward;
                }
            }
        });

        const avgTermReturn = totalStaked > 0 ? totalRewards / totalStaked : 0;

                        
        let nextUnlockText = "--";
        if (nextUnlockTs) {
            const diff = nextUnlockTs - now;
            const days = Math.floor(diff / 86400);
            const hours = Math.floor((diff % 86400) / 3600);
            nextUnlockText = days > 0 ? `${days}d` : `${hours}h`;
        }

                                
        document.getElementById('total-staked').textContent = totalStaked.toFixed(2);
        document.getElementById('monthly-yield').textContent = `${nextUnlockReward.toFixed(4)} VNR`;
        document.getElementById('total-rewards').textContent = `${totalRewards.toFixed(4)} VNR`;
        document.getElementById('next-unlock').textContent = nextUnlockText;
        document.getElementById('avg-apy').textContent = `${(avgTermReturn * 100).toFixed(2)}%`;

                                   
        const table = document.getElementById('stakes-table');
        table.innerHTML = "";
        if (!stakes.length) {
            table.innerHTML = `<tr><td colspan="5" class="text-center py-10 text-on-surface-variant">
                <img src="assets/images/vg.png" alt="VNR" class="w-10 h-10 object-contain mx-auto mb-3 opacity-70">
                <p class="text-sm font-medium">No active stakes yet</p>
                <p class="text-xs mt-1">Stake VNR to start earning rewards.</p>
            </td></tr>`;
        }

        stakes.forEach(stake => {
            const amount = parseFloat(stake.amount || 0);
            const totalReward = Math.max(0, Number(stake.amount_out ?? amount) - amount);
            const termReturn = amount > 0 ? totalReward / amount * 100 : 0;

            const totalTime = stake.timestamp_y - stake.timestamp_x;
            const remaining = stake.timestamp_y - now;
            const progress = totalTime > 0 ? Math.max(0, Math.min(1, 1 - (remaining / totalTime))) : 1;

            let timeText = "Unlocked";
            if (remaining > 0) {
                const days = Math.ceil(remaining / 86400);
                timeText = `${days} Days left`;
            }

            const progressPercent = Math.floor(progress * 100);
            const payout = amount + totalReward;

            const row = `
<!-- 💻 Desktop -->
<tr class="hidden md:table-row hover:bg-white/5 transition-colors group">
    <td class="px-8 py-6 flex items-center gap-3">
        <div class="w-8 h-8 rounded-full bg-surface-container-high flex items-center justify-center text-xs font-bold text-[#ffffff]"><img src="assets/images/vg.png" alt="VNR" class="w-6 h-6 object-contain"></div>
        <span class="font-medium text-[#cc97ff]">Venera</span>
    </td>

    <td class="px-8 py-6 font-mono text-[#cc97ff]">${amount.toFixed(2)} VNR</td>

    <td class="px-8 py-6 text-primary text-[#cc97ff]">${termReturn.toFixed(2)}%</td>

    <td class="px-8 py-6">
        <span class="font-bold text-[#cc97ff]">${totalReward.toFixed(4)} VNR</span>
        <span class="text-[10px] text-primary bg-primary/10 px-2 py-0.5 rounded-full ml-2">
            ${payout.toFixed(4)} at unlock
        </span>
    </td>

    <td class="px-8 py-6">
        <div class="flex items-center gap-2">
            <div class="w-16 h-1 bg-surface-container-highest rounded-full overflow-hidden">
                <div class="bg-primary h-full" style="width:${progressPercent}%"></div>
            </div>
            <span class="text-xs text-slate-400 whitespace-nowrap">${timeText}</span>
        </div>
    </td>
</tr>

<!-- 📱 Mobile -->
<tr class="md:hidden block border-b border-outline-variant/10 px-4 py-4">
    <td class="block space-y-3">

        <div class="flex items-center gap-2">
            <div class="w-7 h-7 rounded-full bg-surface-container-high flex items-center justify-center text-[10px] font-bold"><img src="assets/images/vg.png" alt="VNR" class="w-5 h-5 object-contain"></div>
            <span class="font-medium text-sm text-[#cc97ff]">Venera</span>
        </div>

        <div class="grid grid-cols-2 gap-x-3 gap-y-2 text-xs">
            <div>
                <p class="text-slate-400">Amount</p>
                <p class="font-mono truncate text-[#cc97ff]">${amount.toFixed(2)} VNR</p>
            </div>
            <div>
                <p class="text-slate-400">Term return</p>
                <p class="text-primary">${termReturn.toFixed(2)}%</p>
            </div>
            <div>
                <p class="text-slate-400">Rewards</p>
                <p class="font-bold truncate">${totalReward.toFixed(4)} VNR</p>
            </div>
            <div>
                <p class="text-slate-400">At unlock</p>
                <p class="text-primary truncate">${payout.toFixed(4)} VNR</p>
            </div>
        </div>

        <div class="space-y-1">
            <p class="text-slate-400 text-xs">Lock Period</p>
            <div class="w-full overflow-hidden">
                <div class="h-1 w-full bg-surface-container-highest rounded-full overflow-hidden">
                    <div class="bg-primary h-full max-w-full" style="width:${Math.min(progressPercent, 100)}%"></div>
                </div>
            </div>
            <p class="text-xs text-slate-400">${timeText}</p>
        </div>

    </td>
</tr>
`;

            table.insertAdjacentHTML('beforeend', row);
        });

    } catch (err) {
        console.error('Error loading stakes');
    }
}
document.addEventListener('DOMContentLoaded', loadStakes);
let selectedLock = null;

const modal = document.getElementById('stakeModal');
const backdrop = document.getElementById('stakeBackdrop');
const content = document.getElementById('stakeContent');

document.getElementById('open-stake-modal').onclick = () => {
    modal.classList.remove('hidden');

    setTimeout(() => {
        backdrop.classList.remove('opacity-0');
        content.classList.remove('scale-95', 'opacity-0');
    }, 10);
};

              
backdrop.onclick = closeModal;
content.querySelector('.modal-dismiss').onclick = closeModal;
document.addEventListener('keydown', e => {if(e.key==='Escape' && !modal.classList.contains('hidden')) closeModal();});

function closeModal() {
    backdrop.classList.add('opacity-0');
    content.classList.add('scale-95', 'opacity-0');

    setTimeout(() => modal.classList.add('hidden'), 300);
}

                 
document.querySelectorAll('.lock-btn').forEach(btn => {
    btn.className = "lock-btn bg-white/10 py-2 rounded-lg hover:bg-white/20 transition-all";

    btn.onclick = () => {
        selectedLock = btn.dataset.lock;

        document.querySelectorAll('.lock-btn').forEach(b => {
            b.classList.remove('bg-primary', 'text-white');
            b.classList.add('bg-white/10');
        });

        btn.classList.remove('bg-white/10');
        btn.classList.add('bg-primary', 'text-white');
    };
});
function resetStakeModal() {
    selectedLock = null;

                   
    const btn = document.getElementById('confirm-stake');
    btn.className = "w-full bg-primary text-white py-3 rounded-lg font-bold transition-all hover:opacity-90 active:scale-95";
    btn.textContent = "Confirm Stake";
    btn.disabled = false;

                  
    document.getElementById('stake-amount').value = "";

                         
    document.querySelectorAll('.lock-btn').forEach(b => {
        b.classList.remove('bg-primary', 'text-white');
        b.classList.add('bg-white/10');
    });
}
document.getElementById('confirm-stake').onclick = async () => {

    const amountInput =
        document.getElementById('stake-amount');

    const amount =
        parseFloat(amountInput.value);

    const btn =
        document.getElementById('confirm-stake');

    if (!selectedLock || !amount) return;

    btn.className =
        "w-full bg-primary text-white py-3 rounded-lg font-bold transition-all";

    btn.textContent = "Staking...";
    btn.disabled = true;

    try {

        const res = await fetch(
            "https://wallet.veneralabs.org/dstaked",
            {
                method: "POST",

                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${JWT_TOKEN}`
                },

                body: JSON.stringify({
                    lock: parseInt(selectedLock),
                    amount: document.getElementById("stake-amount").value.trim()
                })
            }
        );

        const data = await res.json();

        if (data.error === "Token expired") {
            window.location.href = "wallet.html";
            return;
        }

        if (res.ok) {

            btn.className =
                "w-full py-3 rounded-lg font-bold bg-green-200 text-green-800";

            btn.textContent =
                "Staked successfully";

            loadStakes();

            setTimeout(() => {

                resetStakeModal();
                closeModal();

            }, 1500);

        } else {

            throw new Error(
                data.error || "Stake failed"
            );

        }

    } catch (err) {

        console.error("error");

        btn.className =
            "w-full py-3 rounded-lg font-bold bg-red-200 text-red-800";

        btn.textContent =
            "Failed to stake";

        setTimeout(() => {

            resetStakeModal();

        }, 2000);
    }
};
