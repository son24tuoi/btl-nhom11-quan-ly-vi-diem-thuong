#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <vector>
#include <string>
#include <filesystem>
#include "user.h"
#include "pendingchange.h"

class UserManager
{
private:
    const std::string userFilePath = "../data/users.txt";
    const std::string backupFilePath = "../backup/users.txt";
    const std::string systemWalletName = "system_wallet";
    const std::string systemWalletPass = "system_pass";
    const double initialSystemWalletBalance = 1000000.0;
    const std::string pendingChangeFilePath = "../data/pendingchanges.txt";
    const std::string transactionFilePath = "../data/transactions.txt";

    std::vector<User> users;
    std::vector<PendingChange> pendingChanges;
    std::vector<Transaction *> transactions;

    /// @brief Lưu dữ liệu người dùng vào file
    void saveUsers();

    /// @brief Tải dữ liệu người dùng từ file
    void loadUsers();

    /// @brief Sao lưu dữ liệu
    /// @return true là thành công, false là thất bại
    bool backup();

    /// @brief Kiểm tra xem đã có user hay chưa
    /// @param username
    /// @return true là tồn tại, false là chưa có
    bool hasUser(std::string username);

    /// @brief Tải dữ liệu các user đang chờ xác nhận thay đổi thông tin
    /// @return true là thành công, false là thất bại
    bool loadPendingChanges();

    /// @brief Lưu dữ liệu các user đang chờ xác nhận thay đổi thông tin
    /// @return true là thành công, false là thất bại
    bool savePendingChanges();

    /// @brief Tải dữ liệu các giao dịch
    /// @return true là thành công, false là thất bại
    bool loadTransactions();

    /// @brief Lưu dữ liệu các giao dịch
    /// @return true là thành công, false là thất bại
    bool saveTransactions();

    /// @brief Khởi tạo ví tổng lần đầu tiên
    void initializeSystemWallet();

public:
    UserManager();

    /// @brief Đăng ký người dùng mới
    /// @param username
    /// @param password
    /// @param fullName
    /// @param isManager
    /// @return true là thành công, false là lỗi
    bool registerUser(std::string username, std::string password, std::string fullName, bool isManager, bool requireChangePassword, double balance = 0);

    User *loginUser(std::string username, std::string password);

    std::string hashPassword(const std::string &password);
    void generateRandomPassword(std::string &password);

    std::vector<User> &getAllUsers();
    std::vector<PendingChange> &getPendingChanges();

    User *getUser(const std::string &username);

    bool changePassword(User *user, const std::string &newPass);
    bool changeFullName(User *user, const std::string &newFullName);

    bool addPendingChange(const std::string &username, const std::string &field, const std::string &newValue, const std::string &otp, long expiryTime);

    PendingChange *getPendingChange(const std::string &username);
    PendingChange *getPendingChange(const std::string &username, const std::string &field);

    bool hasPendingChange(const std::string &username);
    bool removePendingChange(int index);
    bool applyPendingChange(PendingChange *pendingChange, int pendingChangeIndex, User *user, const std::string otp);

    Transaction *getTransactionByID(const std::string &transactionID);
    std::vector<Transaction *> getTransactionByIDs(const std::vector<std::string> &transactionIDs);
    bool transferPoints(User *senderUser, User *receiverUser, double amount);
    void checkRollbackTransaction(User *user);
    void printTransactionHistory(User *user);

};

#endif
