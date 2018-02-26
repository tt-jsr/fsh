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
       ,{"=", 1, TOKEN_ASSIGNMENT, 1}
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

    /*
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
*/

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

    /*
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
    */

    void Parser::tokenize()
    {
        tokens_.clear();
        while (pos_ != input_.size())
        {
            Token tok = get_token();
            if (tok.token == TOKEN_NONE)
                return;
            tokens_.push_back(tok);
        }
    }

    Token Parser::get_token()
    {
        Token tok = parse_identifier();
        if (tok.token != TOKEN_NONE)
            return tok;
        tok = parse_integer();
        if (tok.token != TOKEN_NONE)
            return tok;
        tok = parse_symbol();
        if (tok.token != TOKEN_NONE)
            return tok;
        if (tok.token == TOKEN_NONE)
        {
            tok.token = TOKEN_UNKNOWN;
            tok.text.push_back(peekchar());
            advance(1);
        }
        return tok;
    }

    void Parser::skipWhiteSpace()
    {
        char c = peekchar();
        while (c == ' ' || c == '\r' || c == '\t')
        {
            advance(1);
            c = peekchar();
        }
    }

    Token Parser::parse_symbol()
    {
        Token tok;
        skipWhiteSpace();
        char c = peekchar();
        if (c == '+')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_PLUS;
            advance(1);
        }
        else if (c == '-')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_MINUS;
            advance(1);
        }
        else if (c == '*')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_MULTIPLY;
            advance(1);
        }
        else if (c == '/')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_DIVIDE;
            advance(1);
        }
        else if (c == '%')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_MOD;
            advance(1);
        }
        else if (c == '<')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_LT;
            advance(1);
            if ((c = peekchar())== '=')
            {
                tok.token = TOKEN_LTE;
                tok.text.push_back(c);
            advance(1);
            }
        }
        else if (c == '=')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_ASSIGNMENT;
            advance(1);
            if ((c = peekchar())== '=')
            {
                tok.token = TOKEN_EQ;
                tok.text.push_back(c);
                advance(1);
            }
        }
        else if (c == '>')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_GT;
            advance(1);
            if ((c = peekchar())== '=')
            {
                tok.token = TOKEN_GTE;
                tok.text.push_back(c);
                advance(1);
            }
        }
        else if (c == '[')
        {
            return parse_sequence();
        }
        else if (c == '&')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_AMP;
            advance(1);
            if ((c = peekchar())== '&')
            {
                tok.token = TOKEN_AND;
                tok.text.push_back(c);
                advance(1);
            }
        }
        else if (c == '(')
        {
            return parse_sequence();
        }
        else if (c == '{')
        {
            return parse_sequence();
        }
        else if (c == ',')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_COMMA;
            advance(1);
        }
        else if (c == ':')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_COLON;
            advance(1);
        }
        else if (c == '"')
        {
            return parse_sequence();
        }
        else if (c == '|')
        {
            tok.text.push_back(c);
            tok.token = TOKEN_VERTICLE_BAR;
            advance(1);
            if ((c = peekchar()) == '|')
            {
                tok.text.push_back(c);
                tok.token = TOKEN_OR;
                advance(1);
            }
        }
        else if (c == '\n')
        {
            tok.token = TOKEN_NL;
            tok.text.push_back('\n');
            advance(1);
        }
        return tok;
    }

    Token Parser::parse_sequence()
    {
        Token tok;
        int nest = 0;
        while(true)
        {
            char c = peekchar();
            switch (c)
            {
            case '[':
                if (nest == 0)
                    tok.token = TOKEN_BRACKET;
                if (nest > 0)
                    tok.text.push_back(c);
                ++nest;
                advance(1);
                if (peekchar() == '[')
                {
                    tok.token = TOKEN_SUBSCRIPT;
                    ++nest;
                    advance(1);
                }
                break;
            case '{':
                if (nest == 0)
                    tok.token = TOKEN_BRACE;
                if (nest > 0)
                    tok.text.push_back(c);
                advance(1);
                ++nest;
                break;
            case '(':
                if (nest == 0)
                    tok.token = TOKEN_PAREN;
                if (nest > 0)
                    tok.text.push_back(c);
                ++nest;
                advance(1);
                break;
            case ']':
                if (nest > 1)
                    tok.text.push_back(c);
                --nest;
                advance(1);
                break;
            case '}':
                if (nest > 1)
                    tok.text.push_back(c);
                --nest;
                advance(1);
                break;
            case ')':
                if (nest > 1)
                    tok.text.push_back(c);
                --nest;
                advance(1);
                break;
            case '"':
                advance(1);
                tok.text.push_back(c);
                c = peekchar();
                while (c != '"')
                {
                    tok.text.push_back(c);
                    advance(1);
                    c = peekchar();
                }
                tok.text.push_back(c);
                advance(1);
                break;
            case 0:
                return tok;
                break;
            default:
                tok.text.push_back(c);
                advance(1);
                break;
            }
            if (nest == 0)
                return tok;
        }
    }

    Token Parser::parse_identifier()
    {
        Token tok;
        skipWhiteSpace();

        char c = peekchar();
        if (isalpha(c) || c == '_' || c == '$')
        {
            tok.text.push_back(c);
            advance(1);
        }
        else
            return tok;

        c = peekchar();
        while (c)
        {
            //TODO: Should return NONE if '(' doesnot follow '$'
            if (isalnum(c) || c == '_' || c == '.' || c == '$' || c == '(' || c == ')')
            {
                tok.text.push_back(c);
                advance(1);
                c = peekchar();
            }
            else
                break;
        }
        if (tok.text.size() > 0)
        {
            tok.token = TOKEN_IDENTIFIER;
        }
        if (tok.text == "def")
            tok.token = TOKEN_DEFINE;
        return tok;
    }

    Token Parser::parse_integer()
    {
        Token tok;
        skipWhiteSpace();

        char c = peekchar();
        while(c && isdigit(c))
        {
            tok.text.push_back(c);
            advance(1);
            c = peekchar();
        }
        if (tok.text.size())
        {
            tok.token = TOKEN_INTEGER;
        }
        return tok;
    }
}

