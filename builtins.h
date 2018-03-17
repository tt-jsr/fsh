#pragma once
#include <vector>

namespace fsh
{
    class Machine;

    void RegisterBuiltIns(Machine& machine);

    //Print[arg1, srg2, ...];
    ElementPtr Print(Machine&, std::vector<ElementPtr>&);
    ElementPtr ToInt(Machine&, std::vector<ElementPtr>&);
    ElementPtr ToFloat(Machine&, std::vector<ElementPtr>&);
    ElementPtr ToString(Machine&, std::vector<ElementPtr>&);
    ElementPtr Eval(Machine&, std::vector<ElementPtr>&);
    ElementPtr Bind(Machine&, std::vector<ElementPtr>&);

    /********* List *********/
    // Part[list, startidx];
    // Part[list, startidx, endidx];
    ElementPtr Part(Machine&, std::vector<ElementPtr>&);
    ElementPtr Subscript(Machine&, std::vector<ElementPtr>&);
    ElementPtr DefineRecord(Machine&, std::vector<ElementPtr>&);
    ElementPtr MakeRecord(Machine&, std::vector<ElementPtr>&);
    IntegerPtr Len(Machine&, std::vector<ElementPtr>&);
    ElementPtr Append(Machine&, std::vector<ElementPtr>&);
    ListPtr SetRecordType(Machine&, std::vector<ElementPtr>&);

    /******** Error *********/
    ElementPtr IsError(Machine&, std::vector<ElementPtr>&);
    ElementPtr ErrorString(Machine&, std::vector<ElementPtr>&);
    // Throw["message"];
    ElementPtr Throw(Machine&, std::vector<ElementPtr>&);


    /********** IO ********/

    // OpenFile["filename", "r"];
    FileHandlePtr OpenFile(Machine&, std::vector<ElementPtr>&);

    // OpenProcess["filename", "r"];
    FileHandlePtr OpenProcess(Machine&, std::vector<ElementPtr>&);
    ElementPtr PipeLine(Machine&, std::vector<ElementPtr>&);

    // ReadFile[func, "filename"];
    //      func is defined as &[line : body...];
    //      __stripnl=True to strip the ending newline
    ElementPtr ReadFile(Machine&, std::vector<ElementPtr>&);

    //ReadProcess[func, "processname", "r" or "w"];
    ElementPtr ReadProcess(Machine&, std::vector<ElementPtr>&);
    ElementPtr WriteProcess(Machine&, std::vector<ElementPtr>&);

    /*********** String ************/
    ElementPtr Trim(Machine&, std::vector<ElementPtr>&);
    ElementPtr TrimLeft(Machine&, std::vector<ElementPtr>&);
    ElementPtr TrimRight(Machine&, std::vector<ElementPtr>&);
    // Split["", chars];
    ElementPtr Split(Machine&, std::vector<ElementPtr>&);
    ElementPtr Strcmp(Machine&, std::vector<ElementPtr>&);
    ElementPtr RegMatch(Machine&, std::vector<ElementPtr>&);
    ElementPtr RegSearch(Machine&, std::vector<ElementPtr>&);
    ElementPtr Find(Machine&, std::vector<ElementPtr>&);

    /***********************************************
     * Helpers for built in functions
     */

    ElementPtr UnitTest(Machine&, std::vector<ElementPtr>&);

    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ListPtr GetList(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    IntegerPtr GetInteger(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    IdentifierPtr GetIdentifier(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index);
    ElementPtr CallFunctionImpl(Machine& machine, bool isBind, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);
    std::string toString(Machine& machine, ElementPtr e);
}
