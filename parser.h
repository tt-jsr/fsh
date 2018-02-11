#pragma once
#include <string>
#include <vector>

namespace fsh
{
    class Identifier;
    class Instruction;
    class BinaryOperator;
    class Integer;

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
        ,TOKEN_PLUS
        ,TOKEN_MINUS
        ,TOKEN_MULTIPLY
        ,TOKEN_DIVIDE
        ,TOKEN_MOD
    };

    class Parser
    {
    public:
        Parser();
        Instruction *parse(const std::string& s);
    private:
        char peekchar();
        char getchar();
        const char *peekstr();
        void advance(int);
        void skipWhiteSpace();
        void push(Instruction *);
        Instruction *pop();
        BinaryOperator * parse_binary_operator();
        Identifier *parse_identifier();
        Integer *parse_number();
        Instruction *parse_expression();
        void parse_identifierSequence(std::vector<Identifier *>& vec);
    private:

        std::vector<Instruction *> stack_;
        std::string input_;
        size_t pos_;
    };
}
