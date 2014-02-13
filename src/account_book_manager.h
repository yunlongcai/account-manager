#ifndef ACCOUNT_BOOK_MANAGER_H
#define ACCOUNT_BOOK_MANAGER_H

#include "protocol_buffers_struct/account.pb.h"

#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>


class AccountBookManager
{
public:
    AccountBookManager();
    ~AccountBookManager();

    int addAccount(const proto::Account *account);
    int delAccount(const std::string &name);

    proto::Account *getAccount(const std::string &name);
    proto::AccountBook *getAllAccount();

    void dump();

private:
    void loadAccountBook(const std::string &filename);
    void saveAccountBook(const std::string &filename);

private:
    proto::AccountBook *account_book;

    const static std::string ACCOUNT_FILE;
   
};

#endif
