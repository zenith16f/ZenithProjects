/* ============================================================
   MUSIC.JS — S8
   Autoplay en primera interacción + toggle play/pause.
   ============================================================ */

const Music = (() => {
  const CONFIG = {
    fadeInDuration: 1500,
    fadeSteps: 30,
    volumeTarget: 0.7,
  };

  let btnMusic = null;
  let audio = null;
  let isPlaying = false;
  let fadeTimer = null;
  let autoplayFired = false;

  const hasAudioSource = () => !!(audio.currentSrc && audio.currentSrc !== "");

  const fadeIn = () => {
    if (fadeTimer) clearInterval(fadeTimer);
    audio.volume = 0;

    const stepSize = CONFIG.volumeTarget / CONFIG.fadeSteps;
    const stepInterval = CONFIG.fadeInDuration / CONFIG.fadeSteps;

    fadeTimer = setInterval(() => {
      const next = audio.volume + stepSize;
      if (next >= CONFIG.volumeTarget) {
        audio.volume = CONFIG.volumeTarget;
        clearInterval(fadeTimer);
        fadeTimer = null;
      } else {
        audio.volume = next;
      }
    }, stepInterval);
  };

  const fadeOut = () => {
    if (fadeTimer) clearInterval(fadeTimer);

    const stepSize = audio.volume / CONFIG.fadeSteps;
    const stepInterval = CONFIG.fadeInDuration / CONFIG.fadeSteps;

    fadeTimer = setInterval(() => {
      const next = audio.volume - stepSize;
      if (next <= 0) {
        audio.volume = 0;
        audio.pause();
        clearInterval(fadeTimer);
        fadeTimer = null;
      } else {
        audio.volume = next;
      }
    }, stepInterval);
  };

  const updateButton = (playing) => {
    btnMusic.dataset.playing = playing ? "true" : "false";
    btnMusic.setAttribute(
      "aria-label",
      playing ? "Pausar música" : "Reproducir música",
    );
    btnMusic.dataset.tooltip = playing ? "Pausar" : "Reproducir";
    btnMusic.classList.toggle("music-btn--playing", playing);
  };

  const play = () => {
    if (!hasAudioSource()) return;
    fadeIn();
    audio.play().catch((err) => {
      console.warn("Music: reproducción bloqueada", err);
      isPlaying = false;
      updateButton(false);
    });
    isPlaying = true;
    updateButton(true);
  };

  const pause = () => {
    fadeOut();
    isPlaying = false;
    updateButton(false);
  };

  const toggle = () => {
    isPlaying ? pause() : play();
  };

  /* ----------------------------------------------------------
     autoplayOnFirstInteraction
     Escucha cualquier interacción del usuario una sola vez.
     En cuanto ocurre, arranca la música y se desregistra.
     Eventos cubiertos: click, keydown, touchstart, scroll.
  ---------------------------------------------------------- */
  const autoplayOnFirstInteraction = () => {
    if (autoplayFired) return;
    autoplayFired = true;

    const events = ["click", "keydown", "touchstart", "scroll"];
    const handler = () => {
      play();
      events.forEach((e) => document.removeEventListener(e, handler));
    };
    events.forEach((e) =>
      document.addEventListener(e, handler, { once: true, passive: true }),
    );
  };

  /* ----------------------------------------------------------
     handleVisibilityChange
     Pausa al cambiar de pestaña, reanuda al volver.
  ---------------------------------------------------------- */
  const handleVisibilityChange = () => {
    if (document.visibilityState === "hidden" && isPlaying) {
      if (fadeTimer) clearInterval(fadeTimer);
      audio.volume = 0;
      audio.pause();
    } else if (document.visibilityState === "visible" && isPlaying) {
      audio.play().catch(() => {});
      fadeIn();
    }
  };

  const init = () => {
    btnMusic = document.getElementById("musicBtn");
    audio = document.getElementById("bgMusic");

    if (!btnMusic || !audio) {
      console.warn("Music: elementos no encontrados en el DOM");
      return;
    }

    btnMusic.dataset.tooltip = "Música";
    btnMusic.addEventListener("click", (e) => {
      e.stopPropagation();
      toggle();
    });

    document.addEventListener("visibilitychange", handleVisibilityChange);

    audio.addEventListener("ended", () => {
      isPlaying = false;
      updateButton(false);
    });

    autoplayOnFirstInteraction();
  };

  return { init };
})();
