
#ifndef PENDINGCHANGE_H
#define PENDINGCHANGE_H

#include <string>

class PendingChange
{
private:
    std::string username;
    std::string field;
    std::string newValue;
    std::string otp;
    long timestamp;

public:
    static std::string passwordKey;
    static std::string fullNameKey;

    PendingChange(const std::string &username, const std::string &field, const std::string &newValue, const std::string &otp, long timestamp);

    std::string getUsername() const;
    std::string getField() const;
    std::string getNewValue() const;
    std::string getOTP() const;
    long getTimestamp() const;

    bool isPassword() const;
    bool isFullName() const;
    bool isTimeout() const;

    /// @brief Chuyển sang dữ liệu text, dùng cho lưu file
    /// @return
    std::string toString() const;

    static PendingChange fromString(const std::string str);

    void overrideValue(const std::string &newValue, const std::string &otp, long timestamp);
};

#endif
