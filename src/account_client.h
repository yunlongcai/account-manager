#ifndef ACCOUNT_CLIENT_H
#define ACCOUNT_CLIENT_H

#include "czmq.h"

class AccountClient
{
public:
    AccountClient();
    ~AccountClient();
    
    void connect(const char *hostname, const long port);

    //operation
    void getAccount(const char *name);
    void delAccount(const char *name);
    void addAccount(const char *name, const char *account, const char *password);

    void getReply();

private:
    zctx_t *ctx;
    void *socket;


};

#endif
