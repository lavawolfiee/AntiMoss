#include "obfuscator.h"

std::string IntToLongLong::apply(const std::string& _s, int readability, int code_style) const {
    std::string s = _s;
    std::string long_long = "long long";

    if (code_style >= 6) {
        long_long = "ll";
        std::vector<std::string> lines = Minimizer::split_into_lines(s);
        std::vector<std::string> to_insert = {"", "typedef long long ll;", ""};

        auto it = lines.begin();
        while (it != lines.end() && (it->length() == 0 || it->at(0) == '#')) ++it;
        lines.insert(it, to_insert.begin(), to_insert.end());

        s = Minimizer::join(lines);
    }

    std::regex re("(int)(\\W)(?!main\())");
    s = std::regex_replace(s, re, long_long + "$2");
    return s;
}