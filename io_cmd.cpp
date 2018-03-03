#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    ElementPtr MakeIO(Machine& machine, std::vector<ElementPtr>& args)
    {
        ObjectPtr op = MakeObject(new IOObject(), ioobject_magic);
        return op;
    }

    ErrorPtr SetReadHandler(Machine& machine, std::vector<ElementPtr>& args)
    {
        int errorCode;

        IOObject *pObject = GetIOObject(machine, args, 0, errorCode);
        if (pObject == nullptr)
        {
            return MakeError("Expected IO object", false);
        }

        FunctionDefinitionPtr fd = GetFunctionDefinition(machine, args, 1, errorCode);
        if (fd.get() == nullptr)
        {
            ErrorPtr e = MakeError("Expected function", false);
            return e;
        }

        pObject->readHandler = fd;
        return MakeError("ok", true);
    }

    ErrorPtr ReadFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        int errorCode;
        FunctionDefinitionPtr fd = GetFunctionDefinition(machine, args, 0, errorCode);
        if (fd.get() == nullptr)
        {
            ErrorPtr e = MakeError("Expected function", false);
            return e;
        }

        StringPtr sp = GetString(machine, args, 1, errorCode);
        if (sp.get() == nullptr)
            return MakeError("Expected filename", false);
        FILE *fp = fopen(sp->value.c_str(), "r");
        if (fp == nullptr)
        {
            return MakeError("Unable to open file", false);
        }
        char buffer[1024];
        while (true)
        {
            if (nullptr == fgets(buffer, sizeof(buffer), fp))
            {
                fclose(fp);
                return MakeError("success", true);
            }
            int len = strlen(buffer);
            if (buffer[len-1] ==  '\n')
                buffer[len-1] = '\0';
            ElementPtr e = MakeString(buffer);
            machine.push_data(e);
            try
            {
                ElementPtr r = CallFunction(machine, fd, 1);
                bool b = machine.ConvertToBool(r);
                if (b == false)
                {
                    fclose(fp);
                    return MakeError("", true);
                }
            }
            catch (std::exception)
            {
                fclose(fp);
                return MakeError("Exception caught", false);
            }
        }
        fclose(fp);
        return MakeError("success", true);
    }
}

