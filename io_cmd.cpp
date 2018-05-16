#include <vector>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <signal.h>
#include "common.h"
#include "element.h"
#include "machine.h"
#include "builtins.h"

namespace fsh
{
    StringPtr Input(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr s = GetString(machine, args, 0);
        if (s)
        {
            std::cout << s->value << std::flush;
        }
        std::string line;
        std::getline(std::cin, line);
        return MakeString(line);
    }

    FileHandlePtr OpenFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl(false);
        bool addnl(false);
        machine.get_variable("stripnl", stripnl);
        machine.get_variable("addnl", addnl);

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
        fh->addnl = addnl;
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

    ElementPtr WriteFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("WriteFile: requires file handle");
        if (e->IsFileHandle() == false)
            throw std::runtime_error("WriteFile: requires file handle");
        FileHandlePtr fh = e.cast<FileHandle>();
        if (fh->bRead)
            throw std::runtime_error("WriteFile: file handle is opened for reading");
        ElementPtr data = GetElement(machine, args, 1);
        if (!data)
            throw std::runtime_error("WriteFile: Requires data to be written");
        std::string s = toString(machine, data);

        fputs(s.c_str(), fh->fp);
        int64_t rtn = s.size();
        if (fh->addnl)
        {
            fputs("\n", fh->fp);
            rtn += 1;
        }
        return MakeInteger(rtn);
    }

    ElementPtr CloseFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        ElementPtr e = GetElement(machine, args, 0);
        if (!e)
            throw std::runtime_error("WriteFile: requires file handle");
        if (e->IsFileHandle() == false)
            throw std::runtime_error("WriteFile: requires file handle");
        FileHandlePtr fh = e.cast<FileHandle>();
        if (fh->isPipe)
            pclose(fh->fp);
        else
            fclose(fh->fp);
        fh->fp = nullptr;
        return MakeNone();
    }

    ElementPtr ReadFile(Machine& machine, std::vector<ElementPtr>& args)
    {
        bool stripnl(false);
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
        char buffer[10240];
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
        char buffer[10240];
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


    struct LoopContext
    {
        LoopContext()
        :listIndex(0)
        ,loopCount(0)
        {}

        size_t listIndex;
        ElementPtr mapKey;
        size_t loopCount;
    };

    ElementPtr PipelineHead(Machine& machine, ElementPtr stage, LoopContext& ctx)
    {
        PipeLineActionPtr plExit = MakePipeLineAction();
        plExit->action = PipeLineAction::EXIT;

        stage = machine.resolve(stage);
        switch (stage->type())
        {
        case ELEMENT_TYPE_LIST:
            {
                ListPtr lp = stage.cast<List>();
                if (ctx.listIndex == lp->items.size())
                {
                    return plExit;
                }
                return lp->items[ctx.listIndex++];
            }
            break;
        case ELEMENT_TYPE_MAP:
            {
                MapPtr m = stage.cast<Map>();
                if (!ctx.mapKey)
                {
                    auto it = m->map.begin();
                    if (it == m->map.end())
                        return plExit;
                    ctx.mapKey = it->first;
                    return MakePair(it->first, it->second);
                }
                auto it = m->map.find(ctx.mapKey);
                ++it;
                if (it == m->map.end())
                    return plExit;
                ctx.mapKey = it->first;
                return MakePair(it->first, it->second);
            }
            break;
            /*
        I have commented this out because there are two use cases.
        1. User passes in a function that knows how to iterator through it's
           data and return PIPELINE_EXIT when done.
        2. The user passes in a lambda or function that will repeatedly return
           the same data.
        It's this second case I don't know how it should be handled. I could 
        handle like I do a string, but then case #1 will be broken. What to do, what to do...
        */

        case ELEMENT_TYPE_FUNCTION_DEF_ID:
            {
                throw std::runtime_error("Function cannot be head of pipeline");
                /*
                machine.log() << "stage function" << std::endl;
                FunctionDefIdPtr fdid = stage.cast<FunctionDefId>();
                int64_t funcid = fdid->funcid;
                FunctionDefinition *fd = machine.getFunction(fdid->funcid);
                assert(fd);
                machine.push_context();
                return fd->Call(machine, 0);
                */
            }
            break;
        case ELEMENT_TYPE_FILE_HANDLE:
            {
                machine.log() << "stage file" << std::endl;
                FileHandlePtr file = stage.cast<FileHandle>();
                
                if (file->bRead)
                {
                    char buffer[10240];
                    if (nullptr == fgets(buffer, sizeof(buffer), file->fp))
                    {
                        if (file->isPipe)
                            pclose(file->fp);
                        else
                            fclose(file->fp);
                        file->fp = nullptr;
                        return plExit;
                    }
                    machine.log() << buffer << std::endl;
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
                if (ctx.loopCount > 0)
                    return plExit;
                std::string s = toString(machine, stage);
                return MakeString(s);
            }
            break;
        }
        return plExit;
    }

    ElementPtr PipelineStage(Machine& machine, ElementPtr stage, ElementPtr data)
    {
        PipeLineActionPtr plExit = MakePipeLineAction();
        plExit->action = PipeLineAction::EXIT;

        stage = machine.resolve(stage);
        switch (stage->type())
        {
        case ELEMENT_TYPE_STRING:
            {
                StringPtr sp = stage.cast<String>();
                sp->value += toString(machine, data);
                return data;
            }
            break;
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
        case ELEMENT_TYPE_LIST:
            {
                ListPtr lst = stage.cast<List>();
                lst->items.push_back(data);
                return data;
            }
            break;
        case ELEMENT_TYPE_MAP:
            {
                MapPtr m = stage.cast<Map>();
                if (data->IsPair())
                {
                    PairPtr pp = data.cast<Pair>();
                    m->map.emplace(pp->first, pp->second);
                }
                else
                    throw std::runtime_error("PipeLine: Map stage must receive a Pair");
                return data;
            }
            break;
        case ELEMENT_TYPE_FILE_HANDLE:
            {
                machine.log() << "stage file" << std::endl;
                FileHandlePtr file = stage.cast<FileHandle>();
                
                if (file->bRead)
                {
                    throw std::runtime_error("PipeLine: Cannot cannot have a read in a secondary stage of a pipeline");
                }
                else
                {
                    std::string s = toString(machine, data);
                    fputs(s.c_str(), file->fp);
                    if (ferror(file->fp))
                    {
                        machine.log() << "PipeLine: fputs error, stopping pipeline" << std::endl;
                        return plExit;
                    }
                    if (file->addnl)
                        fputs("\n", file->fp);
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
        return plExit;
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
        LoopContext ctx;
        size_t idx = 0;
        ElementPtr data;

        while (true)
        {
            machine.clear_stack();
            machine.log() << "pl: " << idx << ": Start of stage" << std::endl;
            if (idx == 0)
            {
                data = PipelineHead(machine, args[idx], ctx);
                // If we get a PipeLine action at the head (RESTART or EXIT), we should exit.
                if (data->IsPipeLineAction())
                {
                    return MakeNone();
                }
            }
            else
            {
                data = PipelineStage(machine, args[idx], data);
            }
            machine.log() << "pl: " << idx << ": Stage returned: " << toString(machine, data) << std::endl;
            if (data->IsPipeLineAction())
            {
                PipeLineActionPtr pla = data.cast<PipeLineAction>();
                if (pla->action == PipeLineAction::RESTART)
                    idx = args.size();
                if (pla->action == PipeLineAction::EXIT)
                    return MakeNone();
            }
            ++idx;
            if (idx >= args.size())
            {
                idx = 0;
                ++ctx.loopCount;
            }
        }
        return MakeNone();
    }

    ElementPtr PipeLine(Machine& machine, std::vector<ElementPtr>& args_)
    {
        std::vector<ElementPtr> args;
        for (ElementPtr e : args_)
        {
            if (e->IsString())
            {
                StringPtr sp = e.cast<String>();
                if (sp->value[0] == '>' && sp->value[1] == '>')
                {
                    FileHandlePtr fh = MakeFileHandle();
                    fh->fp = fopen(&sp->value[2], "a");
                    if (!fh->fp)
                    {
                        std::stringstream strm;
                        strm << "Pipeline: Cannot open " << sp->value;
                        throw std::runtime_error(strm.str());
                    }
                    fh->isPipe = false;
                    fh->bRead = false;
                    fh->addnl = true;
                    args.push_back(fh);
                }
                else if (sp->value[0] == '>')
                {
                    FileHandlePtr fh = MakeFileHandle();
                    fh->fp = fopen(&sp->value[1], "w");
                    if (!fh->fp)
                    {
                        std::stringstream strm;
                        strm << "Pipeline: Cannot open " << sp->value;
                        throw std::runtime_error(strm.str());
                    }
                    fh->isPipe = false;
                    fh->bRead = false;
                    fh->addnl = true;
                    args.push_back(fh);
                }
                else if (sp->value.back() == '>')
                {
                    FileHandlePtr fh = MakeFileHandle();
                    sp->value.pop_back();
                    fh->fp = fopen(sp->value.c_str(), "r");
                    if (!fh->fp)
                    {
                        std::stringstream strm;
                        strm << "Pipeline: Cannot open " << sp->value;
                        throw std::runtime_error(strm.str());
                    }
                    fh->isPipe = false;
                    fh->bRead = true;
                    fh->stripnl = true;
                    args.push_back(fh);
                }
                else if (sp->value[0] == '|')
                {
                    FileHandlePtr fh = MakeFileHandle();
                    fh->fp = popen(&sp->value[1], "w");
                    if (!fh->fp)
                    {
                        std::stringstream strm;
                        strm << "Pipeline: Cannot open " << sp->value;
                        throw std::runtime_error(strm.str());
                    }
                    fh->isPipe = true;
                    fh->bRead = false;
                    fh->addnl = true;
                    args.push_back(fh);
                }
                else if (sp->value.back() == '|')
                {
                    FileHandlePtr fh = MakeFileHandle();
                    sp->value.pop_back();
                    fh->fp = popen(sp->value.c_str(), "r");
                    if (!fh->fp)
                    {
                        std::stringstream strm;
                        strm << "Pipeline: Cannot open " << sp->value;
                        throw std::runtime_error(strm.str());
                    }
                    fh->isPipe = true;
                    fh->bRead = true;
                    fh->stripnl = true;
                    args.push_back(fh);
                }
                else
                    args.push_back(sp);
            }
            else
                args.push_back(e);
        }
        ElementPtr e = PipeLineImpl(machine, args);
        ClosePipeLine(machine, args);
        return e;
    }
}

