
#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//sudo apt-get install libgtk-3-dev
//gcc -o simple_paint simple_paint.c `pkg-config --cflags --libs gtk+-3.0`
// Definições para a matriz de pixels e a paleta VGA
#define PIXEL_SIZE 8
#define GRID_SIZE 32
#define NUM_COLORS 16

typedef struct {
    int x;
    int y;
    GdkRGBA color;
} Pixel;

Pixel pixels[GRID_SIZE][GRID_SIZE];
GdkRGBA palette[NUM_COLORS];
GdkRGBA current_color;

// Funções de callback
static void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);
static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
static void m0_clicked(GtkButton *button, gpointer data);
static void m1_clicked(GtkButton *button, gpointer data);
static void m2_clicked(GtkButton *button, gpointer data);
static void m3_clicked(GtkButton *button, gpointer data);
static void m4_clicked(GtkButton *button, gpointer data);
static void m5_clicked(GtkButton *button, gpointer data);
static void m6_clicked(GtkButton *button, gpointer data);
static void m7_clicked(GtkButton *button, gpointer data);
static void m8_clicked(GtkButton *button, gpointer data);
static void m9_clicked(GtkButton *button, gpointer data);
static void m10_clicked(GtkButton *button, gpointer data);
static void m11_clicked(GtkButton *button, gpointer data);
static void m12_clicked(GtkButton *button, gpointer data);
static void m13_clicked(GtkButton *button, gpointer data);
static void m14_clicked(GtkButton *button, gpointer data);
static void m15_clicked(GtkButton *button, gpointer data);
static void on_new_clicked(GtkButton *button, gpointer data);
static void on_save_clicked(GtkButton *button, gpointer data);
static void on_load_clicked(GtkButton *button, gpointer data);

// Inicialização da paleta VGA
void init_palette() {
    double colors[NUM_COLORS][4] = {
        {0.0, 0.0, 0.0,1.0}, {0.0, 0.0, 0.667,1.0}, {0.0, 0.667, 0.0,1.0}, {0.0, 0.667, 0.667,1.0},
        {0.667, 0.0, 0.0,1.0}, {0.667, 0.0, 0.667,1.0}, {0.667, 0.333, 0.0,1.0}, {0.667, 0.667, 0.667,1.0},
        {0.333, 0.333, 0.333,1.0}, {0.333, 0.333, 1.0,1.0}, {0.333, 1.0, 0.333,1.0}, {0.333, 1.0, 1.0,1.0},
        {1.0, 0.333, 0.333,1.0}, {1.0, 0.333, 1.0,1.0}, {1.0, 1.0, 0.333,1.0}, {1.0, 1.0, 1.0,1.0}
    };

    for (int i = 0; i < NUM_COLORS; i++) {
        palette[i].red = colors[i][0];
        palette[i].green = colors[i][1];
        palette[i].blue = colors[i][2];
        palette[i].alpha = 1.0;
    }
    current_color = palette[0];
}

