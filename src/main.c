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

int parseArguments(int, char **, char *);

static void onStartup(GtkApplication *, gconstpointer);
static void onActivate(GtkApplication *, gconstpointer);
static void onShutdown(GtkApplication *, gconstpointer);

static void onActivate(GtkApplication *app, gconstpointer userData)
{
  GtkWidget *window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "changeme");
  gtk_window_set_default_size(GTK_WINDOW(window), 160, 144);
  gtk_widget_show(window);
}

static void onStartup(GtkApplication *app, gconstpointer userData)
{
  const char *cartName = (const char *)userData;
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
  cpuClose();
}

int main(int argc, char **argv)
{
  char *cartPath = "";

  if (!parseArguments(argc, argv, cartPath))
  {
    return 1;
  }
  GtkApplication *app;
  app = gtk_application_new("gamebloat.app", G_APPLICATION_FLAGS_NONE);
  // Refer to https://wiki.gnome.org/HowDoI/GtkApplication/CommandLine
  g_application_add_main_option(G_APPLICATION(app), "file", 'f', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_FILENAME, "Gameboy file path", "Description of file path");
  printf("Cart name = %s\n", cartPath);
  g_signal_connect(app, "startup", G_CALLBACK(onStartup), &cartPath);
  g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(onShutdown), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  return status;
}

int parseArguments(int argc, char **argv, char *cartPath)
{
  if (argc < 2)
  {
    printf("Too few arguments in call, exiting...\n");
    return 0;
  }
  else if (argc == 2)
  {
    cartPath = *argv[1];
    return 1;
  }
  // return "./GB_Games/Tetris.gb";
  // return "./GB_Games/PokemonRed.gb";
}
