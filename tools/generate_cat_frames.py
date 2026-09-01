from PIL import Image, ImageDraw

SIZE = 40
OUT_DIR = "images/cat"


def draw_cat(eyes_open: bool) -> Image.Image:
    img = Image.new("1", (SIZE, SIZE), color=1)
    draw = ImageDraw.Draw(img)

    draw.polygon([(4, 12), (12, 12), (7, 1)], fill=0)
    draw.polygon([(36, 12), (28, 12), (33, 1)], fill=0)

    draw.ellipse((5, 10, 34, 37), outline=0, width=2)

    if eyes_open:
        draw.ellipse((12, 20, 17, 25), fill=0)
        draw.ellipse((23, 20, 28, 25), fill=0)
    else:
        draw.line((12, 22, 17, 22), fill=0, width=2)
        draw.line((23, 22, 28, 22), fill=0, width=2)

    draw.polygon([(18, 25), (22, 25), (20, 28)], fill=0)
    draw.line((20, 28, 20, 30), fill=0, width=1)
    draw.line((20, 30, 16, 33), fill=0, width=1)
    draw.line((20, 30, 24, 33), fill=0, width=1)

    draw.line((0, 24, 10, 23), fill=0, width=1)
    draw.line((0, 28, 10, 28), fill=0, width=1)
    draw.line((39, 24, 29, 23), fill=0, width=1)
    draw.line((39, 28, 29, 28), fill=0, width=1)

    return img


if __name__ == "__main__":
    import os

    os.makedirs(OUT_DIR, exist_ok=True)
    draw_cat(eyes_open=True).save(f"{OUT_DIR}/frame_0.png")
    draw_cat(eyes_open=False).save(f"{OUT_DIR}/frame_1.png")
    with open(f"{OUT_DIR}/frame_rate", "w") as f:
        f.write("2")
    print(f"Wrote {OUT_DIR}/frame_0.png, frame_1.png, frame_rate")
