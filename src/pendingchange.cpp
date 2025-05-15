
#include "pendingchange.h"
#include <sstream>
#include <stdexcept>
#include "otp.h"
#include "utils.h"

std::string PendingChange::passwordKey = "password";
std::string PendingChange::fullNameKey = "fullName";

PendingChange::PendingChange(const std::string &username, const std::string &field, const std::string &newValue, const std::string &otp, long timestamp)
    : username(username), field(field), newValue(newValue), otp(otp), timestamp(timestamp)
{
}

std::string PendingChange::getUsername() const
{
    return username;
}

std::string PendingChange::getField() const
{
    return field;
}

std::string PendingChange::getNewValue() const
{
    return newValue;
}

std::string PendingChange::getOTP() const
{
    return otp;
}

long PendingChange::getTimestamp() const
{
    return timestamp;
}

bool PendingChange::isPassword() const
{
    return field == passwordKey;
}

bool PendingChange::isFullName() const
{
    return field == fullNameKey;
}

bool PendingChange::isTimeout() const
{
    return (Utils::getTimeNowByLong() - timestamp > OTP::otpDuration);
}

std::string PendingChange::toString() const
{
    std::stringstream ss;
    ss << username << "|" << field << "|" << newValue << "|" << otp << "|" << timestamp;
    return ss.str();
}

PendingChange PendingChange::fromString(const std::string str)
{
    if (str.empty())
        throw std::invalid_argument("Input string cannot be empty");

    std::stringstream ss = std::stringstream(str);
    std::string username, field, newValue, otp, timestampStr;

    std::getline(ss, username, '|');
    std::getline(ss, field, '|');
    std::getline(ss, newValue, '|');
    std::getline(ss, otp, '|');
    std::getline(ss, timestampStr, '|');

    if (username.empty() || field.empty() || newValue.empty() || otp.empty() || timestampStr.empty())
    {
        throw std::invalid_argument("Invalid PendingChange string format");
    }

    long timestamp;
    try
    {
        timestamp = std::stol(timestampStr);
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Invalid timestamp in PendingChange string");
    }

    return PendingChange(username, field, newValue, otp, timestamp);
}

void PendingChange::overrideValue(const std::string &newValue, const std::string &otp, long timestamp)
{
    this->newValue = newValue;
    this->otp = otp;
    this->timestamp = timestamp;
}
