#include "otp.h"
#include <random>
#include <iostream>

using namespace std;

int OTP::otpDuration = 300;

OTP::OTP()
    : code(""), expiryTime(0)
{
}

string OTP::getCode() const
{
    return code;
}

long OTP::getExpiryTime() const
{
    return expiryTime;
}

string OTP::generateOTP()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);
    code = "";
    for (int i = 0; i < 6; ++i)
    {
        code += to_string(dis(gen));
    }
    expiryTime = time(NULL) + otpDuration;          // Hết hạn sau 5 phút
    cout << "Ma OTP da duoc gui: " << code << endl; // Giả lập gửi OTP
    return code;
}

bool OTP::verifyOTP(const string &input) const
{
    if (input.empty() || code.empty() || time(NULL) > expiryTime)
    {
        return false;
    }
    return input == code;
}