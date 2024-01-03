#include <gtk/gtk.h>
#include <glib/grand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gprintf.h>

#define GRID_SIZE 20 
#define NUM_BALLS 8
#define NUM_WALLS 15
#define NUM_PLAYER 1

typedef enum ID{
	BALL = 10,
	WALL = 100,
	BASE = 110
}ID;

typedef struct object_data{
	int id;
	int x,y;
	int point;
}object_data;

object_data *player; 


int board[GRID_SIZE][GRID_SIZE];

GtkWidget *window;
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

int convertIntoCoordinate(int num){
	return (num/2)/10;
}

static void draw_image(cairo_t *cr, int x, int y, int width, int height, const gchar *image_path) {
        cairo_surface_t *image_surface = cairo_image_surface_create_from_png(image_path); // Use the appropriate image format
        if (cairo_surface_status(image_surface) == CAIRO_STATUS_SUCCESS) {
                cairo_save(cr);
                cairo_scale(cr, (double)(width) / (cairo_image_surface_get_width(image_surface)),(double)(height) / (cairo_image_surface_get_height(image_surface)));
                int modified_x = (int)(x/ ( (double)width/cairo_image_surface_get_width(image_surface)));
                int modified_y = (int)(y/ ( (double)height/cairo_image_surface_get_height(image_surface)));
                cairo_set_source_surface(cr, image_surface, modified_x, modified_y);
                cairo_stroke(cr);
                cairo_paint(cr);
                cairo_restore(cr);  // 이전 상태로 복원
                cairo_surface_destroy(image_surface);
    }
}

