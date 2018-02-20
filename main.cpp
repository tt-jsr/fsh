#include <iostream>
#include "common.h"
#include "list.h"
#include "machine.h"
#include "parser.h"
#include "instructions.h"

using namespace fsh;

void ListTest()
{
    ListPtr lst = MakeList(HEAD_TYPE_LIST);
    lst->Add("Hello");
    lst->Add("cruel");
    lst->Add("world");

    ListPtr args = MakeList(HEAD_TYPE_LIST);
    args->Add(-4);

    ListPtr l = Part(lst, args);

    Dump(l);
}

int main(int argc, char *argv[])
{
    Machine m;
    Parser parser;

    while(true)
    {
        std::string inbuf;
        std::cout << "> ";
        std::getline(std::cin, inbuf);
        parser.input_ = inbuf;
        parser.pos_ = 0;
        parser.tokenize();
        for (auto& t : parser.tokens_)
        {
            std::cout << t.text << std::endl;
        }
    }
}
