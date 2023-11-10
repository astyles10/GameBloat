#include <gtk/gtk.h>

#include <gtk/gtk.h>

struct aStructure {
  const char* name;
  uint64_t value;
};

static void activate (GtkApplication* app, gconstpointer user_data)
{
  GtkWidget *window;

  struct aStructure* myStruct = (struct aStructure*)(user_data);

  if (user_data)
  {
    printf("User data: %s %d\n", myStruct->name, myStruct->value);
  }

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show (window);
}

static void startup (GtkApplication* app, gconstpointer user_data)
{
  struct aStructure* myStruct = (struct aStructure*)(user_data);

  if (user_data)
  {
    printf("Startup: User data: %s %d\n", myStruct->name, myStruct->value);
  }
}

static void shutdown (GtkApplication* app, gconstpointer user_data)
{
  printf("Shutting down !\n");
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  struct aStructure myStruct;
  const char* myString = "My data struct";
  myStruct.name = myString;
  myStruct.value = 13;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "startup", G_CALLBACK (startup), &myStruct);
  g_signal_connect(app, "shutdown", G_CALLBACK(shutdown), NULL);
  g_signal_connect(app, "activate", G_CALLBACK (activate), &myStruct);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}
