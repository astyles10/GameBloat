#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bios.h"
#include "cartridge.h"
#include "cpu.h"
#include "gpu.h"
#include "interrupt.h"
#include "memory.h"
#include "opcode.h"
#include "registers.h"

int parseArguments(int, char *[], char *);

static void onActivate(GtkApplication *, gconstpointer);
static void onShutdown(GtkApplication *, gconstpointer);
static void launchGameCallback(GtkWidget *widget, gpointer data);
static void runStepCallback(GtkWidget *widget, gpointer data);
static void updateRegisterText();

GtkWidget *statusbar;
static bool gameLaunched = false;

static void launchGameCallback(GtkWidget *widget, gpointer data) {
  const char *cartName = "./GB_Games/PokemonRed.gb";
  if (validateCart(cartName)) {
    loadROM(cartName);
    gpuReset();
    gameLaunched = true;
  }
}

static void runStepCallback(GtkWidget *widget, gpointer data) {
  if (gameLaunched) {
    const int ticks = cpuStep();
    gpuStep(ticks);
    interruptStep();

    updateRegisterText();
  }
}

static void updateRegisterText() {
  if (!gameLaunched) {
    return;
  }
  gchar *msg;

  msg = g_strdup_printf(
      "Register AF: 0x%02X\n\
  Register BC: 0x%02X\n\
  Register DE: 0x%02X\n\
  Register HL: 0x%02X\n\
  Register SP: 0x%02X\n\
  Register PC: 0x%02X",
      registers.AF, registers.BC, registers.DE, registers.HL, registers.SP,
      registers.PC);

  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, msg);

  g_free(msg);
}

static void onActivate(GtkApplication *app, gconstpointer userData) {
  GtkWidget *window;
  GtkWidget *launchButton;
  GtkWidget *runStepButton;
  GtkWidget *box;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),
                       "GameBloat: Yet Another GameBoy Emulator");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

  gtk_window_set_child(GTK_WINDOW(window), box);

  statusbar = gtk_statusbar_new();
  // g_signal_connect(statusbar, "changed", G_CALLBACK(updateRegisterText),
  // statusbar);

  launchButton = gtk_button_new_with_label("Launch Game");
  g_signal_connect(launchButton, "clicked", G_CALLBACK(launchGameCallback),
                   NULL);

  runStepButton = gtk_button_new_with_label("Run Step");
  g_signal_connect(runStepButton, "clicked", G_CALLBACK(runStepCallback), NULL);

  gtk_box_append(GTK_BOX(box), launchButton);
  gtk_box_append(GTK_BOX(box), runStepButton);
  gtk_box_append(GTK_BOX(box), statusbar);
  gtk_widget_show(window);
}

static void onShutdown(GtkApplication *app, gconstpointer userData) {
  printf("Shutting down...\n");
  cpuClose();
}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  app = gtk_application_new("gamebloat.app", G_APPLICATION_FLAGS_NONE);
  // Refer to https://wiki.gnome.org/HowDoI/GtkApplication/CommandLine
  // g_application_add_main_option(G_APPLICATION(app), "file", 'f',
  // G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, "Gameboy file path",
  // "Description of file path");

  // g_signal_connect(app, "startup", G_CALLBACK(onStartup), NULL);
  g_signal_connect(app, "activate", G_CALLBACK(onActivate), NULL);
  g_signal_connect(app, "shutdown", G_CALLBACK(onShutdown), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
