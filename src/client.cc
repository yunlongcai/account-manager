#include <string.h>
#include "account_client.h"
#include "account_constants.h"


int main(int argc, char **argv)
{
    if(argc <= 1 || argc > 5)
    {
        printf("usage: client cmd [option argument]\n");
        return 0;
    }

    AccountClient client;
    client.connect("localhost", 5555);

    char *cmd = argv[1];
    char *name = NULL;
    char *account = NULL;
    char *password = NULL;

    if(strcasecmp(cmd, ACCOUNT::GET) == 0)
    {
        if(argc >= 3)
        {
            name = argv[2];
        }
        client.getAccount(name);
    }
    else if(strcasecmp(cmd, ACCOUNT::ADD) == 0)
    {
        name = argv[2];
        account = argv[3];
        password = argv[4];

        client.addAccount(name, account, password);
    }
    else if(strcasecmp(cmd, ACCOUNT::DEL) == 0)
    {
        name = argv[2];
        client.delAccount(name);
    }
    else if(strcasecmp(cmd, ACCOUNT::SAVE) == 0)
    {
    }
    else
    {
        printf("unknown command [%s]\n", cmd);
        return 0;
    }

    client.getReply();

    return 0;
}
