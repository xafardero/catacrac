# Catacrac

Flipper Zero app to help Jack learn Catalan vocabulary. Opens on a menu
(Play / Sound / Credits). In Play, D-pad left/right cycles through words
and OK reveals/repeats. Back exits from the menu, but requires a **long
press** to leave Play mode — a stray tap won't accidentally kick a kid
out mid-session.

Full idea and roadmap: see the Notion project page linked from this repo's
owner workspace ("Flipper Zero word-learning app for Jack").

## Build

Uses [ufbt](https://github.com/flipperdevices/flipperzero-ufbt), the
lightweight Flipper build tool (no full firmware checkout needed).

```
pip install ufbt
ufbt          # build the .fap
ufbt launch   # build, deploy and launch on a connected Flipper over USB
```

## Status

M1 (build environment + word cycling), M2 (curated word list, 12 animals,
uppercase with a large embedded font), M3 (bitmap animations for every
word), and M4 (sound feedback) are done and confirmed on a real device.
Word list is hardcoded in `catacrac.c`, plain ASCII only so far — Catalan
diacritics (ratolí, lleó, ànec) aren't in the list yet since font
rendering for them hasn't been verified on hardware.

Word navigation plays a short tick and revealing a word plays a rising
three-note chime, both via Flipper's `NotificationApp` service (async,
doesn't block the input loop) — no external audio hardware needed, since
the goal here is reinforcement tones, not real pronunciation.

Words render with `u8g2_font_helvB18_tr`, a bold 18px font vendored from
[u8g2](https://github.com/olikraus/u8g2) in `u8g2_font_helvb18_tr.c` and
set at runtime via `canvas_set_custom_u8g2_font()` — Flipper's stock fonts
(`FontPrimary` etc.) top out too small for this use case.

Every word has a small bounce animation (`images/<animal>/`), sourced
from [OpenMoji](https://openmoji.org)'s black-outline emoji SVGs (CC
BY-SA 4.0, credited on the Credits screen) instead of hand-drawn shapes —
the earlier procedural art was hard to recognize. Regenerate with
`.venv/bin/python tools/fetch_openmoji_frames.py` (needs `cairosvg` and
network access). Compiled through Flipper's standard `fap_icon_assets`
pipeline, not hand-rolled.

For any word with an animation, the word text stays hidden (an "OK?" hint
shows instead) until OK is pressed — a small guess-first flashcard flow.
Pressing OK again just restarts the animation.

The app opens on a menu screen (Play / Sound: ON-OFF / Credits) instead
of jumping straight into Play. Sound can be muted from there. Play mode
ignores a short Back press — only a long press returns to the menu — so
the app can't be exited by an accidental tap.
