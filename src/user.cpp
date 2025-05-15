#include "user.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;

User::User(string u, string p, string f, bool manager, bool requireChangePassword, double initialBalance)
    : username(u), password(p), fullName(f),
      isManager(manager), requireChangePassword(requireChangePassword),
      wallet(u, initialBalance)
{
}

User::User(string u, string p, string f, bool manager, bool requireChangePassword, Wallet wallet)
    : username(u), password(p), fullName(f),
      isManager(manager), requireChangePassword(requireChangePassword),
      wallet(wallet)
{
}

string User::getUsername() const
{
    return username;
}

string User::getPassword() const
{
    return password;
}

string User::getFullName() const
{
    return fullName;
}

bool User::isManagerUser() const
{
    return isManager;
}

bool User::isRequiresChangePassword() const
{
    return requireChangePassword;
}

Wallet &User::getWallet()
{
    return wallet;
}

const Wallet &User::getWallet() const
{
    return wallet;
}

bool User::checkPassword(string p) const
{
    return password == p;
}

void User::updatePassword(string newPass)
{
    password = newPass; // dung ham bam - hash function
}

void User::updateFullName(string newName)
{
    fullName = newName;
}

void User::setRequireChangePassword(const bool &require)
{
    requireChangePassword = require;
}

string User::toString() const
{
    // lưu dưới dạng:
    // user1;hashed_pass1;Nguyen Van A;0;0;user1,100.0,system_wallet|user1|100.0|1698230400|success

    stringstream ss;
    ss << username << "|"
       << password << "|"
       << fullName << "|"
       << (isManager ? "1" : "0") << "|"
       << (requireChangePassword ? "1" : "0") << "|"
       << wallet.toString();
    return ss.str();
}

User User::fromString(const string str)
{
    if (str.empty())
        throw invalid_argument("Input string cannot be empty");

    stringstream ss = stringstream(str);
    string username, password, fullName, isManagerStr, requireChangePassStr, walletStr;

    getline(ss, username, '|');
    getline(ss, password, '|');
    getline(ss, fullName, '|');
    getline(ss, isManagerStr, '|');
    getline(ss, requireChangePassStr, '|');
    getline(ss, walletStr);

    if (username.empty() || password.empty() || fullName.empty() ||
        isManagerStr.empty() || walletStr.empty())
        throw invalid_argument("Information cannot be empty");

    if (isManagerStr != "0" && isManagerStr != "1")
        throw invalid_argument("Invalid isManager value");
    bool isManager = (isManagerStr == "1");

    if (requireChangePassStr != "0" && requireChangePassStr != "1")
        throw invalid_argument("Invalid requireChangePassword value");
    bool requireChangePass = (requireChangePassStr == "1");

    User user(username, password, fullName, isManager, requireChangePass, Wallet::fromString(walletStr));

    return user;
}