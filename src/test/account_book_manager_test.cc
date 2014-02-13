#include "../account_book_manager.h"

int main()
{
    AccountBookManager abm;


    proto::Account account;
    account.set_id(1);
    account.set_name("sina");
    account.set_account("caiyunlong2010@gmail.com");
    account.set_password("07303222_cyl");

    abm.addAccount(&account);   
    abm.dump();

    account.set_id(2);
    account.set_name("baidu");
    abm.addAccount(&account);
    abm.dump();


    std::string name("google");
//    abm.delAccount(name);
    abm.dump();


    return 0;
}
