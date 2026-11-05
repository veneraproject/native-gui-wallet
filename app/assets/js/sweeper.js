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
let audioContext = null;


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

function playTileClickSound() {
    const ctx = initAudio();
    if (!ctx) return;

    const now = ctx.currentTime;

                    
    const osc1 = ctx.createOscillator();
    const gain1 = ctx.createGain();

    osc1.type = "sine";
    osc1.frequency.setValueAtTime(420, now);
    osc1.frequency.exponentialRampToValueAtTime(620, now + 0.07);

    gain1.gain.setValueAtTime(0.001, now);
    gain1.gain.linearRampToValueAtTime(0.075, now + 0.008);
    gain1.gain.exponentialRampToValueAtTime(0.001, now + 0.16);

    osc1.connect(gain1);
    gain1.connect(ctx.destination);

    osc1.start(now);
    osc1.stop(now + 0.17);

                        
    const osc2 = ctx.createOscillator();
    const gain2 = ctx.createGain();

    osc2.type = "sine";
    osc2.frequency.setValueAtTime(900, now + 0.025);
    osc2.frequency.exponentialRampToValueAtTime(1250, now + 0.09);

    gain2.gain.setValueAtTime(0.001, now + 0.025);
    gain2.gain.linearRampToValueAtTime(0.035, now + 0.04);
    gain2.gain.exponentialRampToValueAtTime(0.001, now + 0.15);

    osc2.connect(gain2);
    gain2.connect(ctx.destination);

    osc2.start(now + 0.025);
    osc2.stop(now + 0.16);
}


