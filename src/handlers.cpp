#include "handlers.h"
#include <iostream>
#include <string>
#include <limits>
#include <conio.h>

using namespace std;

void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printHeader(const string &title)
{
    cout << "=== " << title << " ===" << endl;
}

bool printEscInfor()
{
    cout << "Nhan ESC de thoat. Enter de tiep tuc." << endl;
    return _getch() == 27;
}

string generateFakeOTP()
{
    return "123456";
}

bool isValidUsernameOrPassword(const string &input)
{
    for (char c : input)
    {
        if (!isalnum(c) && c != '_' && c != '-')
        {
            return false;
        }
    }
    return !input.empty();
}

bool getInput(const string &prompt, string &value, bool isString, bool checkSpecialChars)
{
    cout << prompt;
    if (isString)
    {
        getline(cin, value);
    }
    else
    {
        cin >> value;
        clearInputBuffer();
    }
    if (cin.fail())
    {
        cout << "Loi doc du lieu dau vao. Vui long thu lai." << endl;
        cin.clear();
        clearInputBuffer();
        return false;
    }
    if (checkSpecialChars && !isValidUsernameOrPassword(value))
    {
        cout << "Khong duoc chua dau cach hoac ky tu dac biet (chi cho phep a-z, A-Z, 0-9, _, -)." << endl;
        return false;
    }
    return true;
}

void getValidInput(const string &prompt, string &value, bool checkSpecialChars)
{
    do
    {
        if (getInput(prompt, value, true, checkSpecialChars))
        {
            break;
        }
    } while (true);
}

void handleRegister(UserManager &userManager)
{
    printHeader("Dang Ky");
    string username, password, fullName, isManagerStr;

    getValidInput("Nhap ten nguoi dung: ", username, true);
    // cout << "Username: [" << username << "]" << endl;
    if (printEscInfor())
        return;

    getValidInput("Nhap mat khau: ", password, true);
    // cout << "Password: [" << password << "]" << endl;
    if (printEscInfor())
        return;

    getValidInput("Nhap ho va ten: ", fullName, false); // Không kiểm tra ký tự đặc biệt cho tên
    // cout << "Ho va ten: [" << fullName << "]" << endl;
    if (printEscInfor())
        return;

    do
    {
        cout << "La quan ly? (1 = Co, 0 = Khong): ";
        cin >> isManagerStr;
        clearInputBuffer();
        if (isManagerStr != "0" && isManagerStr != "1")
        {
            cout << "Khong dung dinh dang. Vui long nhap lai." << endl;
            continue;
        }
        // cout << "La quan ly: [" << isManagerStr << "]" << endl;
        break;
    } while (true);
    if (printEscInfor())
        return;
    bool isManager = (isManagerStr == "1");

    if (userManager.registerUser(username, password, fullName, isManager, false))
    {
        cout << "Dang ky thanh cong" << endl;
    }
    else
    {
        cout << "Dang ky that bai. Ten nguoi dung '" << username << "' da ton tai." << endl;
    }
}

User *handleLogin(UserManager &userManager)
{
    printHeader("Dang Nhap");
    string username, password;

    getValidInput("Nhap username: ", username, true);
    // cout << "Username: [" << username << "]" << endl;
    if (printEscInfor())
        return NULL;

    getValidInput("Nhap password: ", password, true);
    // cout << "Password: [" << password << "]" << endl;
    if (printEscInfor())
        return NULL;

    OTP otp;
    string otpStr;
OTP:
    otp.generateOTP();
    cout << "Ban yeu cau dang nhap [" << username << "]. Nhap OTP de xac nhan." << endl;
    getValidInput("Nhap OTP: ", otpStr, true);

    if (!otp.verifyOTP(otpStr))
    {
        cout << "OTP khong dung hoac het han. Dang nhap that bai!" << endl;
        return NULL;
    }

    User *user = userManager.loginUser(username, password);
    if (user)
    {
        cout << "Dang nhap thanh cong! Xin chao, " << user->getFullName() << endl;

        if (user->isRequiresChangePassword())
        {
            cout << "Tai khoan yeu cau doi mat khau truoc khi su dung. Vui long doi mat khau." << endl;
            handleChangePassword(userManager, user);
            if (!user->isRequiresChangePassword())
            {
                cout << "Da cap nhat trang thai yeu cau doi mat khau." << endl;
            }
            else
            {
                cout << "Doi mat khau that bai. Vui long dang nhap lai de thu lai." << endl;
                return NULL;
            }
        }
        else
        {
            string info = checkPendingChangeForUser(userManager, user);
            if (info == pendingChangeFail)
                return NULL;
        }
    }
    else
    {
        cout << "Dang nhap that bai. Sai username hoac password." << endl;
    }
    return user;
}

