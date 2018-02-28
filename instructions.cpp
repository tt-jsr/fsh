#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "common.h"
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "parser.h"

namespace fsh
{
    
    namespace instruction
    {
        /*****************************************************/
        void ExecuteInteger(Machine& machine, int op, fsh::IntegerPtr lhs, fsh::IntegerPtr rhs)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeInteger(lhs->value + rhs->value));
                break;
            case TOKEN_MINUS:
                machine.push_data(MakeInteger(lhs->value - rhs->value));
                break;
            case TOKEN_MULTIPLY:
                machine.push_data(MakeInteger(lhs->value * rhs->value));
                break;
            case TOKEN_DIVIDE:
                machine.push_data(MakeInteger(lhs->value / rhs->value));
                break;
            case TOKEN_GT:
                machine.push_data(MakeBoolean(lhs->value > rhs->value));
                break;
            case TOKEN_GTE:
                machine.push_data(MakeBoolean(lhs->value >= rhs->value));
                break;
            case TOKEN_LT:
                machine.push_data(MakeBoolean(lhs->value < rhs->value));
                break;
            case TOKEN_LTE:
                machine.push_data(MakeBoolean(lhs->value <= rhs->value));
                break;
            case TOKEN_EQ:
                machine.push_data(MakeBoolean(lhs->value == rhs->value));
                break;
            case TOKEN_NEQ:
                machine.push_data(MakeBoolean(lhs->value != rhs->value));
                break;
            }
        }

        void ExecuteFloat(Machine& machine, int op, fsh::FloatPtr lhs, fsh::FloatPtr rhs)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeFloat(lhs->value + rhs->value));
                break;
            case TOKEN_MINUS:
                machine.push_data(MakeFloat(lhs->value - rhs->value));
                break;
            case TOKEN_MULTIPLY:
                machine.push_data(MakeFloat(lhs->value * rhs->value));
                break;
            case TOKEN_DIVIDE:
                machine.push_data(MakeFloat(lhs->value / rhs->value));
                break;
            case TOKEN_GT:
                machine.push_data(MakeBoolean(lhs->value > rhs->value));
                break;
            case TOKEN_GTE:
                machine.push_data(MakeBoolean(lhs->value >= rhs->value));
                break;
            case TOKEN_LT:
                machine.push_data(MakeBoolean(lhs->value < rhs->value));
                break;
            case TOKEN_LTE:
                machine.push_data(MakeBoolean(lhs->value <= rhs->value));
                break;
            case TOKEN_EQ:
                machine.push_data(MakeBoolean(lhs->value == rhs->value));
                break;
            case TOKEN_NEQ:
                machine.push_data(MakeBoolean(lhs->value != rhs->value));
                break;
            }
        }
        BinaryOperator::~BinaryOperator()
        {
        }

        std::string BinaryOperator::type_str()
        {
            std::stringstream strm;
            strm << "BINARY_OPERERATOR(" << op << ")";
            return strm.str();
        }

        void BinaryOperator::Execute(Machine& machine)
        {
            lhs->Execute(machine);
            rhs->Execute(machine);
            if (op == TOKEN_COMMA)
            {
                return;
            }
            if (op == TOKEN_ASSIGNMENT)
            {
                ElementPtr rdata = machine.pop_data();
                ElementPtr ldata = machine.pop_data();

                if (ldata->type() != fsh::ELEMENT_TYPE_IDENTIFIER)
                {
                    std::stringstream strm;
                    strm << "lhs of assignment is not an identifier, got " << ldata->type();
                    throw std::runtime_error(strm.str());
                }
                machine.store_variable(ldata.cast<fsh::Identifier>()->value, rdata);
                machine.push_data(rdata);
            }
            else
            {
                ElementPtr rdata = machine.pop_data();
                ElementPtr ldata = machine.pop_data();

                if (rdata->IsFloat() && ldata->IsInteger())
                {
                    fsh::FloatPtr l = MakeFloat((double)ldata.cast<fsh::Integer>()->value);
                    fsh::FloatPtr r = rdata.cast<fsh::Float>();
                    ExecuteFloat(machine, op, l, r);
                }
                if (rdata->IsInteger() && ldata->IsFloat())
                {
                    fsh::FloatPtr r = MakeFloat((double)rdata.cast<fsh::Integer>()->value);
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), r);
                }
                if (rdata->IsFloat() && ldata->IsFloat())
                {
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), rdata.cast<fsh::Float>());
                }
                if (rdata->IsInteger() && ldata->IsInteger())
                {
                    ExecuteInteger(machine, op, ldata.cast<fsh::Integer>(), rdata.cast<fsh::Integer>());
                }
            }
        }

        void BinaryOperator::dump(DumpContext& ctx)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                ctx.strm() << "+" << std::endl;
                break;
            case TOKEN_MINUS:
                ctx.strm() << "-" << std::endl;
                break;
            case TOKEN_MULTIPLY:
                ctx.strm() << "*" << std::endl;
                break;
            case TOKEN_DIVIDE:
                ctx.strm() << "/" << std::endl;
                break;
            case TOKEN_LT:
                ctx.strm() << "<" << std::endl;
                break;
            case TOKEN_LTE:
                ctx.strm() << "<=" << std::endl;
                break;
            case TOKEN_GT:
                ctx.strm() << ">" << std::endl;
                break;
            case TOKEN_GTE:
                ctx.strm() << ">=" << std::endl;
                break;
            case TOKEN_EQ:
                ctx.strm() << "==" << std::endl;
                break;
            case TOKEN_NEQ:
                ctx.strm() << "!=" << std::endl;
                break;
            case TOKEN_ASSIGNMENT:
                ctx.strm() << "=" << std::endl;
                break;
            case TOKEN_COMMA:
                ctx.strm() << "," << std::endl;
                break;
            default:
                ctx.strm() << op << std::endl;
            }
            ctx.inc();
            ctx.strm() << "left" << std::endl;
            ctx.inc();
            lhs->dump(ctx);
            ctx.dec();
            ctx.strm() << "right" << std::endl;
            ctx.inc();
            if (rhs)
            {
                rhs->dump(ctx);
            }
            ctx.dec();
            ctx.dec();
        }

        /*****************************************************/
        std::string FunctionCall::type_str()
        {
            std::stringstream strm;
            strm << "FUNCTION_CALL(" << name << ")";
            return strm.str();
        }

        void FunctionCall::Execute(Machine& machine)
        {
            fsh::ElementPtr e;
            std::string fname = name;
            if (fname[0] == '$')
                fname = &fname[1];
            if (machine.get_variable(fname, e) == false)
            {
                std::stringstream strm;
                strm << "Function \"" << name << "\" not found";
                throw std::runtime_error(strm.str());
            }
            if (e->IsFunctionDefinition() == false)
            {
                std::stringstream strm;
                strm << "Name \"" << name << "\" is not a function";
                throw std::runtime_error(strm.str());
            }

            fsh::FunctionDefinitionPtr funcDef = e.cast<FunctionDefinition>();
            if (funcDef->isBuiltIn)
            {
                machine.push_context();
                // now execute the function
                std::vector<ElementPtr> arguments;
                size_t top = machine.size_data();
                functionArguments->Execute(machine);
                while(machine.size_data() > top)
                {
                    arguments.push_back(machine.pop_data());
                }
                std::reverse(arguments.begin(), arguments.end());
                ElementPtr rtn = funcDef->builtInBody(machine, arguments);
                machine.pop_context();
                machine.push_data(rtn);
            }
            else
            {
                std::vector<ElementPtr> dataArgs;
                // Execute each argument and put the result into the dataArgs vector
                size_t top = machine.size_data();
                if (functionArguments)
                {
                    functionArguments->Execute(machine);
                }
                while (machine.size_data() > top)
                {
                    dataArgs.push_back(machine.pop_data());
                }
                std::reverse(dataArgs.begin(), dataArgs.end());
                // Create a new context, create a variable for each named argument
                machine.push_context();
                size_t dataArgIdx = 0;
                for (; dataArgIdx < dataArgs.size() && dataArgIdx < funcDef->arg_names.size(); ++dataArgIdx)
                {
                    machine.store_variable(funcDef->arg_names[dataArgIdx], dataArgs[dataArgIdx]);
                }
                // Any named arguments that the caller did not provide, we set to None
                for (;dataArgIdx < funcDef->arg_names.size(); ++dataArgIdx)
                {
                    ElementPtr none = MakeNone();
                    machine.store_variable(funcDef->arg_names[dataArgIdx], none);
                }
                top = machine.size_data();
                funcDef->functionBody->Execute(machine);
                ElementPtr rtn = machine.peek_data();
                while(machine.size_data() > top)
                {
                    machine.pop_data();
                }
                machine.push_data(rtn); // push back that last statement result
                machine.pop_context();
            }
        }

        void FunctionCall::dump(DumpContext& ctx)
        {
            ctx.strm() << "FunctionCall " << name << std::endl;
            ctx.inc();
            ctx.strm() << "arguments:" << std::endl;
            ctx.inc();
            functionArguments->dump(ctx);
            ctx.dec();
            ctx.dec();
        }

        /*****************************************************/
        std::string FunctionDef::type_str()
        {
            std::stringstream strm;
            strm << "FUNCTION_DEFINITION";
            return strm.str();
        }
        void FunctionDef::Execute(Machine& machine)
        {
            fsh::FunctionDefinitionPtr func = fsh::MakeFunctionDefinition();
            func->arg_names = arg_names;
            func->functionBody = functionBody;
            machine.push_data(func);
        }

        void FunctionDef::dump(DumpContext& ctx)
        {
            ctx.strm() << "FunctionDef" << std::endl;
            ctx.inc();
            ctx.strm() << "arg-names: ";
            for (auto& s: arg_names)
            {
                ctx.strm_ << s;
            }
            ctx.strm_ << std::endl;
            functionBody->dump(ctx);
            ctx.dec();
        }

        /****************************************************/
        void WhileIf::Execute(Machine& machine)
        {
            if (isWhile)
            {
                bool executeElse(true);
                ElementPtr rtn;
                bool run = true;
                while(run)
                {
                    condition->Execute(machine);
                    ElementPtr cond = machine.pop_data();
                    run = machine.ConvertToBool(cond);
                    if (run)
                    {
                        executeElse = false;
                        size_t top = machine.size_data();

                        if_true->Execute(machine);
                        // Clean up the stack
                        rtn = machine.peek_data();
                        while(machine.size_data() > top)
                        {
                            machine.pop_data();
                        }
                    }
                    if (executeElse)
                    {
                        size_t top = machine.size_data();
                        if_false->Execute(machine);
                        // Clean up the stack
                        rtn = machine.peek_data();
                        while(machine.size_data() > top)
                        {
                            machine.pop_data();
                        }
                    }
                }
                machine.push_data(rtn);
            }
            else
            {
                condition->Execute(machine);
                ElementPtr cond = machine.pop_data();
                bool b = machine.ConvertToBool(cond);
                size_t top = machine.size_data();
                if (b)
                {
                    if_true->Execute(machine);
                }
                else
                {
                    if_false->Execute(machine);
                }

                // Clean up the stack
                ElementPtr rtn = machine.peek_data();
                while(machine.size_data() > top)
                {
                    machine.pop_data();
                }
                machine.push_data(rtn);
            }
        }

        std::string WhileIf::type_str()
        {
            return "WhileIf";
        }

        void WhileIf::dump(DumpContext& ctx)
        {
            ctx.strm() << "WhileIf isWhile:" << isWhile << std::endl;
            ctx.inc();
            ctx.strm() << "Conditional" << std::endl;
            ctx.inc();
            condition->dump(ctx);
            ctx.dec();
            ctx.strm() << "true" << std::endl;
            ctx.inc();
            if_true->dump(ctx);
            ctx.dec();
            ctx.strm() << "false" << std::endl;
            ctx.inc();
            if_false->dump(ctx);
            ctx.dec();
            ctx.dec();
        }

        /*****************************************************/
        void Identifier::Execute(Machine& machine)
        {
            if (name[0] == '$')
            {
                ElementPtr e;
                if (machine.get_variable(&name[1], e) == false)
                {
                    std::stringstream strm;
                    strm << "Variable \"" << name << "\" not found";
                    throw std::runtime_error(strm.str());
                }
                machine.push_data(e);
            }
            else
            {
                ElementPtr e = fsh::MakeIdentifier(name);
                machine.push_data(e);
            }
        }

        std::string Identifier::type_str()
        {
            std::stringstream strm;
            strm << "IDENTIFIER(" << name << ")";
            return strm.str();
        }

        void Identifier::dump(DumpContext& ctx)
        {
            ctx.strm() << name << std::endl;
        }

        /*****************************************************/
        void IdentifierList::Execute(Machine& machine)
        {
        }

        std::string IdentifierList::type_str()
        {
            std::stringstream strm;
            strm << "IDENTIFIER_LIST";
            return strm.str();
        }

        void IdentifierList::dump(DumpContext& ctx)
        {
            ctx.strm() << "IdentifierList" << std::endl;
            ctx.inc();
            for (auto& in : identifiers)
            {
                in->dump(ctx);
            }
            ctx.dec();
        }

        /*****************************************************/
        void Integer::Execute(Machine& machine)
        {
            machine.push_data(MakeInteger(value));
        }

        void Integer::dump(DumpContext& ctx)
        {
            ctx.strm() << value << std::endl;
        }

        std::string Integer::type_str()
        {
            std::stringstream strm;
            strm << "INTEGER(" << value << ")";
            return strm.str();
        }

        /*****************************************************/
        void String::Execute(Machine& machine)
        {
            machine.push_data(MakeString(value));
        }

        void String::dump(DumpContext& ctx)
        {
            ctx.strm() << "\"" << value << "\"" << std::endl;
        }

        std::string String::type_str()
        {
            std::stringstream strm;
            strm << "String(" << value << ")";
            return strm.str();
        }

        /*****************************************************/
        void None::Execute(Machine& machine)
        {
            machine.push_data(MakeNone());
        }

        void None::dump(DumpContext& ctx)
        {
            ctx.strm() << "None" << std::endl;
        }

        std::string None::type_str()
        {
            std::stringstream strm;
            strm << "None";
            return strm.str();
        }

        /*****************************************************/
        void Boolean::Execute(Machine& machine)
        {
            machine.push_data(MakeBoolean(value));
        }

        void Boolean::dump(DumpContext& ctx)
        {
            ctx.strm() << (value ? " true " : " false ") << std::endl;
        }

        std::string Boolean::type_str()
        {
            std::stringstream strm;
            strm << "Boolean(" << value << ")";
            return strm.str();
        }

        /*****************************************************/

        void Float::Execute(Machine& machine)
        {
            machine.push_data(MakeFloat(value));
        }

        void Float::dump(DumpContext& ctx)
        {
            ctx.strm() << value << std::endl;
        }

        std::string Float::type_str()
        {
            std::stringstream strm;
            strm << "Float(" << value << ")";
            return strm.str();
        }

        /**************************************************/
        void ElementWrapper::Execute(Machine& machine)
        {
            machine.push_data(element);
        }

        void ElementWrapper::dump(DumpContext& ctx)
        {
            ctx.strm() << "ElementWrapper" << std::endl;
        }

        std::string ElementWrapper::type_str()
        {
            std::stringstream strm;
            strm << "ElementWrapper";
            return strm.str();
        }

        /**************************************************/
        void ExpressionList::Execute(Machine& machine)
        {
            if (isList)
            {
                fsh::ListPtr lp = fsh::MakeList(HEAD_TYPE_LIST);
                for (auto& in : expressions)
                {
                    lp->items.push_back(in);
                }
                machine.push_data(lp);
            }
            else
            {
                for (auto &in : expressions)
                {
                    in->Execute(machine);
                }
            }
        }

        void ExpressionList::dump(DumpContext& ctx)
        {
            ctx.strm() << "Expressionlist" << std::endl;
            ctx.inc();
            for(auto& ex : expressions)
            {
                ex->dump(ctx);
            }
            ctx.dec();
        }

        std::string ExpressionList::type_str()
        {
            std::stringstream strm;
            strm << "ExpressionList";
            return strm.str();
        }

        /***************************************************************/

        DumpContext::DumpContext(std::ostream& os)
        :strm_(os)
        ,indent_(0)
        {}

        void DumpContext::inc() 
        {
            indent_+= 4;
        }
        
        void DumpContext::dec() 
        {
            indent_-= 4;
        }
        
        std::ostream& DumpContext::strm()
        {
            indent();
            return strm_;
        }

        void DumpContext::indent()
        {
            for (int i = 0; i < indent_; i++)
            {
                strm_ << ' ';
            }
        }
    }
 }

