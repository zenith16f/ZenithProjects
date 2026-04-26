/* ============================================================
   PLAN.JS — S4
   ============================================================ */

const Plan = (() => {
  const EVENT_HOUR = 10;
  const EVENT_MINUTE = 0;
  const LS_DATE_KEY = "inv_selectedDate";
  const LS_PLACE_KEY = "inv_selectedPlace";

  /* Genera los 7 días de la semana siguiente al día actual.
     Si hoy es miércoles, muestra lunes-domingo de la semana próxima.
     Así la página nunca muestra fechas pasadas sin necesidad
     de actualizarla manualmente. */
  const DAYS_ES = [
    "Domingo",
    "Lunes",
    "Martes",
    "Miércoles",
    "Jueves",
    "Viernes",
    "Sábado",
  ];
  const MONTHS_ES = [
    "ene",
    "feb",
    "mar",
    "abr",
    "may",
    "jun",
    "jul",
    "ago",
    "sep",
    "oct",
    "nov",
    "dic",
  ];

  const generateNextWeek = () => {
    const today = new Date();

    /* Primer día de la semana siguiente: lunes próximo.
       getDay() → 0=Dom,1=Lun,...,6=Sáb
       Días hasta el lunes siguiente: si hoy es lunes (1) → 7 días,
       si hoy es martes (2) → 6 días, domingo (0) → 1 día. */
    const daysUntilNextMonday = today.getDay() === 0 ? 1 : 8 - today.getDay();
    const nextMonday = new Date(today);
    nextMonday.setDate(today.getDate() + daysUntilNextMonday);

    return Array.from({ length: 7 }, (_, i) => {
      const d = new Date(nextMonday);
      d.setDate(nextMonday.getDate() + i);
      return {
        day: d.getDate(),
        month: d.getMonth() + 1,
        year: d.getFullYear(),
        label: `${d.getDate()} ${MONTHS_ES[d.getMonth()]} · ${DAYS_ES[d.getDay()]}`,
      };
    });
  };

  const DATES = generateNextWeek();

  /* Si el índice guardado en localStorage ya no corresponde a
     la semana actual (porque pasó una semana), se resetea a 0. */
  const savedRaw = parseInt(localStorage.getItem(LS_DATE_KEY) ?? "0");
  const savedDateIndex =
    savedRaw >= 0 && savedRaw < DATES.length ? savedRaw : 0;
  const savedPlace = localStorage.getItem(LS_PLACE_KEY) ?? "lugar-a";

  let selectedDate = DATES[savedDateIndex];
  let selectedPlace = savedPlace;

  const getTargetDate = () =>
    new Date(
      selectedDate.year,
      selectedDate.month - 1,
      selectedDate.day,
      EVENT_HOUR,
      EVENT_MINUTE,
      0,
      0,
    );

  const emitDateSelected = () => {
    document.dispatchEvent(
      new CustomEvent("dateSelected", {
        detail: { targetDate: getTargetDate(), label: selectedDate.label },
        bubbles: true,
      }),
    );
  };

  const buildDateCard = (dateObj, index) => {
    const li = document.createElement("li");
    const button = document.createElement("button");
    button.className = "plan__card";
    button.dataset.index = index;
    button.setAttribute(
      "aria-pressed",
      index === savedDateIndex ? "true" : "false",
    );
    button.innerHTML = `
      <span class="plan__card-dot" aria-hidden="true"></span>
      <span class="plan__card-name">${dateObj.label}</span>
      <span class="plan__card-sub">${dateObj.year}</span>
    `;
    button.addEventListener("click", () => onDateSelect(index));
    li.appendChild(button);
    return li;
  };

  const onDateSelect = (index) => {
    selectedDate = DATES[index];
    localStorage.setItem(LS_DATE_KEY, index);
    document
      .getElementById("dateList")
      .querySelectorAll(".plan__card")
      .forEach((btn, i) =>
        btn.setAttribute("aria-pressed", i === index ? "true" : "false"),
      );
    emitDateSelected();
  };

  const onPlaceSelect = (placeId) => {
    selectedPlace = placeId;
    localStorage.setItem(LS_PLACE_KEY, placeId);
    document
      .getElementById("placeList")
      .querySelectorAll(".plan__card")
      .forEach((btn) =>
        btn.setAttribute(
          "aria-pressed",
          btn.dataset.place === placeId ? "true" : "false",
        ),
      );
  };

  const initDates = () => {
    const dateList = document.getElementById("dateList");
    if (!dateList) return;
    const fragment = document.createDocumentFragment();
    DATES.forEach((dateObj, index) =>
      fragment.appendChild(buildDateCard(dateObj, index)),
    );
    dateList.appendChild(fragment);
  };

  const initPlaces = () => {
    const placeList = document.getElementById("placeList");
    if (!placeList) return;
    placeList.querySelectorAll(".plan__card").forEach((btn) => {
      btn.setAttribute(
        "aria-pressed",
        btn.dataset.place === savedPlace ? "true" : "false",
      );
      btn.addEventListener("click", () => onPlaceSelect(btn.dataset.place));
    });
  };

  const getSelected = () => ({
    date: selectedDate,
    place: selectedPlace,
    targetDate: getTargetDate(),
  });

  const init = () => {
    initDates();
    initPlaces();
    emitDateSelected();
  };

  return { init, getSelected };
})();
