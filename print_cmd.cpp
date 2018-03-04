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
                    Print(machine, e);
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
                for (auto & in : lp->items)
                {
                    in->Execute(machine);
                    ElementPtr e = machine.pop_data();
                    Print(machine, e);
                    std::cout << " ";
                }
            }
            break;
        case ELEMENT_TYPE_HEAD:
            {
                HeadPtr hp = e.cast<Head>();
                switch (hp->value)
                {
                    case HEAD_TYPE_COMMAND:
                        std::cout << "COMMAND";
                        break;
                    case HEAD_TYPE_ERROR:
                        std::cout << "ERROR";
                        break;
                    case HEAD_TYPE_LIST:
                        std::cout << "LIST";
                        break;

                }
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

