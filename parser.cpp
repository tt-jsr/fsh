#include <cctype>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>
#include "common.h"
#include "parser.h"
#include "instructions.h"

//#define PRINT_STACK 1

const int PREC_PAREN = 100;

namespace fsh
{
    /* Precendence
     * 2:  >= <= < > == != 
     * 1: && ||
     * 0: = ,
     *
     * {}[]() [[]] don't have precedence
     */
    struct Operators
    {
        char buf[5];
        int length;
        int token;
        int prec;
    } opTable[] = {
       {"||", 2, TOKEN_OR, 2}
       ,{"!=", 2, TOKEN_NEQ, 7}
       ,{"&&", 2, TOKEN_AND, 2}
       ,{">=", 2, TOKEN_GTE, 7}
       ,{"<=", 2, TOKEN_LTE, 7}
       //,{"[[", 2, TOKEN_OPEN_SUBSCRIPT, 0}
       //,{"]]", 2, TOKEN_CLOSE_SUBSCRIPT, 0}
       //,{"in", 2, TOKEN_IN, 2}
       ,{">", 1, TOKEN_GT, 7}
       ,{"<", 1, TOKEN_LT, 7}
       ,{"=", 1, TOKEN_ASSIGN, 1}
       //,{"{", 1, TOKEN_OPEN_BRACE, 0}
       //,{"}", 1, TOKEN_CLOSE_BRACE, 0}
       //,{"[", 1, TOKEN_OPEN_BRACKET, 0}
       //,{"]", 1, TOKEN_CLOSE_BRACKET, 0}
       //,{"(", 1, TOKEN_OPEN_PAREN, 0}
       //,{")", 1, TOKEN_CLOSE_PAREN, 0}
       ,{",", 1, TOKEN_COMMA, 0}
       ,{"+", 1, TOKEN_PLUS, 10}
       ,{"-", 1, TOKEN_MINUS, 10}
       ,{"*", 1, TOKEN_MULTIPLY, 20}
       ,{"/", 1, TOKEN_DIVIDE, 20}
       ,{"%", 1, TOKEN_MOD, 20}
       ,{0, 0, 0}
    };

    Parser::Parser()
    :pos_(0)
    {}

    Parser::~Parser()
    {
    }

    void dump(instruction::InstructionPtr in)
    {
        std::cout << "**************************" << std::endl;
        in->dump(std::cout);
        std::cout << std::endl;
    }

    void Parser::HandleInstruction(instruction::BinaryOperatorPtr bop)
    {
        if (bop.get() == nullptr)
            return;
        instruction::InstructionType t = peektype();
        switch(t)
        {
        case instruction::INSTRUCTION_IDENTIFIER:
        case instruction::INSTRUCTION_INTEGER:
            bop->lhs = pop();
            push(bop);
            break;
        case instruction::INSTRUCTION_BINARY_OPERATOR:
            {
                instruction::BinaryOperatorPtr op = pop().cast<instruction::BinaryOperator>();
                if (bop->prec == PREC_PAREN)
                {
                    op->rhs = bop;
                    push(op);
                }
                else
                {
                    if (op->prec >= bop->prec)
                    {
                        bop->lhs = op;
                        push(bop);
                    }
                    else
                    {
                        instruction::InstructionPtr inst = op->rhs;
                        op->rhs = bop;
                        bop->lhs = inst;
                        push(op);
                        push(bop);
                    }
                }
            }
            break;
        case instruction::INSTRUCTION_NONE:
            push(bop);
            break;
        }
    }

    void Parser::HandleInstruction(instruction::IntegerPtr i)
    {
        if (i.get() == nullptr)
            return;
        instruction::InstructionType t = peektype();
        switch(t)
        {
        case instruction::INSTRUCTION_IDENTIFIER:
        case instruction::INSTRUCTION_INTEGER:
            push(i);
            break;
        case instruction::INSTRUCTION_BINARY_OPERATOR:
            {
                instruction::BinaryOperatorPtr bop = pop().cast<instruction::BinaryOperator>();
                bop->rhs = i;
                push(bop);
            }
            break;
        case instruction::INSTRUCTION_NONE:
            push(i);
            break;
        }
    }

    void Parser::HandleInstruction(instruction::IdentifierPtr id)
    {
        if (id.get() == nullptr)
            return;
        instruction::InstructionType t = peektype();
        switch(t)
        {
        case instruction::INSTRUCTION_IDENTIFIER:
        case instruction::INSTRUCTION_INTEGER:
            push(id);
            break;
        case instruction::INSTRUCTION_BINARY_OPERATOR:
            {
                instruction::BinaryOperatorPtr bop = pop().cast<instruction::BinaryOperator>();
                bop->rhs = id;
                push(bop);
            }
            break;
        case instruction::INSTRUCTION_NONE:
            push(id);
            break;
        }
    }

    void Parser::HandleInstruction(instruction::InstructionPtr in)
    {
        if (in.get() == nullptr)
            return;
        switch(in->type())
        {
        case instruction::INSTRUCTION_BINARY_OPERATOR:
            HandleInstruction(in.cast<instruction::BinaryOperator>());
            break;
        case instruction::INSTRUCTION_IDENTIFIER:
            HandleInstruction(in.cast<instruction::Identifier>());
            break;
        case instruction::INSTRUCTION_INTEGER:
            HandleInstruction(in.cast<instruction::Integer>());
            break;
        }
    }

