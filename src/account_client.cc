#include <string.h>
#include "account.pb.h"
#include "account_client.h"
#include "account_constants.h"

AccountClient::AccountClient()
{  
}

AccountClient::~AccountClient()
{
}

void AccountClient::connect(const char *hostname, const long port)
{
    char endpoint[120];
    sprintf(endpoint, "tcp://%s:%ld", hostname, port);
    
    ctx = zctx_new();
    socket = zsocket_new(ctx, ZMQ_REQ);
    zsocket_set_rcvtimeo(socket, 5000); // recv time out for 5 seconds
    zsocket_connect(socket, endpoint);
}

//
void AccountClient::getAccount(const char *name)
{
    zmsg_t *req_msg = zmsg_new();
    zmsg_addmem(req_msg, ACCOUNT::GET, strlen(ACCOUNT::GET));
    if(name != NULL && strlen(name) != 0)
    {
        zmsg_addmem(req_msg, name, strlen(name));
    }
    zmsg_send(&req_msg, socket);
}

void AccountClient::delAccount(const char *name)
{
    zmsg_t *req_msg = zmsg_new();
    zmsg_addmem(req_msg, ACCOUNT::DEL, strlen(ACCOUNT::DEL));
    zmsg_addmem(req_msg, name, strlen(name));

    zmsg_send(&req_msg, socket);
}

void AccountClient::addAccount(const char *name, const char *account, const char *password)
{
    zmsg_t *req_msg = zmsg_new();
    zmsg_addmem(req_msg, ACCOUNT::ADD, strlen(ACCOUNT::ADD));
    zmsg_addmem(req_msg, name, strlen(name));
    zmsg_addmem(req_msg, account, strlen(account));
    zmsg_addmem(req_msg, account, strlen(account));

    zmsg_send(&req_msg, socket);
}


void AccountClient::getReply()
{
    zmsg_t *rep_msg = zmsg_recv(socket);
    if(rep_msg == NULL)
    {
        if(errno == EAGAIN)
        {
            printf("recv time out");
            return;
        }
    }
    
    char *status = zmsg_popstr(rep_msg);
    if(status[0] == ACCOUNT::OK)
    {   
        char *cmd = zmsg_popstr(rep_msg);
        //get
        if(strcasecmp(cmd, ACCOUNT::GET) == 0)
        {
            char *type = zmsg_popstr(rep_msg);
            if(type[0] == 0x02)
            {
                proto::AccountBook *account_book = new proto::AccountBook();
                zframe_t *data = zmsg_pop(rep_msg);
                char *data_byte = (char *) zframe_data(data);
                int length = zframe_size(data);
                account_book->ParseFromArray(data_byte, length);

                for(int i = 0; i < account_book->account_size(); i++)
                {
                    const proto::Account &account = account_book->account(i);
                    printf("[%d] %s %s %s\n", i + 1, account.name().c_str(), account.account().c_str(), account.password().c_str());
                }

                zframe_destroy(&data);
                //delete [] data_byte;
                delete account_book;
            }
            else
            {
                proto::Account *account = new proto::Account();
                zframe_t *data = zmsg_pop(rep_msg);
                char *data_byte = (char *) zframe_data(data);
                int length = zframe_size(data);
                account->ParseFromArray(data_byte, length);

                printf("[%d] %s %s %s\n", account->id(), account->name().c_str(), account->account().c_str(), account->password().c_str());

                zframe_destroy(&data);
                //delete [] data_byte;
                delete account;
            }   

        }
        else if(strcasecmp(cmd, ACCOUNT::DEL) == 0)
        {
            char *ret_msg = zmsg_popstr(rep_msg);
            printf("%s\n", ret_msg);
            delete [] ret_msg;
        }
        else if(strcasecmp(cmd, ACCOUNT::ADD) == 0)
        {
            char *ret_msg = zmsg_popstr(rep_msg);
            printf("%s\n", ret_msg);
            delete [] ret_msg;
        }
        else if(strcasecmp(cmd, ACCOUNT::SAVE) == 0)
        {

        }
        else
        {
            printf("unknown command [%s]\n", cmd);
        }

        delete [] cmd;
    }
    else
    {
        char *err_msg = zmsg_popstr(rep_msg);
        printf("%s\n", err_msg);
        delete [] err_msg;
    }
    delete [] status;
    
    zmsg_destroy(&rep_msg);
}
