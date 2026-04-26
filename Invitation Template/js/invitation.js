/* ============================================================
   INVITATION.JS — S3
   ============================================================ */

const Invitation = (() => {
  const CONFETTI_CONFIG = {
    particleCount: 120,
    spread: 80,
    colors: [
      "#5dbe8a",
      "#2d8f5f",
      "#8ed4ad",
      "#b8e8cc",
      "#e2f5eb",
      "#1a6642",
      "#061410",
    ],
    disableForReducedMotion: true,
  };

  const NO_CONFIG = {
    jumpDistance: 140,
    maxJumps: 5,
    shrinkDelay: 600,
  };

  let btnYes = null;
  let btnNo = null;
  let isShrinking = false;
  let isConfirmed = false;
  let jumpCount = 0;

  const isTouchDevice = () => !window.matchMedia("(pointer: fine)").matches;

  /* --- Confetti --- */
  const launchConfetti = () => {
    if (typeof confetti !== "function") return;
    confetti({ ...CONFETTI_CONFIG, origin: { x: 0.4, y: 0.65 }, angle: 60 });
    setTimeout(() => {
      confetti({ ...CONFETTI_CONFIG, origin: { x: 0.6, y: 0.65 }, angle: 120 });
    }, 150);
  };

  /* --- Botón Sí --- */
  const onYesClick = () => {
    if (isConfirmed) return;
    isConfirmed = true;

    launchConfetti();

    setTimeout(() => {
      btnYes.classList.add("is-confirmed");
      btnYes.textContent = "Perfecto!";
    }, 300);
  };

  /* --- Botón No: saltar --- */
  const getJumpPosition = (clickX, clickY) => {
    const r = btnNo.getBoundingClientRect();
    const btnCX = r.left + r.width / 2;
    const btnCY = r.top + r.height / 2;
    const dx = btnCX - clickX;
    const dy = btnCY - clickY;
    const dist = Math.sqrt(dx * dx + dy * dy);

    let nx, ny;
    if (dist === 0) {
      const a = Math.random() * Math.PI * 2;
      nx = Math.cos(a);
      ny = Math.sin(a);
    } else {
      nx = dx / dist;
      ny = dy / dist;
    }

    const margin = 16;
    return {
      x: Math.max(
        margin,
        Math.min(
          window.innerWidth - r.width - margin,
          r.left + nx * NO_CONFIG.jumpDistance,
        ),
      ),
      y: Math.max(
        margin,
        Math.min(
          window.innerHeight - r.height - margin,
          r.top + ny * NO_CONFIG.jumpDistance,
        ),
      ),
    };
  };

  const jumpAway = (clickX, clickY) => {
    const { x, y } = getJumpPosition(clickX, clickY);
    if (btnNo.style.position !== "fixed") btnNo.style.position = "fixed";
    btnNo.style.transition =
      "left 220ms cubic-bezier(0.34,1.56,0.64,1), top 220ms cubic-bezier(0.34,1.56,0.64,1)";
    btnNo.style.left = `${x}px`;
    btnNo.style.top = `${y}px`;
    jumpCount++;
    if (jumpCount >= NO_CONFIG.maxJumps) {
      const scale = Math.max(
        0.3,
        1 - (jumpCount - NO_CONFIG.maxJumps + 1) * 0.15,
      );
      btnNo.style.transform = `scale(${scale})`;
      btnNo.style.opacity = `${Math.max(0.2, scale)}`;
    }
  };

  const initDesktopBehavior = () => {
    btnNo.addEventListener("click", (e) => {
      e.preventDefault();
      if (!isConfirmed) jumpAway(e.clientX, e.clientY);
    });
  };

  const initTouchBehavior = () => {
    btnNo.addEventListener(
      "touchstart",
      (e) => {
        e.preventDefault();
        if (isShrinking || isConfirmed) return;
        isShrinking = true;
        btnNo.classList.add("is-shrinking");
        setTimeout(() => {
          btnNo.classList.remove("is-shrinking");
          isShrinking = false;
        }, NO_CONFIG.shrinkDelay);
      },
      { passive: false },
    );
  };

  const init = () => {
    btnYes = document.getElementById("btnYes");
    btnNo = document.getElementById("btnNo");
    if (!btnYes || !btnNo) {
      console.warn("Invitation: botones no encontrados en el DOM");
      return;
    }

    btnYes.addEventListener("click", onYesClick);
    isTouchDevice() ? initTouchBehavior() : initDesktopBehavior();
  };

  return { init };
})();
