# invitation-template

A customizable single-page invitation web app. Dark elegant design with interactive features. Built with vanilla HTML, CSS and JavaScript — no frameworks, no build tools.

## Features

- Animated hero with letter-by-letter entrance (GSAP)
- Personal message section with photo placeholder
- Interactive invitation card with confetti on accept
- "No" button that jumps away on click and shrinks after multiple attempts
- Date and place selection with localStorage persistence
- Live countdown to selected date (auto-generates next 7 days)
- Background music toggle with fade in/out + autoplay on first interaction
- Custom SVG cursor
- Hidden easter egg
- Fully responsive

## Tech stack

```
HTML5 · CSS3 · Vanilla JS
GSAP 3         — hero animations
canvas-confetti — confetti effect
Normalize.css  — cross-browser consistency
Google Fonts   — Cormorant Garamond + Inter
```

## Project structure

```
invitation-template/
├── index.html
├── css/
│   ├── base.css        ← design tokens, reset, typography
│   ├── layout.css      ← sections, grid, spacing
│   └── component.css   ← buttons, cards, animations
├── js/
│   ├── plan.js         ← date/place selection + localStorage
│   ├── countdown.js    ← live countdown
│   ├── invitation.js   ← yes/no buttons + confetti
│   ├── easterEgg.js    ← hidden easter egg
│   ├── music.js        ← audio toggle with autoplay
│   └── main.js         ← init, hero animation, stars
└── assets/
    ├── cursor/         ← custom SVG cursors
    ├── music/          ← add song.mp3 here
    └── images/         ← add photo.jpg here
```

## How to customize

### Name

In `index.html`, find `data-text="[Nombre]"` and replace with the recipient's name.

### Personal message

Find the `.message__body` and `.t-hint-message` paragraphs in `index.html`.

### Photo

Replace the placeholder div with:

```html
<img
  src="assets/images/photo.jpg"
  alt="Description"
  class="message__photo"
/>
```

### Music

Drop `song.mp3` into `assets/music/` and uncomment the `<source>` tag in `index.html`.

### Dates

Fixed dates instead of dynamic ones — edit `DATES` in `js/plan.js`:

```js
const DATES = [{ day: 20, month: 5, year: 2025, label: "20 may · Wednesday" }];
```

### Places

Edit the `#placeList` in `index.html`. Update `data-place` attributes and card names.

### Easter egg message

Find `.easter-egg__message` in `index.html`.

### Color palette

All colors are CSS custom properties in `css/base.css` under `:root`. Change them there and the entire UI updates automatically.

### Email notifications (optional)

To receive an email when the recipient clicks "Yes", integrate [EmailJS](https://emailjs.com) (free tier: 200 emails/month):

1. Create a template with variables: `{{title}}`, `{{name}}`, `{{time}}`, `{{message}}`
2. Add the SDK to `index.html`: `https://cdn.jsdelivr.net/npm/@emailjs/browser@4/dist/email.min.js`
3. In `js/invitation.js`, call `emailjs.init('YOUR_PUBLIC_KEY')` inside `init()`
4. Call `emailjs.send(serviceId, templateId, { ... })` inside `onYesClick()`

## Color palette

| Token            | Hex       | Usage                          |
| ---------------- | --------- | ------------------------------ |
| `--purple-400`   | `#5dbe8a` | Primary accent (emerald green) |
| `--purple-600`   | `#2d8f5f` | Borders, buttons               |
| `--purple-800`   | `#1a6642` | Dark accent                    |
| `--navy-mid`     | `#1e3d2c` | Card borders                   |
| `--bg-base`      | `#080f0c` | Page background                |
| `--bg-card`      | `#0d1a14` | Card background                |
| `--text-primary` | `#e0f5ea` | Main text                      |

> The token names (`--purple-*`, `--navy-*`) are kept from the original structure for easy forking. The actual colors are emerald green and teal.

## Typography

- **Cormorant Garamond** — headings, hero name, countdown numbers
- **Inter** — labels, body text, buttons

## Deployment

Any static host works.
