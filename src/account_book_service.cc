#include "account_constants.h"
#include "account_book_service.h"

#include <string.h>
#include <ctype.h>

const char  * const RET_OK = "OK";
const char  * const RET_FAIL = "FAIL";

AccountBookService::AccountBookService()
{
    ctx = zctx_new();
    socket = zsocket_new(ctx, ZMQ_REP);
    zsocket_bind(socket, "tcp://*:5555");
}

AccountBookService::~AccountBookService()
{
    zsocket_destroy(ctx, socket);
    zctx_destroy(&ctx);
}

int handle_func_(zloop_t *, zmq_pollitem_t *, void *);
void AccountBookService::startService()
{
    zloop_t *reactor = zloop_new();
    zmq_pollitem_t poller = {socket, 0, ZMQ_POLLIN};

    printf("start service\n");
    zloop_poller(reactor, &poller, handle_func_, this);
    zloop_start(reactor);
    
    zloop_destroy(&reactor);
}

int handle_func_(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
    AccountBookService *abs = (AccountBookService *) arg;
    abs->handle_func(loop, poller, NULL);
}

int AccountBookService::handle_func(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
    /*
        return message
    */
    zmsg_t *ret_msg = zmsg_new();
    int message_data_length = 0;
    char *buf = NULL;
    /*
    *   message:
    *   cmd
    *   [content]
    */
    zmsg_t *msg = zmsg_recv(socket);
    char *cmd = zmsg_popstr(msg);
    
    //
    printf("%s\n", cmd);

    if(strcasecmp(cmd, ACCOUNT::GET) == 0)
    {
        char *name = zmsg_popstr(msg);
        if(name == NULL)
        {
            //if no name specified, show all
            proto::AccountBook *account_book = account_book_manager.getAllAccount();
            zmsg_addmem(ret_msg, &ACCOUNT::OK, 1);
            zmsg_addmem(ret_msg, ACCOUNT::GET, strlen(ACCOUNT::GET));
            zmsg_addmem(ret_msg, &ACCOUNT::PROTO_ACCOUNT_BOOK, 1);
            message_data_length = account_book->ByteSize();
            buf = new char[message_data_length];
            account_book->SerializeToArray(buf, message_data_length);
            zmsg_addmem(ret_msg, buf, message_data_length);
        }
        else
        {
            //
            printf("name [%s]\n", name);
            proto::Account *account = account_book_manager.getAccount(name);
            if(account == NULL)
            {
                zmsg_addmem(ret_msg, &ACCOUNT::FAIL, 1);
                zmsg_addstrf(ret_msg, "%s, [%s]", ACCOUNT::NO_SUCH_ACCOUNT, name);
            }
            else
            {
                zmsg_addmem(ret_msg, &ACCOUNT::OK, 1);
                zmsg_addmem(ret_msg, ACCOUNT::GET, strlen(ACCOUNT::GET));
                zmsg_addmem(ret_msg, &ACCOUNT::PROTO_ACCOUNT, 1);
                message_data_length = account->ByteSize();
                buf = new char[message_data_length];
                account->SerializeToArray(buf, message_data_length);
                zmsg_addmem(ret_msg, buf, message_data_length);
            }
        }
        if(buf != NULL)
        {
            delete [] buf;
            buf = NULL;
        }
    }
    else if(strcasecmp(cmd, ACCOUNT::ADD) == 0)
    {
        //get account data
        char *account_name = zmsg_popstr(msg);
        char *account_account = zmsg_popstr(msg);
        char *account_password = zmsg_popstr(msg);
        //generate account id random?
        int account_id = 0;

        proto::Account *new_account = new proto::Account();
        new_account->set_id(account_id);
        new_account->set_name(account_name);
        new_account->set_account(account_account);
        new_account->set_password(account_password);

        int ret = account_book_manager.addAccount(new_account);
        if(ret == 0)
        {
            zmsg_addmem(ret_msg, &ACCOUNT::OK, 1);
            zmsg_addstr(ret_msg, ACCOUNT::ADD);
            zmsg_addstrf(ret_msg, "[%d] %s %s %s", account_id, account_name, account_account, account_password);
        }
        else
        {
            zmsg_addmem(ret_msg, &ACCOUNT::FAIL, 1);
            zmsg_addstrf(ret_msg, "%s, [%s]", ACCOUNT::DUPLICATED_NAME, account_name);
        }
    }
    else if(strcasecmp(cmd, ACCOUNT::DEL) == 0)
    {
        char *account_name = zmsg_popstr(msg);

        int ret = account_book_manager.delAccount(account_name);
        if(ret == 0)
        {
            zmsg_addmem(ret_msg, &ACCOUNT::OK, 1);
            zmsg_addstr(ret_msg, ACCOUNT::DEL);
            zmsg_addstrf(ret_msg, "Delete [%s] successfully", account_name);
        }
        else
        {
            zmsg_addmem(ret_msg, &ACCOUNT::FAIL, 1);
            zmsg_addstrf(ret_msg, "%s, [%s]", ACCOUNT::NO_SUCH_ACCOUNT, account_name); 
        }
    }
    else if(strcasecmp(cmd, ACCOUNT::SAVE) == 0)
    {
        account_book_manager.save();
        zmsg_addmem(ret_msg, &ACCOUNT::OK, 1);
        zmsg_addmem(ret_msg, ACCOUNT::SAVE, strlen(ACCOUNT::SAVE));
    }
    else
    {
        zmsg_addmem(ret_msg, &ACCOUNT::FAIL, 1);
        zmsg_addstrf(ret_msg, "%s, [%s]", ACCOUNT::NO_SUCH_CMD, cmd);
    }

    zmsg_destroy(&msg);
    zmsg_send(&ret_msg, socket);

    account_book_manager.save();
}

