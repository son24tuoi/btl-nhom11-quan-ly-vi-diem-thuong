#include "wallet.h"
#include <sstream>

using namespace std;

Wallet::Wallet(string id, double initialBalance)
    : id(id), balance(initialBalance)
{
}

string Wallet::getId() const
{
    return id;
}

double Wallet::getBalance() const
{
    return balance;
}

void Wallet::updateBalance(double amount)
{
    balance += amount;
}

void Wallet::addTransactionID(const std::string &transactionID)
{
    transactionIDs.emplace_back(transactionID);
}

const std::vector<std::string> &Wallet::getTransactionIDs() const
{
    return transactionIDs;
}

void Wallet::setBalance(double amount)
{
    balance = amount;
}

string Wallet::toString() const
{
    stringstream ss;
    ss << id << "," << balance;
    for (int i = 0; i < transactionIDs.size(); i++)
    {
        ss << "," << transactionIDs[i];
    }
    return ss.str();
}

Wallet Wallet::fromString(const string str)
{
    if (str.empty())
        throw invalid_argument("Input string cannot be empty");

    stringstream ss = stringstream(str);
    string id, balanceStr;

    getline(ss, id, ',');
    getline(ss, balanceStr, ',');

    if (id.empty())
        throw invalid_argument("Id cannot be empty");
    if (balanceStr.empty())
        throw invalid_argument("Balance cannot be empty");

    double balance;
    try
    {
        balance = stod(balanceStr);
    }
    catch (const std::exception &e)
    {
        throw invalid_argument("Invalid balance format");
    }
    if (balance < 0)
    {
        throw invalid_argument("Balance cannot be negative");
    }

    Wallet wallet(id, balance);
    string transactionStr;
    while (getline(ss, transactionStr, ','))
    {
        try
        {
            wallet.addTransactionID(transactionStr);
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }

    return wallet;
}