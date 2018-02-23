#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
    ElementPtr If(Machine& machine, std::vector<ElementPtr>& args)
    {
        
    }

    ElementPtr Print(Machine& machine, std::vector<ElementPtr>& args)
    {
        std::cout << "Print: " ;
        for (auto& e : args)
        {
            switch(e->type())
            {
            case ELEMENT_TYPE_INTEGER:
                {
                    IntegerPtr n = e.cast<Integer>();
                    std::cout << " " << n->value;
                }
                break;
            case ELEMENT_TYPE_FLOAT:
                {
                    FloatPtr f = e.cast<Float>();
                    std::cout << " " << f->value;
                }
                break;
            case ELEMENT_TYPE_ERROR:
                {
                    ErrorPtr ep = e.cast<Error>();
                    std::cout << ep->msg << std::endl;
                }
                break;
            case ELEMENT_TYPE_IDENTIFIER:
                {
                    IdentifierPtr id = e.cast<Identifier>();
                    std::cout << id->name << std::endl;
                }
                break;
            case ELEMENT_TYPE_NONE:
                {
                    std::cout << "None" << std::endl;
                }
                break;
            case ELEMENT_TYPE_STRING:
                {
                    std::cout << "String" << std::endl;
                }
                break;
            case ELEMENT_TYPE_LIST:
                {
                    std::cout << "List" << std::endl;
                }
                break;
            case ELEMENT_TYPE_HEAD:
                {
                    std::cout << "Head" << std::endl;
                }
                break;
            case ELEMENT_TYPE_FUNCTION_DEFINITION:
                {
                    std::cout << "FunctionDefinition" << std::endl;
                }
                break;
            default:
                std::cout << " Print[] no handler ";
                break;
            }
        }
        std::cout << std::endl;
        ElementPtr e = MakeNone();
        return e;
    }
}