// Função principal
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    init_palette();

    // Criar janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Paint");
    gtk_window_set_default_size(GTK_WINDOW(window), GRID_SIZE * PIXEL_SIZE + 20, GRID_SIZE * PIXEL_SIZE + 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Layout vertical
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Canvas de desenho
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, GRID_SIZE * PIXEL_SIZE, GRID_SIZE * PIXEL_SIZE);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "button-press-event", G_CALLBACK(on_button_press), NULL);
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK);
    
    init_palette();
    // Botões
    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
    GtkWidget *m_button;
    char css[1024];
    
    m_button = gtk_button_new_with_label("black");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m0_clicked), NULL);
    m_button = gtk_button_new_with_label("blue");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m1_clicked), NULL);
    m_button = gtk_button_new_with_label("green");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m2_clicked), NULL);
    m_button = gtk_button_new_with_label("cyan");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m3_clicked), NULL);
    m_button = gtk_button_new_with_label("red");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m4_clicked), NULL);
    m_button = gtk_button_new_with_label("magenta");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m5_clicked), NULL);
    m_button = gtk_button_new_with_label("yellow");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m6_clicked), NULL);
    m_button = gtk_button_new_with_label("white");
    gtk_box_pack_start(GTK_BOX(hbox2), m_button, TRUE, TRUE, 0);
    GtkWidget *hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
    g_signal_connect(m_button, "clicked", G_CALLBACK(m7_clicked), NULL);
    m_button = gtk_button_new_with_label("black");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m8_clicked), NULL);
    m_button = gtk_button_new_with_label("blue");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m9_clicked), NULL);
    m_button = gtk_button_new_with_label("green");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m11_clicked), NULL);
    m_button = gtk_button_new_with_label("cyan");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m11_clicked), NULL);
    m_button = gtk_button_new_with_label("red");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m12_clicked), NULL);
    m_button = gtk_button_new_with_label("magenta");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m13_clicked), NULL);
    m_button = gtk_button_new_with_label("yellow");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m14_clicked), NULL);
    m_button = gtk_button_new_with_label("white");
    gtk_box_pack_start(GTK_BOX(hbox3), m_button, TRUE, TRUE, 0);
    
    g_signal_connect(m_button, "clicked", G_CALLBACK(m15_clicked), NULL);
    
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
    GtkWidget *new_button = gtk_button_new_with_label("New");
    gtk_box_pack_start(GTK_BOX(hbox), new_button, TRUE, TRUE, 0);
    g_signal_connect(new_button, "clicked", G_CALLBACK(on_new_clicked), drawing_area);

    

    GtkWidget *save_button = gtk_button_new_with_label("Save");
    gtk_box_pack_start(GTK_BOX(hbox), save_button, TRUE, TRUE, 0);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), NULL);

    GtkWidget *load_button = gtk_button_new_with_label("Load");
    gtk_box_pack_start(GTK_BOX(hbox), load_button, TRUE, TRUE, 0);
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_clicked), drawing_area);

    gtk_widget_show_all(window);
    
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            pixels[y][x].color = (GdkRGBA) {1.0, 1.0, 1.0, 1.0}; // Branco
        }
    }
    gtk_widget_queue_draw(GTK_WIDGET(NULL));
    
    gtk_main();
    return 0;
}

// Função de callback para desenhar no canvas
static void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            cairo_set_source_rgba(cr, pixels[y][x].color.red, pixels[y][x].color.green, pixels[y][x].color.blue, pixels[y][x].color.alpha);
            cairo_rectangle(cr, x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
            cairo_fill(cr);
        }
    }
}

// Função de callback para clicar no canvas
static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    int x = event->x / PIXEL_SIZE;
    int y = event->y / PIXEL_SIZE;

    if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
        pixels[y][x].color = current_color;
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}
static void m0_clicked(GtkButton *button, gpointer data) {
current_color = palette[0];

}

static void m1_clicked(GtkButton *button, gpointer data) {
current_color = palette[1];

}
static void m2_clicked(GtkButton *button, gpointer data) {
current_color = palette[2];

}
static void m3_clicked(GtkButton *button, gpointer data) {
current_color = palette[3];

}
static void m4_clicked(GtkButton *button, gpointer data) {
current_color = palette[4];

}
static void m5_clicked(GtkButton *button, gpointer data) {
current_color = palette[5];

}
static void m6_clicked(GtkButton *button, gpointer data) {
current_color = palette[6];

}
static void m7_clicked(GtkButton *button, gpointer data) {
current_color = palette[7];

}
static void m8_clicked(GtkButton *button, gpointer data) {
current_color = palette[8];

}
static void m9_clicked(GtkButton *button, gpointer data) {
current_color = palette[9];

}
static void m10_clicked(GtkButton *button, gpointer data) {
current_color = palette[10];

}
static void m11_clicked(GtkButton *button, gpointer data) {
current_color = palette[11];

}
static void m12_clicked(GtkButton *button, gpointer data) {
current_color = palette[12];

}
static void m13_clicked(GtkButton *button, gpointer data) {
current_color = palette[13];

}
static void m14_clicked(GtkButton *button, gpointer data) {
current_color = palette[14];

}
static void m15_clicked(GtkButton *button, gpointer data) {
current_color = palette[15];

}
// Função para criar um novo bitmap
static void on_new_clicked(GtkButton *button, gpointer data) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            pixels[y][x].color = (GdkRGBA) {1.0, 1.0, 1.0, 1.0}; // Branco
        }
    }
    gtk_widget_queue_draw(GTK_WIDGET(data));
}

