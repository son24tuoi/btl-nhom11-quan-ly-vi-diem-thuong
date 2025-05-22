#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "usermanager.h"
#include "utils.h"

using namespace std;

UserManager::UserManager()
{
    loadUsers();
    loadPendingChanges();
    loadTransactions();
    initializeSystemWallet();
    cout << "Da khoi tao UserManager" << endl;
}

void UserManager::initializeSystemWallet()
{
    if (users.empty())
    {
        cout << "Khong co nguoi dung, khoi tao vi tong" << endl;
    }
    if (hasUser(systemWalletName))
        return;

    cout << "Khoi tao vi tong" << endl;
    users.emplace_back(systemWalletName, hashPassword(systemWalletPass), systemWalletName, true, false, initialSystemWalletBalance);
    saveUsers();
}

void UserManager::loadUsers()
{
    ifstream file(userFilePath);

    if (!file.is_open())
    {
        // cout << "Khong the mo file" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        try
        {
            // cout << line << endl;
            users.push_back(User::fromString(line));
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }
    file.close();
}

void UserManager::saveUsers()
{
    ofstream file(userFilePath);

    if (!file.is_open())
    {
        // cout << "Khong the mo file" << endl;
        return;
    }

    for (int i = 0; i < users.size(); i++)
    {
        file << users[i].toString() << endl;
    }
    file.close();
}

bool UserManager::backup()
{
    ofstream backup(backupFilePath);

    if (!backup.is_open())
        return false;

    backup << Utils::getTimeNowByLong() << endl;
    for (int i = 0; i < users.size(); i++)
    {
        backup << users[i].toString() << endl;
    }
    backup.close();
    return true;
}

string UserManager::hashPassword(const string &password)
{
    return Utils::hashPassword(password);
}

void UserManager::generateRandomPassword(string &password)
{
    Utils::generateRandomPassword(password);
}

vector<User> &UserManager::getAllUsers()
{
    return users;
}

std::vector<PendingChange> &UserManager::getPendingChanges()
{
    return pendingChanges;
}

User *UserManager::getUser(const std::string &username)
{
    for (User &user : users)
    {
        if (user.getUsername() == username)
            return &user;
    }
    return NULL;
}

bool UserManager::changePassword(User *user, const string &newPass)
{
    if (!user || newPass.empty())
    {
        return false;
    }

    user->updatePassword(hashPassword(newPass));
    user->setRequireChangePassword(false);
    saveUsers();
    return true;
}

bool UserManager::changeFullName(User *user, const string &newFullName)
{
    if (!user || newFullName.empty())
    {
        return false;
    }

    user->updateFullName(newFullName);
    saveUsers();
    return true;
}

bool UserManager::hasUser(string username)
{
    if (users.empty())
        return false;

    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username)
            return true;
    }

    return false;
}

bool UserManager::registerUser(string username, string password, string fullName, bool isManager, bool requireChangePassword, double balance)
{
    if (hasUser(username))
        return false;

    try
    {
        users.emplace_back(username, hashPassword(password), fullName, isManager, requireChangePassword, balance);
        saveUsers();
        return true;
    }
    catch (const invalid_argument &e)
    {
        return false;
    }
}

User *UserManager::loginUser(string username, string password)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].getUsername() == username &&
            users[i].checkPassword(hashPassword(password)))
        {
            checkRollbackTransaction(&users[i]);
            return &users[i];
        }
    }

    return NULL;
}

