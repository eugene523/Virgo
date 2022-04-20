#include <iostream>
#include <fstream>
#include <sstream>
#include "Mem.h"
#include "None.h"
/*
#include "Bool.h"
#include "VM.h"
#include "Tokenizer.h"
#include "Parser.h"
*/

/*
void test() {
    VM::Init();
}

void runScript() {
    VM::Init();
    std::fstream f;
    //f.open(R"(C:\code\projects\Virgo\tests\for.v)");
    f.open(R"(C:\code\projects\Virgo\tests\first.v)");
    std::stringstream s;
    s << f.rdbuf();
    std::string sourceCode = s.str();
    Parser p;
    p.Interpret(sourceCode);
    VM::contextStack.Print();
    VM::PrintStatus();
}

void Mem2Test() {
    std::cout << sizeof(ObjHnd);
}
*/

int main() {
    None::InitType();

    auto * m = new MemDomain();
    m->PrintStatus("Just created.");

    for (int k = 0; k < 10; k++) {
        std::cout << "\nIteration " << k << "------------------------\n";

        for (int i = 0; i < 256; i++) {
            auto * obj = new None();
            Ref r = m->NewRef(obj);
        }
        m->PrintStatus("Populated.");

        m->CollectGarbage();
        m->PrintStatus("Garbage collected.");
    }
    return 0;
}