#include <vector>
#include <iostream>
#include "common.h"
#include "element.h"
#include "machine.h"

namespace fsh
{
    void Print(Machine& machine, ElementPtr e)
    {
        switch(e->type())
        {
        case ELEMENT_TYPE_INTEGER:
            {
                IntegerPtr n = e.cast<Integer>();
                std::cout << n->value;
            }
            break;
        case ELEMENT_TYPE_BOOLEAN:
            {
                BooleanPtr n = e.cast<Boolean>();
                if (n->value)
                    std::cout << "True";
                else
                    std::cout << "False";
            }
            break;
        case ELEMENT_TYPE_FLOAT:
            {
                FloatPtr f = e.cast<Float>();
                std::cout << f->value;
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
                ElementPtr e;
                if (machine.get_variable(id->value, e))
                {
                    if (e->IsIdentifier())
                        std::cout << e.cast<Identifier>()->value;
                    else
                        Print(machine, e);
                }
                else
                    std::cout << id->value;
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
                ListPtr lp = e.cast<List>();
                for (size_t idx = 0; idx < lp->items.size(); ++idx)
                {
                    ElementPtr item = lp->items[idx];
                    std::cout << "'";
                    Print(machine, item);
                    std::cout << "'";
                    if (idx < lp->items.size()-1)
                        std::cout << ",";
                }
            }
            break;
        case ELEMENT_TYPE_FUNCTION_DEFINITION:
            {
                std::cout << "FunctionDefinition";
            }
            break;
        default:
            std::cout << "(" << e->type() << ")";
            break;
        }
    }

    ElementPtr Print(Machine& machine, std::vector<ElementPtr>& args)
    {
        for (auto& e : args)
        {
            Print(machine, e);
        }
        std::cout << std::endl;
        return MakeNone();
    }

}

