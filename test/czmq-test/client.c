#include "czmq.h"

#include <cstdio>

int main()
{
    zctx_t *ctx = zctx_new();
    void *client = zsocket_new(ctx, ZMQ_REQ);
    zsocket_connect(client, "tcp://localhost:5555");
    zstr_send(client, "HELLO");
    char *reply = zstr_recv(client);

    if(reply)
    {
        printf("Client: %s\n", reply);
        free(reply);
    }

    zsocket_destroy(ctx, client);
    zctx_destroy(&ctx);
    return 0;
}
