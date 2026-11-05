const API_BASE = "https://wallet.veneralabs.org";
const JWT_TOKEN = localStorage.getItem("jwt_token");
if (!JWT_TOKEN) window.location.href = "wallet.html";
const authHeaders = (extra = {}) => ({ ...extra, "Authorization": `Bearer ${JWT_TOKEN}` });
async function guiLogout() {
  try { if (JWT_TOKEN) await fetch(`${API_BASE}/dinvalidate`, { method: "POST", headers: authHeaders() }); } catch (_) {}
  localStorage.removeItem("jwt_token");
  window.location.href = "wallet.html";
}
function handleJwtFailure(response, data) {
  if (response.status === 401 || data?.error === "Token expired" || data?.error === "Invalid token") {
    localStorage.removeItem("jwt_token"); window.location.href = "wallet.html"; return true;
  }
  return false;
}

document.getElementById("sign-out-btn")?.addEventListener("click", guiLogout);
const caseRewards = {
    1: { image: "1.png", name: "Basic", weight: 94.15 },
    2: { image: "2.png", name: "Tether", weight: 0.8 },
    3: { image: "3.png", name: "Platinum", weight: 5 },
    4: { image: "4.png", name: "Supreme", weight: 0.05 }
};

const visualRewards = [
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,
    2,2,2,2,
    3,3,
    4
];

const caseReel = document.getElementById("case-reel");
const caseWindow = document.getElementById("case-window");
const spinButton = document.getElementById("case-play");
const spinButtonText = document.getElementById("case-play-text");
const resultPlaceholder = document.getElementById("case-result-placeholder");
const resultContent = document.getElementById("case-result-content");
const resultName = document.getElementById("case-result-name");
const spinsElement = document.getElementById("spins");

let spinning = false;
let animationFrame = null;
let audioContext = null;
let lastCenterIndex = -1;
let currentOffset = 0;

const ITEMS_PER_SPIN = 45;
const SPIN_DURATION = 9000;


                                                            
                
                                                            

function getVisualReward() {
    return visualRewards[Math.floor(Math.random() * visualRewards.length)];
}


                                                            
                   
                                                            

function createReelItem(rewardNumber, index) {
    const reward = caseRewards[rewardNumber];
    const item = document.createElement("div");

    item.className = "case-item venus-case-item shrink-0 flex items-center justify-center mx-1.5 md:mx-2";

    item.innerHTML = `
        <div class="venus-segment">
            <div class="venus-segment-divider venus-segment-divider-left"></div>
            <div class="venus-segment-divider venus-segment-divider-right"></div>

            <div class="venus-token">
                <img
                    src="${reward.image}"
                    alt="${reward.name}"
                    class="max-w-[82%] max-h-[82%] object-contain drop-shadow-[0_8px_16px_rgba(0,0,0,0.4)]"
                >
            </div>
        </div>
    `;

    item.dataset.index = index;
    item.dataset.rewardNumber = String(rewardNumber);
    item.dataset.image = reward.image;

    return item;
}


                                                            
                    
                                                            

function appendReelItems(count, winningIndex = null, winningReward = null) {
    const startIndex = caseReel.children.length;

    for (let i = 0; i < count; i++) {
        const absoluteIndex = startIndex + i;

        let rewardNumber;

        if (winningIndex !== null && absoluteIndex === winningIndex) {
            rewardNumber = winningReward;
        } else {
            rewardNumber = getVisualReward();
        }

        caseReel.appendChild(
            createReelItem(rewardNumber, absoluteIndex)
        );
    }
}


                                                            
               
                                                            

function initializeReel() {
    caseReel.innerHTML = "";
    currentOffset = 0;

    appendReelItems(80);

    caseReel.style.transition = "none";
    caseReel.style.transform = "translate3d(0, 0, 0)";
}


                                                            
              
                                                            

function getCenterItemIndex() {
    if (!caseReel.children.length) return -1;

    const windowRect = caseWindow.getBoundingClientRect();
    const centerX = windowRect.left + windowRect.width / 2;

    let closestIndex = -1;
    let closestDistance = Infinity;

    Array.from(caseReel.children).forEach((item, index) => {
        const rect = item.getBoundingClientRect();
        const itemCenter = rect.left + rect.width / 2;
        const distance = Math.abs(itemCenter - centerX);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = index;
        }
    });

    return closestIndex;
}


                                                            
                
                                                            