bool UserManager::loadPendingChanges()
{
    pendingChanges.clear();
    ifstream file(pendingChangeFilePath);

    if (!file.is_open())
    {
        // cout << "Khong the mo file" << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        try
        {
            pendingChanges.push_back(PendingChange::fromString(line));
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }

    return true;
}

bool UserManager::savePendingChanges()
{
    ofstream file(pendingChangeFilePath);

    if (!file.is_open())
    {
        // cout << "Khong the mo file" << endl;
        return false;
    }

    for (int i = 0; i < pendingChanges.size(); i++)
    {
        file << pendingChanges[i].toString() << endl;
    }

    file.close();
    return true;
}

bool UserManager::loadTransactions()
{
    transactions.clear();
    ifstream file(transactionFilePath);

    if (!file.is_open())
    {
        return false;
    }

    string line;
    Transaction *transaction;
    while (getline(file, line))
    {
        try
        {
            transaction = new Transaction(Transaction::fromString(line));
            if (transaction != NULL)
                transactions.push_back(transaction);
        }
        catch (const std::exception &e)
        {
            continue;
        }
    }

    file.close();
    return true;
}

bool UserManager::saveTransactions()
{
    ofstream file(transactionFilePath);

    if (!file.is_open())
    {
        return false;
    }

    for (int i = 0; i < transactions.size(); i++)
    {
        file << transactions[i]->toString() << endl;
    }

    file.close();
    return true;
}

bool UserManager::addPendingChange(const std::string &username, const std::string &field, const std::string &newValue, const std::string &otp, long expiryTime)
{
    PendingChange *pending = getPendingChange(username, field);

    if (pending != NULL)
    {
        pending->overrideValue(newValue, otp, expiryTime);
    }
    else
    {
        pendingChanges.emplace_back(username, field, newValue, otp, expiryTime);
    }

    savePendingChanges();
    return true;
}

PendingChange *UserManager::getPendingChange(const std::string &username)
{
    for (PendingChange &pendingChange : pendingChanges)
    {
        if (pendingChange.getUsername() == username)
            return &pendingChange;
    }
    return NULL;
}

PendingChange *UserManager::getPendingChange(const std::string &username, const std::string &field)
{
    for (PendingChange &pendingChange : pendingChanges)
    {
        if (pendingChange.getUsername() == username && pendingChange.getField() == field)
            return &pendingChange;
    }
    return NULL;
}

bool UserManager::hasPendingChange(const std::string &username)
{
    for (const PendingChange &pendingChange : pendingChanges)
    {
        if (pendingChange.getUsername() == username)
            return true;
    }
    return false;
}

bool UserManager::removePendingChange(int index)
{
    if (index < 0 && index >= pendingChanges.size())
        return false;

    pendingChanges.erase(pendingChanges.begin() + index);
    savePendingChanges();
    return true;
}

bool UserManager::applyPendingChange(PendingChange *pendingChange, int pendingChangeIndex, User *user, const std::string otp)
{
    if (!pendingChange || !user)
        return false;

    if (pendingChange->getOTP() != otp)
        return false;

    if (pendingChange->isTimeout())
        return false;

    string field = pendingChange->getField();
    string newValue = pendingChange->getNewValue();
    bool success = false;

    if (field == PendingChange::passwordKey)
    {
        success = changePassword(user, newValue);
    }
    else if (field == PendingChange::fullNameKey)
    {
        success = changeFullName(user, newValue);
    }

    if (success)
    {
        removePendingChange(pendingChangeIndex);
        return true;
    }

    return false;
}

Transaction *UserManager::getTransactionByID(const string &transactionID)
{
    for (int i = 0; i < transactions.size(); i++)
    {
        if (transactions[i]->getTransactionID() == transactionID)
        {
            return transactions[i];
        }
    }

    return NULL;
}

vector<Transaction *> UserManager::getTransactionByIDs(const vector<string> &transactionIDs)
{
    vector<Transaction *> transactions;
    Transaction *transaction;
    for (const string &transactionID : transactionIDs)
    {
        transaction = getTransactionByID(transactionID);
        if (transaction != NULL)
        {
            transactions.push_back(transaction);
        }
    }
    return transactions;
}

bool UserManager::transferPoints(User *senderUser, User *receiverUser, double amount)
{
    if (senderUser == NULL || receiverUser == NULL)
        return false;

    double senderBalanceBefore = senderUser->getWallet().getBalance();
    double receiverBalanceBefore = receiverUser->getWallet().getBalance();

    if (senderUser->getWallet().getBalance() < amount)
    {
        cout << "Chuyen diem that bai. So du trong vi khong du." << endl;
        return false;
    }

    // Tạo Transaction
    std::string timestamp = std::to_string(Utils::getTimeNowByLong());
    Transaction *transaction = new Transaction(
    senderUser->getUsername(),
    receiverUser->getUsername(),
    amount,
    timestamp,
    Transaction::begin,
    senderBalanceBefore,
    receiverBalanceBefore);

    transactions.push_back(transaction);
    Wallet *senderWallet = &senderUser->getWallet();
    Wallet *receiverWallet = &receiverUser->getWallet();
    senderWallet->addTransactionID(transaction->getTransactionID());
    receiverWallet->addTransactionID(transaction->getTransactionID());
    saveTransactions();
    saveUsers();

    try
    {
        senderWallet->updateBalance(-amount);
        receiverWallet->updateBalance(amount);
        saveUsers();
        transaction->setStatus(Transaction::commit);
        saveTransactions();
        return true;
    }
    catch (const std::exception &e)
    {
        // Rollback
        senderWallet->setBalance(transaction->getSenderBalanceBefore());
        receiverWallet->setBalance(transaction->getReceiverBalanceBefore());
        saveUsers();
        transaction->setStatus(Transaction::failed);
        saveTransactions();
        cout << "Giao dich bi huy do loi he thong." << endl;
        return false;
    }
}

void UserManager::checkRollbackTransaction(User *user)
{
    vector<Transaction *> transactions = getTransactionByIDs(user->getWallet().getTransactionIDs());
    Transaction *transaction;
    Wallet *senderWallet;
    Wallet *receiverWallet;
    std::string currentTimestamp = std::to_string(Utils::getTimeNowByLong());

    for (int i = 0; i < transactions.size(); i++)
    {
        transaction = transactions[i];
        if (transaction->isBegin() && transaction->getTimestamp() != currentTimestamp)
        {
            senderWallet = &getUser(transaction->getSender())->getWallet();
            receiverWallet = &getUser(transaction->getReceiver())->getWallet();

            senderWallet->setBalance(transaction->getSenderBalanceBefore());
            receiverWallet->setBalance(transaction->getReceiverBalanceBefore());

            transaction->setStatus(Transaction::failed);
        }
    }
    saveUsers();
    saveTransactions();
}

void UserManager::printTransactionHistory(User *user)
{
    if (!user)
    {
        std::cout << "Nguoi dung khong hop le.\n";
        return;
    }

    std::vector<std::string> transactionIDs = user->getWallet().getTransactionIDs();
    std::vector<Transaction *> userTransactions = getTransactionByIDs(transactionIDs);

    if (userTransactions.empty())
    {
        std::cout << "Khong co giao dich nao.\n";
        return;
    }

    std::cout << "Lich su giao dich cua " << user->getUsername() << ":\n";
    for (Transaction *transaction : userTransactions)
    {
        std::cout << "----------------------------------------\n";
        std::cout << "Ma giao dich: " << transaction->getTransactionID() << "\n";
        std::cout << "Nguoi gui: " << transaction->getSender() << "\n";
        std::cout << "Nguoi nhan: " << transaction->getReceiver() << "\n";
        std::cout << "So điemm: " << transaction->getAmount() << "\n";
        std::cout << "Thoi gian: " << transaction->getPrintTime() << "\n";
        std::cout << "Trang thai: " << transaction->getStatus() << "\n";
    }
    std::cout << "----------------------------------------\n";
}
