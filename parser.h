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

        typedef fsh::instrusive_ptr<Instruction> InstructionPtr;
        typedef fsh::instrusive_ptr<BinaryOperator> BinaryOperatorPtr;
        typedef fsh::instrusive_ptr<Identifier> IdentifierPtr;
        typedef fsh::instrusive_ptr<Integer> IntegerPtr;
    }


    class Parser
    {
    public:
        Parser();
        ~Parser();
        instruction::InstructionPtr parse(const std::string& s);
    private:
        friend class ::ParserTest;
        char peekchar(int n = 0);
        char getchar();
        const char *peekstr();
        void advance(int);
        void skipWhiteSpace();

        void push(instruction::InstructionPtr);
        instruction::InstructionPtr pop();
        instruction::InstructionType peektype();

        void HandleInstruction(instruction::InstructionPtr in);
        void HandleInstruction(instruction::IdentifierPtr id);
        void HandleInstruction(instruction::IntegerPtr id);
        void HandleInstruction(instruction::BinaryOperatorPtr bop);
        instruction::BinaryOperatorPtr  parse_binary_operator();
        instruction::IdentifierPtr parse_identifier();
        instruction::IntegerPtr parse_number();
        instruction::InstructionPtr parse_expression();
        instruction::InstructionPtr parse_paren();
    private:
        std::vector<instruction::InstructionPtr> stack_;
        std::string input_;
        size_t pos_;
    };
}
