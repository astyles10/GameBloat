#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ws.h>

int millisleep(unsigned int microseconds) {
  const struct timespec ts = {
      microseconds / 1000,                /* seconds */
      (microseconds % 1000) * 1000 * 1000 /* nano seconds */
  };
  return nanosleep(&ts, NULL);
}

/**
 * @brief This function is called whenever a new connection is opened.
 * @param client Client connection.
 */
void onopen(ws_cli_conn_t *client) {
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection opened, addr: %s\n", cli);
}

/**
 * @brief This function is called whenever a connection is closed.
 * @param client Client connection.
 */
void onclose(ws_cli_conn_t *client) {
  char *cli;
  cli = ws_getaddress(client);
  printf("Connection closed, addr: %s\n", cli);
}

/**
 * @brief Message events goes here.
 * @param client Client connection.
 * @param msg    Message content.
 * @param size   Message size.
 * @param type   Message type.
 */
void onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size,
               int type) {
  char *cli;
  cli = ws_getaddress(client);
  printf("I received a message: %s (%zu), from: %s\n", msg, size, cli);
  // static int msg_count = 1;
  char msg_buffer[100];

  // snprintf(msg_buffer, 100, "%d", msg_count++);
  for (int i = 0; i < 100; ++i) {
    snprintf(msg_buffer, 100, "%d", i);
    millisleep(17);
    ws_sendframe_txt(client, msg_buffer);
  }
}

int main(int argc, char **argv) {
  uint16_t port_number = 8087;
  if (argc > 1) {
    port_number = strtoul(argv[1], NULL, 10);
  }

  /* Register events. */
  struct ws_events evs;
  evs.onopen = &onopen;
  evs.onclose = &onclose;
  evs.onmessage = &onmessage;

  /*
   * Main loop, this function never* returns.
   *
   * *If the third argument is != 0, a new thread is created
   * to handle new connections.
   */
  ws_socket(&evs, port_number, 0, 1000);

  return (0);
}