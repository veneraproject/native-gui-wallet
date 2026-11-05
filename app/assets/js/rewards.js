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
async function loadAvailableCrates() {
    const controller = new AbortController();
    const timeout = setTimeout(() => controller.abort(), 10000);
    try {
        const response = await fetch(`${API_BASE}/dgold`, {
            signal: controller.signal,
            method: "POST",
            
            headers: {
                "Accept": "application/json", "Content-Type": "application/json", "Authorization": `Bearer ${JWT_TOKEN}`
            }
        });

        const data = await response.json();

        if (handleJwtFailure(response, data)) return;
        if (!response.ok) {
            throw new Error(data.error || "Failed to load crate availability");
        }

        document.getElementById("basic-count").textContent = `${Number(data.gold || 0)} available`;
        document.getElementById("platinum-count").textContent = `${Number(data.platinum || 0)} available`;
        document.getElementById("tether-count").textContent = `${Number(data.tether || 0)} available`;
        document.getElementById("supreme-count").textContent = `${Number(data.supreme || 0)} available`;

    } catch (error) {
        console.error("Failed to load crate availability:", error);

        document.getElementById("basic-count").textContent = "0 available";
        document.getElementById("platinum-count").textContent = "0 available";
        document.getElementById("tether-count").textContent = "0 available";
        document.getElementById("supreme-count").textContent = "0 available";
    } finally {
        clearTimeout(timeout);
    }
}
loadAvailableCrates();


function initAudio() {
    const AudioContextClass = window.AudioContext || window.webkitAudioContext;

    if (!AudioContextClass) return null;

    if (!audioContext) {
        audioContext = new AudioContextClass();
    }

    if (audioContext.state === "suspended") {
        audioContext.resume().catch(() => {});
    }

    return audioContext;
}

let audioContext = null;
const delay = ms => new Promise(resolve => setTimeout(resolve, ms));
const reducedMotion = () => window.matchMedia("(prefers-reduced-motion: reduce)").matches;
function claimParticles(card) {
    if (reducedMotion()) return;
    for (let i = 0; i < 24; i++) {
        const particle = document.createElement("span");
        particle.className = "crate-particle";
        const angle = (i / 24) * Math.PI * 2;
        const distance = 65 + Math.random() * 90;
        particle.style.setProperty("--x", `${Math.cos(angle) * distance}px`);
        particle.style.setProperty("--y", `${Math.sin(angle) * distance}px`);
        particle.style.setProperty("--delay", `${Math.random() * 120}ms`);
        card.appendChild(particle);
        setTimeout(() => particle.remove(), 1500);
    }
}
document.querySelectorAll(".crate-card").forEach(card => {
    const type = card.dataset.crate;
    if (type === "supreme") return;
    const reward = card.querySelector(".crate-reward");
    const progressBar = card.querySelector(".crate-progress-bar");
    const originalReward = reward.innerHTML;
    reward.setAttribute("aria-live", "polite");
    card.addEventListener("click", async () => {
        if (card.dataset.cooldown === "true") return;
        const endpoint = { basic: "dcrate", platinum: "dplatinum", tether: "dtether" }[type];
        if (!endpoint) return;
        card.dataset.cooldown = "true";
        card.disabled = true;
        card.setAttribute("aria-busy", "true");
        card.classList.add("crate-opening");
                                                                              
        try { initAudio(); } catch (_) {}
        try {
            const [result] = await Promise.all([openCrate(`${API_BASE}/${endpoint}`), delay(reducedMotion() ? 0 : 850)]);
            card.classList.remove("crate-opening");
            if (!result.success) {
                const message = document.createElement("div");
                message.className = "text-sm font-bold text-red-400";
                message.textContent = result.error;
                reward.replaceChildren(message);
            } else {
                                                                               
                reward.innerHTML = originalReward;
                try { playClaimSound(); } catch (_) {}
                claimParticles(card);
            }
            card.classList.add("crate-revealed");
            progressBar.style.transition = "none";
            progressBar.style.width = "100%";
            void progressBar.offsetWidth;
            progressBar.style.transition = "width 3200ms linear";
            progressBar.style.width = "0%";
            await loadAvailableCrates();
            await delay(3200);
        } catch (error) {
            console.error("Crate presentation failed:", error);
        } finally {
            card.classList.remove("crate-opening", "crate-revealed", "pointer-events-none");
            reward.innerHTML = originalReward;
            progressBar.style.transition = "none";
            progressBar.style.width = "0%";
            card.dataset.cooldown = "false";
            card.disabled = false;
            card.removeAttribute("aria-busy");
            card.querySelectorAll(".crate-particle").forEach(p => p.remove());
        }
    });
});
async function openCrate(endpoint) {
    const controller = new AbortController();
    const timeout = setTimeout(() => controller.abort(), 25000);
    try {
        const response = await fetch(endpoint, {
            signal: controller.signal,
            method: "POST",
            
            headers: {
                "Accept": "application/json", "Content-Type": "application/json", "Authorization": `Bearer ${JWT_TOKEN}`
            }
        });

        let data = {};

        try {
            data = await response.json();
        } catch (_) {}

        if (handleJwtFailure(response, data)) return { success: false, error: "Session expired" };
        if (!response.ok || data.success === false || data.error) {
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
            error: safeWalletError(error, "Unable to confirm the result. Refresh your crate count before trying again.")
        };
    } finally {
        clearTimeout(timeout);
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