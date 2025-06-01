#ifndef HANDLERS_H
#define HANDLERS_H

#include "user.h"
#include "wallet.h"
#include "usermanager.h"
#include "otp.h"
#include "utils.h"

const std::string noPendingChange = "noPendingChange";
const std::string pendingChangeSuccess = "pendingChangeSuccess";
const std::string pendingChangeFail = "pendingChangeFail";

/// @brief Xóa bộ đệm bàn phím
void clearInputBuffer();

/// @brief Tạm dừng, Enter để tiếp tục
void pause();

/// @brief In tiêu đề
/// @param title
void printHeader(const std::string &title);

/// @brief Hiện ý nghĩa nút Esc
/// @return true là ấn nút Esc, false là không ấn nút Esc
bool printEscInfor();

/// @brief Fake OTP
/// @return
std::string generateFakeOTP();

/// @brief Kiểm tra không chứa ký tự đặc biệt
/// @param input
/// @return
bool isValidUsernameOrPassword(const std::string &input);

/// @brief Lấy ký tự từ bàn phím
/// @param prompt
/// @param value
/// @param isString
/// @param checkSpecialChars
/// @return
bool getInput(const std::string &prompt, std::string &value, bool isString, bool checkSpecialChars);

/// @brief Kiểm tra và nhận chuỗi ký tự từ bàn phím
/// @param prompt
/// @param value
/// @param checkSpecialChars
void getValidInput(const std::string &prompt, std::string &value, bool checkSpecialChars);

void handleRegister(UserManager &userManager);
User *handleLogin(UserManager &userManager);
void handleChangePassword(UserManager &userManager, User *currentUser);
void handleChangeFullName(UserManager &userManager, User *currentUser);
void handleViewAccountInfo(UserManager &userManager, User *currentUser);
void handleViewWalletInfo(UserManager &userManager, User *currentUser);
void handleViewTransactionHistory(UserManager &userManager, User *currentUser);
void handleManagerCreateAccount(UserManager &userManager);
void handleTrackAccounts(UserManager &userManager);
void handleAdjustAccount(UserManager &userManager);
void processPendingChange(UserManager &userManager, const std::string &username, const std::string &field, const std::string &newValue);
std::string checkPendingChangeForUser(UserManager &userManager, User *currentUser);
void handleTransferPoints(UserManager &userManager, User *currentUser);

/// @brief Hiển thị thông tin sao lưu dữ liệu
void showBackupdData(UserManager &userManager);

/// @brief Xử lý khôi phục dữ liệu
void handleRestoreBackupData(UserManager &userManager, const std::string filePath);

#endif