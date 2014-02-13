#ifndef ACCOUNT_CONSTANTS_H
#define ACCOUNT_CONSTANTS_H

class ACCOUNT
{
public:
    /*cmd type*/
    static const char *GET;
    static const char *ADD;
    static const char *DEL;
    static const char *SAVE;

    /*return code*/
    static const char OK;
    static const char FAIL;

    /*return content type*/
    static const char PROTO_ACCOUNT;
    static const char PROTO_ACCOUNT_BOOK;

    /*error message*/
    static const char *NO_SUCH_ACCOUNT;
    static const char *NO_SUCH_CMD;
    static const char *DUPLICATED_NAME;
};


#endif
