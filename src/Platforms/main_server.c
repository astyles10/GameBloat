#include <cJSON.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ws.h>

#include "bios.h"
#include "cpu.h"
#include "interrupt.h"
#include "gpu.h"
#include "logger.h"

void* InitGameboyLoop(void*);
void* InitWebsocketThread(void*);
void RunGameboyLoop(void);
void ResetGame(char *inGameFilename);
void OnOpen(ws_cli_conn_t *client);
void OnClose(ws_cli_conn_t *client);
void OnMessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size, int type);

pthread_cond_t connSignal;
pthread_mutex_t connMutex;
atomic_bool wsConnected;
atomic_bool restartGame;
char* gameFileName;

void OnOpen(ws_cli_conn_t *client) {
  pthread_mutex_lock(&connMutex);
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection opened, addr: %s\n", cli);
  pthread_mutex_unlock(&connMutex);
  atomic_store(&wsConnected, true);
  pthread_cond_signal(&connSignal);
}

void OnClose(ws_cli_conn_t *client) {
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection closed, addr: %s\n", cli);
  atomic_store(&wsConnected, false);
}

void OnMessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size,
               int type) {
  char *cli;
  cli = ws_getaddress(client);
  printf("I received a message: %s (%zu), from: %s\n", msg, size, cli);
  if (!strcmp((const char *)msg, "reset")) {
    atomic_store(&restartGame, true);
  } else if (!strcmp((const char *)msg, "step")) {

  }

  cJSON* test = cJSON_CreateObject();
  if (cJSON_AddStringToObject(test, "name", "name_value") == NULL) {
    printf("Failed to add value to object\n");
    return;
  }
  char* testString = cJSON_Print(test);
  if (testString == NULL) {
    printf("Failed to print string\n");
    return;
  }
  printf("Sending string %s\n", testString);
  ws_sendframe_txt(client, testString);
  cJSON_Delete(test);
}

void ResetGame(char *inGameFilename) {
  if (validateCart(inGameFilename)) {
    loadROM(inGameFilename);
    gpuReset();
    printf("Game restarted!\n");
  }
}

void* InitGameboyLoop(void* args) {
  pthread_mutex_lock(&connMutex);
  pthread_cond_wait(&connSignal, &connMutex);
  pthread_mutex_unlock(&connMutex);
  RunGameboyLoop();
  return NULL;
}

void* InitWebsocketThread(void* args) {
  struct ws_events evs;
  evs.onopen = &OnOpen;
  evs.onclose = &OnClose;
  evs.onmessage = &OnMessage;
  uint16_t* portNumber = (uint16_t*)(args);
  ws_socket(&evs, *portNumber, 0, 1000);
  return NULL;
}

void RunGameboyLoop(void) {
  printf("Start GB Loop!\n");
  while (1) {
    if (atomic_load(&restartGame) == true) {
      ResetGame(gameFileName);
      atomic_store(&restartGame, false);
    }
    if (atomic_load(&wsConnected) == true) {
      const int aCpuTicks = cpuStep();
      gpuStep(aCpuTicks);
      // interruptStep();
    }
  }
  printf("Exiting GB Loop\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("GB file path required!\n");
    exit(1);
  }
  cJSON_Hooks hooks;
  hooks.free_fn = free;
  hooks.malloc_fn = malloc;
  cJSON_InitHooks(&hooks);
  uint16_t portNumber = 9500;
  pthread_t websocketThread, gbThread;
  atomic_init(&wsConnected, false);
  atomic_init(&restartGame, false);
  pthread_cond_init(&connSignal, NULL);
  pthread_mutex_init(&connMutex, NULL);
  pthread_create(&websocketThread, NULL, InitWebsocketThread, (void*)&portNumber);

  gameFileName = (char *)malloc(strlen(argv[1]) + 1);
  strcpy(gameFileName, argv[1]);
  pthread_create(&gbThread, NULL, InitGameboyLoop, (void*)gameFileName);

  InitLogFiles(NULL);
  char *aGameFilename = argv[1];
  ResetGame(aGameFilename);

  pthread_join(websocketThread, NULL);
  pthread_join(gbThread, NULL);

  return (0);
}