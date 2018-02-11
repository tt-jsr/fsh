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

    instruction::Instruction *inst = parser.parse("22 + 13");
    assert(inst);
    inst->Execute(m);
}
