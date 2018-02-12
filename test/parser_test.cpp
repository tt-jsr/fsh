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
    parser.input_ = "MyVariable";
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
    parser.input_ = "788678";
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
