#ifndef ACCOUNT_BOOK_SERVICE_H
#define ACCOUNT_BOOK_SERVICE_H

/*
    this is a class as a reactor
*/


#include "account_book_manager.h"
#include "czmq.h"

class AccountBookService
{
public:
    AccountBookService();
    ~AccountBookService();

    void startService();

    int handle_func(zloop_t *loop, zmq_pollitem_t *poller, void *arg);

private:
    void addAccount(int id, const std::string &name, const std::string &account, const std::string &password);
    void delAccount(const std::string &name);
    void getAccount(const std::string &name);


private:
    AccountBookManager account_book_manager;

    //czmq
    zctx_t *ctx;
    void *socket;
};

#endif
