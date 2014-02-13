#include "czmq.h"

#include <cstdio>
#include <string.h>

int count = 0;

int handle_func(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
    void *socket = arg;
    char *msg = zstr_recv(socket);

    if(msg)
    {
        printf("[%d] %s\n", count, msg);
        zstr_send(socket, "WORLD");
    }

    return 0;
}   

int main()
{
    zctx_t *ctx = zctx_new();
    void *socket = zsocket_new(ctx, ZMQ_REP);
    zsocket_bind(socket, "tcp://*:5555");

    zloop_t *reactor = zloop_new();
    zmq_pollitem_t poller = {socket, 0, ZMQ_POLLIN};

    zloop_poller(reactor, &poller, handle_func, socket);
    zloop_start(reactor);
    zloop_destroy(&reactor);

    zsocket_destroy(ctx, socket);
    zctx_destroy(&ctx);

    printf("END");
    return 0;
}
