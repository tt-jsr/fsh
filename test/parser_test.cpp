#include <iostream>
#include "parser_test.h"
#include "../common.h"
#include "../parser.h"
#include "../instructions.h"
#include "../element.h"

void ParserTest::setUp()
{

}

void ParserTest::tearDown()
{

}

void ParserTest::testParseIdentifier()
{
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "MyVariable junk";
    fsh::instruction::Identifier *id = parser.parse_identifier();
    
    CPPUNIT_ASSERT(id);

    CPPUNIT_ASSERT(id->name == "MyVariable");

    parser.pos_ = 0;
    parser.input_ = "7MyVariable";
    id = parser.parse_identifier();
    
    CPPUNIT_ASSERT(id == nullptr);
}

void ParserTest::testParseNumber()
{
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "  788678 ";
    fsh::instruction::Integer *p = parser.parse_number();
    
    CPPUNIT_ASSERT(p);

    CPPUNIT_ASSERT(p->value == 788678);

    parser.pos_ = 0;
    parser.input_ = "-1234";
    p = parser.parse_number();
    
    CPPUNIT_ASSERT(p->value == -1234);

    parser.pos_ = 0;
    parser.input_ = "+12345";
    p = parser.parse_number();
    
    CPPUNIT_ASSERT(p == nullptr);
}

void ParserTest::testBinaryOperator()
{
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "+junk";
    fsh::instruction::BinaryOperator *p = parser.parse_binary_operator();
    
    CPPUNIT_ASSERT(p);

    CPPUNIT_ASSERT(p->op == fsh::TOKEN_PLUS);
}

void ParserTest::testParseCombo()
{
    fsh::Parser parser;
    parser.pos_ = 0;
    parser.input_ = "78 + 128 - $var";
    fsh::instruction::Integer *n1 = parser.parse_number();
    fsh::instruction::BinaryOperator *op = parser.parse_binary_operator();
    fsh::instruction::Integer *n2 = parser.parse_number();
    fsh::instruction::BinaryOperator *op2 = parser.parse_binary_operator();
    fsh::instruction::Identifier *id = parser.parse_identifier();
    CPPUNIT_ASSERT(n1->value == 78);
    CPPUNIT_ASSERT(op->op == fsh::TOKEN_PLUS);
    CPPUNIT_ASSERT(n2->value == 128);
    CPPUNIT_ASSERT(op2->op == fsh::TOKEN_MINUS);
    CPPUNIT_ASSERT(id->name == "$var");
}

void ParserTest::testParseExpression()
{
    fsh::Parser parser;
    fsh::instruction::Instruction *inst = parser.parse("27 + 35");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 27 + 35" << std::endl;

    inst = parser.parse("27 + 35 * 3");
    inst->dump(std::cout);
    std::cout << std::endl << "prev output should read 27 + 35 * 3" << std::endl;
}