void handleChangePassword(UserManager &userManager, User *currentUser)
{
    printHeader("Thay Doi Mat Khau");
    string oldPassword, newPassword;
Nhap:
    getValidInput("Nhap mat khau cu: ", oldPassword, true);
    if (printEscInfor())
        return;

    getValidInput("Nhap mat khau moi: ", newPassword, true);
    if (printEscInfor())
        return;

    if (userManager.hashPassword(oldPassword) == currentUser->getPassword())
    {
        OTP otp;
        string otpStr;
    OTP:
        otp.generateOTP();
        cout << "Ban yeu cau doi mat khau thanh [" << newPassword << "]. Nhap OTP de xac nhan." << endl;
        getValidInput("Nhap OTP: ", otpStr, true);
        if (printEscInfor())
            return;

        if (!otp.verifyOTP(otpStr))
        {
            cout << "OTP khong dung hoac het han. Thay doi mat khau that bai!" << endl;
            return;
        }

        if (userManager.changePassword(currentUser, newPassword))
        {
            cout << "Thay doi mat khau thanh cong." << endl;
        }
        else
        {
            cout << "Thay doi mat khau that bai." << endl;
        }
    }
    else
    {
        cout << "Mat khau cu khong dung." << endl;
        goto Nhap;
    }
}

void handleChangeFullName(UserManager &userManager, User *currentUser)
{
    printHeader("Thay Doi Ho Ten");
    string newFullName;

    getValidInput("Nhap ho va ten moi: ", newFullName, false);
    if (printEscInfor())
        return;

    OTP otp;
    string otpStr;
OTP:
    otp.generateOTP();
    cout << "Ban yeu cau doi ho ten thanh [" << newFullName << "]. Nhap OTP de xac nhan." << endl;
    getValidInput("Nhap OTP: ", otpStr, true);
    if (printEscInfor())
        return;

    if (!otp.verifyOTP(otpStr))
    {
        cout << "OTP khong dung hoac het han. Thay doi ho ten that bai!" << endl;
        return;
    }

    if (userManager.changeFullName(currentUser, newFullName))
    {
        cout << "Thay doi ho ten thanh cong: " << newFullName << endl;
    }
    else
    {
        cout << "Thay doi ho ten that bai." << endl;
    }
}

void handleViewAccountInfo(UserManager &userManager, User *currentUser)
{
    printHeader("Xem Thong Tin Tai Khoan");
    cout << "Username: " << currentUser->getUsername() << endl;
    cout << "Ho va ten: " << currentUser->getFullName() << endl;
    cout << "Quyen: " << (currentUser->isManagerUser() ? "Quan ly" : "Nguoi dung") << endl;
}

void handleViewWalletInfo(UserManager &userManager, User *currentUser)
{
    printHeader("Xem Thong Tin Vi");
    cout << "Wallet ID: " << currentUser->getWallet().getId() << endl;
    cout << "So du: " << currentUser->getWallet().getBalance() << endl;
}

void handleViewTransactionHistory(UserManager &userManager, User *currentUser)
{
    printHeader("Lich Su Giao Dich");
    const vector<string> &transactionIDs = currentUser->getWallet().getTransactionIDs();
    if (transactionIDs.empty())
    {
        cout << "Khong co giao dich nao." << endl;
    }
    else
    {
        vector<Transaction *> transactions = userManager.getTransactionByIDs(currentUser->getWallet().getTransactionIDs());
        string username = currentUser->getUsername();
        string sender;
        for (Transaction *transaction : transactions)
        {
            sender = transaction->getSender();
            cout << ((username == sender) ? "SEND: " : "RECEIVE: ")
                 << "Tu tai khoan: " << sender
                 << ", Toi tai khoan : " << transaction->getReceiver()
                 << ", So diem: " << transaction->getAmount()
                 << ", Thoi gian: " << transaction->getPrintTime()
                 << ", Trang thai: " << transaction->getStatus() << endl;
        }
    }
    cout << endl;
}

