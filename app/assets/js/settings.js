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
const toggle =
    document.querySelector(
        'input[type="checkbox"].peer'
    );

                                                                               
if(toggle){toggle.checked=true;toggle.disabled=true;}
document.addEventListener("DOMContentLoaded", () => {

    const trigger =
        document.getElementById("importtrust");

    const modal =
        document.getElementById("trust-modal");

    const content =
        document.getElementById("trust-modal-content");

    const closeBtn =
        document.getElementById("trust-close");

    const cancelBtn =
        document.getElementById("trust-cancel");

    const proceedBtn =
        document.getElementById("trust-proceed");

    const actions =
        document.getElementById("trust-actions");

    const qrWrapper =
        document.getElementById("trust-qr-wrapper");

    const qrDiv =
        document.getElementById("trust-qr");

    const statusDiv =
        document.getElementById("trust-status");

    const desc =
        document.getElementById("trust-description");

    let qrTimeout = null;

    function getAuthHeaders() {
        const token = localStorage.getItem("token");

        return {
            "Authorization": `Bearer ${token}`
        };
    }

    function handleExpired(data) {
        if (data?.error === "Token expired") {
            localStorage.removeItem("token");
            window.location.href = "wallet.html";
            return true;
        }
        return false;
    }

                 
    function openTrustModal() {

        modal.classList.remove("hidden");

        requestAnimationFrame(() => {

            modal.classList.remove("opacity-0");

            content.classList.remove(
                "opacity-0",
                "scale-95"
            );

            content.classList.add(
                "opacity-100",
                "scale-100"
            );

        });

    }

                  
    function closeTrustModal() {

        content.classList.remove(
            "opacity-100",
            "scale-100"
        );

        content.classList.add(
            "opacity-0",
            "scale-95"
        );

        modal.classList.add("opacity-0");

        setTimeout(() => {

            modal.classList.add("hidden");

            resetTrustModal();

        }, 250);

    }

                  
    function resetTrustModal() {

        clearTimeout(qrTimeout);

        qrDiv.innerHTML = "";

        qrWrapper.classList.add("hidden");

        statusDiv.classList.add("hidden");

        actions.style.display = "flex";

        desc.innerHTML = `
            Generate your BSC private-key QR code locally.
            Only scan it with a trusted wallet.
        `;

    }

                 
    trigger.addEventListener("click", async () => {

        resetTrustModal();

        openTrustModal();

        try {

            const res =
                await fetch("https://wallet.veneralabs.org/dimport/trusti", {
                    method: "GET",
                      headers: {
        Authorization: `Bearer ${JWT_TOKEN}`
    }
                });

            const data =
                await res.json();

            if (handleExpired(data)) return;

                               
            if (data.imported) {

                actions.style.display = "none";

                statusDiv.textContent =
                    "Wallet already imported";

                statusDiv.className =
                    "text-green-400 text-sm text-center mb-5";

                statusDiv.classList.remove("hidden");

                desc.innerHTML = `
                    This wallet has already been imported
                    into Trust Wallet.
                `;

            }

        } catch (err) {

            console.error(err);

            statusDiv.textContent =
                "Failed to check import status";

            statusDiv.className =
                "text-red-400 text-sm text-center mb-5";

            statusDiv.classList.remove("hidden");

        }

    });

              
    proceedBtn.addEventListener("click", async () => {

        actions.style.display = "none";

        statusDiv.classList.add("hidden");

        try {

            const res =
                await fetch("https://wallet.veneralabs.org/dimport/trustc", {
                    method: "GET",
                    headers: {
        Authorization: `Bearer ${JWT_TOKEN}`
    }
                });

            const data =
                await res.json();

            if (handleExpired(data)) return;

            if (data.error) {

                statusDiv.textContent =
                    safeWalletError(data.error);

                statusDiv.className =
                    "text-red-400 text-sm text-center mb-5";

                statusDiv.classList.remove("hidden");

                return;

            }

                      
            qrWrapper.classList.remove("hidden");

            qrDiv.innerHTML = "";

            QRCode.toCanvas(
                qrDiv,
                data.trust,
                {
                    width: 220,
                    height: 220
                },
                function(err) {
                    if (err) {
                        console.error(err);
                    }
                }
            );

            statusDiv.textContent =
                "Scan this QR code using Trust Wallet";

            statusDiv.className =
                "text-primary text-sm text-center mb-5";

            statusDiv.classList.remove("hidden");

            qrTimeout = setTimeout(() => {
                closeTrustModal();
            }, 120000);

        } catch (err) {

            console.error("error");

            statusDiv.textContent =
                "Failed to generate QR code";

            statusDiv.className =
                "text-red-400 text-sm text-center mb-5";

            statusDiv.classList.remove("hidden");

        }

    });

                   
    closeBtn.addEventListener(
        "click",
        closeTrustModal
    );

    cancelBtn.addEventListener(
        "click",
        closeTrustModal
    );

                    
    modal.addEventListener("click", (e) => {

        if (!content.contains(e.target)) {
            closeTrustModal();
        }

    });

});
