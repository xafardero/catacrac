# Catacrac

Flipper Zero app to help Jack learn Catalan vocabulary. D-pad left/right cycles
through words; OK repeats the current word; Back exits.

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

M1 (build environment + word cycling) and M2 (first curated word list, 12
animals, shown in uppercase with a large embedded font) are done and
confirmed on a real device. Word list is hardcoded in `catacrac.c`, plain
ASCII only so far — Catalan diacritics (ratolí, lleó, ànec) aren't in the
list yet since font rendering for them hasn't been verified on hardware.
Animations and sound are not implemented yet.

Words render with `u8g2_font_helvB18_tr`, a bold 18px font vendored from
[u8g2](https://github.com/olikraus/u8g2) in `u8g2_font_helvb18_tr.c` and
set at runtime via `canvas_set_custom_u8g2_font()` — Flipper's stock fonts
(`FontPrimary` etc.) top out too small for this use case.
