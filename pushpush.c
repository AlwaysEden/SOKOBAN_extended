#include <gtk/gtk.h>
#include <glib/grand.h>

#define GRID_SIZE 40
#define NUM_CIRCLES 5
#define ADDITIONAL_LABELS 8

typedef enum ID{
	BALL = 10,
	HURDLE = 100
}ID;

int board[GRID_SIZE][GRID_SIZE];

void
display_board()
{
        printf("===========\n") ;
        for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                        if (board[i][j] == 0) {
                                printf("+ ") ;
                        } else {
                                printf("%d ", board[i][j]) ;
                        }
                }
                printf("\n") ;
        }
        printf("===========\n") ;
}

static void draw_circle(cairo_t *cr, int x, int y, int size) {
    double radius = size / 2.0;
    double center_x = x + radius;
    double center_y = y + radius;

    cairo_arc(cr, center_x, center_y, radius, 0, 2 * G_PI);
    cairo_fill(cr);
}

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int window_width = gdk_window_get_width(gtk_widget_get_window(widget));
    int window_height = gdk_window_get_height(gtk_widget_get_window(widget));
    int cell_size = window_height/ (GRID_SIZE);

    // Draw grid
    cairo_set_source_rgb(cr, 0, 0, 0);
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int x = i * cell_size;
            int y = j * cell_size;

            // Draw cell outline
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_stroke(cr);
        }
    }

    // Draw random circles
    memset(board, 0, GRID_SIZE * GRID_SIZE);
    cairo_set_source_rgb(cr, 1, 0, 0); // Set circle color to red
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        int x = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4) * cell_size;
        int y = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4) * cell_size;
	g_print("(x = %d, y = %d\n",x,y);
	board[(y/2)/10][(x/2)/10] = BALL;
        draw_circle(cr, x, y, cell_size);
    }
    display_board();
/*
    // Draw additional content on the right side
    cairo_set_source_rgb(cr, 0, 0, 1); // Set color to blue
    int right_margin = cell_size * 10;
    int additional_width = window_width - GRID_SIZE * cell_size;
    int x = GRID_SIZE * cell_size;

    cairo_rectangle(cr, x, 0, additional_width, window_height);
    cairo_fill(cr);

   // Add labels on the right side
    cairo_set_source_rgb(cr, 1, 1, 1); // Set label color to white
    int label_height = window_height / ADDITIONAL_LABELS;
    for (int i = 0; i < ADDITIONAL_LABELS; ++i) {
        GtkWidget *label = gtk_label_new("Label");
        gtk_widget_set_size_request(label, additional_width, label_height);
        gtk_container_add(GTK_CONTAINER(widget), label);

        int label_y = i * label_height;
        gtk_fixed_put(GTK_FIXED(widget), label, x, label_y);
    }
*/
    return FALSE;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Push Push!!");
    gtk_window_set_default_size(GTK_WINDOW(window), (GRID_SIZE + 10) * 20, GRID_SIZE * 20);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw_area);

    g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(draw_callback), NULL);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.example.pushpush", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

