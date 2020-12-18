#include "minimizer.h"

// trim from start (in place)
inline void Minimizer::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void Minimizer::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void Minimizer::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string Minimizer::remove_double_spaces(const std::string& s) {
    std::string result;
    int space_row = 0;
    for (char i : s) {
        if (std::isspace(i)) {
            ++space_row;
        } else {
            space_row = 0;
        }
        if (space_row <= 1) {
            result += i;
        }
    }

    return result;
}

std::string Minimizer::minimize(const std::string& s, bool indents, bool unnecessary_new_lines,
                                bool remove_comments, bool double_spaces) {
    std::vector<std::string> lines;
    int last = 0;

    for (int i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == '\n') {
            lines.push_back(s.substr(last, i - last));
            last = i + 1;
        } else if (i < s.size() - 2 && (s[i] == '/' && s[i + 1] == '*' ||
                                        s[i] == '/' && s[i + 1] == '/')) {
            if (i != last)
                lines.push_back(s.substr(last, i - last));
            last = i;
        } else if (i < s.size() - 2 && (s[i] == '*' && s[i + 1] == '/')) {
            lines.push_back(s.substr(last, i - last + 2));
            last = i + 2;
        }
    }

    if (indents) {
        for (std::string& l : lines)
            trim(l);
    }

    if (double_spaces)
        for (std::string& l : lines)
            remove_double_spaces(l);

    if (remove_comments) {
        bool in_comment = false;
        for (auto it = lines.begin(); it != lines.end(); ++it) {
            std::string& l = *it;
            if (l.length() >= 2 && l[0] == '/' && l[1] == '*') {
                in_comment = true;
            }

            if (in_comment) {
                if (l.length() >= 2 && l[l.length() - 2] == '*' && l[l.length() - 1] == '/')
                    in_comment = false;
                auto next_it = lines.erase(it);
                it = std::prev(next_it);
            } else if (l.length() >= 2 && l[0] == '/' && l[1] == '/') {
                auto next_it = lines.erase(it);
                it = std::prev(next_it);
            }
        }
    }

    int empty_row = 0;
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        if (it->empty()) {
            ++empty_row;
            if (empty_row >= 3) {
                auto new_it = lines.erase(it);
                it = std::prev(new_it);
            }
        } else {
            empty_row = 0;
        }
    }

    if (unnecessary_new_lines) {
        for (int i = 0; i < lines.size(); ++i) {
            std::string& l = lines.at(i);
            if (l.length() == 0) continue;
            if (l.length() > 0 && l[l.size() - 1] == '\\')
                l.erase(l.size() - 1, 1);
            char last_char = l.at(l.size() - 1);

            if (l[0] != '#' && last_char != ';' && last_char != '{' && last_char != '}') {
                if (i == lines.size() - 1) continue;
                std::string& next_line = lines.at(i + 1);
                if (next_line.length() > 0 && next_line[0] == '{') {
                    l += " {";
                    next_line.erase(0, 1);
                    trim(next_line);
                    if (next_line.length() == 0) {
                        lines.erase(lines.begin() + i + 1);
                    }
                } else if (next_line.length() > 0 && l.length() > 0 &&
                        next_line[0] == '"' && l[l.size() - 1] == '"') {
                    l.erase(l.size() - 1, 1);
                    next_line.erase(0, 1);
                    l += next_line;
                    lines.erase(lines.begin() + i + 1);
                    --i;
                } else if (next_line.length() > 0 && l.length() > 0 &&
                           next_line[0] == '+' && l[l.size() - 1] == '+') {
                    next_line.erase(0, 1);
                    trim(next_line);
                    l += " ";
                    l += next_line;
                    lines.erase(lines.begin() + i + 1);
                    --i;
                } else {
                    l += " ";
                    l += next_line;
                    lines.erase(lines.begin() + i + 1);
                    --i;
                }
            }
        }
    }

    return join(lines);
}

std::string Minimizer::join(const std::vector<std::string>& v, const char d) {
    std::string result;
    for (int i = 0; i < v.size(); ++i) {
        result += v.at(i);
        if (i < v.size() - 1) result += d;
    }
    return result;
}

std::vector<std::string> Minimizer::split_into_lines(const std::string& s) {
    std::vector<std::string> lines;
    int last = 0;

    for (int i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == '\n') {
            lines.push_back(s.substr(last, i - last));
            last = i + 1;
        }
    }

    return lines;
}