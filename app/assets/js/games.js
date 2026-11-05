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
