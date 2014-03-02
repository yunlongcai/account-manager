#include "account_book_service.h"
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>

static void daemonize()
{
    pid_t pid;
    struct sigaction sa;

    umask(0);

    if((pid = fork()) != 0)
    {
        exit(0);
    }
    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);

    if((pid = fork()) != 0)
    {
        exit(0);
    }
   
}

int main()
{
    daemonize();

    AccountBookService account_book_service;
    account_book_service.startService();

    return 0;
}
