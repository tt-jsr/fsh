#pragma once
#include <string>
#include <vector>

namespace fsh
{
    class Parser
    {
    public:
        Parser();
        void parse(const std::string& s, ExpressionPtr&);
    private:
        void parse_identifier(const char *&s);
        void parse_number(const char *&s);
        void parse_operator(const char *&s);
        void infix2postfix();
        class Token;
    private:
        enum TokenType
        {
            TOKEN_NONE
            ,TOKEN_GT
            ,TOKEN_GTE
            ,TOKEN_LT
            ,TOKEN_LTE
            ,TOKEN_EQ
            ,TOKEN_NEQ
            // For the OPEN and CLOSE pairs, CLOSE must occur after the OPEN
            ,TOKEN_OPEN_PAREN
            ,TOKEN_CLOSE_PAREN
            ,TOKEN_OPEN_BRACKET
            ,TOKEN_CLOSE_BRACKET
            ,TOKEN_OPEN_SUBSCRIPT
            ,TOKEN_CLOSE_SUBSCRIPT
            ,TOKEN_OPEN_BRACE
            ,TOKEN_CLOSE_BRACE
            ,TOKEN_VERTICLE_BAR
            ,TOKEN_DOLLAR_GT
            ,TOKEN_AND
            ,TOKEN_OR
            ,TOKEN_IN
            ,TOKEN_IDENTIFIER
            ,TOKEN_INT
            ,TOKEN_ASSIGN
            ,TOKEN_COMMA
        };
        enum Category
        {
            CATEGORY_NONE
            ,CATEGORY_OPERATOR
            ,CATEGORY_OPERAND
        };

        struct Token
        {
            Token() 
                : token(TOKEN_NONE)
                , prec(0)
                , category(CATEGORY_NONE)
                , prevOutput(nullptr)
            {
            }

            std::string str;
            TokenType token;
            int prec;
            Category category;
        };

        void infix2postfix(std::vector<Token>::iterator&);

        ExpressionPtr expr;
        std::vector<Token> op_stack;
        std::vector<Token> input;
        std::vector<Token> output;
    };
}
