/* ============================================================
   MAIN.JS
   ============================================================ */

/* --- Hero: animación de entrada con GSAP --- */
const initHero = () => {
  const nameEl = document.getElementById("heroName");
  if (!nameEl) return;

  const text = nameEl.dataset.text || nameEl.textContent.trim();
  nameEl.textContent = "";

  text.split("").forEach((char) => {
    const span = document.createElement("span");
    span.textContent = char === " " ? "\u00A0" : char;
    span.className = "hero-letter";
    span.style.display = "inline-block";
    span.style.opacity = "0";
    span.style.transform = "translateY(20px)";
    nameEl.appendChild(span);
  });

  if (typeof gsap === "undefined") {
    nameEl.querySelectorAll(".hero-letter").forEach((s) => {
      s.style.opacity = "1";
      s.style.transform = "none";
    });
    return;
  }

  gsap.to(".hero-letter", {
    opacity: 1,
    y: 0,
    duration: 0.6,
    stagger: 0.08,
    delay: 0.4,
    ease: "power2.out",
    clearProps: "transform",
  });

  const subtitle = document.querySelector(".hero__subtitle");
  if (subtitle)
    gsap.fromTo(
      subtitle,
      { opacity: 0, y: 10 },
      { opacity: 1, y: 0, duration: 0.5, delay: 1.2, ease: "power2.out" },
    );

  const dividers = document.querySelectorAll(".hero__content .divider");
  if (dividers.length)
    gsap.fromTo(
      dividers,
      { opacity: 0, scaleX: 0 },
      {
        opacity: 1,
        scaleX: 1,
        duration: 0.4,
        delay: 0.2,
        ease: "power2.out",
        stagger: 0.1,
      },
    );
};

/* --- Estrellas del hero --- */
const initStars = () => {
  const container = document.getElementById("heroStars");
  if (!container) return;

  const fragment = document.createDocumentFragment();

  for (let i = 0; i < 40; i++) {
    const star = document.createElement("span");
    const size = Math.random() * 2 + 1;
    star.className = "star";
    star.style.width = `${size}px`;
    star.style.height = `${size}px`;
    star.style.left = `${Math.random() * 100}%`;
    star.style.top = `${Math.random() * 100}%`;
    star.style.setProperty(
      "--star-opacity",
      (Math.random() * 0.5 + 0.2).toFixed(2),
    );
    star.style.setProperty(
      "--star-duration",
      `${(Math.random() * 3 + 2).toFixed(1)}s`,
    );
    star.style.animationDelay = `${(Math.random() * 3).toFixed(1)}s`;
    if (Math.random() > 0.6) star.style.backgroundColor = "#AFA9EC";
    fragment.appendChild(star);
  }

  container.appendChild(fragment);
};

/* --- Inicialización --- */
document.addEventListener("DOMContentLoaded", () => {
  EasterEgg.init();
  Music.init();
  Countdown.init();
  Plan.init();
  Invitation.init();
  initStars();
  initHero();
});
