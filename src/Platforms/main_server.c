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
#include "gpu.h"
#include "interrupt.h"
#include "logger.h"

void InitGlobals(char **argv);
void InitThreads(void);
void *InitGameboyLoop(void *);
void *InitWebsocketThread(void *);
void OnOpen(ws_cli_conn_t *client);
void OnClose(ws_cli_conn_t *client);
void OnMessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size,
               int type);
void ResetGame(char *inGameFilename);
void RunGameboyLoop(void);
void LoopCycle(void);
void SendWebsocketData(void);
int msleep(long tms);

pthread_t websocketThread, gbThread;
pthread_cond_t connSignal;
pthread_mutex_t connMutex;
char *gameFileName;
uint16_t portNumber;

atomic_bool wsConnected;
atomic_bool restartGame;
atomic_bool startGame;
atomic_bool step;

ws_cli_conn_t *wsClient;

int msleep(long tms) {
  struct timespec ts;
  int ret;

  if (tms < 0) {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = tms / 1000;
  ts.tv_nsec = (tms % 1000) * 1000000;

  do {
    ret = nanosleep(&ts, &ts);
  } while (ret && errno == EINTR);

  return ret;
}

void InitGlobals(char **argv) {
  portNumber = 9500;
  pthread_cond_init(&connSignal, NULL);
  pthread_mutex_init(&connMutex, NULL);
  atomic_init(&wsConnected, false);
  atomic_init(&restartGame, false);
  atomic_init(&startGame, false);
  atomic_init(&step, false);
  gameFileName = (char *)malloc(strlen(argv[1]) + 1);
  strcpy(gameFileName, argv[1]);
}

void InitThreads() {
  pthread_create(&websocketThread, NULL, InitWebsocketThread,
                 (void *)&portNumber);
  pthread_create(&gbThread, NULL, InitGameboyLoop, (void *)gameFileName);
}

void *InitGameboyLoop(void *args) {
  pthread_mutex_lock(&connMutex);
  pthread_cond_wait(&connSignal, &connMutex);
  pthread_mutex_unlock(&connMutex);
  RunGameboyLoop();
  return NULL;
}

void *InitWebsocketThread(void *args) {
  struct ws_events evs;
  evs.onopen = &OnOpen;
  evs.onclose = &OnClose;
  evs.onmessage = &OnMessage;
  // uint16_t *portNumber = (uint16_t *)(args);
  ws_socket(&evs, portNumber, 0, 1000);
  return NULL;
}

void OnOpen(ws_cli_conn_t *client) {
  pthread_mutex_lock(&connMutex);
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection opened, addr: %s\n", cli);
  pthread_mutex_unlock(&connMutex);
  atomic_store(&wsConnected, true);
  wsClient = client;
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
    atomic_store(&step, true);
  } else if (!strcmp((const char *)msg, "start")) {
    atomic_store(&startGame, true);
  } else if (!strcmp((const char *)msg, "stop")) {
    atomic_store(&startGame, false);
  }
}

void SendWebsocketData(void) {
  cJSON *cpuJson = GetCPUDataAsJSON();
  char *jsonString = cJSON_Print(cpuJson);
  if (jsonString == NULL) {
    printf("Failed to print string\n");
    return;
  }
  ws_sendframe_txt(wsClient, jsonString);
  cJSON_Delete(cpuJson);
}

void ResetGame(char *inGameFilename) {
  if (validateCart(inGameFilename)) {
    loadROM(inGameFilename);
    gpuReset();
    printf("Game restarted!\n");
  }
}

void RunGameboyLoop(void) {
  // TODO: Experiment with game loop mutex
  printf("Start GB Loop!\n");
  while (1) {
    if (atomic_load(&restartGame) == true) {
      ResetGame(gameFileName);
      SendWebsocketData();
      atomic_store(&restartGame, false);
      atomic_store(&startGame, false);
    }
    if (atomic_load(&wsConnected) == true && atomic_load(&step) == true) {
      LoopCycle();
      atomic_store(&step, false);
    } else if (atomic_load(&wsConnected) == true &&
               atomic_load(&startGame) == true) {
      // TODO: Figure out throttling technique, websocket messages swamp the
      // client causing freezes
      LoopCycle();
      msleep(1);
    }
  }
  printf("Exiting GB Loop\n");
}

void LoopCycle(void) {
  const int aCpuTicks = cpuStep();
  gpuStep(aCpuTicks);
  interruptStep();
  SendWebsocketData();
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("GB file path required!\n");
    exit(1);
  }

  InitGlobals(argv);
  InitThreads();
  InitLogFiles(NULL);
  ResetGame(gameFileName);

  pthread_join(websocketThread, NULL);
  pthread_join(gbThread, NULL);

  return (0);
}