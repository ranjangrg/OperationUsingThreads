#include "BankAccount.hpp"

#define TRANSACTION_DELAY 500

BankAccount::BankAccount() {
    this->setBalance(0);
}

void BankAccount::printBalance() {
    std::cout << "[ LOG ] Balance: £" << this->getBalance() << std::endl;
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
    Transaction::status transactionCode = Transaction::SUCCESS;
    std::cout << "[ DEPOSIT  ] (£" << depositAmount << ")" << std::endl;
    try {
        double newBalance = this->getBalance() + depositAmount;
        this->setBalance(newBalance);
    } catch(...) {
        transactionCode = Transaction::DEPOSIT_FAILED;
        std::cout << "[ ERR ] Deposit failed!" << std::endl;
    }

    return transactionCode;
}

Transaction::status BankAccount::withdraw(double withdrawAmount) {
    Transaction::status transactionCode = Transaction::SUCCESS;
    
    double newBalance = this->getBalance() - withdrawAmount;
    std::cout << "[ WITHDRAW ] (£" << withdrawAmount << ")" << std::endl;
    if (newBalance > 0) {
        this->setBalance(newBalance);
    } else {
        std::cout << "[ ERR ] Failed to withdraw £" << withdrawAmount << "! Insufficient funds." << std::endl;
        transactionCode = Transaction::FAILED_INSUFFICIENT_FUNDS;
    }

    return transactionCode;
}
