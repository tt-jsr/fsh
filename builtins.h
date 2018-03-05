#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    //Print[arg1, srg2, ...];
    ElementPtr Print(Machine&, std::vector<ElementPtr>&);

    /********* List *********/
    // Part[list, startidx];
    // Part[list, startidx, endidx];
    ElementPtr Part(Machine&, std::vector<ElementPtr>&);

    /******** Error *********/
    ElementPtr IsError(Machine&, std::vector<ElementPtr>&);
    ElementPtr ErrorString(Machine&, std::vector<ElementPtr>&);
    // Throw["message"];
    ElementPtr Throw(Machine&, std::vector<ElementPtr>&);


    /********** IO ********/

    // OpenFile["filename", "r"];
    FileHandlePtr OpenFile(Machine&, std::vector<ElementPtr>&);
    ElementPtr PipeLine(Machine&, std::vector<ElementPtr>&);

    // ReadFile[func, "filename"];
    //      func is defined as &[line : body...];
    ErrorPtr ReadFile(Machine&, std::vector<ElementPtr>&);

    /***********************************************
     * Helpers for built in functions
     */

    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    IntegerPtr GetInteger(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);
}
