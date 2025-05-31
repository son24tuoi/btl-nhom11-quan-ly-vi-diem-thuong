#include <iostream>
#include <limits>
#include "handlers.h"

using namespace std;

void clearScreen()
{
    // system("cls");
    for (int i = 0; i < 50; i++)
        cout << endl;
}

void pause()
{
    cout << "Nhan Enter de tiep tuc...";
    clearInputBuffer();
    // system("pause");
}

void showMainMenu()
{
    cout << "=== He Thong Quan Ly Vi ===" << endl;
    cout << "1. Dang ky" << endl;
    cout << "2. Dang nhap" << endl;
    cout << "3. Thoat" << endl;
    cout << "--------------------------" << endl;
    cout << "Chon: ";
}

void showInnerMenu(bool isManager)
{
    cout << "=== Menu Chuc Nang ===" << endl;
    cout << "1. Thay doi mat khau" << endl;
    cout << "2. Thay doi ho ten" << endl;
    cout << "3. Xem thong tin tai khoan" << endl;
    cout << "4. Xem thong tin vi" << endl;
    cout << "5. Xem thong tin giao dich" << endl;
    cout << "6. Chuyen diem" << endl;
    if (isManager)
    {
        cout << "7. Tao tai khoan moi (Quan ly)" << endl;
        cout << "8. Theo doi danh sach (Quan ly)" << endl;
        cout << "9. Dieu chinh thong tin tai khoan khac (Quan ly)" << endl;
        cout << "10. Dang xuat" << endl;
    }
    else
    {
        cout << "7. Dang xuat" << endl;
    }
    cout << "--------------------------" << endl;
    cout << "Chon: ";
}

void handleInnerMenu(UserManager &userManager, User *user)
{
    clearScreen();
    int choice;
    bool isManager = user->isManagerUser();

    while (true)
    {
        showInnerMenu(isManager);
        cin >> choice;
        cin.ignore();

        int exitChoice = isManager ? 10 : 7;
        if (choice == exitChoice)
        {
            cout << "Dang xuat thanh cong!\n";
            break;
        }

        switch (choice)
        {
        case 1:
            handleChangePassword(userManager, user);
            break;
        case 2:
            handleChangeFullName(userManager, user);
            break;
        case 3:
            handleViewAccountInfo(userManager, user);
            break;
        case 4:
            handleViewWalletInfo(userManager, user);
            break;
        case 5:
            handleViewTransactionHistory(userManager, user);
            break;
        case 6:
            handleTransferPoints(userManager, user);
            break;
        case 7:
            if (isManager)
            {
                handleManagerCreateAccount(userManager);
            }
            else
            {
                cout << "Lua chon khong hop le!" << endl;
            }
            break;
        case 8:
            if (isManager)
            {
                handleTrackAccounts(userManager);
            }
            else
            {
                cout << "Lua chon khong hop le!" << endl;
            }
            break;
        case 9:
            if (isManager)
            {
                handleAdjustAccount(userManager);
            }
            else
            {
                cout << "Lua chon khong hop le!" << endl;
            }
            break;
        default:
            break;
        }
        cout << endl;
        pause();
        clearScreen();
    }
}

int main()
{
    UserManager userManager; // Quản lý người dùng
    cout << endl;
    int choice;

    while (true)
    {
        showMainMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 3)
        {
            cout << "Tam biet!" << endl;
            break;
        }

        switch (choice)
        {
        case 1:
            handleRegister(userManager);
            break;
        case 2:
        {
            User *user = handleLogin(userManager);
            if (user)
            {
                pause();
                handleInnerMenu(userManager, user);
            }
        }
        break;
        default:
            cout << "Lua chon khong hop le!" << endl;
        }
        cout << endl;
        pause();

        clearScreen();
    }

    return 0;
}