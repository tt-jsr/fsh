#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    FileHandlePtr OpenFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr filename = GetString(machine, args, 0);
        if (filename.get() == nullptr)
        {
            throw "Openfile: arg0 is not a string";
        }
        StringPtr mode = GetString(machine, args, 1);
        if (mode.get() == nullptr)
        {
            throw std::runtime_error("Openfile: arg1 is not a string");
        }
        FileHandlePtr fh = MakeFileHandle();
        if (mode->value == "r" || mode->value == "rw")
            fh->bRead = true;
        fh->fp = fopen(filename->value.c_str(), mode->value.c_str());
        if (fh->fp == nullptr)
        {
            std::stringstream strm;
            strm << "Openfile: cannot open " << filename->value;
            throw std::runtime_error(strm.str());
        }
        return fh;
    }

    ErrorPtr ReadFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl;
        machine.get_variable("__stripnl", stripnl);

        FunctionDefinitionPtr fd = GetFunctionDefinition(machine, args, 0);
        if (fd.get() == nullptr)
        {
            ErrorPtr e = MakeError("Expected function", false);
            return e;
        }

        StringPtr sp = GetString(machine, args, 1);
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
            if (stripnl)
            {
                int len = strlen(buffer);
                if (buffer[len-1] ==  '\n')
                    buffer[len-1] = '\0';
            }
            ElementPtr e = MakeString(buffer);
            machine.push_data(e);
            try
            {
                machine.push_context();
                ElementPtr r = CallFunctionImpl(machine, fd, 1);
                machine.pop_context();
                bool b = machine.ConvertToBool(r);
                if (b == false)
                {
                    fclose(fp);
                    return MakeError("", true);
                }
            }
            catch (std::exception)
            {
                machine.pop_context();
                fclose(fp);
                return MakeError("Exception caught", false);
            }
        }
        fclose(fp);
        return MakeError("success", true);
    }

    ElementPtr PipelineStage(Machine& machine, ElementPtr stage, ElementPtr data)
    {
                    //std::cout << "stage" << std::endl;
        stage = machine.resolve(stage);
        switch (stage->type())
        {
        case ELEMENT_TYPE_FUNCTION_DEFINITION:
            {
                    //std::cout << "stage function" << std::endl;
                FunctionDefinitionPtr func = stage.cast<FunctionDefinition>();

                machine.push_data(data);
                machine.push_context();
                try
                {
                    ElementPtr rtn =  CallFunctionImpl(machine, func, 1);
                    machine.pop_context();
                    return rtn;
                }
                catch (std::exception e)
                {
                    machine.pop_context();
                    throw;
                }
            }
            break;
        case ELEMENT_TYPE_FILE_HANDLE:
            {
                    //std::cout << "stage file" << std::endl;
                FileHandlePtr file = stage.cast<FileHandle>();
                
                if (file->bRead)
                {
                    char buffer[1024];
                    if (nullptr == fgets(buffer, sizeof(buffer), file->fp))
                    {
                        fclose(file->fp);
                        file->fp = nullptr;
                        return MakeBoolean(false);
                    }
                    //std::cout << buffer << std::endl;
                    return MakeString(buffer);
                }
                else
                {
                    if (data->IsString())
                    {
                        StringPtr sp = data.cast<String>();
                        fputs(sp->value.c_str(), file->fp);
                        return data;
                    }
                }
            }
            break;
        default:
            {
                std::stringstream strm;
                strm << "Pipeline: unsupported element: " << stage->type();
                throw std::runtime_error(strm.str());
            }
            break;
        }
        return MakeBoolean(false);
    }

    ElementPtr PipeLine(Machine& machine, std::vector<ElementPtr>& args)
    {
        size_t idx = 0;
        ElementPtr data;
        while (true)
        {

                    //std::cout << "pipeline" << std::endl;
            data = PipelineStage(machine, args[idx], data);
            if (data->IsBoolean())
            {
                if (data.cast<Boolean>()->value == false)
                    return data;
                else
                    idx = args.size();
            }
            ++idx;
            if (idx >= args.size())
                idx = 0;
        }
        return MakeNone();
    }
}

