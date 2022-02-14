#include <gtk/gtk.h>

// Surface to draw scribbles

struct rgba {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
};

struct rgba lcdScreen[160*144];

static cairo_surface_t *surface = NULL;

static void clear_surface(void)
{
  cairo_t *cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  cairo_destroy(cr);
}

// Creates new surface of an appropriate size
static void resize_cb(GtkWidget *widget, int width, int height, gpointer data)
{
  if (surface)
  {
    cairo_surface_destroy(surface);
    surface = NULL;
  }
  if (gtk_native_get_surface(gtk_widget_get_native(widget)))
  {
    surface = gdk_surface_create_similar_surface(gtk_native_get_surface(gtk_widget_get_native(widget)),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_width(widget),
                                                 gtk_widget_get_height(widget));
    clear_surface();
  }
}

// Redraw screen from surface
static void draw_cb(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data)
{
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);
}

static double red = 0.0;
static double green = 0.0;
static double blue = 0.0;

static void draw_brush(GtkWidget *widget, double x, double y)
{
  cairo_t *cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, red, green, blue);
  cairo_rectangle(cr, x - 0.5, y - 0.5, 1, 1);
  cairo_fill(cr);

  cairo_destroy(cr);

  gtk_widget_queue_draw(widget);
}

static double start_x = 0;
static double start_y = 0;

static void draw_pixel(GtkGestureClick *gesture, int n_press, double x, double y, GtkWidget *area)
{
  double i = 0;
  for (i = 0; i < 160; i += 1)
  {
    double j;
    for (j = 0; j < 144; j += 1)
    {
      draw_brush(area, i, j);
    }
  }
  gtk_widget_queue_draw(area);
}

static void clear(GtkGestureClick *gesture, int n_press, double x, double y, GtkWidget *area)
{
  clear_surface();
  start_x = 0, start_y = 0;
  gtk_widget_queue_draw(area);
}

static void close_window(void)
{
  if (surface)
    cairo_surface_destroy(surface);
}

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window, *frame, *drawing_area, *drag, *lpress, *rpress;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");

  g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

  frame = gtk_frame_new(NULL);
  gtk_window_set_child(GTK_WINDOW(window), frame);

  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 160, 144);

  gtk_frame_set_child(GTK_FRAME(frame), drawing_area);

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_cb, NULL, NULL);
  g_signal_connect_after(drawing_area, "resize", G_CALLBACK(resize_cb), NULL);

  lpress = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(lpress), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(lpress));

  g_signal_connect(lpress, "pressed", G_CALLBACK(draw_pixel), drawing_area);

  rpress = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(rpress), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(rpress));

  g_signal_connect(rpress, "pressed", G_CALLBACK(clear), drawing_area);

  gtk_widget_show(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status = 0;
  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
