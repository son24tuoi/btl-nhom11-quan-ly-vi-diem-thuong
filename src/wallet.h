#ifndef WALLET_H
#define WALLET_H
#include <string>
#include <vector>
#include "transaction.h"

class Wallet
{
private:
    std::string id;
    double balance;
    std::vector<std::string> transactionIDs;

public:
    Wallet(std::string id, double initialBalance = 0.0);
    std::string getId() const;
    double getBalance() const;
    void updateBalance(double amount);
    void addTransactionID(const std::string &transactionID);
    const std::vector<std::string> &getTransactionIDs() const;

    void setBalance(double amount);

    /// @brief Chuyển sang dữ liệu text, dùng cho lưu file
    /// @return
    std::string toString() const;

    static Wallet fromString(const std::string str);
};

#endif