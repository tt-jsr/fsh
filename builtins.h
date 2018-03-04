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


    /********** IO ********/
    ElementPtr MakeIO(Machine&, std::vector<ElementPtr>&);
    // ReadFile[func, "filename"];
    //      func is defined as &[line : body...];
    ErrorPtr ReadFile(Machine&, std::vector<ElementPtr>&);
    ErrorPtr SetReadHandler(Machine&, std::vector<ElementPtr>&);



    // Throw["message"];
    ElementPtr Throw(Machine&, std::vector<ElementPtr>&);



    /***********************************************
     * Helpers for built in functions
     */
    enum
    {
        GET_OK
        , GET_NOT_ENOUGH_ARGUMENTS
        , GET_NOT_EXPECTED_TYPE
        , GET_VARIABLE_NOT_FOUND
    };

    class IOObject : public ObjectBase
    {
    public:
        IOObject()
        :fpIn(nullptr)
        ,fpOut(nullptr)
        {}

        ~IOObject()
        {
            if (fpIn)
                fclose(fpIn);
            if (fpOut)
                fclose(fpOut);
        }

        FunctionDefinitionPtr readHandler;
        FILE *fpIn;
        FILE *fpOut;
    };

    static const uint64_t ioobject_magic = 0x7e89ab46;

    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);

    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    IOObject * GetIOObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);
}
