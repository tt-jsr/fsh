#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "common.h"
#include "common.h"
#include "instructions.h"
#include "machine.h"
#include "builtins.h"

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
        
        void ExecuteString(Machine& machine, int op, fsh::StringPtr lhs, fsh::StringPtr rhs)
        {
            switch(op)
            {
            case TOKEN_PLUS:
                machine.push_data(MakeString(lhs->value + rhs->value));
                break;
            case TOKEN_EQ:
                machine.push_data(MakeBoolean(lhs->value == rhs->value));
                break;
            case TOKEN_NEQ:
                machine.push_data(MakeBoolean(lhs->value != rhs->value));
                break;
            default:
                throw std::runtime_error("Invalid operator with strings");
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
            if (op == TOKEN_AND)
            {
                lhs->Execute(machine);
                ElementPtr ldata = machine.pop_data();
                ldata = machine.resolve(ldata);
                bool b = machine.ConvertToBool(ldata);
                if (b == false)
                {
                    ElementPtr e = MakeBoolean(false);
                    machine.push_data(e);
                    return;
                }
                rhs->Execute(machine);
                ElementPtr rdata = machine.pop_data();
                rdata = machine.resolve(rdata);
                b = machine.ConvertToBool(rdata);
                if (b == false)
                {
                    ElementPtr e = MakeBoolean(false);
                    machine.push_data(e);
                    return;
                }
                ElementPtr e = MakeBoolean(true);
                machine.push_data(e);
                return;
            }
            if (op == TOKEN_OR)
            {
                lhs->Execute(machine);
                ElementPtr ldata = machine.pop_data();
                ldata = machine.resolve(ldata);
                bool b = machine.ConvertToBool(ldata);
                if (b == true)
                {
                    ElementPtr e = MakeBoolean(true);
                    machine.push_data(e);
                    return;
                }
                rhs->Execute(machine);
                ElementPtr rdata = machine.pop_data();
                rdata = machine.resolve(rdata);
                b = machine.ConvertToBool(rdata);
                if (b == false)
                {
                    ElementPtr e = MakeBoolean(false);
                    machine.push_data(e);
                    return;
                }
                ElementPtr e = MakeBoolean(true);
                machine.push_data(e);
                return;
            }
            if (op == TOKEN_COMMA)
            {
                lhs->Execute(machine);
                rhs->Execute(machine);
                return;
            }
            if (op == TOKEN_ASSIGNMENT)
            {
                rhs->Execute(machine);
                ElementPtr rdata = machine.pop_data();
                rdata = machine.resolve(rdata);

                lhs->Execute(machine);
                ElementPtr ldata = machine.pop_data();

                if (ldata->type() != fsh::ELEMENT_TYPE_IDENTIFIER)
                {
                    std::stringstream strm;
                    strm << "lhs of assignment is not an identifier, got " << ldata->type() << " line: " << lineno;
                    throw std::runtime_error(strm.str());
                }
                machine.store_variable(ldata.cast<fsh::Identifier>()->value, rdata);
                machine.push_data(rdata);
                return;
            }
            else
            {
                lhs->Execute(machine);
                ElementPtr ldata = machine.pop_data();
                ldata = machine.resolve(ldata);

                rhs->Execute(machine);
                ElementPtr rdata = machine.pop_data();
                rdata = machine.resolve(rdata);

                if (rdata->IsString() && ldata->IsString())
                {
                    ExecuteString(machine, op, ldata.cast<fsh::String>(), rdata.cast<fsh::String>());
                }
                else if (rdata->IsFloat() && ldata->IsInteger())
                {
                    fsh::FloatPtr l = MakeFloat((double)ldata.cast<fsh::Integer>()->value);
                    fsh::FloatPtr r = rdata.cast<fsh::Float>();
                    ExecuteFloat(machine, op, l, r);
                }
                else if (rdata->IsInteger() && ldata->IsFloat())
                {
                    fsh::FloatPtr r = MakeFloat((double)rdata.cast<fsh::Integer>()->value);
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), r);
                }
                else if (rdata->IsFloat() && ldata->IsFloat())
                {
                    ExecuteFloat(machine, op, ldata.cast<fsh::Float>(), rdata.cast<fsh::Float>());
                }
                else if (rdata->IsInteger() && ldata->IsInteger())
                {
                    ExecuteInteger(machine, op, ldata.cast<fsh::Integer>(), rdata.cast<fsh::Integer>());
                }
                else
                {
                    std::stringstream strm;
                    strm << "Binary Operator: Invalid types, line: " << lineno;
                    throw std::runtime_error(strm.str());
                }
            }
        }

        void BinaryOperator::dump(DumpContext& ctx)
        {
            ctx.inc();
            ctx.strm() << "bop left" << std::endl;
            ctx.inc();
            lhs->dump(ctx);
            ctx.dec();
            ctx.dec();
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
            ctx.strm() << "bop right" << std::endl;
            ctx.inc();
            if (rhs)
            {
                rhs->dump(ctx);
            }
            ctx.dec();
            ctx.dec();
        }

        /*****************************************************/
        DotOperator::~DotOperator()
        {
        }

        std::string DotOperator::type_str()
        {
            std::stringstream strm;
            strm << "DOT_OPERERATOR";
            return strm.str();
        }

        void DotOperator::Execute(Machine& machine)
        {
            lhs->Execute(machine);
            ElementPtr ldata = machine.pop_data();
            ldata = machine.resolve(ldata);
            if (ldata->IsList() == false)
            {
                std::stringstream strm;
                strm << "lhs is required tone an identifier, line: " << lineno;
                throw std::runtime_error(strm.str());
            }
            ListPtr lst = ldata.cast<List>();
            if (rhs->type() == INSTRUCTION_BINARY_OPERATOR)
            {
                BinaryOperatorPtr bop = rhs.cast<BinaryOperator>();
                if (bop->op != TOKEN_ASSIGNMENT)
                {
                    std::stringstream strm;
                    strm << "Assignment must be right of dot operator, line: " << lineno;
                    throw std::runtime_error(strm.str());
                }
                bop->lhs->Execute(machine);
                ElementPtr e = machine.pop_data();
                int64_t idx = machine.get_record_field(lst->listtype, e);
                bop->rhs->Execute(machine);
                e = machine.pop_data();
                e = machine.resolve(e);
                lst->items[idx] = e;
                machine.push_data(e);
                return;
            }
            rhs->Execute(machine);
            ElementPtr rdata = machine.pop_data();
            size_t idx = machine.get_record_field(lst->listtype, rdata);
            if (idx >= lst->items.size())
                throw std::runtime_error("record field out of range");
            machine.push_data(lst->items[idx]);
        }

        void DotOperator::dump(DumpContext& ctx)
        {
            ctx.inc();
            ctx.strm() << "dot left" << std::endl;
            ctx.inc();
            lhs->dump(ctx);
            ctx.dec();
            ctx.dec();
            ctx.strm() << "." << std::endl;
            ctx.inc();
            ctx.strm() << "dot right" << std::endl;
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
            strm << "FUNCTION_CALL";
            return strm.str();
        }

        void FunctionCall::Execute(Machine& machine)
        {
            fsh::ElementPtr e;
            call->Execute(machine);
            e = machine.pop_data();
            bool isBind(false);
            if (e->IsIdentifier())
            {
                fsh::IdentifierPtr id = e.cast<fsh::Identifier>();
                if (id->value == "Bind")
                    isBind = true;
                if (machine.get_variable(id->value, e) == false)
                {
                    std::stringstream strm;
                    strm << "Function \"" << id->value << "\" not found, line: " << lineno;
                    throw std::runtime_error(strm.str());
                }
            }
            if (e->IsFunctionDefinition() == false)
            {
                std::stringstream strm;
                strm << "expression is not a function, line: " << lineno;
                throw std::runtime_error(strm.str());
            }

            fsh::FunctionDefinitionPtr funcDef = e.cast<FunctionDefinition>();
            machine.push_context();
            size_t top = machine.size_data();
            if (functionArguments)
                functionArguments->Execute(machine);

            size_t nItemsOnStack = machine.size_data() - top;
            try
            {
                ElementPtr rtn = fsh::CallFunctionImpl(machine, isBind, funcDef, nItemsOnStack);
                assert(!rtn->IsIdentifier());
                machine.pop_context();
                machine.push_data(rtn);
            }
            catch (std::exception& e)
            {
                machine.pop_context();
                throw;
            }
        }

        void FunctionCall::dump(DumpContext& ctx)
        {
            ctx.strm() << "FunctionCall "<< std::endl;
            ctx.inc();
            ctx.strm() << "call:" << std::endl;
            ctx.inc();
            call->dump(ctx);
            ctx.dec();
            ctx.strm() << "arguments:" << std::endl;
            ctx.inc();
            if (functionArguments)
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
        void For::Execute(Machine& machine)
        {
            if (identifier->type() != INSTRUCTION_IDENTIFIER)
            {
                std::stringstream strm;
                strm << "For requires an identifier, line: " << lineno;
                throw std::runtime_error(strm.str());
            }
            std::string varname = identifier.cast<Identifier>()->name;
            list->Execute(machine);
            ElementPtr e = machine.pop_data();
            e = machine.resolve(e);
            if (!e->IsList())
            {
                std::stringstream strm;
                strm << "For requires a list, line: " << lineno;
                throw std::runtime_error(strm.str());
            }
            ListPtr lst = e.cast<List>();
            ElementPtr rtn = MakeNone();
            for (size_t idx = 0; idx < lst->items.size(); ++idx)
            {
                ElementPtr item = lst->items[idx];
                machine.store_variable(varname, item);
                body->Execute(machine);
                rtn = machine.pop_data();
            }
            machine.push_data(rtn);
        }

        std::string For::type_str()
        {
            return "For ";
        }

        void For::dump(DumpContext& ctx)
        {
            ctx.strm() << "For ";
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
                    cond = machine.resolve(cond);
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
                cond = machine.resolve(cond);
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

        /****************************************************/
        void TryCatch::Execute(Machine& machine)
        {
            try 
            {
                try_->Execute(machine);
            }
            catch (std::exception& e)
            {
                ElementPtr msg = MakeString(e.what());
                machine.store_variable("_exception", msg);
                catch_->Execute(machine);
            }
        }

        std::string TryCatch::type_str()
        {
            return "TryCatch";
        }

        void TryCatch::dump(DumpContext& ctx)
        {
            ctx.strm() << "TryCatch" << std::endl;
            ctx.inc();
            ctx.strm() << "try" << std::endl;
            ctx.inc();
            try_->dump(ctx);
            ctx.dec();
            ctx.strm() << "catch" << std::endl;
            ctx.inc();
            catch_->dump(ctx);
            ctx.dec();
            ctx.dec();
        }
        /*****************************************************/
        void Identifier::Execute(Machine& machine)
        {
            ElementPtr e = fsh::MakeIdentifier(name);
            machine.push_data(e);
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

        /*****************************************************/

        void Attribute::Execute(Machine& machine)
        {
            name->Execute(machine);
            ElementPtr n = machine.pop_data();
            assert(n->IsIdentifier());
            value->Execute(machine);
            ElementPtr v = machine.pop_data();
            machine.push_data(MakeAttribute(n.cast<fsh::Identifier>(), v));
        }

        void Attribute::dump(DumpContext& ctx)
        {
            ctx.strm() << "attribute" << std::endl;
            ctx.inc();
            ctx.strm() << "name:" << std::endl;
            ctx.inc();
            name->dump(ctx);
            ctx.dec();
            ctx.strm() << "value:" << std::endl;
            ctx.inc();
            value->dump(ctx);
            ctx.dec();
            ctx.dec();
        }

        std::string Attribute::type_str()
        {
            std::stringstream strm;
            strm << "Attribute";
            return strm.str();
        }

        /*****************************************************/

        void System::Execute(Machine& machine)
        {
            system(value.c_str());
            machine.push_data(MakeNone());
        }

        void System::dump(DumpContext& ctx)
        {
            ctx.strm() << "system: " << value << std::endl;
        }

        std::string System::type_str()
        {
            std::stringstream strm;
            strm << "System";
            return strm.str();
        }

        /**************************************************/
        /*
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
        */

        /**************************************************/
        void ExpressionList::Execute(Machine& machine)
        {
            if (isList)
            {
                fsh::ListPtr lp = fsh::MakeList("__list__");
                for (auto& in : expressions)
                {
                    in->Execute(machine);
                    ElementPtr e = machine.pop_data();
                    lp->items.push_back(machine.resolve(e));
                }
                machine.push_data(lp);
            }
            else
            {
                for (auto &in : expressions)
                {
                    in->Execute(machine);
                    ElementPtr e = machine.pop_data();
                    machine.push_data(machine.resolve(e));
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

