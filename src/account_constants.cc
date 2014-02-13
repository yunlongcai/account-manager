#include "account_constants.h"


const char *ACCOUNT::GET = "get";
const char *ACCOUNT::ADD = "add";
const char *ACCOUNT::DEL = "del";
const char *ACCOUNT::SAVE = "save";

const char *ACCOUNT::NO_SUCH_ACCOUNT = "no such account";
const char *ACCOUNT::NO_SUCH_CMD = "no such command";
const char *ACCOUNT::DUPLICATED_NAME = "duplicated name";

const char ACCOUNT::OK = 0x01;
const char ACCOUNT::FAIL = 0x02;
const char ACCOUNT::PROTO_ACCOUNT = 0x01;
const char ACCOUNT::PROTO_ACCOUNT_BOOK = 0x02;
