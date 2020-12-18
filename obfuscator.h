#ifndef ANTIMOSS_OBFUSCATOR_H
#define ANTIMOSS_OBFUSCATOR_H

#include <string>
#include <vector>
#include <regex>
#include "minimizer.h"

class Obfuscator {
public:
    std::string name;
    std::string description;

    virtual std::string apply(const std::string& s, int readability, int code_style) const = 0;
};

class IntToLongLong: public Obfuscator {
public:
    std::string name = "IntToLongLong";
    std::string description = "Replaces all 'int' with 'long long' or "
                              "with 'll' (and defines type ll = long long)"
                              "if code_style >= 6";

    std::string apply(const std::string &_s, int readability, int code_style) const override;
};

#endif //ANTIMOSS_OBFUSCATOR_H
