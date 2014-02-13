#include "account.pb.h"

#include <cstdio>
#include <string.h>
void test();
void test_proto_repeat_field();

int main(void)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
//    test();
    test_proto_repeat_field();
    return 0;
}

void test()
{
    //init
    printf("==TEST==\n");
    proto::Account account;
    account.set_id(0);
    account.set_name("google");
    account.set_account("caiyunlong");
    account.set_password("1234");
  
    //serialization
    int length = account.ByteSize();
    char *buf = new char[length];
    account.SerializeToArray(buf, length);

    //deserialization
    proto::Account account_copy;
    account_copy.ParseFromArray(buf, length);
    printf("id=%d, name=%s, account=%s, password=%s\n", account_copy.id(), account_copy.name().c_str(), account_copy.account().c_str(), account_copy.password().c_str());

}


void test_proto_repeat_field()
{
    printf("TEST REPEAT\n");
    
    proto::AccountBook account_book;

    //add an account
    proto::Account *account = account_book.add_account();
    account->set_id(1);
    account->set_name("google");
    account->set_account("caiyunlong");
    account->set_password("12345");

    //serialize
    int length = account_book.ByteSize();
    char *buf = new char[length];
    account_book.SerializeToArray(buf, length);

    //
    proto::AccountBook account_book_1;
    account_book_1.ParseFromArray(buf, length);
    for(int i = 0; i < account_book_1.account_size(); i++)
    {
        const proto::Account& account = account_book_1.account(i);
        printf("id=%d, name=%s, account=%s, password=%s\n", account.id(), account.name().c_str(), account.account().c_str(), account.password().c_str());

        
    }
}
