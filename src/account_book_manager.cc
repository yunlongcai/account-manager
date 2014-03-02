#include "account_book_manager.h"
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

const std::string AccountBookManager::ACCOUNT_FILE("/tmp/account-book-data/account.dat");


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
    checkFile(filename);
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

void AccountBookManager::checkFile(const std::string &filename)
{
    //if file exists, do nothing
    if(access(filename.c_str(), F_OK | R_OK | W_OK) == 0)
        return;
    
    //get file path
    std::string::size_type pos = filename.rfind("/");
    std::string file_path = filename.substr(0, pos);
    
    std::string cur_path;
    std::string::size_type start_pos = 0;
    pos = file_path.find("/", start_pos);
    while(pos != std::string::npos)
    {
        cur_path = file_path.substr(0, pos + 1);
        std::cout << cur_path << std::endl;
        if(access(cur_path.c_str(), F_OK) != 0)
        {
            mkdir(cur_path.c_str(), S_IRWXU);
        }
        start_pos = pos + 1;
        pos = file_path.find("/", start_pos);
    }
    mkdir(file_path.c_str(), S_IRWXU);

    //creat file
    FILE *file = fopen(filename.c_str(), "w");
    if(file != NULL)
    {
        fclose(file);
        file = NULL;
    }
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
