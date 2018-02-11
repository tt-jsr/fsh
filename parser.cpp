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
        int token;
        int prec;
    } opTable[] = {
       {"||", TOKEN_OR, 2}
       ,{"!=", TOKEN_NEQ, 7}
       ,{"&&", TOKEN_AND, 2}
       ,{">=", TOKEN_GTE, 7}
       ,{"<=", TOKEN_LTE, 7}
       //,{"[[", TOKEN_OPEN_SUBSCRIPT, 0}
       //,{"]]", TOKEN_CLOSE_SUBSCRIPT, 0}
       //,{"in", TOKEN_IN, 2}
       ,{">", TOKEN_GT, 7}
       ,{"<", TOKEN_LT, 7}
       ,{"=", TOKEN_ASSIGN, 1}
       //,{"{", TOKEN_OPEN_BRACE, 0}
       //,{"}", TOKEN_CLOSE_BRACE, 0}
       //,{"[", TOKEN_OPEN_BRACKET, 0}
       //,{"]", TOKEN_CLOSE_BRACKET, 0}
       //,{"(", TOKEN_OPEN_PAREN, 0}
       //,{")", TOKEN_CLOSE_PAREN, 0}
       ,{",", TOKEN_COMMA, 0}
       ,{"+", TOKEN_PLUS, 10}
       ,{"-", TOKEN_MINUS, 10}
       ,{"+", TOKEN_MULTIPLY, 20}
       ,{"/", TOKEN_DIVIDE, 20}
       ,{"%", TOKEN_MOD, 20}
       ,{0, 0, 0}
    };

    Parser::Parser()
    {}

    Instruction * Parser::parse(const std::string& s)
    {
        input_ = s;
        pos_ = 0;
        while(true)
        {
            Instruction *inst = parse_expression();
            if (inst == nullptr)
            {
                assert(stack_.size() == 1);
                return pop();
            }
            push(inst);
        }
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

    void Parser::push(Instruction *p)
    {
        stack_.push_back(p);
    }

    Instruction *Parser::pop()
    {
        assert(stack_.size() > 0);
        if (stack_.size() == 0)
            return nullptr;
        Instruction *p = stack_.back();
        stack_.pop_back();
        return p;
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

    BinaryOperator * Parser::parse_binary_operator()
    {
        skipWhiteSpace();
        Operators *op = opTable;
        const char *s = peekstr();
        while(op->buf[0])
        {
            const char *buf = op->buf;
            while(true)
            {
                if (*buf == 0)
                {
                    BinaryOperator *bop = new BinaryOperator();
                    bop->op = op->token;
                    bop->prec = op->prec;
                    advance(strlen(op->buf));
                    return bop;
                }
                if (*s != *buf)
                {
                    return nullptr;
                }
                ++s;
                ++buf;
            }
            ++op;
        }
        return nullptr;
    }

    Instruction * Parser::parse_expression()
    {
        Instruction *inst = parse_identifier();
        if (inst)
            return inst;
        inst = parse_number();
        if (inst)
            return inst;
        /*
        inst = parse_string();
        if (inst)
            return list;
        inst = parse_list();
        if (inst)
            return inst;
        */

        BinaryOperator *bop = parse_binary_operator();
        if (bop)
        {
            bop->lhs = pop();
            bop->rhs = parse_expression();
            return bop;
        }
        return nullptr;
    }

    void Parser::parse_identifierSequence(std::vector<Identifier *>& vec)
    {
        while(true)
        {
            Identifier *ident = parse_identifier();
            if (ident == nullptr)
                return;
            vec.push_back(ident);
            skipWhiteSpace();
            if (peekchar() != ',')
                return;
            advance(1);
        }
    }

    Identifier * Parser::parse_identifier()
    {
        skipWhiteSpace();
        std::string s;
        while (char c = peekchar())
        {
            if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == '(' || c == ')')
            {
                s.push_back(c);
                advance(1);
            }
            else
                break;
        }
        if (s.size() > 0)
        {
            Identifier *ident = new Identifier();
            ident->name = std::move(s);
            return ident;
        }
        return nullptr;
    }

    Integer * Parser::parse_number()
    {
        skipWhiteSpace();

        char buf[32];
        char *p = buf;
        char c = peekchar();
        if (isdigit(c) || c == '-')
        {
            *p = c;
            ++p;
            advance(1);
            c = peekchar();
        }
        while(c = peekchar())
        {
            *p = c;
            ++p;
            advance(1);
        }
        if (p != buf)
        {
            Integer *inter = new Integer();
            inter->value = strtol(buf, nullptr, 0);
            return inter;
        }
        return nullptr;
    }

}

