#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    ElementPtr Print(Machine&, std::vector<ElementPtr>&);

    /********* List *********/
    ElementPtr Part(Machine&, std::vector<ElementPtr>&);

    /******** Error *********/
    ElementPtr IsError(Machine&, std::vector<ElementPtr>&);
    ElementPtr ErrorString(Machine&, std::vector<ElementPtr>&);


    /********** IO ********/
    ElementPtr MakeIO(Machine&, std::vector<ElementPtr>&);
    ErrorPtr ReadFile(Machine&, std::vector<ElementPtr>&);
    ErrorPtr SetReadHandler(Machine&, std::vector<ElementPtr>&);


    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);
}
