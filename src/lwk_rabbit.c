#include "lwk_rabbit.h"

int lwk_rabbit_llen(char *server, int port, char *queue, int passive, int durable, int exclusive, int auto_delete)
{

  int rst, status;
  amqp_connection_state_t conn;
  amqp_socket_t *socket = NULL;
  amqp_rpc_reply_t arrt;

  // connect
  conn = amqp_new_connection();

  // socket
  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    LOG_PRINT(LOG_DEBUG, "creating TCP socket failed\n");
  }

  status = amqp_socket_open(socket, server, port);
  if (status != AMQP_STATUS_OK) {
    LOG_PRINT(LOG_DEBUG, "opening TCP socket failed\n");
  }

  // login
  arrt = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
  if (arrt.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
    LOG_PRINT(LOG_DEBUG, "login failed\n");
  }
  amqp_channel_open(conn, 1);
  amqp_get_rpc_reply(conn);

  // 务必保持 durable auto_delete passive C/S两端参数一致
  // amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue), 0, 1, 0, 1, amqp_empty_table);
  amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue), passive, durable, exclusive, auto_delete, amqp_empty_table);

  rst = r->message_count;

  amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
  amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
  amqp_destroy_connection(conn);

  return rst;
}
