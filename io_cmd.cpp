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
        bool stripnl;
        machine.get_variable("__stripnl", stripnl);

        StringPtr filename = GetString(machine, args, 0);
        if (filename.get() == nullptr)
        {
            throw "Openfile: process name is not a string";
        }
        StringPtr mode = GetString(machine, args, 1);
        if (mode.get() == nullptr)
        {
            throw std::runtime_error("OpenProcess: mode is not a string");
        }
        FileHandlePtr fh = MakeFileHandle();
        if (mode->value == "r" || mode->value == "rw")
            fh->bRead = true;
        else
            fh->bRead = false;
        fh->fp = fopen(filename->value.c_str(), mode->value.c_str());
        if (fh->fp == nullptr)
        {
            std::stringstream strm;
            strm << "Openfile: cannot open " << filename->value;
            throw std::runtime_error(strm.str());
        }
        fh->stripnl= stripnl;
        return fh;
    }

    FileHandlePtr OpenProcess(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl(false);
        bool addnl(false);
        machine.get_variable("stripnl", stripnl);
        machine.get_variable("addnl", addnl);

        StringPtr processName = GetString(machine, args, 0);
        if (processName.get() == nullptr)
        {
            throw "OpenProcess: process name is not a string";
        }
        StringPtr mode = GetString(machine, args, 1);
        if (mode.get() == nullptr)
        {
            throw std::runtime_error("Openfile: mode is not a string");
        }
        FileHandlePtr fh = MakeFileHandle();
        if (mode->value == "r" || mode->value == "rw")
            fh->bRead = true;
        else
            fh->bRead = false;
        fh->isPipe = true;
        fh->fp = popen(processName->value.c_str(), mode->value.c_str());
        if (fh->fp == nullptr)
        {
            std::stringstream strm;
            strm << "OpenProcess: cannot open " << processName->value;
            throw std::runtime_error(strm.str());
        }
        fh->stripnl = stripnl;
        fh->addnl = addnl;
        return fh;
    }

    ElementPtr ReadFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl;
        machine.get_variable("stripnl", stripnl);

        FunctionDefIdPtr fdid = GetFunctionDefId(machine, args, 0);
        if (fdid.get() == nullptr)
        {
            throw std::runtime_error("ReadFile: Expected function for arg 0");
        }

        FunctionDefinition *fd = machine.getFunction(fdid->funcid);
        if (fd == nullptr)
            throw std::runtime_error("Function not found");

        StringPtr sp = GetString(machine, args, 1);
        if (sp.get() == nullptr)
            throw std::runtime_error("ReadFile: Expected filename for arg 2");

        FILE *fp = nullptr;
        if (sp->value == "stdin")
            fp = stdin;
        else
        {
            fp = fopen(sp->value.c_str(), "r");
            if (fp == nullptr)
            {
                std::stringstream strm;
                strm << "ReadFile: Unable to open " << sp->value;
                throw std::runtime_error(strm.str());
            }
        }
        char buffer[1024];
        while (true)
        {
            if (nullptr == fgets(buffer, sizeof(buffer), fp))
            {
                fclose(fp);
                return MakeNone();
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
                ElementPtr r = fd->Call(machine, 1);
                if (r->IsBoolean() && r.cast<Boolean>()->value == false)
                {
                    fclose(fp);
                    return MakeNone();
                }
            }
            catch (std::exception)
            {
                fclose(fp);
                throw;
            }
        }
        fclose(fp);
        return MakeNone();
    }

    ElementPtr ReadProcess(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl;
        machine.get_variable("stripnl", stripnl);

        FunctionDefIdPtr fdid = GetFunctionDefId(machine, args, 0);
        if (fdid.get() == nullptr)
        {
            throw std::runtime_error("ReadProcess: Expected function for arg 0");
        }

        FunctionDefinition *fd = machine.getFunction(fdid->funcid);
        if (fd == nullptr)
            throw std::runtime_error("Function not found");

        StringPtr sp = GetString(machine, args, 1);
        if (sp.get() == nullptr)
            throw std::runtime_error("ReadProcess: Expected process name for arg 2");

        FILE *fp = popen(sp->value.c_str(), "r");
        if (fp == nullptr)
        {
            std::stringstream strm;
            strm << "ReadProcess: Unable to open " << sp->value;
            throw std::runtime_error(strm.str());
        }
        char buffer[1024];
        while (true)
        {
            if (nullptr == fgets(buffer, sizeof(buffer), fp))
            {
                pclose(fp);
                return MakeNone();
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
                ElementPtr r = fd->Call(machine, 1);
                if (r->IsBoolean() && r.cast<Boolean>()->value == false)
                {
                    pclose(fp);
                    return MakeNone();
                }
            }
            catch (std::exception)
            {
                pclose(fp);
                throw;
            }
        }
        pclose(fp);
        return MakeNone();
    }


    ElementPtr PipelineHead(Machine& machine, ElementPtr stage, size_t& listIdx)
    {
        stage = machine.resolve(stage);
        switch (stage->type())
        {
        case ELEMENT_TYPE_LIST:
            {
                ListPtr lp = stage.cast<List>();
                if (listIdx == lp->items.size())
                    return MakeBoolean(false);
                return lp->items[listIdx++];
            }
            break;
        case ELEMENT_TYPE_FUNCTION_DEF_ID:
            {
                    //std::cout << "stage function" << std::endl;
                FunctionDefIdPtr fdid = stage.cast<FunctionDefId>();
                int64_t funcid = fdid->funcid;
                FunctionDefinition *fd = machine.getFunction(fdid->funcid);
                assert(fd);
                machine.push_context();
                return fd->Call(machine, 0);
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
                        if (file->isPipe)
                            pclose(file->fp);
                        else
                            fclose(file->fp);
                        file->fp = nullptr;
                        return MakeBoolean(false);
                    }
                    //std::cout << buffer << std::endl;
                    if (file->stripnl)
                    {
                        int len = strlen(buffer);
                        if (buffer[len-1] ==  '\n')
                            buffer[len-1] = '\0';
                    }
                    return MakeString(buffer);
                }
                else
                {
                    throw std::runtime_error("Pipeline head cannot be a writer");
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

    ElementPtr PipelineStage(Machine& machine, ElementPtr stage, ElementPtr data)
    {
        stage = machine.resolve(stage);
        switch (stage->type())
        {
        case ELEMENT_TYPE_FUNCTION_DEF_ID:
            {
                FunctionDefIdPtr fdid = stage.cast<FunctionDefId>();
                int64_t funcid = fdid->funcid;
                FunctionDefinition *fd = machine.getFunction(fdid->funcid);
                assert(fd);
                machine.push_context();
                machine.push_data(data);
                return fd->Call(machine, 1);
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
                        if (file->isPipe)
                            pclose(file->fp);
                        else
                            fclose(file->fp);
                        file->fp = nullptr;
                        return MakeBoolean(false);
                    }
                    //std::cout << buffer << std::endl;
                    if (file->stripnl)
                    {
                        int len = strlen(buffer);
                        if (buffer[len-1] ==  '\n')
                            buffer[len-1] = '\0';
                    }
                    return MakeString(buffer);
                }
                else
                {
                    if (data->IsString())
                    {
                        StringPtr sp = data.cast<String>();
                        fputs(sp->value.c_str(), file->fp);
                        if (file->addnl)
                            fputs("\n", file->fp);
                    }
                    return data;
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

    void ClosePipeLine(Machine& machine, std::vector<ElementPtr>& args)
    {
        for (ElementPtr e : args)
        {
            if(e->IsFileHandle())
            {
                FileHandlePtr fh = e.cast<FileHandle>();
                if (fh->isPipe && fh->bRead == false)
                {
                    pclose(fh->fp);
                    fh->fp = nullptr;
                }
            }
        }
    }

    ElementPtr PipeLineImpl(Machine& machine, std::vector<ElementPtr>& args)
    {
        size_t idx = 0;
        ElementPtr data;
        size_t listIdx = 0;
        while (true)
        {
            if (idx == 0)
                data = PipelineHead(machine, args[idx], listIdx);
            else
                data = PipelineStage(machine, args[idx], data);
            if (data->IsBoolean())
            {
                if (data.cast<Boolean>()->value == false)
                    return MakeNone();
                else
                    idx = args.size();
            }
            ++idx;
            if (idx >= args.size())
                idx = 0;
        }
        return MakeNone();
    }

    ElementPtr PipeLine(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = PipeLineImpl(machine, args);
        ClosePipeLine(machine, args);
        return e;
    }
}

