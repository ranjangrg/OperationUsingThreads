#include "BankAccount.hpp"

#define TRANSACTION_DELAY 50

BankAccount::BankAccount() {
    this->setBalance(0);
}

void BankAccount::printBalance(const std::string& msg) {
    std::cout << "[ LOG ] " << msg << ": £" << this->getBalance() << std::endl;
}

double BankAccount::getBalance() {
    std::lock_guard<std::mutex> balanceGuard(this->mutexBalance);
    std::this_thread::sleep_for(std::chrono::milliseconds(TRANSACTION_DELAY));
    return this->balance;
}

void BankAccount::setBalance(double newBalance) {
    std::lock_guard<std::mutex> balanceGuard(this->mutexBalance);
    // making writing balance twice as taxing as reading balance
    std::this_thread::sleep_for(std::chrono::milliseconds(TRANSACTION_DELAY * 2));
    this->balance = newBalance;
}

Transaction::status BankAccount::deposit(double depositAmount) {
    assert(depositAmount > 0);
    std::string output;
    Transaction::status transactionCode = Transaction::SUCCESS;
    // output casted to 'int' only to fit transactions within same line
    output +=  "(+£" + std::to_string((int)depositAmount);
    try {
        double newBalance = this->getBalance() + depositAmount;
        this->setBalance(newBalance);
    } catch(...) {
        transactionCode = Transaction::DEPOSIT_FAILED;
        output += "->D.ERR!";
    }
    output += "), ";
    std::cout << output;

    return transactionCode;
}

Transaction::status BankAccount::withdraw(double withdrawAmount) {
    assert(withdrawAmount > 0);
    Transaction::status transactionCode = Transaction::SUCCESS;
    std::string output;
    double newBalance = this->getBalance() - withdrawAmount;
    // output casted to 'int' only to fit transactions within same line
    output +=  "(-£" + std::to_string((int)withdrawAmount);
    if (newBalance > 0) {
        this->setBalance(newBalance);
    } else {
        output += "->W.ERR!";
        transactionCode = Transaction::FAILED_INSUFFICIENT_FUNDS;
    }
    output += "), ";
    std::cout << output;

    return transactionCode;
}
