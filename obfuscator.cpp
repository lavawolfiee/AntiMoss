#include <iostream>
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

std::string Defines::random_string() {
    static std::string alphanum =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "_";
    static std::string alpha =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "_";

    int len = 9 + rand() % 11;
    std::string s(1, alpha[rand() % (alpha.length() - 1)]);
    s.reserve(len);

    for (int i = 0; i < len; ++i)
        s += alphanum[rand() % (alphanum.length() - 1)];

    return s;
}

void Defines::replaceAll(std::string& s, const std::string& from, const std::string& to) {
    size_t index = s.find(from);
    while (index != std::string::npos) {
        s.replace(index, from.length(), to);
        index = s.find(from, index + to.length());
    }
}

bool Defines::good_quotes(const std::string& s) {
    bool in_quotes = false;
    int quote_type = 0; // 0 - ', 1 - "
    bool last_backslash = false;

    for (char i : s) {
        if (i == '\\') {
            last_backslash = true;
            continue;
        }

        if (i == '\'' || i == '"') {
            int current_quote_type = 0;
            if (i == '"') current_quote_type = 1;
            if (!in_quotes) {
                if (last_backslash) return false;
                in_quotes = true;
                quote_type = current_quote_type;
            } else if (current_quote_type == quote_type && !last_backslash)
                in_quotes = false;
        }

        last_backslash = false;
    }

    if (in_quotes || last_backslash) return false;
    return true;
}

std::string Defines::apply(const std::string& _s, int readability, int code_style) const {
    if (readability <= 2 && code_style <= 2) {
        std::string s = _s;
        std::regex re(R"(([^\w"'])([^:<>'"\-&|=^*+\\][^'"]{13,28}[^:<>'"\-&|=^*+\\])([^\w"']))");

        std::vector<std::string> lines = Minimizer::split_into_lines(s);
        std::vector<std::tuple<std::string, std::string, std::string, std::string>> defines; // (replaced string, macro name)
        for (const std::string& l : lines) {
            if (l == "std::tie(l, first->left) = split_key_(first->left, key, comp);")
                std::cout << "";
            if (l.length() > 0 && l.at(0) != '#') {
                for (std::sregex_iterator i = std::sregex_iterator(l.begin(), l.end(), re);
                     i != std::sregex_iterator(); ++i) {
                    std::smatch m = *i;
                    std::string prefix = m.prefix();
                    std::string match2 = m.str(2);
                    if (good_quotes(prefix) && good_quotes(match2)
                    && (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) <= replace_probability)) {
                        std::string rand_str = random_string();
                        defines.emplace_back(match2, rand_str,
                                             m.str(0), m.str(1) + rand_str + m.str(3));
                    }
                }
            }
        }

        for (const auto& [a, b, c, d] : defines) {
            replaceAll(s, c, d);
        }

        lines = Minimizer::split_into_lines(s);
        auto it = lines.begin();
        while (it != lines.end() && it->length() != 0 && it->at(0) == '#')
            ++it;

        std::vector<std::string> str_defines;
        str_defines.reserve(defines.size());
        for (const auto& [a, b, c, d] : defines) {
            std::string def = "#define ";
            def += b;
            def += " ";
            def += a;
            str_defines.push_back(def);
        }

        lines.insert(it, str_defines.begin(), str_defines.end());

        return Minimizer::join(lines);
    } else {
        return _s;
    }
}