void handleManagerCreateAccount(UserManager &userManager)
{
    printHeader("Tao Tai Khoan Moi");
    string username, fullName, isManagerStr, password;

    getValidInput("Nhap ten nguoi dung: ", username, true);
    // cout << "Username: [" << username << "]" << endl;
    if (printEscInfor())
        return;

    getValidInput("Nhap ho va ten: ", fullName, false); // Không kiểm tra ký tự đặc biệt cho tên
    // cout << "Ho va ten: [" << fullName << "]" << endl;
    if (printEscInfor())
        return;

    do
    {
        cout << "La quan ly? (1 = Co, 0 = Khong): ";
        cin >> isManagerStr;
        clearInputBuffer();
        if (isManagerStr != "0" && isManagerStr != "1")
        {
            cout << "Khong dung dinh dang. Vui long nhap lai." << endl;
            continue;
        }
        // cout << "La quan ly: [" << isManagerStr << "]" << endl;
        break;
    } while (true);
    if (printEscInfor())
        return;
    bool isManager = (isManagerStr == "1");

    userManager.generateRandomPassword(password);
    cout << "Mat khau tu dong tao: " << password << endl;

    if (userManager.registerUser(username, password, fullName, isManager, true))
    {
        cout << "Tao tai khoan thanh cong." << endl;
        cout << "Vui long thong bao mat khau [" << password << "] cho nguoi dung." << endl;
    }
    else
    {
        cout << "Tao tai khoan that bai. Ten nguoi dung '" << username << "' da ton tai." << endl;
    }
}

void handleTrackAccounts(UserManager &userManager)
{
    printHeader("Danh Sach Tai Khoan");
    const vector<User> &users = userManager.getAllUsers();
    if (users.empty())
    {
        cout << "Khong co tai khoan nao." << endl;
    }
    else
    {
        for (const auto &user : users)
        {
            cout << "Username: " << user.getUsername()
                 << ", Ho va ten: " << user.getFullName()
                 << ", Quyen: " << (user.isManagerUser() ? "Quan ly" : "Nguoi dung")
                 << ", So du: " << user.getWallet().getBalance() << endl;
        }
    }
}

void handleAdjustAccount(UserManager &userManager)
{
    printHeader("Dieu Chinh Thong Tin Tai Khoan Khac");
    string targetUsername;

    getValidInput("Nhap username can dieu chinh: ", targetUsername, true);
    if (printEscInfor())
        return;

    User *targetUser = userManager.getUser(targetUsername);
    if (targetUser == NULL)
    {
        cout << "Tai khoan [" << targetUsername << "] khong ton tai." << endl;
        return;
    }

    if (targetUser->isRequiresChangePassword())
    {
        cout << "Tai khoan [" << targetUsername << "] chua duoc kich hoat." << endl;
        cout << "Tai khoan [" << targetUsername << "] can tu dang nhap lan dau tien de doi mat khau." << endl;
        return;
    }

    int choice;
    string newValue;
    while (true)
    {
        cout << "Chon thong tin can dieu chinh:" << endl;
        cout << "1. Thay doi mat khau" << endl;
        cout << "2. Thay doi ho va ten" << endl;
        cout << "3. Thoat" << endl;
        cout << "--------------------------" << endl;
        cout << "Chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 3)
        {
            break;
        }

        switch (choice)
        {
        case 1:
            getValidInput("Nhap mat khau moi: ", newValue, true);
            if (newValue == targetUser->getPassword())
            {
                cout << "Mat khau moi trung voi mat khau cu" << endl;
                break;
            }
            processPendingChange(userManager, targetUsername, PendingChange::passwordKey, newValue);
            return;
        case 2:
            cout << "Ho ten hien tai: " << targetUser->getFullName() << endl;
            getValidInput("Nhap ho ten moi: ", newValue, false);
            if (newValue == targetUser->getFullName())
            {
                cout << "Ho ten moi trung voi ho ten cu" << endl;
                break;
            }
            processPendingChange(userManager, targetUsername, PendingChange::fullNameKey, newValue);
            return;
        default:
            cout << "Lua chon khong hop le!" << endl;
            break;
        }
    }
}

