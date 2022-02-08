#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

// Copied from https://stackoverflow.com/questions/8352027/gtk-timer-how-to-make-a-timer-within-a-frame

/* Determines if to continue the timer or not */
static gboolean continue_timer = FALSE;

/* Determines if the timer has started */
static gboolean start_timer = FALSE;

/* Display seconds expired */
static int sec_expired = 0;

static gboolean
_label_update(gpointer data)
{
  GtkLabel *label = (GtkLabel *)data;
  char buf[256];
  memset(&buf, 0x0, 256);
  snprintf(buf, 255, "Time elapsed: %d secs", ++sec_expired);
  gtk_label_set_label(label, buf);
  return continue_timer;
}

static void
_start_timer(GtkWidget *button, gpointer data)
{
  (void)button; /*Avoid compiler warnings*/
  GtkWidget *label = data;
  if (!start_timer)
  {
    g_timeout_add_seconds(1, _label_update, label);
    start_timer = TRUE;
    continue_timer = TRUE;
  }
}

static void
_pause_resume_timer(GtkWidget *button, gpointer data)
{
  (void)button; /*Avoid compiler warnings*/
  if (start_timer)
  {
    GtkWidget *label = data;
    continue_timer = !continue_timer;
    if (continue_timer)
    {
      g_timeout_add_seconds(1, _label_update, label);
    }
    else
    {
      /*Decrementing because timer will be hit one more time before expiring*/
      sec_expired--;
    }
  }
}

static void
_reset_timer(GtkWidget *button, gpointer data)
{
  (void)button;
  (void)data; /*Avoid compiler warnings*/
  /*Setting to -1 instead of 0, because timer will be triggered once more before expiring*/
  sec_expired = -1;
  continue_timer = FALSE;
  start_timer = FALSE;
}

static void activate(GtkApplication *app, gpointer data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *start_button;
  GtkWidget *pause_resume_button;
  GtkWidget *reset_button;
  GtkWidget *quit_button;
  GtkWidget *label;

  window = gtk_application_window_new(app);

  grid = gtk_grid_new();
  gtk_window_set_child(GTK_WINDOW(window), grid);

  label = gtk_label_new("Time elapsed: 0 secs");

  start_button = gtk_button_new_with_label("Start");
  g_signal_connect(G_OBJECT(start_button), "clicked", G_CALLBACK(_start_timer), label);

  pause_resume_button = gtk_button_new_with_label("Pause/Resume");
  g_signal_connect(G_OBJECT(pause_resume_button), "clicked", G_CALLBACK(_pause_resume_timer), label);

  reset_button = gtk_button_new_with_label("Reset");
  g_signal_connect(G_OBJECT(reset_button), "clicked", G_CALLBACK(_reset_timer), label);

  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), start_button, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), pause_resume_button, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), reset_button, 1, 2, 1, 1);
  // gtk_grid_attach(GTK_GRID(grid), quit_button, 1, 3, 1, 1);

  gtk_widget_show(window);

  g_timeout_add_seconds(1, _label_update, label);
  continue_timer = TRUE;
  start_timer = TRUE;
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