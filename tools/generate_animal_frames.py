import os

from PIL import Image, ImageDraw

SIZE = 40
IMAGES_DIR = "images"


def canvas():
    img = Image.new("1", (SIZE, SIZE), color=1)
    return img, ImageDraw.Draw(img)


def eyes(draw, cx, cy, spread, open_eyes, radius=3):
    lx, rx = cx - spread, cx + spread
    if open_eyes:
        draw.ellipse((lx - radius, cy - radius, lx + radius, cy + radius - 1), fill=0)
        draw.ellipse((rx - radius, cy - radius, rx + radius, cy + radius - 1), fill=0)
    else:
        draw.line((lx - radius, cy, lx + radius, cy), fill=0, width=2)
        draw.line((rx - radius, cy, rx + radius, cy), fill=0, width=2)


def gat(open_eyes):
    img, d = canvas()
    d.polygon([(4, 12), (12, 12), (7, 1)], fill=0)
    d.polygon([(36, 12), (28, 12), (33, 1)], fill=0)
    d.ellipse((5, 10, 34, 37), outline=0, width=2)
    eyes(d, 20, 22, 5, open_eyes)
    d.polygon([(18, 25), (22, 25), (20, 28)], fill=0)
    d.line((20, 28, 20, 30), fill=0, width=1)
    d.line((20, 30, 16, 33), fill=0, width=1)
    d.line((20, 30, 24, 33), fill=0, width=1)
    d.line((0, 24, 10, 23), fill=0, width=1)
    d.line((0, 28, 10, 28), fill=0, width=1)
    d.line((39, 24, 29, 23), fill=0, width=1)
    d.line((39, 28, 29, 28), fill=0, width=1)
    return img


def gos(open_eyes):
    img, d = canvas()
    d.ellipse((5, 10, 34, 37), outline=0, width=2)
    d.ellipse((0, 12, 10, 32), fill=0)
    d.ellipse((30, 12, 40, 32), fill=0)
    eyes(d, 20, 21, 5, open_eyes)
    d.ellipse((15, 27, 25, 33), fill=0)
    d.line((20, 33, 20, 35), fill=0, width=1)
    d.line((20, 35, 15, 37), fill=0, width=1)
    d.line((20, 35, 25, 37), fill=0, width=1)
    return img


def ocell(open_eyes):
    img, d = canvas()
    d.polygon([(17, 6), (21, 6), (19, 1)], fill=0)
    d.ellipse((12, 4, 28, 20), outline=0, width=2)
    d.ellipse((6, 16, 34, 38), outline=0, width=2)
    d.arc((5, 22, 17, 34), start=100, end=220, fill=0, width=2)
    eyes(d, 20, 12, 5, open_eyes, radius=2)
    d.polygon([(20, 12), (26, 10), (26, 15)], fill=0)
    return img


def peix(open_eyes):
    img, d = canvas()
    d.ellipse((3, 12, 29, 28), outline=0, width=2)
    d.polygon([(29, 11), (29, 29), (39, 20)], outline=0, width=2)
    d.polygon([(13, 12), (19, 12), (16, 5)], outline=0, width=2)
    if open_eyes:
        d.ellipse((9, 16, 14, 21), fill=0)
    else:
        d.line((9, 18, 14, 18), fill=0, width=2)
    d.line((1, 19, 5, 19), fill=0, width=1)
    d.line((1, 21, 5, 21), fill=0, width=1)
    return img


def cavall(open_eyes):
    img, d = canvas()
    d.polygon([(13, 6), (18, 6), (15, 0)], fill=0)
    d.polygon([(27, 6), (22, 6), (25, 0)], fill=0)
    d.ellipse((11, 4, 29, 34), outline=0, width=2)
    eyes(d, 20, 14, 5, open_eyes)
    d.ellipse((15, 28, 18, 31), fill=0)
    d.ellipse((22, 28, 25, 31), fill=0)
    d.line((29, 8, 33, 8), fill=0, width=1)
    d.line((29, 13, 33, 13), fill=0, width=1)
    d.line((29, 18, 33, 18), fill=0, width=1)
    return img


def vaca(open_eyes):
    img, d = canvas()
    d.ellipse((5, 10, 34, 37), outline=0, width=2)
    d.ellipse((11, 2, 16, 7), outline=0, width=2)
    d.ellipse((23, 2, 28, 7), outline=0, width=2)
    d.ellipse((0, 15, 7, 22), outline=0, width=2)
    d.ellipse((32, 15, 39, 22), outline=0, width=2)
    d.ellipse((23, 13, 31, 20), fill=0)
    eyes(d, 20, 21, 5, open_eyes)
    d.ellipse((13, 28, 27, 35), outline=0, width=2)
    d.ellipse((16, 30, 18, 32), fill=0)
    d.ellipse((22, 30, 24, 32), fill=0)
    return img