function getTargetOffset(item) {
    const itemRect = item.getBoundingClientRect();
    const windowRect = caseWindow.getBoundingClientRect();

    const itemCenter = itemRect.left + itemRect.width / 2;
    const windowCenter = windowRect.left + windowRect.width / 2;

    const currentTransform = getComputedStyle(caseReel).transform;
    const currentX = currentTransform === "none"
        ? 0
        : new DOMMatrix(currentTransform).m41;

    const adjustment = windowCenter - itemCenter;

    return currentX + adjustment;
}


                                                            
        
                                                            

function initAudio() {
    const AudioContextClass = window.AudioContext || window.webkitAudioContext;

    if (!AudioContextClass) return null;

    if (!audioContext) {
        audioContext = new AudioContextClass();
    }

    if (audioContext.state === "suspended") {
        audioContext.resume();
    }

    return audioContext;
}


function playTick() {
    const ctx = initAudio();
    if (!ctx) return;

    const now = ctx.currentTime;

    const oscillator = ctx.createOscillator();
    const gain = ctx.createGain();

    oscillator.type = "square";
    oscillator.frequency.setValueAtTime(1050, now);
    oscillator.frequency.exponentialRampToValueAtTime(620, now + 0.025);

    gain.gain.setValueAtTime(0.028, now);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.045);

    oscillator.connect(gain);
    gain.connect(ctx.destination);

    oscillator.start(now);
    oscillator.stop(now + 0.05);
}




document.querySelectorAll(".crate-card").forEach(card => {
    card.addEventListener("click", async () => {
        if (card.dataset.cooldown === "true") return;

        const type = card.dataset.crate;
        const img = card.querySelector(".crate-img");
        const reward = card.querySelector(".crate-reward");
        const progress = card.querySelector(".crate-progress");
        const progressBar = card.querySelector(".crate-progress-bar");

        if (!img || !reward || !progress || !progressBar) return;

                                         
        if (type === "supreme") return;

        let endpoint = null;

        if (type === "basic") endpoint = `${API_BASE}/dcrate`;
        if (type === "platinum") endpoint = `${API_BASE}/dplatinum`;
        if (type === "tether") endpoint = `${API_BASE}/dtether`;

        if (!endpoint) return;

        card.dataset.cooldown = "true";
        card.classList.add("pointer-events-none");



                       
        const result = await openCrate(endpoint);

        if (!result.success) {
            reward.textContent = "No crates available";

            reward.classList.remove(
                "scale-0",
                "opacity-0",
                "text-on-surface"
            );

            reward.classList.add(
                "scale-100",
                "opacity-100",
                "text-red-400"
            );

            img.classList.remove("scale-100", "opacity-100");
            img.classList.add("scale-0", "opacity-0");

            progress.classList.remove("opacity-0");

            progressBar.style.transition = "none";
            progressBar.style.width = "100%";

            void progressBar.offsetWidth;

            progressBar.style.transition = "width 6500ms linear";
            progressBar.style.width = "0%";

            await new Promise(resolve => setTimeout(resolve, 6500));

            reward.classList.remove(
                "scale-100",
                "opacity-100"
            );

            reward.classList.add(
                "scale-0",
                "opacity-0"
            );

            reward.classList.remove("text-red-400");
            reward.classList.add("text-on-surface");

            img.classList.remove("scale-0", "opacity-0");
            img.classList.add("scale-100", "opacity-100");

            progress.classList.add("opacity-0");

            progressBar.style.transition = "none";
            progressBar.style.width = "100%";

            card.dataset.cooldown = "false";
            card.classList.remove("pointer-events-none");

            await loadAvailableCrates();

            return;
        }

                playClaimSound();
        reward.classList.remove(
            "scale-0",
            "opacity-0",
            "text-red-400"
        );

        reward.classList.add(
            "scale-100",
            "opacity-100",
            "text-on-surface"
        );

        img.classList.remove("scale-100", "opacity-100");
        img.classList.add("scale-0", "opacity-0");

        progress.classList.remove("opacity-0");

        progressBar.style.transition = "none";
        progressBar.style.width = "100%";

        void progressBar.offsetWidth;

        progressBar.style.transition = "width 6500ms linear";
        progressBar.style.width = "0%";

        await new Promise(resolve => setTimeout(resolve, 6500));

        reward.classList.remove(
            "scale-100",
            "opacity-100"
        );

        reward.classList.add(
            "scale-0",
            "opacity-0"
        );

        img.classList.remove("scale-0", "opacity-0");
        img.classList.add("scale-100", "opacity-100");

        progress.classList.add("opacity-0");

        progressBar.style.transition = "none";
        progressBar.style.width = "100%";

        card.dataset.cooldown = "false";
        card.classList.remove("pointer-events-none");

                                             
        await loadAvailableCrates();
    });
});
async function openCrate(endpoint) {
    try {
        const response = await fetch(endpoint, {
            method: "POST",
            
            headers: {
                "Accept": "application/json",
                "Authorization": `Bearer ${JWT_TOKEN}`
            }
        });

        let data = {};

        try {
            data = await response.json();
        } catch (_) {}

        if (!response.ok) {
            throw new Error(data.error || "No crates available");
        }

        return {
            success: true,
            data
        };

    } catch (error) {
        console.error(`Crate open failed (${endpoint}):`, error);

        return {
            success: false,
            error: safeWalletError(error)
        };
    }
}
function playWinSound() {
    const ctx = initAudio();
    if (!ctx) return;

    const now = ctx.currentTime;

    const oscillator = ctx.createOscillator();
    const gain = ctx.createGain();

    oscillator.type = "sine";
    oscillator.frequency.setValueAtTime(500, now);
    oscillator.frequency.exponentialRampToValueAtTime(900, now + 0.2);

    gain.gain.setValueAtTime(0.001, now);
    gain.gain.linearRampToValueAtTime(0.08, now + 0.03);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.45);

    oscillator.connect(gain);
    gain.connect(ctx.destination);

    oscillator.start(now);
    oscillator.stop(now + 0.5);
}
function playClaimSound() {
    const ctx = initAudio();
    if (!ctx) return;

    const now = ctx.currentTime;

    const oscillator = ctx.createOscillator();
    const gain = ctx.createGain();

    oscillator.type = "sine";
    oscillator.frequency.setValueAtTime(600, now);
    oscillator.frequency.exponentialRampToValueAtTime(1100, now + 0.16);

    gain.gain.setValueAtTime(0.001, now);
    gain.gain.linearRampToValueAtTime(0.1, now + 0.025);
    gain.gain.exponentialRampToValueAtTime(0.001, now + 0.4);

    oscillator.connect(gain);
    gain.connect(ctx.destination);

    oscillator.start(now);
    oscillator.stop(now + 0.45);
}

                                                            
                     
                                                            

