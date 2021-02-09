#pragma once

#ifndef __BANK_ACCOUNT_HPP
#define __BANK_ACCOUNT_HPP

#include "Core.hpp"
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

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
    void printBalance(const std::string& msg);
    double getBalance();
    void setBalance(double newBalance);
    Transaction::status deposit(double depositAmount);
    Transaction::status withdraw(double withdrawAmount);

};


#endif