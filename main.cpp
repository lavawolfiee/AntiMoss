#include <iostream>
#include "minimizer.h"
#include "obfuscator.h"

int main() {
    std::string s = "#include <iostream>\n"
                    "#include <vector>\n"
                    "\n"
                    "int main() {\n"
                    "  int a = 3;\n"
                    "  std::vector<int> b;\n"
                    "  Tree<int, int, std::string, int>;\n"
                    "}";

    s = Minimizer::minimize(s);
    int readability = 0;
    int code_style = 10;
    std::vector<Obfuscator*> obfuscators;
    obfuscators.push_back(new IntToLongLong());

    for (const Obfuscator* obf : obfuscators) {
        s = obf->apply(s, readability, code_style);
    }

    std::cout << s << '\n';

    return 0;
}