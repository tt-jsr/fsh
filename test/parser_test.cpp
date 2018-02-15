#include <iostream>
#include "parser_test.h"
#include "../common.h"
#include "../parser.h"
#include "../instructions.h"
#include "../element.h"
#include "../machine.h"

void ParserTest::setUp()
{

}

void ParserTest::tearDown()
{

}

void ParserTest::testParseIdentifier()
{
    std::cout << "testParseIdentifier" << std::endl;
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "MyVariable junk";
    fsh::instruction::IdentifierPtr id = parser.parse_identifier();
    
    CPPUNIT_ASSERT(id);

    CPPUNIT_ASSERT(id->name == "MyVariable");

    parser.pos_ = 0;
    parser.input_ = "7MyVariable";
    id = parser.parse_identifier();
    
    CPPUNIT_ASSERT(id.get() != nullptr);
}

void ParserTest::testParseNumber()
{
    std::cout << "testParseNumber" << std::endl;
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "  788678 ";
    fsh::instruction::IntegerPtr p = parser.parse_number();
    
    CPPUNIT_ASSERT(p);

    CPPUNIT_ASSERT(p->value == 788678);

    parser.pos_ = 0;
    parser.input_ = "-1234";
    p = parser.parse_number();
    
    CPPUNIT_ASSERT(p->value == -1234);

    parser.pos_ = 0;
    parser.input_ = "+12345";
    p = parser.parse_number();
    
    CPPUNIT_ASSERT(p.get() != nullptr);
}

void ParserTest::testBinaryOperator()
{
    std::cout << "testBinaryOperator" << std::endl;
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "+junk";
    fsh::instruction::BinaryOperatorPtr p = parser.parse_binary_operator();
    
    CPPUNIT_ASSERT(p);

    CPPUNIT_ASSERT(p->op == fsh::TOKEN_PLUS);
}

void ParserTest::testParseCombo()
{
    std::cout << "testParseCombo" << std::endl;
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "78 + 128 - $var";
    fsh::instruction::IntegerPtr n1 = parser.parse_number();
    fsh::instruction::BinaryOperatorPtr op = parser.parse_binary_operator();
    fsh::instruction::IntegerPtr n2 = parser.parse_number();
    fsh::instruction::BinaryOperatorPtr op2 = parser.parse_binary_operator();
    fsh::instruction::IdentifierPtr id = parser.parse_identifier();
    CPPUNIT_ASSERT(n1->value == 78);
    CPPUNIT_ASSERT(op->op == fsh::TOKEN_PLUS);
    CPPUNIT_ASSERT(n2->value == 128);
    CPPUNIT_ASSERT(op2->op == fsh::TOKEN_MINUS);
    CPPUNIT_ASSERT(id->name == "$var");
}

void ParserTest::testParseParse()
{
    std::cout << "testParseParse" << std::endl;
    fsh::Parser parser;
    fsh::instruction::InstructionPtr inst = parser.parse("27 + 35");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 27 35 +" << std::endl;

    inst = parser.parse("27 + 35 * 3");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 27 35 3 * +" << std::endl;

    inst = parser.parse("( 4 + 7 )");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 4 7 +" << std::endl;

    inst = parser.parse("27 + 35 * 3 * ( 4 + 7 )");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 27 35 3 * 4 7 + * +" << std::endl;
}

void ParserTest::testParseParen()
{
    std::cout << "testParseParen" << std::endl;
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = " ( 128 - 35 )";
    fsh::instruction::InstructionPtr in = parser.parse_paren();
    CPPUNIT_ASSERT(in);
    in->dump(std::cout);
    std::cout << std::endl << "prev output should read 128 35 -" << std::endl;
}

void ParserTest::testExecuteBinaryOperator(const std::string& expr, int64_t expected)
{
    fsh::Parser parser;
    fsh::Machine machine;
    fsh::instruction::InstructionPtr inst = parser.parse(expr);
    fsh::ElementPtr e = machine.Execute(inst);
    CPPUNIT_ASSERT(e->type() == fsh::ELEMENT_TYPE_INTEGER);
    CPPUNIT_ASSERT_EQUAL(expected, e.cast<fsh::Integer>()->value);
}

void ParserTest::testExecuteBinaryOperator()
{
    std::cout << "testExecuteBinaryOperator" << std::endl;
    testExecuteBinaryOperator("27 + 35", 62);
    testExecuteBinaryOperator("27 + 35 * 3", 132);
    testExecuteBinaryOperator("27 + 35 - 9", 53);
    testExecuteBinaryOperator("10889/35", 311);
    testExecuteBinaryOperator("17 + 10889/35", 328);
    testExecuteBinaryOperator("2 * ( 10 + 2 )", 24);
}
