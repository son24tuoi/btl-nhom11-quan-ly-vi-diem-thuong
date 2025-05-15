#ifndef USER_H
#define USER_H
#include <vector>
#include <string>
#include "wallet.h"

class User
{
private:
    std::string username;
    std::string password;
    std::string fullName;
    bool isManager;
    bool requireChangePassword;
    Wallet wallet;

public:
    User(std::string u, std::string p, std::string f, bool manager, bool requireChangePassword, double initialBalance);
    User(std::string u, std::string p, std::string f, bool manager, bool requireChangePassword, Wallet wallet);

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getFullName() const;
    bool isManagerUser() const;
    bool isRequiresChangePassword() const;
    Wallet &getWallet();
    const Wallet &getWallet() const;

    bool checkPassword(std::string p) const;
    void updatePassword(std::string newPass);
    void updateFullName(std::string newName);
    void setRequireChangePassword(const bool &require);

    /// @brief Chuyển sang dữ liệu text, dùng cho lưu file
    /// @return
    std::string toString() const;

    static User fromString(const std::string str);
};

#endif