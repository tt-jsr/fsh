#include "parser.h"
#include <cctype>
#include <cstdlib>
#include <iostream>

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
    } opTable = {
        {"$>", TOKEN_DOLLAR_GREATER, 2}   // Should this be zero?
       ,{"||", TOKEN_OR, 2}
       ,{"==", TOKEN_LTEEQUAL, 7}
       ,{"!=", TOKEN_NOT_EQUAL, 7}
       ,{"&&", TOKEN_AND, 2}
       ,{">=", TOKEN_GTE, 7}
       ,{"<=", TOKEN_LTE, 7}
       ,{"[[", TOKEN_OPEN_SUBSCRIPT, 0}
       ,{"]]", TOKEN_CLOSE_SUBSCRIPT, 0}
       ,{"in", TOKEN_IN, 2}
       ,{">", TOKEN_GT, 7}
       ,{"<", TOKEN_LT, 7}
       ,{"=", TOKEN_ASSIGN, 1}
       ,{"{", TOKEN_OPEN_BRACE, 0}
       ,{"}", TOKEN_CLOSE_BRACE, 0}
       ,{"[", TOKEN_OPEN_BRACKET, 0}
       ,{"]", TOKEN_CLOSE_BRACKET, 0}
       ,{"(", TOKEN_OPEN_PAREN, 0}
       ,{")", TOKEN_CLOSE_PAREN, 0}
       ,{",", TOKEN_COMMA, 0}
       ,{"+", TOKEN_PLUS, 10}
       ,{"-", TOKEN_MINUS, 10}
       ,{"+", TOKEN_TIMES, 20}
       ,{"/", TOKEN_DIVIDE, 20}
       ,{"%", TOKEN_MOD, 20}
       ,{0, 0, 0}
    };

    Parser::Parser()
    {}

    void Parser::skipWhiteSpace(const char *&s)
    {
        while (*s == ' ' || *s == '\r' || *s == '\n' || *s == '\t')
            ++s;
    }

    bool Parser::parseOperator(const char *&s)
    {
        skipWhiteSpace(s);
        const char *start = s;
        Operator *op = opTable;
        while(op->buf[0])
        {
            start = s;
            const char *buf = op->buf;
            while(true)
            {
                if (*buf == 0)
                {
                    Token token;
                    token.token = op->token;
                    ++start;
                    token.prec = op->prec;
                    token.category = CATEGORY_OPERATOR;
                    input.push_back(token);
                    s = start;
                    return true;
                }
                if (*start != *buf)
                    return false;
                ++start;
                ++buf;
            }
            ++op;
        }
        return false;
    }

    void Parser::parse(const std::string& expr, ExpressionPtr& exprPtr)
    {
        expr = exprPtr;
        op_stack.clear();
        input.clear();
        const char *s = expr.c_str();
        while (*s)
        {
            parse_operator(s);
            parse_indentifier(s);
            parse_number(s);
        }
        infix2postfix();
    }

    void Parser::tok2rule(Token& tok)
    {
        switch (tok.token)
        {
            case TOKEN_IDENTIFIER:
#if defined(PRINT_STACK)
                std::cout << tok.str << std::endl;
#endif
                expr->add(LoadFieldValueByName(tok.str));
                break;
            case TOKEN_GT:
#if defined(PRINT_STACK)
                std::cout << ">" << std::endl;
#endif
                pRule->add(GreaterThan());
                break;
            case TOKEN_GTE:
#if defined(PRINT_STACK)
                std::cout << ">=" << std::endl;
#endif
                pRule->add(GreaterThanEQ());
                break;
            case TOKEN_LT:
#if defined(PRINT_STACK)
                std::cout << "<" << std::endl;
#endif
                pRule->add(LessThan());
                break;
            case TOKEN_LTE:
#if defined(PRINT_STACK)
                std::cout << "<=" << std::endl;
#endif
                pRule->add(LessThanEQ());
                break;
            case TOKEN_EQ:
#if defined(PRINT_STACK)
                std::cout << "==" << std::endl;
#endif
                pRule->add(Equal());
                break;
            case TOKEN_NEQ:
#if defined(PRINT_STACK)
                std::cout << "!=" << std::endl;
#endif
                pRule->add(NotEqual());
                break;
            case TOKEN_AND:
#if defined(PRINT_STACK)
                std::cout << "&&" << std::endl;
#endif
                pRule->add(And());
                break;
            case TOKEN_OR:
#if defined(PRINT_STACK)
                std::cout << "||" << std::endl;
#endif
                pRule->add(Or());
                break;
            case TOKEN_INT:
#if defined(PRINT_STACK)
                std::cout << tok.str << std::endl;
#endif
                pRule->add(Integer(strtoull(tok.str.c_str(), nullptr, 10)));
                break;
            case TOKEN_DOUBLE:
#if defined(PRINT_STACK)
                std::cout << tok.str << std::endl;
#endif
                pRule->add(Double(strtod(tok.str.c_str(), nullptr)));
                break;
            case TOKEN_VARIABLE:
#if defined(PRINT_STACK)
                std::cout << "$" << tok.str << std::endl;
#endif
                pRule->add(Variable(tok.str));
                break;
            case TOKEN_ASSIGN:
#if defined(PRINT_STACK)
                std::cout << "=" << std::endl;
#endif
                pRule->add(Assignment());
                break;
            case TOKEN_COMMA:
#if defined(PRINT_STACK)
                std::cout << "," << std::endl;
#endif
                pRule->add(Comma());
                break;
        }
    }

    void Parser::infix2postfix()
    {
        std::vector<Token>::iterator it = input.begin();
        while (it != input.end())
        {
            infix2postfix(it);
        }
    }

    void Parser::infix2postfix(std::vector<Token>::iterator& it)
    {
        for (; it != input.end(); ++it)
        {
            Token& tok = *it;
            // Always just send an operand to the output
            if (tok.category == CATEGORY_OPERAND)
            {
                output.push_back(tok);
            }
            if (tok.category == CATEGORY_OPERATOR)
            {
                if (tok.token == TOKEN_COMMA)
                {
                    // end of statement, cleanup. There's probably another one to follow...
                    ++it;
                    break;
                }
                // If the operator stack is empty, push it
                if (op_stack.empty())
                {
                    op_stack.push_back(tok);
                }
                // I'm not using precedence for these, so always just push
                else if (tok.token == TOKEN_OPEN_PAREN
                      || tok.token == TOKEN_OPEN_BRACKET
                      || tok.token == TOKEN_OPEN_BRACE
                      || tok.token == TOKEN_OPEN_SUBSCRIPT)
                {
                    op_stack.push_back(tok);
                }
                else if (tok.token == TOKEN_CLOSE_PAREN
                      || tok.token == TOKEN_CLOSE_BRACKET
                      || tok.token == TOKEN_CLOSE_BRACE
                      || tok.token == TOKEN_CLOSE_SUBSCRIPT)
                {
                    // We just read a CLOSE, pop and output operators until we get to the OPEN
                    while (!op_stack.empty() && op_stack.back().token != (tok.token - 1))
                    {
                        output.push_back(op_stack.back());
                        op_stack.pop_back();
                    }
                    op_stack.pop_back();    // pop the OPEN
                }
                else
                {   
                    // Pop and output operators based on precedence
                    while (!op_stack.empty() && op_stack.back().prec >= tok.prec)
                    {
                        output.push_back(op_stack.back());
                        op_stack.pop_back();
                    }
                    op_stack.push_back(tok);
                }
            }
        }
        // No more input, pop whats left
        while (!op_stack.empty())
        {
            output.push_back(op_stack.back());
            op_stack.pop_back();
        }
    }

    bool Parser::parse_identifier(const char *&start)
    {
        Token tok;
        tok.token = TOKEN_IDENTIFIER;
        tok.category = CATEGORY_OPERAND;

        skipWhiteSpace(s);

        const char *s = start;
        while (*s)
        {
            if (isalnum(*s) || *s == '_' || *s == '.' || *s == '$' || *s == '(' || *s == ')')
            {
                tok.str.push_back(*s);
                ++s;
            }
            else
                break;
        }
        if (tok.str.empty())
        {
            return false;
        }
        input.push_back(tok);
        start = s;
        return false;
    }

    bool Parser::parse_number(const char *&start)
    {
        Token tok;
        tok.token = TOKEN_INT;
        tok.category = CATEGORY_OPERAND;

        skipWhiteSpace(s);

        const char *s = start;
        if (isdigit(*s) || *s == '+' || *s == '-' )
        {
            tok.str.push_back(*s);
            ++s;
        }
        while (*s)
        {
            if (isdigit(*s))
            {
                tok.str.push_back(*s);
                ++s;
            }
            else
                break;
        }
        if (tok.str.empty())
        {
            return false;
        }
        input.push_back(tok);
        start = s;
        return true;
    }

}

