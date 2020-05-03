extern "C" bool FindListAsm(void*, const char*);
extern "C" unsigned int hash5(const char*);

#include <iostream>
#include "inc/hash.h"

enum class Type{
    ASM, 
    DEFAULT,
};

void Test(Type t, std::string filename = "hamlet.txt") {
    HashTable table((Type::ASM == t) ? hash5 : TestHash::hash5, 5000);
    std::ifstream input(filename);
    std::stringstream buff;
    buff << input.rdbuf();
    std::vector<std::string> str;
    std::string tmp;

    while (!buff.eof()) {
        tmp.clear();
        buff >> tmp;
        str.emplace_back(tmp);
    }

    for (std::string& el : str) {
        table.insert(el.c_str());
    }
    auto f = &HashTable::find_asm;
    if (t == Type::DEFAULT) {
        f = &HashTable::find;
    }
    for (int i = 0; i < 5000; i++) {
        for (std::string& el : str) {
            if (!(table.*f)(el.c_str())) {
                std::cout << "err";
            }
        }
    }
}


int main(int argc, char* argv[]) {

    if (argc == 2) {
        if (strcmp(argv[1], "plot") == 0) {
            TestHash::TestAllFunction("hamlet.txt");
        } else if (strcmp(argv[1], "asm") == 0) {
            Test(Type::ASM);
        } else {
            Test(Type::DEFAULT);
        }
    } else {
        Test(Type::DEFAULT);
    }
    return 0;
}