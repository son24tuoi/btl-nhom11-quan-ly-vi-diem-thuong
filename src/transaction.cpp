
#include "transaction.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

string Transaction::begin = "BEGIN";
string Transaction::commit = "COMMIT";
string Transaction::failed = "FAILED";

Transaction::Transaction(const std::string &transactionID, const std::string &sender,
                         const std::string &receiver, double amount, const std::string &timestamp,
                         const std::string &status, double senderBalanceBefore, double receiverBalanceBefore)
    : transactionID(transactionID), sender(sender), receiver(receiver),
      amount(amount), timestamp(timestamp), status(status),
      senderBalanceBefore(senderBalanceBefore), receiverBalanceBefore(receiverBalanceBefore)
{
}

Transaction::Transaction(const std::string &sender, const std::string &receiver,
                         double amount, const std::string &timestamp, const std::string &status,
                         double senderBalanceBefore, double receiverBalanceBefore)
    : sender(sender), receiver(receiver), amount(amount), timestamp(timestamp), status(status),
      senderBalanceBefore(senderBalanceBefore), receiverBalanceBefore(receiverBalanceBefore)
{
    transactionID = sender + "_" + receiver + "_" + timestamp;
}

std::string Transaction::getTransactionID() const
{
    return transactionID;
}

string Transaction::getSender() const
{
    return sender;
}

string Transaction::getReceiver() const
{
    return receiver;
}

double Transaction::getAmount() const
{
    return amount;
}

string Transaction::getTimestamp() const
{
    return timestamp;
}

string Transaction::getStatus() const
{
    return status;
}

double Transaction::getSenderBalanceBefore() const
{
    return senderBalanceBefore;
}

double Transaction::getReceiverBalanceBefore() const
{
    return receiverBalanceBefore;
}

bool Transaction::isBegin() const
{
    return status == begin;
}

std::string Transaction::getPrintTime() const
{
    try
    {
        time_t rawTime = static_cast<time_t>(std::stoll(timestamp));
        std::tm timeInfo;
        if (localtime_r(&rawTime, &timeInfo) == nullptr)
        {
            return "Invalid time";
        }
        std::ostringstream oss;
        oss << std::put_time(&timeInfo, "%c");
        return oss.str();
    }
    catch (const std::exception &)
    {
        return "Invalid timestamp";
    }
}

void Transaction::setStatus(std::string status)
{
    this->status = status;
}

string Transaction::toString() const
{
    stringstream ss;
    ss << transactionID << "|" << sender << "|" << receiver << "|"
       << amount << "|" << timestamp << "|" << status << "|"
       << senderBalanceBefore << "|" << receiverBalanceBefore;
    return ss.str();
}

Transaction Transaction::fromString(const string &str)
{
    if (str.empty())
        throw invalid_argument("Input string cannot be empty");

    stringstream ss = stringstream(str);
    string transactionID, sender, receiver, amountStr, timestamp, status, senderBalanceStr, receiverBalanceStr;

    getline(ss, transactionID, '|');
    getline(ss, sender, '|');
    getline(ss, receiver, '|');
    getline(ss, amountStr, '|');
    getline(ss, timestamp, '|');
    getline(ss, status, '|');
    getline(ss, senderBalanceStr, '|');
    getline(ss, receiverBalanceStr, '|');

    if (transactionID.empty() || sender.empty() || receiver.empty() || amountStr.empty() ||
        timestamp.empty() || status.empty() || senderBalanceStr.empty() || receiverBalanceStr.empty())
        throw invalid_argument("Information cannot be empty");

    double amount;
    try
    {
        amount = stod(amountStr);
    }
    catch (const exception &e)
    {
        throw invalid_argument("Invalid amount format");
    }
    if (amount < 0)
    {
        throw invalid_argument("Amount cannot be negative");
    }

    double senderBalanceBefore;
    try
    {
        senderBalanceBefore = stod(senderBalanceStr);
    }
    catch (const exception &e)
    {
        throw invalid_argument("Invalid Sender Balance format");
    }
    if (senderBalanceBefore < 0)
    {
        throw invalid_argument("Sender Balance cannot be negative");
    }

    double receiverBalanceBefore;
    try
    {
        receiverBalanceBefore = stod(receiverBalanceStr);
    }
    catch (const exception &e)
    {
        throw invalid_argument("Invalid Receiver Balance format");
    }
    if (receiverBalanceBefore < 0)
    {
        throw invalid_argument("Receiver Balance cannot be negative");
    }

    return Transaction(transactionID, sender, receiver, amount, timestamp, status, senderBalanceBefore, receiverBalanceBefore);
}
