#ifndef ACCOUNT_CONSTANTS_H
#define ACCOUNT_CONSTANTS_H

class ACCOUNT
{
public:
    /*cmd type*/
    static const char *GET;
    static const char *DEL;
    static const char *SAVE;

    /*return code*/
    static const char OK = 0x01;
    static const char FAIL = 0x02;

    /*return content type*/
    static const char PROTO_ACCOUNT = 0x01;
    static const char PROTO_ACCOUNT_BOOK = 0x02;

    /*error message*/
    static const char *NO_SUCH_ACCOUNT;
};

const char *ACCOUNT::GET = "get";
const char *ACCOUNT::DEL = "del";
const char *ACCOUNT::SAVE = "save";

const char *ACCOUNT::NO_SUCH_ACCOUNT = "no such account";

#endif