// Função para salvar o bitmap em um arquivo BMP
static void on_save_clicked(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File", NULL,
        GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "wb");

        if (file) {
            // Cabeçalho BMP
            unsigned char bmp_file_header[14] = {'B', 'M'};
            unsigned char bmp_info_header[40] = {0};
            int file_size = 54 + GRID_SIZE * GRID_SIZE * 3;
            int width = GRID_SIZE * PIXEL_SIZE;
            int height = GRID_SIZE * PIXEL_SIZE;

            bmp_file_header[2] = (unsigned char)(file_size);
            bmp_file_header[3] = (unsigned char)(file_size >> 8);
            bmp_file_header[4] = (unsigned char)(file_size >> 16);
            bmp_file_header[5] = (unsigned char)(file_size >> 24);

            bmp_file_header[10] = 54;

            bmp_info_header[0] = 40;
            bmp_info_header[4] = (unsigned char)(width);
            bmp_info_header[5] = (unsigned char)(width >> 8);
            bmp_info_header[6] = (unsigned char)(width >> 16);
            bmp_info_header[7] = (unsigned char)(width >> 24);
            bmp_info_header[8] = (unsigned char)(height);
            bmp_info_header[9] = (unsigned char)(height >> 8);
            bmp_info_header[10] = (unsigned char)(height >> 16);
            bmp_info_header[11] = (unsigned char)(height >> 24);
            bmp_info_header[12] = 1;
            bmp_info_header[14] = 24;

            fwrite(bmp_file_header, 1, 14, file);
            fwrite(bmp_info_header, 1, 40, file);

            for (int y = GRID_SIZE - 1; y >= 0; y--) {
                for (int x = 0; x < GRID_SIZE; x++) {
                    unsigned char r = (unsigned char)(pixels[y][x].color.red * 255);
                    unsigned char g = (unsigned char)(pixels[y][x].color.green * 255);
                    unsigned char b = (unsigned char)(pixels[y][x].color.blue * 255);
                    fwrite(&b, 1, 1, file);
                    fwrite(&g, 1, 1, file);
                    fwrite(&r, 1, 1, file);
                }
            }

            fclose(file);
            g_free(filename);
        }
    }

    gtk_widget_destroy(dialog);
}

// Função para carregar um bitmap de um arquivo BMP
static void on_load_clicked(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File", NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "rb");

        if (file) {
            fseek(file, 54, SEEK_SET); // Pular cabeçalho BMP

            for (int y = GRID_SIZE - 1; y >= 0; y--) {
                for (int x = 0; x < GRID_SIZE; x++) {
                    unsigned char b, g, r;
                    fread(&b, 1, 1, file);
                    fread(&g, 1, 1, file);
                    fread(&r, 1, 1, file);
                    pixels[y][x].color.red = r / 255.0;
                    pixels[y][x].color.green = g / 255.0;
                    pixels[y][x].color.blue = b / 255.0;
                    pixels[y][x].color.alpha = 1.0;
                }
            }

            fclose(file);
            g_free(filename);
            gtk_widget_queue_draw(GTK_WIDGET(data));
        }
    }

    gtk_widget_destroy(dialog);
}
