#ifndef OTP_H
#define OTP_H
#include <string>

class OTP
{
private:
    std::string code;
    long expiryTime; // Thời gian hết hạn
public:
    static int otpDuration;

    OTP();
    std::string getCode() const;
    long getExpiryTime() const;
    std::string generateOTP();
    bool verifyOTP(const std::string &input) const;
};

#endif
