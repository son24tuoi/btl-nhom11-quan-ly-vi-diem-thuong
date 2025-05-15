#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace Utils
{
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    std::string hashPassword(const std::string &input);
    void generateRandomPassword(std::string &password);


}

#endif