void makeInitialState(){

	int img_x;
	int img_y;

	//make ball location
	board[1][2] = BALL;
	board[1][5] = BALL;
/*	balls = (object_data *)malloc(sizeof(object_data)*NUM_BALLS);
	for(int i = 0; i < NUM_IMAGES; i++){
        	img_x = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
        	img_y = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
		
		 balls[i].id = BALL;
		 balls[i].x = img_x;
		 balls[i].y = img_y;

		int converted_x = convertIntoCoordinate(img_x);
		int converted_y = convertIntoCoordinate(img_y);
		board[converted_y][converted_x] = BALL;
	
	}
*/
	board[3][3] = WALL;
	board[2][5] = WALL;

	board[1][1] = BASE;

	board[3][4] = 1;
/*	walls = (object_data *)malloc(sizeof(object_data)*NUM_WALLS);
	for(int i = 0; i < NUM_IMAGES; i++){
        	img_x = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
        	img_y = g_rand_int_range(g_rand_new(), 4, (GRID_SIZE-1)-4)*cell_size;
		
		int converted_x = convertIntoCoordinate(img_x);
		int converted_y = convertIntoCoordinate(img_y);
		

		int flag = 0;
		if(img_x < img_y){
			for(int j = 0; j < 3; j++){
				int alpha = j*cell_size;
				if(board[converted_y][((img_x+alpha)/2)/10] == 0){
					i++;
					flag == 1;
					break;
				}
			}
			if(flag == 0){
				for(int z = 0; z < 3; z++){
					int alpha = z * cell_size;	
					board[converted_y][((img_x+alpha)/2)/10] == WALL;
				}
			}
		}else{
			for(int j = 0; j < 3; j++){
				int alpha = j*cell_size;
				if(board[((img_y+alpha)/2)/10][converted_x] == 0){
					i++;
					flag == 1;
					break;
				}
			}
			if(flag == 0){
				for(int z = 0; z < 3; z++){
					int alpha = z * cell_size;	
					board[((img_y+alpha)/2)/10][converted_x] == WALL;
				}
			}

		}
	}
		if(board[converted_y][((img_x+alpha)/2)/10] != 0) continue;
		balls[i].id = BALL;
		balls[i].x = img_x;
		balls[i].y = img_y;

		board[converted_y][converted_x] = BALL;
        	balls[i].id = BALL;
        	balls[i].x = converted_x;
	
	}
*/
}
static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int window_width = gdk_window_get_width(gtk_widget_get_window(widget));
    int window_height = gdk_window_get_height(gtk_widget_get_window(widget));
    int cell_size = window_height/ (GRID_SIZE);

    cairo_set_source_rgb(cr, 0, 0, 0);
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int x = j * cell_size;
            int y = i * cell_size;

            // Draw cell outline
	    cairo_set_source_rgb(cr, 0, 0, 0);  // 테두리 색상을 검정색으로 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_stroke(cr);

            // Draw cell background
            cairo_set_source_rgb(cr, 0.6, 0.6, 0.4);  // 배경색 설정
            cairo_rectangle(cr, x, y, cell_size, cell_size);
            cairo_fill(cr);

	    if(board[i][j] == BALL){
		gchar *ball_path = "./image/basketball.png"; 
		g_print("y:%d x: %d\n",y, x);
		draw_image(cr, x, y, cell_size, cell_size, ball_path);
	    }else if(board[i][j] == WALL){
		gchar *wall_path = "./image/wall.png";
		draw_image(cr, x, y, cell_size, cell_size, wall_path);
	    }else if(board[i][j] == BASE){
            	cairo_set_source_rgb(cr, 0, 0, 0);  // 배경색 설정
            	cairo_rectangle(cr, x, y, cell_size, cell_size);
            	cairo_fill(cr);
	    }else if(board[i][j] < BALL && board[i][j] != 0){ //It means player
		gchar *player_path = "./image/player1.png";
		draw_image(cr, x, y, cell_size, cell_size, player_path);
		player = (object_data *)malloc(sizeof(object_data));
		player->id = 1; //TODO: Multi-player일 때 수정.
		player->x = j;
		player->y = i;
	    }

  	    if (j == GRID_SIZE - 1) {  // 수정된 부분
                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
                cairo_set_font_size(cr, 15.0);

                // Draw names
		
                for (int nameIndex = 0; nameIndex < NUM_PLAYER; ++nameIndex) {
                    int nameY = nameIndex * (window_height / 8);
                    cairo_move_to(cr, GRID_SIZE*cell_size + cell_size, nameY + 30);  // Adjust the position as needed
		    gchar content[128];
		    g_sprintf(content,"You get %d score\n.",player->point);
                    //cairo_show_text(cr, content);  // Replace with your name or text
                }
            }
       	}

    }

/*
    cairo_set_source_rgb(cr, 0, 0, 1); // Set color to blue
    int right_margin = cell_size * 10;
    int additional_width = window_width - GRID_SIZE * cell_size;
    int x = GRID_SIZE * cell_size;
    cairo_rectangle(cr, x, 0, additional_width, cell_size);
    cairo_fill(cr);
 
    GtkWidget *label = gtk_label_new("Label");
    gtk_widget_set_size_request(label, additional_width, cell_size);
    gtk_container_add(GTK_CONTAINER(window),label);
    int width = (GRID_SIZE * cell_size) + cell_size;
	    int height = cell_size * 2;
    for (int i = 0; i < 4; ++i) {
        GtkWidget *label = gtk_label_new("Label");
        gtk_widget_set_size_request(label, width, height+(i*cell_size));
        gtk_container_add(GTK_CONTAINER(window), label);
	
        //int label_y = i * label_height;
        //gtk_fixed_put(GTK_FIXED(widget), label, x, label_y);
    }
*/

	display_board();
    return FALSE;
}

