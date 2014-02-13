#include "czmq.h"

#include <cstdio>

int main()
{
    zctx_t *ctx = zctx_new();
    void *server = zsocket_new(ctx, ZMQ_REP);
    zsocket_bind(server, "tcp://*:5555");

    char *msg = zstr_recv(server);
    printf("%s\n", msg);

    free(msg);
   
    zstr_send(server, "WORLD");

    zsocket_destroy(ctx, server);
    zctx_destroy(&ctx);

    return 0;
}
