#include <cctype>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>
#include "common.h"
#include "parser.h"
#include "instructions.h"

//#define PRINT_STACK 1

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
    {}

    void Parser::HandleInstruction(instruction::BinaryOperator *bop)
    {
        if (bop == nullptr)
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
                instruction::BinaryOperator *op = static_cast<instruction::BinaryOperator *>(pop());
                if (op->prec >= bop->prec)
                {
                    bop->lhs = op;
                    push(bop);
                }
                else
                {
                    instruction::Instruction *inst = op->rhs;
                    op->rhs = bop;
                    bop->lhs = inst;
                    push(bop);
                }
            }
            break;
        case instruction::INSTRUCTION_NONE:
            //TODO throw exception
            break;
        }
    }

    void Parser::HandleInstruction(instruction::Integer *i)
    {
        if (i == nullptr)
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
                instruction::BinaryOperator *bop = static_cast<instruction::BinaryOperator *>(pop());
                bop->rhs = i;
                push(bop);
            }
            break;
        case instruction::INSTRUCTION_NONE:
            push(i);
            break;
        }
    }

    void Parser::HandleInstruction(instruction::Identifier *id)
    {
        if (id == nullptr)
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
                instruction::BinaryOperator *bop = static_cast<instruction::BinaryOperator *>(pop());
                bop->rhs = id;
                push(bop);
            }
            break;
        case instruction::INSTRUCTION_NONE:
            push(id);
            break;
        }
    }

    void Parser::HandleInstruction(instruction::Instruction *in)
    {
        if (in == nullptr)
            return;
        switch(in->type())
        {
        case instruction::INSTRUCTION_BINARY_OPERATOR:
            HandleInstruction((instruction::BinaryOperator *)in);
            break;
        case instruction::INSTRUCTION_IDENTIFIER:
            HandleInstruction((instruction::Identifier *)in);
            break;
        case instruction::INSTRUCTION_INTEGER:
            HandleInstruction((instruction::Integer *)in);
            break;
        }
    }

    instruction::Instruction * Parser::parse(const std::string& s)
    {
        input_ = s;
        pos_ = 0;
        while(peekchar() != 0)
        {
            HandleInstruction(parse_identifier());
            HandleInstruction(parse_number());
            HandleInstruction(parse_binary_operator());
        }
        assert (stack_.size() == 1);
        return pop();
    }


    char Parser::peekchar()
    {
        if (pos_ >= input_.size())
            return 0;
        return input_[pos_];
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

    void Parser::push(instruction::Instruction *p)
    {
        assert(p != nullptr);
        //std::cout << "=== push " << p->type() << std::endl;
        stack_.push_back(p);
    }

    instruction::Instruction *Parser::pop()
    {
        assert(stack_.size() > 0);
        if (stack_.size() == 0)
            return nullptr;
        instruction::Instruction *p = stack_.back();
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

    instruction::BinaryOperator * Parser::parse_binary_operator()
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
                instruction::BinaryOperator *bop = new instruction::BinaryOperator();
                bop->op = op->token;
                bop->prec = op->prec;
                advance(op->length);
                return bop;
            }
            ++op;
        }
        return nullptr;
    }

    void Parser::parse_identifierSequence(std::vector<instruction::Identifier *>& vec)
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

    instruction::Identifier * Parser::parse_identifier()
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
            instruction::Identifier *ident = new instruction::Identifier();
            ident->name = std::move(s);
            return ident;
        }
        return nullptr;
    }

    instruction::Integer * Parser::parse_number()
    {
        skipWhiteSpace();

        char buf[32];
        char *p = buf;
        char c = peekchar();
        if (c && isdigit(c) || c == '-')
        {
            *p = c;
            ++p;
            *p = '\0';
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
            instruction::Integer *inter = new instruction::Integer();
            inter->value = strtol(buf, nullptr, 0);
            return inter;
        }
        return nullptr;
    }

}