function playRewardSound() {
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


function playBombSound() {
    const ctx = initAudio();
    if (!ctx) return;

    const now = ctx.currentTime;

                    
    const impact = ctx.createOscillator();
    const impactGain = ctx.createGain();

    impact.type = "triangle";
    impact.frequency.setValueAtTime(180, now);
    impact.frequency.exponentialRampToValueAtTime(70, now + 0.18);

    impactGain.gain.setValueAtTime(0.001, now);
    impactGain.gain.linearRampToValueAtTime(0.25, now + 0.01);
    impactGain.gain.exponentialRampToValueAtTime(0.001, now + 0.35);

    impact.connect(impactGain);
    impactGain.connect(ctx.destination);

    impact.start(now);
    impact.stop(now + 0.4);

                         
    const tone = ctx.createOscillator();
    const toneGain = ctx.createGain();

    tone.type = "sine";
    tone.frequency.setValueAtTime(420, now);
    tone.frequency.exponentialRampToValueAtTime(180, now + 0.3);

    toneGain.gain.setValueAtTime(0.001, now);
    toneGain.gain.linearRampToValueAtTime(0.13, now + 0.015);
    toneGain.gain.exponentialRampToValueAtTime(0.001, now + 0.4);

    tone.connect(toneGain);
    toneGain.connect(ctx.destination);

    tone.start(now);
    tone.stop(now + 0.45);
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

(() => {
    const playButton = document.getElementById("coinsweeper-play");
    const againButton = document.getElementById("coinsweeper-again");
    const takeButton = document.getElementById("coinsweeper-take");

    const startScreen = document.getElementById("coinsweeper-start");
    const gameScreen = document.getElementById("coinsweeper-game");
    const resultScreen = document.getElementById("coinsweeper-result");

    const grid = document.getElementById("coinsweeper-grid");
    const stats = document.getElementById("coinsweeper-stats");

    const rewardCountEl = document.getElementById("cs-reward-count");
    const sweepsEl = document.getElementById("cs-sweeps");

    const messageEl = document.getElementById("coinsweeper-message");
    const errorEl = document.getElementById("coinsweeper-error");
    const resultContent = document.getElementById("coinsweeper-result-content");

    let gameId = null;
    let rewardCount = 0;
    let basicCount = 0;
    let platinumCount = 0;
    let revealedTiles = new Set();
    let gameOver = false;
    let requestInProgress = false;


    function showError(message) {
        errorEl.textContent = message;
        errorEl.classList.remove("hidden");
    }


    function clearError() {
        errorEl.textContent = "";
        errorEl.classList.add("hidden");
    }


    function resetState() {
        gameId = null;
        rewardCount = 0;
        basicCount = 0;
        platinumCount = 0;
        revealedTiles = new Set();
        gameOver = false;
        requestInProgress = false;

        rewardCountEl.textContent = "0";
        messageEl.textContent = "";
        grid.innerHTML = "";
        takeButton.disabled = true;
    }


    async function startGame() {
        if (requestInProgress) return;

        requestInProgress = true;
        clearError();

        playButton.disabled = true;
        playButton.classList.add("opacity-50");

        try {
            const response = await fetch(`${API_BASE}/drewards/coinsweeper`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${JWT_TOKEN}`
                }
            });

            const data = await response.json();

            if (!response.ok) {
                throw new Error(data.error || "Unable to start Coinsweeper.");
            }

            gameId = data.game_id;

            resetState();
            gameId = data.game_id;

            sweepsEl.textContent = data.sweeps_remaining ?? 0;

            startScreen.classList.add("hidden");

            gameScreen.classList.remove("hidden");
            gameScreen.classList.add("flex");

            stats.classList.remove("hidden");
            stats.classList.add("flex");

            createGrid(data.tiles);
            if(data.resumed){basicCount=data.basic||0;platinumCount=data.platinum||0;rewardCount=basicCount+platinumCount;
              for(const [key,kind] of Object.entries(data.revealed_tiles||{})){const tile=document.querySelector('[data-tile="'+key+'"]');if(tile){revealedTiles.add(key);tile.disabled=true;revealReward(tile,kind);}}
              const countNode=document.getElementById('cs-reward-count');if(countNode)countNode.textContent=rewardCount;if(rewardCount>=5){takeButton.disabled=false;messageEl.textContent='You have enough rewards to cash out.';}
            }

        } catch (error) {
            showError(safeWalletError(error));
        } finally {
            requestInProgress = false;
            playButton.disabled = false;
            playButton.classList.remove("opacity-50");
        }
    }


function createGrid(tiles) {
    grid.innerHTML = "";

    const HOLE_X = [
        10.35,
        26.00,
        41.70,
        57.35,
        73.05,
        88.70
    ];

    const HOLE_Y = [
        9.20,
        24.10,
        39.25,
        54.50,
        69.85,
        85.30
    ];

                                   
    const TILE_SIZE = 11.25;

    tiles.forEach((tileId, index) => {
        const tile = document.createElement("button");

        const row = Math.floor(index / 6);
        const col = index % 6;

        tile.type = "button";
        tile.dataset.tile = tileId;

          
                                 
          
               
                                
                                
          
                  
                               
                                 
           

        const columnFromCenter = (col - 2.5) / 2.5;
        const rowFromCenter = (row - 2.5) / 2.5;

        const perspectiveX =
            columnFromCenter *
            rowFromCenter *
            0.85;

        const x = HOLE_X[col] + perspectiveX;

          
                                           
                                                             
           
        const rowYCorrection = [
            0.45,
            0.22,
            0.08,
            0.00,
            0.02,
            0.10
        ];

        const y =
            HOLE_Y[row] +
            rowYCorrection[row];

        tile.style.position = "absolute";
        tile.style.left = `${x}%`;
        tile.style.top = `${y}%`;

        tile.style.translate = "-50% -50%";

        tile.style.width = `${TILE_SIZE}%`;
        tile.style.height = `${TILE_SIZE}%`;

tile.className = `
    group
    relative
    rounded-full
    flex items-center justify-center
    bg-transparent
    border-0
    outline-none
    cursor-pointer
    transition-all duration-200
    opacity-0
    scale-75
    hover:scale-105
    active:scale-95
`;

tile.innerHTML = `
    <span
        class="
            cs-question
            material-symbols-outlined
            pointer-events-none
            text-white/95
            text-[clamp(22px,3.2vw,34px)]
            drop-shadow-[0_3px_2px_rgba(0,0,0,0.65)]
        "
        style="animation-delay: -${(index * 0.17).toFixed(2)}s;"
    >
        help
    </span>
`;

        tile.addEventListener("click", () => revealTile(tile));

        grid.appendChild(tile);

        setTimeout(() => {
            tile.classList.remove("opacity-0", "scale-75");
            tile.classList.add("opacity-100", "scale-100");
        }, index * 35);
    });
}

async function playDigAnimation(tile) {
    const question = tile.querySelector(".cs-question");

    tile.classList.add("cs-hole-dig");

    if (question) {
        question.classList.add("cs-question-dig");
    }

    spawnDirtParticles(tile);

    await new Promise(resolve => setTimeout(resolve, 170));

    if (question) {
        question.remove();
    }

    setTimeout(() => {
        tile.classList.remove("cs-hole-dig");
    }, 200);
}
    async function revealTile(tile) {
        if (gameOver || requestInProgress) return;
        if (tile.disabled) return;

        const tileId = tile.dataset.tile;

        if (revealedTiles.has(tileId)) return;

        requestInProgress = true;
        tile.disabled = true;

                                 
        await playDigAnimation(tile);

        try {
            const response = await fetch(`${API_BASE}/drewards/sweep`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${JWT_TOKEN}`
                },
                body: JSON.stringify({
                    game_id: gameId,
                    tile: Number(tileId)
                })
            });

            const data = await response.json();

            if (!response.ok) {
                throw new Error(data.error || "Unable to sweep tile.");
            }

            revealedTiles.add(tileId);

            if (data.result === "reward") {
                revealReward(tile, data.reward);
                playTileClickSound() 
                rewardCount++;
                rewardCountEl.textContent = rewardCount;

                if (data.reward === "basic") {
                    basicCount++;
                }

                if (data.reward === "platinum") {
                    platinumCount++;
                }

                if (rewardCount >= 5) {
                    takeButton.disabled = false;
                    messageEl.textContent = "You have enough rewards to cash out.";
                } else {
                    messageEl.textContent =
                        `${5 - rewardCount} more reward${5 - rewardCount === 1 ? "" : "s"} needed.`;
                }

            } else if (data.result === "bomb") {
                revealBomb(tile);
                playBombSound();
                gameOver = true;

                messageEl.textContent = "Sweeping over";
                messageEl.className =
                    "min-h-[20px] text-sm text-red-400 font-semibold text-center";

                                               
                await revealAllTiles(data.board);

                setTimeout(() => {
                    endGame(false);
                }, 900);
            }

        } catch (error) {
            tile.disabled = false;
            messageEl.textContent = safeWalletError(error);
            messageEl.className =
                "min-h-[20px] text-sm text-red-400 font-medium text-center";
        } finally {
            requestInProgress = false;
            tile.classList.remove("scale-95");
        }
    }


function revealReward(tile, reward) {
    tile.disabled = true;

    tile.className = `
        relative
        rounded-full
        flex items-center justify-center
        bg-transparent
        border-0
        outline-none
        transition-all duration-300
        scale-100
    `;

    tile.innerHTML = `
        <div class="relative flex items-center justify-center w-full h-full">
            <img
                src="${reward === "platinum" ? "3.png" : "1.png"}"
                alt="${reward}"
                class="
                    cs-reward-reveal
                    w-[60%] h-[60%]
                    object-contain
                    drop-shadow-[0_7px_6px_rgba(0,0,0,0.55)]
                "
            >
        </div>
    `;
}


function revealBomb(tile) {
    tile.disabled = true;

    tile.className = `
        relative
        rounded-full
        flex items-center justify-center
        bg-transparent
        border-0
        outline-none
        transition-all duration-300
    `;

    tile.innerHTML = `
        <div class="relative flex items-center justify-center w-full h-full">
            <span class="material-symbols-outlined cs-bomb-reveal text-[clamp(30px,5vw,54px)] text-red-400 drop-shadow-[0_7px_7px_rgba(0,0,0,0.6)]">bomb</span>
        </div>
    `;

    const board = document.getElementById("coinsweeper-board");

    if (board) {
        board.classList.remove("cs-board-shake");

                                      
        void board.offsetWidth;

        board.classList.add("cs-board-shake");

        setTimeout(() => {
            board.classList.remove("cs-board-shake");
        }, 400);
    }
}


    async function revealAllTiles(board) {
        if (!board) return;

        const buttons = [...grid.querySelectorAll("button")];

        for (let i = 0; i < buttons.length; i++) {
            const tile = buttons[i];
            const tileId = Number(tile.dataset.tile);

            if (revealedTiles.has(String(tileId))) {
                continue;
            }

            const tileResult = board[String(tileId)];

            if (!tileResult) continue;

            await new Promise(resolve => setTimeout(resolve, 45));

            if (tileResult === "bomb") {
                revealBomb(tile);
            } else {
                revealReward(tile, tileResult);
            }

            revealedTiles.add(String(tileId));
        }
    }


    async function takeRewards() {
        if (rewardCount < 5 || !gameId || requestInProgress) {
            return;
        }

        requestInProgress = true;
        takeButton.disabled = true;

        try {
            const response = await fetch(`${API_BASE}/drewards/sweep`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${JWT_TOKEN}`
                },
                body: JSON.stringify({
                    game_id: gameId,
                    action: "take"
                })
            });

            const data = await response.json();

            if (!response.ok) {
                throw new Error(data.error || "Unable to take rewards.");
            }

            basicCount = data.basic;
            platinumCount = data.platinum;

            endGame(true);

        } catch (error) {
            messageEl.textContent = safeWalletError(error);
            takeButton.disabled = false;
        } finally {
            requestInProgress = false;
        }
    }

function spawnDirtParticles(tile) {
    const particles = 10;

    for (let i = 0; i < particles; i++) {
        const particle = document.createElement("span");

        particle.className = "cs-dirt-particle";

        const angle = (Math.PI * 2 * i) / particles
            + (Math.random() * 0.45);

        const distance = 18 + Math.random() * 22;

        const dx = Math.cos(angle) * distance;
        const dy = Math.sin(angle) * distance;

        particle.style.setProperty("--dx", `${dx}px`);
        particle.style.setProperty("--dy", `${dy}px`);

        const size = 3 + Math.random() * 4;

        particle.style.width = `${size}px`;
        particle.style.height = `${size}px`;

        const dirtColors = [
            "#6f4b2c",
            "#80552d",
            "#5c3b22",
            "#93623a"
        ];

        particle.style.background =
            dirtColors[Math.floor(Math.random() * dirtColors.length)];

        particle.style.animationDelay =
            `${Math.random() * 40}ms`;

        tile.appendChild(particle);

        setTimeout(() => {
            particle.remove();
        }, 500);
    }
}
    function endGame(won) {
        gameOver = true;

        gameScreen.classList.add("hidden");
        gameScreen.classList.remove("flex");

        stats.classList.add("hidden");
        stats.classList.remove("flex");

        resultScreen.classList.remove("hidden");
        resultScreen.classList.add("flex");

        if (won) {
            resultContent.innerHTML = `
                <div class="flex flex-col gap-3 mt-3">

                    <div class="flex items-center justify-center gap-3">
                        <span class="text-base font-bold">
                            ${basicCount}x
                        </span>

                        <img
                            src="1.png"
                            alt="Basic crate"
                            class="w-10 h-10 object-contain"
                        >

                        <span class="text-on-surface-variant">
                            Basic
                        </span>
                    </div>

                    <div class="flex items-center justify-center gap-3">
                        <span class="text-base font-bold">
                            ${platinumCount}x
                        </span>

                        <img
                            src="3.png"
                            alt="Platinum crate"
                            class="w-10 h-10 object-contain"
                        >

                        <span class="text-on-surface-variant">
                            Platinum
                        </span>
                    </div>

                </div>
            `;
        } else {
            resultContent.innerHTML = `
                <div class="text-red-400 font-semibold">
                    Sweeping over
                </div>

                <div class="mt-2">
                    You found ${rewardCount} reward${rewardCount === 1 ? "" : "s"}
                    before hitting a bomb.
                </div>
            `;
        }
    }


    function resetGameUI() {
        resetState();

        resultScreen.classList.add("hidden");
        resultScreen.classList.remove("flex");

        gameScreen.classList.add("hidden");
        gameScreen.classList.remove("flex");

        stats.classList.add("hidden");
        stats.classList.remove("flex");

        startScreen.classList.remove("hidden");

        playButton.disabled = false;
    }


    playButton.addEventListener("click", startGame);
    takeButton.addEventListener("click", takeRewards);
    againButton.addEventListener("click", resetGameUI);

})();

