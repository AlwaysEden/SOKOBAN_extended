#include <gtk/gtk.h>
#include <glib/grand.h>
#include <math.h>

#define GRID_SIZE 20
#define NUM_IMAGES 5

static void draw_image(cairo_t *cr, int x, int y, int width, int height, const gchar *image_path) {
	double radius = width / 2.0;
    	double center_x = x + radius;
    	double center_y = y + radius;
    	cairo_surface_t *image_surface = cairo_image_surface_create_from_png(image_path); // Use the appropriate image format
    	if (cairo_surface_status(image_surface) == CAIRO_STATUS_SUCCESS) {
		cairo_save(cr);
        	cairo_scale(cr, (double)(width) / (cairo_image_surface_get_width(image_surface)),(double)(height) / (cairo_image_surface_get_height(image_surface)));
		//cairo_set_source_surface(cr, image_surface, center_x / ((double)width / cairo_image_surface_get_width(image_surface)),center_y / ((double)height / cairo_image_surface_get_height(image_surface)));
		//cairo_set_source_surface(cr, image_surface, x, y);
		int modified_x = (int)(x/ ( (double)width/cairo_image_surface_get_width(image_surface)));	
		int modified_y = (int)(y/ ( (double)height/cairo_image_surface_get_height(image_surface)));	
		cairo_set_source_surface(cr, image_surface, modified_x, modified_y); 
		cairo_stroke(cr);
        	cairo_paint(cr);
		cairo_restore(cr);  // 이전 상태로 복원
        	cairo_surface_destroy(image_surface);
    }
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

           /* // Draw cell outline
	    cairo_set_source_rgb(cr, 0.8, 0.5, 0.2);
            cairo_rectangle(cr, x, y, cell_size, cell_size);
	    cairo_fill(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
	    cairo_stroke(cr);
        */	
	    cairo_set_source_rgb(cr, 0, 0, 0);  // 테두리 색상을 검정색으로 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_stroke(cr);

            // Draw cell background
            cairo_set_source_rgb(cr, 0.8, 0.5, 0.2);  // 배경색 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_fill(cr);
	}
    }

/*    // Draw additional content on the right side
    cairo_set_source_rgb(cr, 0, 0, 1); // Set color to blue
    int right_margin = cell_size * 10;
    int additional_width = window_width - GRID_SIZE * cell_size;
    int x = GRID_SIZE * cell_size;

    cairo_rectangle(cr, x, 0, additional_width, window_height);
    cairo_fill(cr);
*/
    // Draw random images
    gchar *image_paths = "./ball.png";
    for(int i = 0; i < NUM_IMAGES; i++){
	int x = GRID_SIZE * cell_size;
        int img_x = g_rand_int_range(g_rand_new(), 0, (GRID_SIZE-1))*cell_size;
        int img_y = g_rand_int_range(g_rand_new(), 0, ((GRID_SIZE-1)))*cell_size;
        int img_width = cell_size; // Adjust as needed
        int img_height = cell_size; // Adjust as needed
	printf("x: %d, y: %d, width: %d, height: %d\n",img_x, (img_y), img_width, img_height);
        draw_image(cr, img_x, img_y, img_width, img_height, image_paths);
    }
    
   /* for (int i = 0; i < NUM_IMAGES; ++i) {
        int img_x = g_rand_int_range(g_rand_new(), 0, (GRID_SIZE-1) * cell_size);
        int img_y = g_rand_int_range(g_rand_new(), 0, (GRID_SIZE-1) * cell_size);

        draw_image(cr, img_x, img_y, cell_size, image_paths);
    }
*/
    return FALSE;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grid with Images");
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

    app = gtk_application_new("org.example.grid_with_images", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
