const API_BASE = "https://wallet.veneralabs.org";
const JWT_TOKEN = localStorage.getItem("jwt_token");
                                                       
const authHeaders = (extra = {}) => ({ ...extra, "Authorization": `Bearer ${JWT_TOKEN}` });
async function guiLogout() {
  try { if (JWT_TOKEN) await fetch(`${API_BASE}/dinvalidate`, { method: "POST", headers: authHeaders() }); } catch (_) {}
                                         
                                         
}
function handleJwtFailure(response, data) {
  if (response.status === 401 || data?.error === "Token expired" || data?.error === "Invalid token") {
                                                                                              
  }
  return false;
}

document.getElementById("sign-out-btn")?.addEventListener("click", guiLogout);
async function loadBurnWalletBalance() {
  const el = document.getElementById("burn-available-balance");
  if (!el) return;
  try {
    const response = await fetch(`${API_BASE}/totalbalanced`, { method: "POST", headers: authHeaders({"Content-Type":"application/json"}) });
    const data = await response.json();
    if (handleJwtFailure(response, data)) return;
    const balance = Number(data.vnr_balance || 0);
    el.dataset.balance = String(balance);
    el.textContent = `${balance.toLocaleString("en-US", {maximumFractionDigits:9})} VNR`;
  } catch (_) { el.textContent = "-- VNR"; }
}
document.addEventListener("DOMContentLoaded", loadBurnWalletBalance);


        (() => {
            "use strict";

            const burnCard = document.getElementById("burn-card");
            const amountInput = document.getElementById("burn-amount");
            const burnBtn = document.getElementById("burn-btn");
            const cancelBtn = document.getElementById("burn-cancel-btn");
            const maxBtn = document.getElementById("burn-max");
            const particles = document.getElementById("burn-particles");
            const successFlame = document.getElementById("burn-success-flame");
            const burnOverlayAmountEl = document.getElementById("burn-overlay-amount");
            const statusBox = document.getElementById("burn-status");
            const statusIcon = document.getElementById("burn-status-icon");
            const statusTitle = document.getElementById("burn-status-title");
            const statusMessage = document.getElementById("burn-status-message");
            const txWrap = document.getElementById("burn-tx-wrap");
            const txHashEl = document.getElementById("burn-tx-hash");
            const copyHashBtn = document.getElementById("copy-burn-hash");
            const balanceEl = document.getElementById("burn-available-balance");
            const totalBurnedEl = document.getElementById("total-burned");
            const burnCountEl = document.getElementById("burn-count");
            const burnPointsEl = document.getElementById("burn-points");
            const estimatedBurnPointsEl = document.getElementById("estimated-burn-points");
            const burnPointRateEl = document.getElementById("burn-point-rate");
            const accumulatedUsdtEl = document.getElementById("accumulated-usdt");
            const collectYieldBtn = document.getElementById("collect-yield-btn");
            const collectYieldStatus = document.getElementById("collect-yield-status");
            const collectedYieldAmountEl = document.getElementById("collected-yield-amount");
            const collectOverlayAmountEl = document.getElementById("collect-overlay-amount");
            const burnStatsCard = document.getElementById("burn-stats-card");
            const collectParticles = document.getElementById("collect-particles");
            const collectSuccessOverlay = document.getElementById("collect-success-overlay");

            let collectSuccessHoldTimer = null;
            let collectSuccessHideTimer = null;
            let collectStatusHideTimer = null;
            let currentPointsPerVnr = null;
            let currentAccumulatedUsdt = 0;
            let collectingYield = false;
            let burning = false;
            let lastTxHash = "";
            let successFlameHoldTimer = null;
            let successFlameHideTimer = null;
            let txHashHideTimer = null;
            let burnStatusHideTimer = null;
            if (burnCard && statusBox) burnCard.appendChild(statusBox);

            function clearSuccessFlame() {
                if (successFlameHoldTimer) clearTimeout(successFlameHoldTimer);
                if (successFlameHideTimer) clearTimeout(successFlameHideTimer);
                successFlameHoldTimer = null;
                successFlameHideTimer = null;
                if (!successFlame) return;
                successFlame.classList.add("hidden");
                successFlame.classList.remove("flex", "is-entering", "is-leaving");
            }

            function clearVisualState() {
                burnCard?.classList.remove("burn-pending", "burn-success", "burn-error");
                if (burnStatusHideTimer) clearTimeout(burnStatusHideTimer);
                burnStatusHideTimer = null;
                statusBox?.classList.add("hidden");
                statusBox?.classList.remove("burn-status-hiding");
                if (txHashHideTimer) clearTimeout(txHashHideTimer);
                txHashHideTimer = null;
                txWrap?.classList.add("hidden");
                txWrap?.classList.remove("burn-tx-hiding");
                if (particles) particles.innerHTML = "";
                clearSuccessFlame();
                lastTxHash = "";
            }

            function setButtonLoading(active) {
                burning = active;
                if (!burnBtn) return;
                burnBtn.disabled = active;
                burnBtn.classList.toggle("opacity-70", active);
                burnBtn.classList.toggle("cursor-not-allowed", active);
                burnBtn.innerHTML = active
                    ? '<span class="material-symbols-outlined text-lg burn-spin">progress_activity</span><span>Burning...</span>'
                    : '<span class="material-symbols-outlined text-lg">local_fire_department</span><span>Burn VNR</span>';
            }

            function spawnParticles(kind = "success", count = 24) {
                if (!particles) return;
                particles.innerHTML = "";
                const colors = kind === "error"
                    ? ["#ff4968", "#ff6e84", "#ff894d", "#ffb35c"]
                    : ["#ff5b1f", "#ff7b26", "#ff9d38", "#ffc45a", "#ffe29a", "#fff0bf"];

                for (let i = 0; i < count; i++) {
                    const ember = document.createElement("span");
                    const isSpark = Math.random() < 0.28;
                    ember.className = `burn-particle${isSpark ? " is-spark" : ""}`;

                    const size = isSpark
                        ? 1.3 + Math.random() * 2.2
                        : 1.8 + Math.random() * 4.8;

                    ember.style.setProperty("--ember-size", `${size.toFixed(2)}px`);
                    ember.style.setProperty("--ember-x", `${(-78 + Math.random() * 156).toFixed(1)}px`);
                    ember.style.setProperty("--ember-y", `${(-125 - Math.random() * 115).toFixed(1)}px`);
                    ember.style.setProperty("--ember-rotate", `${(-110 + Math.random() * 220).toFixed(1)}deg`);
                    ember.style.setProperty("--ember-left", `${(3 + Math.random() * 94).toFixed(1)}%`);
                    ember.style.setProperty("--ember-duration", `${(1.15 + Math.random() * 1.85).toFixed(2)}s`);
                    ember.style.setProperty("--ember-delay", `${(Math.random() * .9).toFixed(2)}s`);
                    ember.style.setProperty("--ember-glow", `${(8 + Math.random() * 10).toFixed(1)}px`);
                    ember.style.setProperty("--ember-color", colors[Math.floor(Math.random() * colors.length)]);
                    particles.appendChild(ember);
                }
            }

            function playSuccessFlame(amount) {
                if (burnOverlayAmountEl) {
                    burnOverlayAmountEl.textContent = `Burned ${formatBurnAmount(amount || 0)} VNR`;
                }

                if (!successFlame) {
                    spawnParticles("success", 48);
                    return;
                }

                clearSuccessFlame();
                successFlame.classList.remove("hidden");
                successFlame.classList.add("flex");

                                                                      
                void successFlame.offsetWidth;
                successFlame.classList.add("is-entering");

                                                                    
                                                              
                successFlameHoldTimer = setTimeout(() => {
                    successFlame.classList.remove("is-entering");
                    successFlame.classList.add("is-leaving");
                    spawnParticles("success", 62);

                    successFlameHideTimer = setTimeout(() => {
                        clearSuccessFlame();
                    }, 850);
                }, 2500);
            }

            document.getElementById("dismiss-burn-status")?.addEventListener("click", () => {
                if (!burning) clearVisualState();
            });
            txHashEl?.addEventListener("click", () => { if (lastTxHash) window.openExplorerModal(lastTxHash); });
            function showState(state, data = {}) {
                document.getElementById("dismiss-burn-status").disabled = state === "pending";
                burnCard?.classList.remove("burn-pending", "burn-success", "burn-error");
                if (state !== "success") clearSuccessFlame();
                if (burnStatusHideTimer) clearTimeout(burnStatusHideTimer);
                burnStatusHideTimer = null;
                statusBox?.classList.remove("hidden", "burn-status-hiding", "border-primary/15", "bg-primary/5", "border-orange-300/15", "bg-orange-300/[0.045]", "border-error/20", "bg-error/[0.055]");
                if (txHashHideTimer) clearTimeout(txHashHideTimer);
                txHashHideTimer = null;
                txWrap?.classList.remove("burn-tx-hiding");
                txWrap?.classList.add("hidden");

                if (state === "pending") {
                    burnCard?.classList.add("burn-pending");
                    statusBox?.classList.add("border-primary/15", "bg-primary/5");
                    statusIcon.className = "w-10 h-10 rounded-lg flex items-center justify-center shrink-0 bg-primary/10 border border-primary/15 text-primary";
                    statusIcon.innerHTML = '<span class="material-symbols-outlined burn-spin">progress_activity</span>';
                    statusTitle.textContent = "Creating burn transaction";
                    statusMessage.textContent = "Signing and submitting your burn to the Venera network.";
                    return;
                }

                if (state === "success") {
                    burnCard?.classList.add("burn-success");
                    statusBox?.classList.add("border-orange-300/15", "bg-orange-300/[0.045]");
                    statusIcon.className = "w-10 h-10 rounded-lg flex items-center justify-center shrink-0 bg-orange-300/10 border border-orange-300/15 text-orange-200";
                    statusIcon.innerHTML = '<span class="material-symbols-outlined">local_fire_department</span>';
                    statusTitle.textContent = "VNR burned successfully";
                    statusMessage.textContent = data.count > 1
                        ? `Burn submitted as ${data.count} transactions.`
                        : "The burn transaction was submitted successfully.";
                    lastTxHash = data.txHash || "";
                    if (lastTxHash) {
                        txHashEl.textContent = lastTxHash;
                        txWrap?.classList.remove("hidden");
                    }

                                                                               
                                                                                       
                    burnStatusHideTimer = setTimeout(() => {
                        statusBox?.classList.add("burn-status-hiding");
                        setTimeout(() => {
                            statusBox?.classList.add("hidden");
                            statusBox?.classList.remove("burn-status-hiding");
                            txWrap?.classList.add("hidden");
                            burnCard?.classList.remove("burn-success");
                            lastTxHash = "";
                            burnStatusHideTimer = null;
                        }, 430);
                    }, 8000);

                    playSuccessFlame(data.amount);
                    return;
                }

                burnCard?.classList.add("burn-error");
                statusBox?.classList.add("border-error/20", "bg-error/[0.055]");
                statusIcon.className = "w-10 h-10 rounded-lg flex items-center justify-center shrink-0 bg-error/10 border border-error/15 text-error";
                statusIcon.innerHTML = '<span class="material-symbols-outlined">error</span>';
                statusTitle.textContent = "Burn failed";
                statusMessage.textContent = "Error";
                spawnParticles("error", 24);
            }

            function formatBurnAmount(value) {
                const n = Number(value);
                if (!Number.isFinite(n)) return "0";
                return n.toLocaleString("en-US", {
                    minimumFractionDigits: 0,
                    maximumFractionDigits: 9
                });
            }

            function formatBurnPoints(value) {
                const n = Number(value);
                if (!Number.isFinite(n) || n <= 0) return "0";
                return Math.ceil(n).toLocaleString("en-US", {
                    minimumFractionDigits: 0,
                    maximumFractionDigits: 0
                });
            }

            function formatUsdt(value) {
                const n = Number(value);
                if (!Number.isFinite(n) || n <= 0) return "0.00";
                return n.toLocaleString("en-US", {
                    minimumFractionDigits: 2,
                    maximumFractionDigits: 8
                });
            }

            function updateCollectYieldButton() {
                if (!collectYieldBtn) return;
                const disabled = collectingYield || !(currentAccumulatedUsdt > 0);
                collectYieldBtn.disabled = disabled;
            }

            function setCollectYieldLoading(active) {
                collectingYield = active;
                if (!collectYieldBtn) return;
                collectYieldBtn.innerHTML = active
                    ? '<span class="material-symbols-outlined text-[18px] burn-spin">progress_activity</span><span>Collecting...</span>'
                    : '<span class="material-symbols-outlined text-[18px]">payments</span><span>Collect Yield</span>';
                updateCollectYieldButton();
            }

            function clearCollectSuccessAnimation() {
                if (collectSuccessHoldTimer) clearTimeout(collectSuccessHoldTimer);
                if (collectSuccessHideTimer) clearTimeout(collectSuccessHideTimer);
                collectSuccessHoldTimer = null;
                collectSuccessHideTimer = null;
                burnStatsCard?.classList.remove("collect-success");
                if (!collectSuccessOverlay) return;
                collectSuccessOverlay.classList.add("hidden");
                collectSuccessOverlay.classList.remove("flex", "is-entering", "is-leaving");
            }

            function spawnCollectParticles(count = 42) {
                if (!collectParticles) return;
                collectParticles.innerHTML = "";
                const colors = ["#00A86B", "#20D489", "#55E8A5", "#8DFFCA", "#C6FFE2"];

                for (let i = 0; i < count; i++) {
                    const particle = document.createElement("span");
                    particle.className = "collect-particle";
                    const size = 2 + Math.random() * 4.5;
                    particle.style.setProperty("--collect-particle-size", `${size}px`);
                    particle.style.setProperty("--collect-particle-x", `${(-48 + Math.random() * 96).toFixed(1)}px`);
                    particle.style.setProperty("--collect-particle-left", `${(7 + Math.random() * 86).toFixed(1)}%`);
                    particle.style.setProperty("--collect-particle-duration", `${(1.2 + Math.random() * 1.65).toFixed(2)}s`);
                    particle.style.setProperty("--collect-particle-delay", `${(Math.random() * .72).toFixed(2)}s`);
                    particle.style.setProperty("--collect-particle-color", colors[Math.floor(Math.random() * colors.length)]);
                    collectParticles.appendChild(particle);
                }
            }

            function playCollectSuccessAnimation() {
                clearCollectSuccessAnimation();
                burnStatsCard?.classList.add("collect-success");

                if (!collectSuccessOverlay) {
                    spawnCollectParticles();
                    return;
                }

                collectSuccessOverlay.classList.remove("hidden");
                collectSuccessOverlay.classList.add("flex");
                void collectSuccessOverlay.offsetWidth;
                collectSuccessOverlay.classList.add("is-entering");

                                                                                           
                collectSuccessHoldTimer = setTimeout(() => {
                    collectSuccessOverlay.classList.remove("is-entering");
                    collectSuccessOverlay.classList.add("is-leaving");
                    spawnCollectParticles(46);

                    collectSuccessHideTimer = setTimeout(() => {
                        clearCollectSuccessAnimation();
                    }, 880);
                }, 2550);
            }

            function hideCollectedYieldStatus() {
                if (collectStatusHideTimer) {
                    clearTimeout(collectStatusHideTimer);
                    collectStatusHideTimer = null;
                }
                if (!collectYieldStatus || collectYieldStatus.classList.contains("hidden")) return;
                collectYieldStatus.style.transition = "opacity .32s ease, transform .32s ease";
                collectYieldStatus.style.opacity = "0";
                collectYieldStatus.style.transform = "translateY(-4px) scale(.99)";
                setTimeout(() => {
                    collectYieldStatus.classList.add("hidden");
                    collectYieldStatus.style.opacity = "";
                    collectYieldStatus.style.transform = "";
                    collectYieldStatus.style.transition = "";
                }, 340);
            }

            function showCollectedYield(amount) {
                if (!collectYieldStatus || !collectedYieldAmountEl) return;

                const formatted = `${formatUsdt(amount)} USDT`;
                collectedYieldAmountEl.textContent = formatted;
                if (collectOverlayAmountEl) collectOverlayAmountEl.textContent = formatted;

                if (collectStatusHideTimer) clearTimeout(collectStatusHideTimer);
                collectYieldStatus.classList.add("hidden");
                collectYieldStatus.style.opacity = "";
                collectYieldStatus.style.transform = "";
                collectYieldStatus.style.transition = "";
                void collectYieldStatus.offsetWidth;
                collectYieldStatus.classList.remove("hidden");

                playCollectSuccessAnimation();

                                                                                                               
                collectStatusHideTimer = setTimeout(hideCollectedYieldStatus, 6000);
            }

            function updateBurnPointEstimate() {
                const amount = Number(String(amountInput?.value || "").trim());
                const rate = Number(currentPointsPerVnr);

                if (burnPointRateEl) {
                    burnPointRateEl.textContent = Number.isFinite(rate) && rate > 0
                        ? rate.toLocaleString("en-US", { maximumFractionDigits: 2 })
                        : "—";
                }

                if (!estimatedBurnPointsEl) return;

                if (!Number.isFinite(amount) || amount <= 0 || !Number.isFinite(rate) || rate <= 0) {
                    estimatedBurnPointsEl.textContent = "0 BP";
                    return;
                }

                estimatedBurnPointsEl.textContent = `${formatBurnPoints(amount * rate)} BP`;
            }

            async function loadBurnStats() {
                try {
                    const response = await fetch("https://wallet.veneralabs.org/dtransfers/burn_stats", {
                        method: "GET",
                        headers: { "Accept": "application/json", "Authorization": `Bearer ${localStorage.getItem("jwt_token")}` },
                        cache: "no-store"
                    });

                    const payload = await response.json();
                    if (!response.ok || payload?.error) return;

                    if (totalBurnedEl) {
                        totalBurnedEl.textContent = formatBurnAmount(payload.total_burned ?? 0);
                    }

                    if (burnCountEl) {
                        const count = Number(payload.burn_transactions ?? 0);
                        burnCountEl.textContent = Number.isFinite(count)
                            ? Math.max(0, Math.trunc(count)).toLocaleString("en-US")
                            : "0";
                    }

                    if (burnPointsEl) {
                        burnPointsEl.textContent = formatBurnPoints(payload.burn_points ?? 0);
                    }

                    const accumulated = Number(payload.accumulated_usdt ?? 0);
                    currentAccumulatedUsdt = Number.isFinite(accumulated) && accumulated > 0
                        ? accumulated
                        : 0;
                    if (accumulatedUsdtEl) {
                        accumulatedUsdtEl.textContent = formatUsdt(currentAccumulatedUsdt);
                    }
                    updateCollectYieldButton();

                    const rate = Number(payload.current_points_per_vnr);
                    if (Number.isFinite(rate) && rate > 0) {
                        currentPointsPerVnr = rate;
                    }
                    updateBurnPointEstimate();
                    checkYieldClaim();
                } catch (_) {
                                                                                    
                }
            }

            let yieldClaimPollTimer = null;
            let checkingYieldClaim = false;
            const animatedYieldClaims = new Set();

            function applyYieldClaim(payload) {
                if (payload.status === "paid") {
                    const collected = Number(payload.amount_usdt ?? payload.amount_collected ?? 0);
                    if (!Number.isFinite(collected) || collected <= 0) throw new Error("Invalid collected yield amount");
                    if (!animatedYieldClaims.has(payload.request_id)) {
                        animatedYieldClaims.add(payload.request_id);
                        currentAccumulatedUsdt = 0;
                        if (accumulatedUsdtEl) accumulatedUsdtEl.textContent = formatUsdt(0);
                        showCollectedYield(collected);
                    }
                    setCollectYieldLoading(false);
                    setTimeout(loadBurnStats, 900);
                } else if (["prepared", "pending"].includes(payload.status)) {
                    setCollectYieldLoading(true);
                    clearTimeout(yieldClaimPollTimer);
                    yieldClaimPollTimer = setTimeout(checkYieldClaim, 5000);
                } else {
                    setCollectYieldLoading(false);
                }
            }

            async function checkYieldClaim() {
                if (checkingYieldClaim) return;
                checkingYieldClaim = true;
                clearTimeout(yieldClaimPollTimer);
                try {
                    const response = await fetch("https://wallet.veneralabs.org/dtransfer/collect_yield_status", {method: "POST"});
                    const payload = await response.json();
                    if (!response.ok || payload.error) throw new Error(payload.error || "Claim status unavailable");
                    applyYieldClaim(payload);
                } catch (_) {
                                                                           
                    if (collectingYield) yieldClaimPollTimer = setTimeout(checkYieldClaim, 5000);
                } finally { checkingYieldClaim = false; }
            }

            async function collectYield() {
                if (collectingYield || !(currentAccumulatedUsdt > 0)) return;
                if (collectStatusHideTimer) clearTimeout(collectStatusHideTimer);
                collectYieldStatus?.classList.add("hidden");
                setCollectYieldLoading(true);
                try {
                    const response = await fetch("https://wallet.veneralabs.org/dtransfer/collect_yield", {
                        method: "POST", headers: {"Accept": "application/json"}
                    });
                    const payload = await response.json();
                    if (!response.ok || payload.error) throw new Error(payload.error || "Claim unavailable");
                    applyYieldClaim(payload);
                } catch (_) {
                    setCollectYieldLoading(false);
                    loadBurnStats();
                }
            }

            async function submitBurn() {
                if (burning) return;
                const amount = String(amountInput?.value || "").trim();
                if (!amount || !/^\d+(?:\.\d{1,9})?$/.test(amount) || Number(amount) <= 0) {
                    showState("error");
                    return;
                }

                setButtonLoading(true);
                showState("pending");

                try {
                    const response = await fetch("https://wallet.veneralabs.org/dtransfer/burn", {
                        method: "POST",
                        headers: { "Content-Type": "application/json", "Accept": "application/json", "Authorization": `Bearer ${localStorage.getItem("jwt_token")}` },
                        body: JSON.stringify({ amount })
                    });

                    let payload = {};
                    try { payload = await response.json(); } catch (_) {}

                    if (!response.ok || payload?.error || !payload?.tx_hash) {
                        throw new Error("Error");
                    }

                    const hashes = Array.isArray(payload.tx_hashes) && payload.tx_hashes.length
                        ? payload.tx_hashes
                        : [payload.tx_hash];

                    showState("success", { txHash: payload.tx_hash, count: hashes.length, amount });
                    loadBurnStats();
                                                                                                       
                    setTimeout(loadBurnStats, 35000);
                } catch (_) {
                    showState("error");
                } finally {
                    setButtonLoading(false);
                }
            }

            loadBurnStats();
            window.addEventListener('venera:ready',loadBurnStats);
            setInterval(loadBurnStats, 30000);

            burnBtn?.addEventListener("click", submitBurn);
            collectYieldBtn?.addEventListener("click", collectYield);
            amountInput?.addEventListener("keydown", (event) => {
                if (event.key === "Enter") submitBurn();
            });
            amountInput?.addEventListener("input", () => {
                updateBurnPointEstimate();
                if (!burning && (burnCard?.classList.contains("burn-success") || burnCard?.classList.contains("burn-error"))) {
                    clearVisualState();
                }
            });

            cancelBtn?.addEventListener("click", () => {
                if (burning) return;
                if (amountInput) amountInput.value = "";
                clearVisualState();
                amountInput?.focus();
            });

            maxBtn?.addEventListener("click", () => {
                if (burning || !amountInput || !balanceEl) return;
                const raw = balanceEl.dataset.balance || balanceEl.textContent || "";
                const match = String(raw).replace(/,/g, "").match(/\d+(?:\.\d+)?/);
                if (match) {
                    amountInput.value = match[0];
                    amountInput.dispatchEvent(new Event("input", { bubbles: true }));
                }
            });

            copyHashBtn?.addEventListener("click", async () => {
                if (!lastTxHash) return;
                try {
                    await navigator.clipboard.writeText(lastTxHash);
                    const old = copyHashBtn.textContent;
                    copyHashBtn.textContent = "Copied";
                    setTimeout(() => { copyHashBtn.textContent = old; }, 1200);
                } catch (_) {}
            });
        })();
        