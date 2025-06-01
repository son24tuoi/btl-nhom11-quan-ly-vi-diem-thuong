#include "utils.h"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

string Utils::hashPassword(const string &input)
{
    unsigned long hash = 0;
    int prime = 31; // số nguyên tố nhỏ giúp phân bố đều hơn

    for (char c : input)
    {
        hash = hash * prime + c; // nhân và cộng từng ký tự
    }

    return to_string(hash);
}

void Utils::generateRandomPassword(string &password)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, chars.size() - 1);
    for (int i = 0; i < 8; ++i)
    {
        password += chars[dis(gen)];
    }
}

string Utils::getTimeNow()
{
    return to_string(time(NULL));
}

long Utils::getTimeNowByLong()
{
    return time(NULL);
}

void Utils::openFile(const string &filePath)
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        cout << "Khong mo duoc file" << endl;
        return;
    }

    string line;
    string filename = filesystem::path(filePath).filename().string();

    cout << "--------------------------------------------------------------------" << endl
         << "Bat dau " << filename << endl
         << "--------------------------------------------------------------------" << endl
         << endl;

    while (getline(file, line))
    {
        cout << line << endl;
    }

    cout << endl
         << "--------------------------------------------------------------------" << endl
         << "Ket thuc " << filename << endl
         << "--------------------------------------------------------------------" << endl
         << endl;

    file.close();
}
