import os

import cairosvg
from PIL import Image

CANVAS_W = 128
CANVAS_H = 44
ICON_SIZE = 50
ICON_Y_OFFSET = -2
BOUNCE_OFFSET = 2
THRESHOLD = 140
IMAGES_DIR = "images"

OPENMOJI_BASE = "https://raw.githubusercontent.com/hfg-gmuend/openmoji/master/black/svg"

ANIMALS = {
    "cat": "1F431",
    "dog": "1F436",
    "bird": "1F426",
    "fish": "1F41F",
    "horse": "1F40E",
    "cow": "1F42E",
    "pig": "1F437",
    "sheep": "1F411",
    "rabbit": "1F430",
    "frog": "1F438",
    "giraffe": "1F992",
    "hen": "1F414",
}


def render_icon(codepoint: str) -> Image.Image:
    png_bytes = cairosvg.svg2png(
        url=f"{OPENMOJI_BASE}/{codepoint}.svg", output_width=400, output_height=400
    )
    import io

    im = Image.open(io.BytesIO(png_bytes)).convert("RGBA")
    bg = Image.new("RGBA", im.size, (255, 255, 255, 255))
    im = Image.alpha_composite(bg, im).convert("L")
    im = im.resize((ICON_SIZE, ICON_SIZE), Image.LANCZOS)
    return im.point(lambda p: 255 if p > THRESHOLD else 0, mode="L")


def frame(icon: Image.Image, y_offset: int) -> Image.Image:
    canvas = Image.new("L", (CANVAS_W, CANVAS_H), color=255)
    x = (CANVAS_W - ICON_SIZE) // 2
    y = ICON_Y_OFFSET + y_offset
    canvas.paste(icon, (x, y))
    return canvas.convert("1")


if __name__ == "__main__":
    for slug, codepoint in ANIMALS.items():
        icon = render_icon(codepoint)
        out_dir = os.path.join(IMAGES_DIR, slug)
        os.makedirs(out_dir, exist_ok=True)
        frame(icon, -BOUNCE_OFFSET).save(os.path.join(out_dir, "frame_0.png"))
        frame(icon, BOUNCE_OFFSET).save(os.path.join(out_dir, "frame_1.png"))
        with open(os.path.join(out_dir, "frame_rate"), "w") as f:
            f.write("2")
        print(f"Wrote {out_dir}/frame_0.png, frame_1.png, frame_rate")
