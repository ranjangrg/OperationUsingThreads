#pragma once

#ifndef __BANK_ACCOUNT_HPP
#define __BANK_ACCOUNT_HPP

#include "Core.hpp"
#include <chrono>
#include <thread>
#include <mutex>

namespace Transaction {
    enum status {SUCCESS, DEPOSIT_FAILED, FAILED_INSUFFICIENT_FUNDS};
}

class BankAccount {
private:
    std::mutex mutexBalance;
    double balance;
public:
    BankAccount();
    //~BankAccount();
    void printBalance();
    double getBalance();
    Transaction::status deposit(double depositAmount);
    Transaction::status withdraw(double withdrawAmount);

};


#endif