#ifndef ANTIMOSS_MINIMIZER_H
#define ANTIMOSS_MINIMIZER_H

#include <string>
#include <vector>
#include <iterator>

class Minimizer {
public:
    Minimizer() = default;
    static std::string minimize(const std::string&, bool indents=true, bool unnecessary_new_lines=true,
                                bool remove_comments=true, bool double_spaces=true);
    static inline void ltrim(std::string& s);
    static inline void rtrim(std::string& s);
    static inline void trim(std::string& s);
    static std::string remove_double_spaces(const std::string& s);
    static std::string join(const std::vector<std::string>& v, char d = '\n');
    static std::vector<std::string> split_into_lines(const std::string& s);
};

#endif //ANTIMOSS_MINIMIZER_H
