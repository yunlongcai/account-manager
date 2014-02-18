#include "account_book_manager.h"
#include <string>

const std::string AccountBookManager::ACCOUNT_FILE("data/account.dat");


AccountBookManager::AccountBookManager()
{
    account_book = new proto::AccountBook();
    loadAccountBook(ACCOUNT_FILE);
}

AccountBookManager::~AccountBookManager()
{
    saveAccountBook(ACCOUNT_FILE);
    delete account_book;
}

int AccountBookManager::addAccount(const proto::Account *account)
{
    std::string name = account->name();
    for(int i = 0; i < account_book->account_size(); i++)
    {
        const proto::Account &account_ = account_book->account(i);
        if(strcmp(name.c_str(), account_.name().c_str()) == 0)
        {
            return 1;
        }
    }

    proto::Account *account_ = account_book->add_account();
    account_->CopyFrom(*account);
    return 0;
}

int AccountBookManager::delAccount(const std::string &name)
{
    int ret = 1;

    proto::AccountBook *account_book_tmp = new proto::AccountBook();
    for(int i = 0; i < account_book->account_size(); i++)
    {
        const proto::Account &account = account_book->account(i);
        if(strcmp(name.c_str(), account.name().c_str()) == 0)
        {
            ret = 0; 
        }
        else
        {
            proto::Account *account_add = account_book_tmp->add_account();
            account_add->CopyFrom(account);
        }
    }
    //swap new and old
    account_book->Swap(account_book_tmp);
    //delete the tmp
    delete account_book_tmp;
    
    return ret;
}

void AccountBookManager::dump()
{
    for(int i = 0; i < account_book->account_size(); i++)
    {
        const proto::Account &account = account_book->account(i);
        printf("[%d] %s %s %s\n", i + 1, account.name().c_str(), account.account().c_str(), account.password().c_str());
    }
}

void AccountBookManager::loadAccountBook(const std::string &filename)
{
    std::fstream input(filename.c_str(), std::ios::in | std::ios::binary);
    account_book->ParseFromIstream(&input);
    input.close();
}

void AccountBookManager::saveAccountBook(const std::string &filename)
{
    std::fstream output(filename.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    account_book->SerializeToOstream(&output);
    output.close();
}

proto::Account *AccountBookManager::getAccount(const std::string &name)
{
    proto::Account *account_ = NULL;
    for(int i = 0; i < account_book->account_size(); i++)
    {
        const proto::Account &account = account_book->account(i);
        if(!strcmp(name.c_str(), account.name().c_str()))
        {
            account_ = new proto::Account();
            account_->CopyFrom(account);
            break;
        }
    }
    return account_;
}

proto::AccountBook *AccountBookManager::getAllAccount()
{
    proto::AccountBook *account_book_ = new proto::AccountBook();
    account_book_->CopyFrom(*account_book);
    return account_book_;
}

int AccountBookManager::save()
{
    saveAccountBook(ACCOUNT_FILE);
    return 0;
}