void processPendingChange(UserManager &userManager, const std::string &username, const std::string &field, const std::string &newValue)
{
    OTP otp;
    otp.generateOTP();
    cout << "Yeu cau thay doi " << (field == PendingChange::passwordKey ? "mat khau" : "ho va ten")
         << " cho tai khoan [" << username << "] thanh [" << newValue << "] da duoc gui." << endl;

    if (userManager.addPendingChange(username, field, newValue, otp.getCode(), otp.getExpiryTime()))
    {
        cout << "Yeu cau thay doi " << (field == "password" ? "mat khau" : "ho va ten")
             << " da duoc xu ly. Chu tai khoan can xac nhan OTP khi dang nhap." << endl;
    }
    else
    {
        cout << "Loi khi luu yeu cau thay doi." << endl;
    }
}

string checkPendingChangeForUser(UserManager &userManager, User *currentUser)
{
    vector<PendingChange> &pendingChange = userManager.getPendingChanges();

    string username = currentUser->getUsername();
    if (!userManager.hasPendingChange(username))
        return noPendingChange;

    PendingChange *pending = NULL;
    int pendingChangeIndex;
    User *user = userManager.getUser(username);

    while (userManager.hasPendingChange(username))
    {
        for (int i = 0; i < pendingChange.size(); i++)
        {
            if (pendingChange[i].getUsername() == username)
            {
                pending = &pendingChange[i];
                pendingChangeIndex = i;
            }
        }

        if (pending->isTimeout())
        {
            userManager.removePendingChange(pendingChangeIndex);
            continue;
        }

        string field = pending->getField();
        string newValue = pending->getNewValue();

        cout << "Co yeu cau thay doi " << (field == PendingChange::passwordKey ? "mat khau" : "ho va ten")
             << " thanh [" << newValue << "]. Nhap OTP de xac nhan [" << pending->getOTP() << "]." << endl;

        string otpStr;
        getValidInput("Nhap OTP: ", otpStr, true);
        if (userManager.applyPendingChange(pending, pendingChangeIndex, user, otpStr))
        {
            cout << "Da ap dung thay doi " << (field == PendingChange::passwordKey ? "mat khau" : "ho va ten") << "." << endl;
            if (userManager.hasPendingChange(username))
            {
                continue;
            }
            return pendingChangeSuccess;
        }
        else
        {
            cout << "Xac nhan that bai. OTP khong dung hoac da het han." << endl;
            return pendingChangeFail;
        }
    }

    return noPendingChange;
}

void handleTransferPoints(UserManager &userManager, User *currentUser)
{
    printHeader("Chuyen Diem");
    string receiver;

    getValidInput("Nhap username nguoi nhan: ", receiver, true);

    if (receiver == currentUser->getUsername())
    {
        cout << "Chuyen diem that bai. Khong the chuyen diem cho chinh minh." << endl;
        return;
    }

    User *receiverUser = userManager.getUser(receiver);

    if (receiverUser == NULL)
    {
        cout << "Chuyen diem that bai. Tai khoan [" << receiver << "] khong ton tai." << endl;
        return;
    }

    string amountStr;
    double amount;
    getValidInput("Nhap so diem can chuyen: ", amountStr, false);
    try
    {
        amount = stod(amountStr);
        if (amount <= 0)
        {
            cout << "Chuyen diem that bai. So diem phai lon hon 0." << endl;
            return;
        }
    }
    catch (const std::exception &e)
    {
        cout << "Chuyen diem that bai. So diem khong hop le." << endl;
        return;
    }

    OTP otp;
    otp.generateOTP();
    string otpStr;
    getValidInput("Nhap ma OTP: ", otpStr, true);
    if (!otp.verifyOTP(otpStr))
    {
        cout << "Chuyen diem that bai. OTP khong dung hoac het han." << endl;
        return;
    }

    if (userManager.transferPoints(currentUser, receiverUser, amount))
    {
        cout << "Chuyen khoan thanh cong: " << amount
             << " tu [" << currentUser->getUsername()
             << "] den [" << receiverUser->getUsername() << "]" << endl;
    }
    else
    {
        cout << "Chuyen diem that bai. Kiem tra nguoi nhan hoac so du." << endl;
    }
}
