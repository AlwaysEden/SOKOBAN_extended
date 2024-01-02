#include <gtk/gtk.h>

// 그리기 콜백 함수
gboolean draw_callback(GtkDrawingArea *widget, cairo_t *cr, gpointer data) {
    // 원 그리기
    cairo_arc(cr, 100, 100, 50, 0, 2 * G_PI);  // 중심 (100, 100), 반지름 50인 원
    cairo_set_source_rgb(cr, 0, 0, 1);        // 파란색으로 설정
    cairo_fill(cr);                           // 원을 채우기

    return FALSE;
}

// 메인 함수
int main(int argc, char *argv[]) {
    // GTK 초기화
    gtk_init(&argc, &argv);

    // 윈도우 생성
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "원 그리기");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    // 그리기 위젯 생성
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // 그리기 콜백 함수 연결
   // g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area),draw_callback, NULL, NULL);
    // 윈도우가 닫힐 때 프로그램 종료
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 윈도우 표시
    gtk_widget_show_all(window);

    // GTK 이벤트 루프 시작
    gtk_main();

    return 0;
}