static gboolean on_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    // Respond to key press events
    switch (event->keyval) {
	    case GDK_KEY_Right:
		    if(board[player->y][player->x + 1] == 0){
			    	if(player->x+1 < GRID_SIZE){
					g_print("Just move\n");
					board[player->y][player->x] = 0;
					board[player->y][player->x+1] = player->id;
				}
		    }else if(board[player->y][player->x + 1] == BALL){
				if(player->x+2 < GRID_SIZE && (board[player->y][player->x+2] == 0 || board[player->y][player->x+2] == BASE)){
					g_print("Ball move\n");
					board[player->y][player->x] = 0;
					board[player->y][player->x+1] = player->id;
					
					if(board[player->y][player->x+2] == BASE){
						player->point++;
						break;		
					}
					board[player->y][player->x+2] = BALL;
				}
		    }else if(board[player->y][player->x + 1] == WALL || board[player->y][player->x + 1] == BASE){
				//nothing
		    }else{ //Multi_play에서 상대방과 부딪혔을 때
			//nothing
		    }
		    break;
	    case GDK_KEY_Left:
		    if(board[player->y][player->x - 1] == 0){
				if(player->x-1 >= 0){	
			    		board[player->y][player->x] = 0;
					board[player->y][player->x-1] = player->id;
				}
		    }else if(board[player->y][player->x - 1] == BALL){
				if(player->x-2 >= 0 && (board[player->y][player->x-2] == 0 || board[player->y][player->x-2] == BASE)){
					board[player->y][player->x] = 0;
					board[player->y][player->x-1] = player->id;
					if(board[player->y][player->x-2] == BASE){
						player->point++;
						break;
					}
					board[player->y][player->x-2] = BALL;
				}
		    }else if(board[player->y][player->x - 1] == WALL || board[player->y][player->x - 1] == BASE){
				//nothing
		    }else{ //Multi_play에서 상대방과 부딪혔을 때
			//nothing
		    }
		    break;
	    case GDK_KEY_Up:

		    if(board[player->y-1][player->x] == 0){
			    	if(player->y-1 >= 0){
					board[player->y][player->x] = 0;
					board[player->y-1][player->x] = player->id;
				}
		    }else if(board[player->y-1][player->x] == BALL){
				if(player->y-2 >= 0 && (board[player->y-2][player->x] == 0 || board[player->y-2][player->x] == BASE)){
					board[player->y][player->x] = 0;
					board[player->y-1][player->x] = player->id;
					if(board[player->y-2][player->x] == BASE){
						player->point++;
						break;
					}
					board[player->y-2][player->x] = BALL;
				}
		    }else if(board[player->y-1][player->x] == WALL || board[player->y-1][player->x] == BASE){
				//nothing
		    }else{ //Multi_play에서 상대방과 부딪혔을 때
			//nothing
		    }
		    break;
	    case GDK_KEY_Down:

		    if(board[player->y+1][player->x] == 0){
			    	if(player->y+1 < GRID_SIZE){
					board[player->y][player->x] = 0;
					board[player->y+1][player->x] = player->id;
				}
		    }else if(board[player->y+1][player->x] == BALL){
				if(player->y+2 < GRID_SIZE && (board[player->y+2][player->x] == 0 || board[player->y+2][player->x] == BASE)){
					board[player->y][player->x] = 0;
					board[player->y+1][player->x] = player->id;
					if(board[player->y+2][player->x] == BASE){
						player->point++;
						break;
					}
					board[player->y+2][player->x] = BALL;
				}
		    }else if(board[player->y+1][player->x] == WALL || board[player->y+1][player->x] == BASE){
				//nothing
		    }else{ //Multi_play에서 상대방과 부딪혔을 때
			//nothing
		    }
		    break;
    }

    // Request a redraw
    gtk_widget_queue_draw(widget);

    return FALSE;
}

static void activate(GtkApplication *app, gpointer user_data) {
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Push Push!!");
    gtk_window_set_default_size(GTK_WINDOW(window), (GRID_SIZE + 10) * 20, GRID_SIZE * 20);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw_area);
    
    g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press_event), NULL);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(draw_callback), NULL);

    gtk_widget_show_all(window);
}


int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    makeInitialState();
    app = gtk_application_new("org.example.pushpush", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
   
    g_object_unref(app);
    gtk_main();

    free(player);
    return status;
}

