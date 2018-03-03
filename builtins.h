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


    ElementPtr GetElement(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    ObjectPtr GetObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    StringPtr GetString(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    IOObject * GetIOObject(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    FunctionDefinitionPtr GetFunctionDefinition(Machine& machine, std::vector<ElementPtr>& args, size_t index, int& errorCode);
    ElementPtr CallFunction(Machine& machine, FunctionDefinitionPtr funcDef, size_t nItemsOnStack);
}
