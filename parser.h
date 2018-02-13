#pragma once
#include <string>
#include <vector>

class ParserTest;

namespace fsh
{
    namespace instruction
    {
        class Identifier;
        class Instruction;
        class BinaryOperator;
        class Integer;
    }


    class Parser
    {
    public:
        Parser();
        instruction::Instruction *parse(const std::string& s);
    private:
        friend class ::ParserTest;
        char peekchar();
        char getchar();
        const char *peekstr();
        void advance(int);
        void skipWhiteSpace();

        void push(instruction::Instruction *);
        instruction::Instruction *pop();
        instruction::InstructionType peektype();

        void HandleInstruction(instruction::Instruction *in);
        void HandleInstruction(instruction::Identifier *id);
        void HandleInstruction(instruction::Integer *id);
        void HandleInstruction(instruction::BinaryOperator *bop);
        instruction::BinaryOperator * parse_binary_operator();
        instruction::Identifier *parse_identifier();
        instruction::Integer *parse_number();
        instruction::Instruction *parse_expression();
        void parse_identifierSequence(std::vector<instruction::Identifier *>& vec);
    private:

        std::vector<instruction::Instruction *> stack_;
        std::string input_;
        size_t pos_;
    };
}
