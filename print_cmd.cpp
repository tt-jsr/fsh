#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
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
                    std::cout << ep->msg;
                }
                break;
            case ELEMENT_TYPE_IDENTIFIER:
                {
                    IdentifierPtr id = e.cast<Identifier>();
                    std::cout << id->name;
                }
                break;
            case ELEMENT_TYPE_NONE:
                {
                    std::cout << "None";
                }
                break;
            case ELEMENT_TYPE_STRING:
                {
                    StringPtr sp = e.cast<String>();
                    std::cout << sp->value;
                }
                break;
            case ELEMENT_TYPE_LIST:
                {
                    std::cout << "List";
                }
                break;
            case ELEMENT_TYPE_HEAD:
                {
                    std::cout << "Head";
                }
                break;
            case ELEMENT_TYPE_FUNCTION_DEFINITION:
                {
                    std::cout << "FunctionDefinition";
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