function startTickSync() {
    lastCenterIndex = getCenterItemIndex();

    function update() {
        if (!spinning) return;

        const currentIndex = getCenterItemIndex();

        if (currentIndex !== -1 && currentIndex !== lastCenterIndex) {
            playTick();
            lastCenterIndex = currentIndex;
        }

        animationFrame = requestAnimationFrame(update);
    }

    animationFrame = requestAnimationFrame(update);
}


function stopTickSync() {
    if (animationFrame) {
        cancelAnimationFrame(animationFrame);
        animationFrame = null;
    }
}


                                                            
             
                                                            

async function loadSpins() {
    if (!spinsElement) return;

    try {
        const response = await fetch(`${API_BASE}/drewards/me`, {
            method: "GET",
            
            headers: {
                "Accept": "application/json",
                "Authorization": `Bearer ${JWT_TOKEN}`
            }
        });

        const data = await response.json();

        if (!response.ok) {
            throw new Error(data.error || "Failed to load spins");
        }

        const spins = Math.max(0, Number(data.spins) || 0);

        spinsElement.textContent =
            `${spins} available spin${spins === 1 ? "" : "s"}`;

        updateButtonState(spins);

    } catch (error) {
        console.error("Failed to load spins:", error);
        spinsElement.textContent = "Unable to load spins";
    }
}


                                                            
               
                                                            

function updateButtonState(spins) {
    if (spinning) return;

    if (spins <= 0) {
        spinButton.disabled = true;
        spinButton.classList.add("opacity-50", "cursor-not-allowed");
    } else {
        spinButton.disabled = false;
        spinButton.classList.remove("opacity-50", "cursor-not-allowed");
    }
}


                                                            
               
                                                            

async function requestSpinResult() {
    const response = await fetch(`${API_BASE}/drewards/spin`, {
        method: "POST",
        
        headers: {
            "Content-Type": "application/json",
            "Accept": "application/json",
            "Authorization": `Bearer ${JWT_TOKEN}`
        }
    });

    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Unable to spin");
    }

    const reward = Number(data.reward);

    if (![1, 2, 3, 4].includes(reward)) {
        throw new Error("Invalid reward returned by server");
    }

    return {
        reward,
        hash: data.hash,
        spinsRemaining: Number(data.spins_remaining)
    };
}


                                                            
              
                                                            

