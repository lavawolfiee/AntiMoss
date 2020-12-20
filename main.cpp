#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "minimizer.h"
#include "obfuscator.h"

int main() {
    srand(time(nullptr));

    std::ifstream fin("input.txt");

    std::string s((std::istreambuf_iterator<char>(fin)),
                  (std::istreambuf_iterator<char>()));

    fin.close();

    s = Minimizer::minimize(s);
    int readability = 0;
    int code_style = 0;
    std::vector<Obfuscator*> obfuscators;
    // obfuscators.push_back(new IntToLongLong());
    obfuscators.push_back(new Defines());

    for (const Obfuscator* obf : obfuscators) {
        s = obf->apply(s, readability, code_style);
    }

    std::ofstream fout("output.txt");
    fout << s;
    fout.close();

    return 0;
}