#include "BankAccount.hpp"

BankAccount::BankAccount() {
    this->balance = 0;  // maybe use getter/setters
}

void BankAccount::printBalance() {
    std::lock_guard<std::mutex> balanceGuard(this->mutexBalance);
    std::cout << "[ LOG ] Balance: £" << this->balance << std::endl;
}

double BankAccount::getBalance() {
    return this->balance;
}

Transaction::status BankAccount::deposit(double depositAmount) {
    std::lock_guard<std::mutex> depositGuard(this->mutexBalance);

    Transaction::status transactionCode = Transaction::SUCCESS;
    try {
        this->balance += depositAmount;
        std::cout << "[ DEPOSIT  ] (£" << depositAmount << ") New balance: £" << this->getBalance() << std::endl;
    } catch(...) {
        transactionCode = Transaction::DEPOSIT_FAILED;
        std::cout << "[ DEPOSIT  ] (£" << depositAmount << ") ERROR! Deposit failed!" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(0));

    return transactionCode;
}

Transaction::status BankAccount::withdraw(double withdrawAmount) {
    std::lock_guard<std::mutex> withdrawGuard(this->mutexBalance);

    Transaction::status transactionCode = Transaction::SUCCESS;
    
    double newBalance = this->balance - withdrawAmount;
    if (newBalance > 0) {
        this->balance = newBalance;
        std::cout << "[ WITHDRAW ] (£" << withdrawAmount << ") New balance: £" << this->getBalance() << std::endl;
    } else {
        std::cout << "[ WITHDRAW ] (£" << withdrawAmount << ") Failed to withdraw £" << withdrawAmount << "! Insufficient funds." << std::endl;
        transactionCode = Transaction::FAILED_INSUFFICIENT_FUNDS;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(0));

    return transactionCode;
}