function showReward(rewardNumber, hash) {
    const reward = caseRewards[rewardNumber];

    if (!reward) return;

    resultName.textContent = reward.name;

    resultPlaceholder.classList.add("hidden");
    resultContent.classList.remove("hidden");

    console.log("Reward:", rewardNumber);
    console.log("Hash:", hash);
}


                                                            
                    
                                                            

function prepareNextSpin(serverReward) {
      
                                               
       
    const currentCenter = getCenterItemIndex();

      
                                                      
                       
       
    const startIndex = caseReel.children.length;

      
                                                          
                                                          
       
    const winningIndex =
        startIndex + ITEMS_PER_SPIN - 5;

      
                                      
       
    appendReelItems(
        ITEMS_PER_SPIN,
        winningIndex,
        serverReward
    );

    return caseReel.children[winningIndex];
}


                                                            
            
                                                            

async function playCase() {
    if (spinning) return;

    spinning = true;

    spinButton.disabled = true;
    spinButton.classList.add("opacity-60", "cursor-not-allowed");
    spinButtonText.textContent = "Opening...";

    resultPlaceholder.textContent = "Opening your case...";
    resultPlaceholder.classList.remove("hidden");
    resultContent.classList.add("hidden");

    try {
                                                            
                                       
                                                            

        const serverResult = await requestSpinResult();

          
                     
                                                       
          
                                                              
           
        const winningItem =
            prepareNextSpin(serverResult.reward);

        if (!winningItem) {
            throw new Error("Unable to prepare winning item");
        }

          
                                                        
                                          
           
        caseReel.offsetHeight;

        const targetOffset =
            getTargetOffset(winningItem);

          
                                      
           
        initAudio();
        startTickSync();

          
                                                        
                        
           
        caseReel.style.transition =
            `transform ${SPIN_DURATION}ms cubic-bezier(0.055, 0.78, 0.12, 1)`;

        caseReel.style.transform =
            `translate3d(${targetOffset}px, 0, 0)`;


                                                            
                             
                                                            

        await new Promise(resolve => {
            setTimeout(resolve, SPIN_DURATION);
        });


                                                            
                     
                                                            

        stopTickSync();


          
                                                          
          
                    
                      
           
        currentOffset = targetOffset;

        caseReel.style.transition = "none";
        caseReel.style.transform =
            `translate3d(${currentOffset}px, 0, 0)`;


                                                            
                      
                                                            

        playWinSound();

        showReward(
            serverResult.reward,
            serverResult.hash
        );


                                                            
                       
                                                            

        await loadSpins();

    } catch (error) {
        console.error("Spin failed:", error);

        stopTickSync();

        resultPlaceholder.textContent =
            safeWalletError(error);

        resultPlaceholder.classList.remove("hidden");
        resultContent.classList.add("hidden");

        await loadSpins();

    } finally {
        spinning = false;

        spinButtonText.textContent = "Roll";

        await loadSpins();
    }
}


                                                            
                  
                                                            

initializeReel();
loadSpins();

spinButton.addEventListener("click", playCase);

const rewardsToggle = document.getElementById("rewards-toggle");
const rewardsDropdown = document.getElementById("rewards-dropdown");
const rewardsChevron = document.getElementById("rewards-chevron");

let rewardsOpen = false;

function setRewardsDropdown(open) {
    rewardsOpen = open;

    if (open) {
        rewardsDropdown.classList.remove("opacity-0", "scale-95", "-translate-y-2", "pointer-events-none");
        rewardsDropdown.classList.add("opacity-100", "scale-100", "translate-y-0");
        rewardsChevron.classList.add("rotate-180");
    } else {
        rewardsDropdown.classList.remove("opacity-100", "scale-100", "translate-y-0");
        rewardsDropdown.classList.add("opacity-0", "scale-95", "-translate-y-2", "pointer-events-none");
        rewardsChevron.classList.remove("rotate-180");
    }
}

rewardsToggle.addEventListener("click", (event) => {
    event.stopPropagation();
    setRewardsDropdown(!rewardsOpen);
});

document.addEventListener("click", (event) => {
    if (!rewardsDropdown.contains(event.target) && !rewardsToggle.contains(event.target)) {
        setRewardsDropdown(false);
    }
});