    instruction::InstructionPtr Parser::parse(const std::string& s)
    {
        //std::cout << "enter parse for " << s << std::endl;
        input_ = s;
        pos_ = 0;
        stack_.clear();
        while(peekchar() != 0)
        {
            instruction::InstructionPtr in = parse_paren();
            if (in.get() && in->type() == instruction::INSTRUCTION_BINARY_OPERATOR)
            {
                instruction::BinaryOperatorPtr bop = in.cast<instruction::BinaryOperator>();
                // Artificially inflate precendence
                bop->prec = PREC_PAREN;
            }
            HandleInstruction(in);
            HandleInstruction(parse_identifier());
            HandleInstruction(parse_number());
            HandleInstruction(parse_binary_operator());
        }
        //std::cout << "exit parse for " << s << std::endl;
        assert (stack_.size() > 0);
        //stack_.front()->dump(std::cout);
        //std::cout << std::endl;
        return stack_.front();
    }


    char Parser::peekchar(int n)
    {
        if ((pos_+n) >= input_.size())
            return 0;
        return input_[pos_ + n];
    }

    const char * Parser::peekstr()
    {
        if (pos_ >= input_.size())
            return 0;
        return &input_[pos_];
    }

    void Parser::advance(int n)
    {
        if (n < 0 && -n > pos_)
            pos_ = 0;
        else
            pos_ += n;
    }

    char Parser::getchar()
    {
        if (pos_ >= input_.size())
            return 0;
        char c = input_[pos_];
        ++pos_;
        return c;
    }

    void Parser::push(instruction::InstructionPtr p)
    {
        assert(p.get() != nullptr);
        //std::cout << "=== push " << p->type() << std::endl;
        stack_.push_back(p);
    }

    instruction::InstructionPtr Parser::pop()
    {
        assert(stack_.size() > 0);
        if (stack_.size() == 0)
            return nullptr;
        instruction::InstructionPtr p = stack_.back();
        stack_.pop_back();
        //std::cout << "=== pop " << p->type() << std::endl;
        return p;
    }

    instruction::InstructionType Parser::peektype()
    {
        if (stack_.size() == 0)
            return instruction::INSTRUCTION_NONE;
        return stack_.back()->type();
    }

    void Parser::skipWhiteSpace()
    {
        char c = peekchar();
        while (c == ' ' || c == '\r' || c == '\n' || c == '\t')
        {
            advance(1);
            c = peekchar();
        }
    }

    instruction::InstructionPtr Parser::parse_paren()
    {
        skipWhiteSpace();
        if (peekchar() != '(')
            return nullptr;
        std::string buf;
        int nest = 0;
        advance(1);
        char c = peekchar();
        while (nest == 0 && c != ')')
        {
            if (c == '(')
                ++nest;
            if (c == ')')
                --nest;
            buf.push_back(c);
            advance(1);
            c = peekchar();
        }
        advance(1);
        Parser parser;
        return parser.parse(buf);
    }

    instruction::BinaryOperatorPtr Parser::parse_binary_operator()
    {
        skipWhiteSpace();
        Operators *op = opTable;
        const char *s = peekstr();
        if (s == nullptr)
            return nullptr;
        while(op->buf[0])
        {
            const char *buf = op->buf;
            if (strncmp(s, op->buf, op->length) == 0)
            {
                instruction::BinaryOperatorPtr bop(new instruction::BinaryOperator());
                bop->op = op->token;
                bop->prec = op->prec;
                advance(op->length);
                return bop;
            }
            ++op;
        }
        return nullptr;
    }

    /*
    void Parser::parse_identifierSequence(std::vector<instruction::IdentifierPtr>& vec)
    {
        while(true)
        {
            instruction::Identifier *ident = parse_identifier();
            if (ident == nullptr)
                return;
            vec.push_back(ident);
            skipWhiteSpace();
            if (peekchar() != ',')
                return;
            advance(1);
        }
    }
    */

    instruction::IdentifierPtr Parser::parse_identifier()
    {
        skipWhiteSpace();
        std::string s;
        char c = peekchar();
        if (isalpha(c) || c == '_' || c == '$')
        {
            s.push_back(c);
            advance(1);
        }
        else
            return nullptr;

        c = peekchar();
        while (c)
        {
            if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == '(' || c == ')')
            {
                s.push_back(c);
                advance(1);
                c = peekchar();
            }
            else
                break;
        }
        if (s.size() > 0)
        {
            instruction::IdentifierPtr ident(new instruction::Identifier());
            ident->name = std::move(s);
            return ident;
        }
        return nullptr;
    }

    instruction::IntegerPtr Parser::parse_number()
    {
        skipWhiteSpace();

        if (stack_.back()->type()  == instruction::INSTRUCTION_BINARY_OPERATOR
        char buf[32];
        char *p = buf;
        char c = peekchar();
        if (c && (isdigit(c) || c == '-'))
        {
            *p = c;
            ++p;
            *p = '\0';
            // Need to make sure next char is a digit, otherwise this is an operator
            if (c == '-')
            {
                c = peekchar(1);
                if (c==0 || isdigit(c) == 0)
                {
                    return nullptr;
                }
            }
            else
                advance(1);
        }
        else
        {
            return nullptr;
        }
        c = peekchar();
        while(c && isdigit(c))
        {
            *p = c;
            ++p;
            *p = '\0';
            advance(1);
            c = peekchar();
        }
        if (p != buf)
        {
            instruction::IntegerPtr inter(new instruction::Integer());
            inter->value = strtol(buf, nullptr, 0);
            return inter;
        }
        return nullptr;
    }

}

