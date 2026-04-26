/* ============================================================
   COUNTDOWN.JS — S5
   ============================================================ */

const Countdown = (() => {
  let elDays = null;
  let elHours = null;
  let elMinutes = null;
  let elSeconds = null;
  let elTarget = null;

  let targetDate = null;
  let intervalId = null;
  let lastValues = { days: -1, hours: -1, minutes: -1, seconds: -1 };

  const pad = (n) => String(n).padStart(2, "0");

  const calcTimeLeft = (target) => {
    const diff = target - new Date();
    if (diff <= 0) return { days: 0, hours: 0, minutes: 0, seconds: 0 };

    const totalSeconds = Math.floor(diff / 1000);
    const totalMinutes = Math.floor(totalSeconds / 60);
    const totalHours = Math.floor(totalMinutes / 60);
    const totalDays = Math.floor(totalHours / 24);

    return {
      days: totalDays,
      hours: totalHours % 24,
      minutes: totalMinutes % 60,
      seconds: totalSeconds % 60,
    };
  };

  const flipAndUpdate = (el, newValue) => {
    el.classList.add("is-flipping");
    setTimeout(() => {
      el.textContent = newValue;
    }, 200);
    const onAnimEnd = () => {
      el.classList.remove("is-flipping");
      el.removeEventListener("animationend", onAnimEnd);
    };
    el.addEventListener("animationend", onAnimEnd);
  };

  const updateDisplay = (values) => {
    const { days, hours, minutes, seconds } = values;
    if (days !== lastValues.days) flipAndUpdate(elDays, pad(days));
    if (hours !== lastValues.hours) flipAndUpdate(elHours, pad(hours));
    if (minutes !== lastValues.minutes) flipAndUpdate(elMinutes, pad(minutes));
    if (seconds !== lastValues.seconds) flipAndUpdate(elSeconds, pad(seconds));
    lastValues = { days, hours, minutes, seconds };
  };

  const setInitialDisplay = (values) => {
    const { days, hours, minutes, seconds } = values;
    elDays.textContent = pad(days);
    elHours.textContent = pad(hours);
    elMinutes.textContent = pad(minutes);
    elSeconds.textContent = pad(seconds);
    lastValues = { days, hours, minutes, seconds };
  };

  const onCountdownEnd = () => {
    elDays.textContent = "¡";
    elHours.textContent = "Es";
    elMinutes.textContent = "hoy";
    elSeconds.textContent = "!";
    if (elTarget) elTarget.textContent = "¡El día llegó!";
  };

  const startTicking = () => {
    if (intervalId) {
      clearInterval(intervalId);
      intervalId = null;
    }

    intervalId = setInterval(() => {
      const values = calcTimeLeft(targetDate);
      updateDisplay(values);
      if (!values.days && !values.hours && !values.minutes && !values.seconds) {
        clearInterval(intervalId);
        intervalId = null;
        onCountdownEnd();
      }
    }, 1000);
  };

  const onDateSelected = (event) => {
    const { targetDate: newTarget, label } = event.detail;
    targetDate = newTarget;
    if (elTarget) elTarget.textContent = `→ ${label} a las 10:00 AM`;
    setInitialDisplay(calcTimeLeft(targetDate));
    startTicking();
  };

  const cacheElements = () => {
    elDays = document.getElementById("cdDays");
    elHours = document.getElementById("cdHours");
    elMinutes = document.getElementById("cdMinutes");
    elSeconds = document.getElementById("cdSeconds");
    elTarget = document.getElementById("cdTarget");
    return elDays && elHours && elMinutes && elSeconds;
  };

  const init = () => {
    if (!cacheElements()) {
      console.warn("Countdown: elementos del DOM no encontrados");
      return;
    }
    document.addEventListener("dateSelected", onDateSelected);
  };

  const destroy = () => {
    if (intervalId) {
      clearInterval(intervalId);
      intervalId = null;
    }
    document.removeEventListener("dateSelected", onDateSelected);
  };

  return { init, destroy };
})();
