#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpu.h"
#include "registers.h"
#include "memory.h"
#include "opcode.h"
#include "cartridge.h"
#include "bios.h"

#include <gtk/gtk.h>

int parseArguments(int, char *[], char *);

static void onStartup(GtkApplication *, gconstpointer);
static void onActivate(GtkApplication *, gconstpointer);
static void onShutdown(GtkApplication *, gconstpointer);
static void onClick(GtkWidget *widget, gpointer data);

static void onClick(GtkWidget *widget, gpointer data)
{
  const char *cartName = "./GB_Games/PokemonRed.gb";
  if (validateCart(cartName))
  {
    loadROM(cartName);

    int i;
    for (i = 0; i < 20; i++)
    {
      printf("%d: ", i);
      cpuCycle();
    }
  }
  cpuClose();
}

static void onActivate(GtkApplication *app, gconstpointer userData)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "GameBloat: Yet Another GameBoy Emulator");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

  gtk_window_set_child(GTK_WINDOW(window), box);

  button = gtk_button_new_with_label("Launch Game");

  g_signal_connect(button, "clicked", G_CALLBACK(onClick), NULL);
  // g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);

  gtk_box_append(GTK_BOX(box), button);
  gtk_widget_show(window);
}

static void onStartup(GtkApplication *app, gconstpointer userData)
{
  const char *cartName = "./GB_Games/PokemonRed.gb";
  if (validateCart(cartName))
  {
    loadROM(cartName);

    int i;
    for (i = 0; i < 20; i++)
    {
      printf("%d: ", i);
      cpuCycle();
    }
  }
}

static void onShutdown(GtkApplication *app, gconstpointer userData)
{
  printf("Shutting down...\n");
  cpuClose();
}

int main(int argc, char *argv[])
{
  GtkApplication *app;
  app = gtk_application_new("gamebloat.app", G_APPLICATION_FLAGS_NONE);
  // Refer to https://wiki.gnome.org/HowDoI/GtkApplication/CommandLine
  // g_application_add_main_option(G_APPLICATION(app), "file", 'f', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, "Gameboy file path", "Description of file path");

  // g_signal_connect(app, "startup", G_CALLBACK(onStartup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(onShutdown), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
