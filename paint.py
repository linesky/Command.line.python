import tkinter as tk
from tkinter import filedialog, colorchooser
import numpy as np
from PIL import Image

# Dimensões e cores
PIXEL_SIZE = 8
GRID_SIZE = 32
PALETA_VGA = [
    "#000000", "#0000AA", "#00AA00", "#00AAAA", "#AA0000", "#AA00AA", "#AA5500", "#AAAAAA",
    "#555555", "#5555FF", "#55FF55", "#55FFFF", "#FF5555", "#FF55FF", "#FFFF55", "#FFFFFF"
]

class PaintApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Bitmap Paint")
        
        self.bitmap = np.zeros((GRID_SIZE, GRID_SIZE, 3), dtype=np.uint8)
        self.current_color = PALETA_VGA[15]  # Cor branca

        # Configuração dos botões
        self.new_button = tk.Button(root, text="New", command=self.new_bitmap)
        self.new_button.pack(side=tk.LEFT)

        self.save_button = tk.Button(root, text="Save", command=self.save_bitmap)
        self.save_button.pack(side=tk.LEFT)

        self.load_button = tk.Button(root, text="Load", command=self.load_bitmap)
        self.load_button.pack(side=tk.LEFT)

        # Área de desenho
        self.canvas = tk.Canvas(root, width=GRID_SIZE * PIXEL_SIZE, height=GRID_SIZE * PIXEL_SIZE, bg="white")
        self.canvas.pack()

        # Paleta de cores
        self.palette_frame = tk.Frame(root)
        self.palette_frame.pack(side=tk.LEFT)
        for color in PALETA_VGA:
            btn = tk.Button(self.palette_frame, bg=color, width=2, height=1, command=lambda c=color: self.set_color(c))
            btn.pack(side=tk.TOP)

        # Eventos de desenho
        self.canvas.bind("<B1-Motion>", self.paint_pixel)
        self.canvas.bind("<Button-1>", self.paint_pixel)
        
        # Desenha o grid inicial
        self.draw_grid()

    def set_color(self, color):
        self.current_color = color

    def new_bitmap(self):
        self.bitmap.fill(0)  # Pinta tudo de preto
        self.draw_grid()

    def save_bitmap(self):
        file_path = filedialog.asksaveasfilename(defaultextension=".bmp", filetypes=[("Bitmap files", "*.bmp")])
        if file_path:
            img = Image.fromarray(self.bitmap, 'RGB')
            img.save(file_path)

    def load_bitmap(self):
        file_path = filedialog.askopenfilename(filetypes=[("Bitmap files", "*.bmp")])
        if file_path:
            img = Image.open(file_path)
            img = img.resize((GRID_SIZE, GRID_SIZE), Image.NEAREST)
            self.bitmap = np.array(img)
            self.draw_grid()

    def paint_pixel(self, event):
        x = event.x // PIXEL_SIZE
        y = event.y // PIXEL_SIZE
        if 0 <= x < GRID_SIZE and 0 <= y < GRID_SIZE:
            color = self.canvas.winfo_rgb(self.current_color)
            self.bitmap[y, x] = [int(c/256) for c in color]
            self.draw_grid()

    def draw_grid(self):
        self.canvas.delete("all")
        for y in range(GRID_SIZE):
            for x in range(GRID_SIZE):
                color = "#{:02x}{:02x}{:02x}".format(*self.bitmap[y, x])
                self.canvas.create_rectangle(
                    x * PIXEL_SIZE, y * PIXEL_SIZE,
                    (x + 1) * PIXEL_SIZE, (y + 1) * PIXEL_SIZE,
                    fill=color, outline="gray"
                )

if __name__ == "__main__":
    root = tk.Tk()
    app = PaintApp(root)
    root.configure(bg='black')
    root.mainloop()

