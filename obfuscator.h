#ifndef ANTIMOSS_OBFUSCATOR_H
#define ANTIMOSS_OBFUSCATOR_H

#include <string>
#include <vector>
#include <regex>
#include <cstdlib>
#include <map>
#include <algorithm>
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

class Defines: public Obfuscator {
private:
    static std::string random_string();
    static void replaceAll(std::string& s, const std::string& from, const std::string& to);
    static bool good_quotes(const std::string& s);

public:
    std::string name = "Defines";
    std::string description = "Creates defines from random pieces of code."
                              "Code becomes unreadable";
    double replace_probability = 0.5;

    explicit Defines(double replace_probability=0.5): replace_probability(replace_probability) {}

    std::string apply(const std::string &_s, int readability, int code_style) const override;
};

#endif //ANTIMOSS_OBFUSCATOR_H
