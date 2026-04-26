/* ============================================================
   EASTEREGG.JS — S1
   ============================================================ */

const EasterEgg = (() => {
  const CONFIG = {
    fadeOutDuration: 300,
    fadeInDelay: 200,
  };

  let btnIcon = null;
  let card = null;

  const isDiscovered = () => btnIcon.dataset.discovered === "true";

  const markDiscovered = () => {
    btnIcon.dataset.discovered = "true";
    btnIcon.disabled = true;
  };

  const hideIcon = () => {
    btnIcon.style.transition = `opacity ${CONFIG.fadeOutDuration}ms ease`;
    btnIcon.style.opacity = "0";
    btnIcon.style.pointerEvents = "none";
  };

  const showCard = () => card.classList.remove("js-hidden");

  const onIconClick = () => {
    if (isDiscovered()) return;
    markDiscovered();
    hideIcon();
    setTimeout(showCard, CONFIG.fadeOutDuration + CONFIG.fadeInDelay);
  };

  const init = () => {
    btnIcon = document.getElementById("easterEgg");
    card = document.getElementById("easterEggCard");

    if (!btnIcon || !card) {
      console.warn("EasterEgg: elementos no encontrados en el DOM");
      return;
    }

    if (isDiscovered()) {
      btnIcon.style.opacity = "0";
      btnIcon.style.pointerEvents = "none";
      showCard();
      return;
    }

    btnIcon.addEventListener("click", onIconClick);
    btnIcon.addEventListener("keydown", (e) => {
      if (e.key === "Enter" || e.key === " ") {
        e.preventDefault();
        onIconClick();
      }
    });
  };

  return { init };
})();
