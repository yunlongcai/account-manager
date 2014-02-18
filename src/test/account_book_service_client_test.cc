#include "czmq.h"
#include <string.h>

#include "../account.pb.h"
#include "../account_constants.h"

int main(int argc, char **argv)
{
    zctx_t *ctx = zctx_new();
    void *socket = zsocket_new(ctx, ZMQ_REQ);
    zsocket_connect(socket, "tcp://localhost:5555");

    zmsg_t *req_msg = zmsg_new();
    char *name = NULL;
    if(argc == 2)
    {
        name = argv[1];
    }
    zframe_t *cmd_frame = zframe_new(ACCOUNT::GET, strlen(ACCOUNT::GET));
    zmsg_append(req_msg, &cmd_frame);
    if(argc == 2)
    {
        zframe_t *name_frame = zframe_new(name, strlen(name));
        zmsg_append(req_msg, &name_frame);
    }
    zmsg_send(&req_msg, socket);

    //recv
    zmsg_t *res = zmsg_recv(socket);
    
    char *status = zmsg_popstr(res);
    printf("status [%x]\n", status[0]);
    
    if(status[0] == ACCOUNT::OK)
    {
        char *cmd = zmsg_popstr(res);
        char *type = zmsg_popstr(res);
    
        if(type[0] == 0x02)
        {
            proto::AccountBook *account_book = new proto::AccountBook();
            zframe_t *data = zmsg_pop(res);
            char *data_byte = (char *) zframe_data(data);
            int length = zframe_size(data);
            account_book->ParseFromArray(data_byte, length);

            for(int i = 0; i < account_book->account_size(); i++)
            {
                const proto::Account &account = account_book->account(i);
                printf("[%d] %s %s %s\n", i + 1, account.name().c_str(), account.account().c_str(), account.password().c_str());
            }
        }
        else
        {
            proto::Account *account = new proto::Account();
            zframe_t *data = zmsg_pop(res);
            char *data_byte = (char *) zframe_data(data);
            int length = zframe_size(data);
            account->ParseFromArray(data_byte, length);

            printf("[%d] %s %s %s\n", account->id(), account->name().c_str(), account->account().c_str(), account->password().c_str());

        }   
    }
    else
    {
        char *error_msg = zmsg_popstr(res);
        printf("ERR: %s\n", error_msg);
    }

    zmsg_destroy(&res);
    //

    zsocket_destroy(ctx, socket);
    zctx_destroy(&ctx);
    
    return 0;
}
