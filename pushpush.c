#include <gtk/gtk.h>
#include <glib/grand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 20 
#define NUM_IMAGES 5
#define ADDITIONAL_LABELS 8

typedef enum ID{
	BALL = 10,
	WALL = 100,
	BASE = 110
}ID;

typedef struct object_data{
	int id;
	int x,y;
}object_data;

object_data *players;
object_data *balls;

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
static void draw_image(cairo_t *cr, int x, int y, int width, int height, const gchar *image_path) {
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

            // Draw cell outline
	    cairo_set_source_rgb(cr, 0, 0, 0);  // 테두리 색상을 검정색으로 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_stroke(cr);

            // Draw cell background
            cairo_set_source_rgb(cr, 0.6, 0.6, 0.4);  // 배경색 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_fill(cr);
       	}
    }

    // Draw random images
    
    memset(board, 0, GRID_SIZE * GRID_SIZE);
    balls = (object_data *)malloc(sizeof(object_data) * NUM_IMAGES);
    gchar *ball_path = "./image/basketball.png"; 
    for(int i = 0; i < NUM_IMAGES; i++){
        int img_x = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
        int img_y = g_rand_int_range(g_rand_new(), 4, ((GRID_SIZE-1)-4))*cell_size;
        int img_width = cell_size; 
        int img_height = cell_size; 
        draw_image(cr, img_x, img_y, img_width, img_height, ball_path);
	
	board[(img_y/2)/10][(img_x/2)/10] = BALL;
        balls[i].id = BALL;
        balls[i].x = (img_x/2)/10;
        balls[i].y = (img_y/2)/10;
    } 
    
    display_board();

    gchar *wall_path = "./image/wall.png";
    for(int i = 0; i < NUM_IMAGES; i++){
        int img_x = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
        int img_y = g_rand_int_range(g_rand_new(), 4, ((GRID_SIZE-1)-4))*cell_size;
        int img_width = cell_size; 
        int img_height = cell_size;
	
	if(img_x < img_y){
		for(int j = 0; j < 3; j++){
			int alpha = j*cell_size;
       			draw_image(cr, img_x+alpha, img_y, img_width, img_height, wall_path);

			board[(img_y/2)/10][((img_x+alpha)/2)/10] = WALL;
		}
	}else{
		for(int j = 0; j < 3; j++){
			int alpha = j*cell_size;
			draw_image(cr, img_x, img_y+alpha,img_width, img_height, wall_path);
			board[( (img_y+alpha)/2)/10][((img_x)/2)/10] = WALL;
		}
	}
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
	
    gtk_main();

    free(balls);
    return status;
}

