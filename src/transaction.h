
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>

class Transaction
{
private:
    std::string transactionID;
    std::string sender;
    std::string receiver;
    double amount;
    std::string timestamp;
    std::string status;
    double senderBalanceBefore;
    double receiverBalanceBefore;

public:
    static std::string begin;
    static std::string commit;
    static std::string failed;

    Transaction(const std::string &transactionID,
                const std::string &sender,
                const std::string &receiver,
                double amount,
                const std::string &timestamp,
                const std::string &status,
                double senderBalanceBefore,
                double receiverBalanceBefore);

    Transaction(const std::string &sender,
                const std::string &receiver,
                double amount,
                const std::string &timestamp,
                const std::string &status,
                double senderBalanceBefore,
                double receiverBalanceBefore);

    std::string getTransactionID() const;
    std::string getSender() const;
    std::string getReceiver() const;
    double getAmount() const;
    std::string getTimestamp() const;
    std::string getStatus() const;
    double getSenderBalanceBefore() const;
    double getReceiverBalanceBefore() const;
    bool isBegin() const;
    std::string getPrintTime() const;

    void setStatus(std::string status);

    /// @brief Chuyển sang dữ liệu text, dùng cho lưu file
    /// @return
    std::string toString() const;

    static Transaction fromString(const std::string &str);
};

#endif