def porc(open_eyes):
    img, d = canvas()
    d.ellipse((5, 10, 34, 37), outline=0, width=2)
    d.polygon([(6, 8), (13, 9), (9, 2)], fill=0)
    d.polygon([(34, 8), (27, 9), (31, 2)], fill=0)
    eyes(d, 20, 20, 5, open_eyes)
    d.ellipse((13, 26, 27, 35), outline=0, width=2)
    d.ellipse((16, 29, 18, 32), fill=0)
    d.ellipse((22, 29, 24, 32), fill=0)
    return img


def ovella(open_eyes):
    img, d = canvas()
    for cx, cy, r in [(13, 9, 7), (27, 9, 7), (7, 18, 7), (33, 18, 7), (20, 6, 7)]:
        d.ellipse((cx - r, cy - r, cx + r, cy + r), outline=0, width=2)
    d.ellipse((8, 12, 32, 34), outline=0, width=2)
    d.ellipse((2, 20, 9, 28), outline=0, width=2)
    d.ellipse((31, 20, 38, 28), outline=0, width=2)
    d.ellipse((14, 18, 26, 30), outline=0, width=2)
    eyes(d, 20, 22, 5, open_eyes, radius=2)
    d.polygon([(18, 25), (22, 25), (20, 27)], fill=0)
    return img


def conill(open_eyes):
    img, d = canvas()
    d.ellipse((9, 0, 16, 21), outline=0, width=2)
    d.ellipse((24, 0, 31, 21), outline=0, width=2)
    d.ellipse((7, 14, 33, 38), outline=0, width=2)
    eyes(d, 20, 23, 5, open_eyes)
    d.polygon([(18, 27), (22, 27), (20, 30)], fill=0)
    d.line((20, 30, 20, 32), fill=0, width=1)
    d.line((20, 32, 16, 34), fill=0, width=1)
    d.line((20, 32, 24, 34), fill=0, width=1)
    return img


def granota(open_eyes):
    img, d = canvas()
    d.ellipse((4, 14, 36, 36), outline=0, width=2)
    if open_eyes:
        d.ellipse((6, 4, 16, 14), outline=0, width=2)
        d.ellipse((24, 4, 34, 14), outline=0, width=2)
        d.ellipse((9, 7, 13, 11), fill=0)
        d.ellipse((27, 7, 31, 11), fill=0)
    else:
        d.ellipse((6, 4, 16, 14), outline=0, width=2)
        d.ellipse((24, 4, 34, 14), outline=0, width=2)
        d.line((8, 9, 14, 9), fill=0, width=2)
        d.line((26, 9, 32, 9), fill=0, width=2)
    d.arc((10, 20, 30, 34), start=10, end=170, fill=0, width=2)
    return img


def girafa(open_eyes):
    img, d = canvas()
    d.line((15, 6, 15, 1), fill=0, width=2)
    d.line((25, 6, 25, 1), fill=0, width=2)
    d.ellipse((13, -1, 17, 3), fill=0)
    d.ellipse((23, -1, 27, 3), fill=0)
    d.ellipse((11, 6, 29, 34), outline=0, width=2)
    eyes(d, 20, 15, 6, open_eyes, radius=2)
    d.ellipse((12, 20, 16, 24), fill=0)
    d.ellipse((25, 23, 29, 27), fill=0)
    d.line((17, 31, 23, 31), fill=0, width=1)
    return img


def gallina(open_eyes):
    img, d = canvas()
    d.ellipse((11, 2, 19, 10), fill=0)
    d.ellipse((16, -2, 24, 6), fill=0)
    d.ellipse((21, 2, 29, 10), fill=0)
    d.ellipse((8, 8, 32, 32), outline=0, width=2)
    eyes(d, 20, 19, 5, open_eyes)
    d.polygon([(16, 25), (24, 25), (20, 32)], fill=0)
    d.polygon([(18, 32), (22, 32), (20, 36)], fill=0)
    return img


ANIMALS = {
    "cat": gat,
    "dog": gos,
    "bird": ocell,
    "fish": peix,
    "horse": cavall,
    "cow": vaca,
    "pig": porc,
    "sheep": ovella,
    "rabbit": conill,
    "frog": granota,
    "giraffe": girafa,
    "hen": gallina,
}


if __name__ == "__main__":
    for slug, draw_fn in ANIMALS.items():
        out_dir = os.path.join(IMAGES_DIR, slug)
        os.makedirs(out_dir, exist_ok=True)
        draw_fn(True).save(os.path.join(out_dir, "frame_0.png"))
        draw_fn(False).save(os.path.join(out_dir, "frame_1.png"))
        with open(os.path.join(out_dir, "frame_rate"), "w") as f:
            f.write("2")
        print(f"Wrote {out_dir}/frame_0.png, frame_1.png, frame_rate")
