#pragma once
#include "vm/rules.h"
#include "vm/parser.h"
#include "vm/machine.h"

namespace achtung
{
    class ParserTest : public Rules
    {
    public:
        ParserTest()
        {}

        void init(const std::string& rule_name)
        {
            Rules::init(rule_name);
        }

        bool get_field_value_from_message(field_id_t fid, Data& data)
        {
            return false;
        }

        bool get_field_value_from_message(const std::string& field_name, Data& data)
        {
            if (field_name == "cum_qty")
            {
                data.set_uint64(11);
                return true;
            }
            if (field_name == "order_qty")
            {
                data.set_uint64(22);
                return true;
            }
            if (field_name == "ord_status")
            {
                data.set_uint64(2);
                return true;
            }
            if (field_name == "leaves_qty")
            {
                data.set_uint64(33);
                return true;
            }
            return false;
        }

        bool get_enum_value_constant(const std::string& enum_name, Data& data)
        {
            if (enum_name == "ord_status.ORD_STATUS_NEW")
            {
                data.set_uint64(1);
                return true;
            }
            if (enum_name == "ord_status.ORD_STATUS_FILLED")
            {
                data.set_uint64(2);
                return true;
            }
            if (enum_name == "ord_status.ORD_STATUS_CANCELLED")
            {
                data.set_uint64(3);
                return true;
            }
            return false;
        }
    };

    struct Result
    {
        bool expected;
        std::string expr;

        void operator()(Machine& m)
        {
            Data d = m.pop_data();
            std::cout << "testing: " << expr << " : ";
            if (d.get_uint64() != 0 && expected)
                std::cout << "OK" << std::endl;
            if (d.get_uint64() != 0 && expected == false)
                std::cout << "FAILED" << std::endl;
            if (d.get_uint64() == 0 && expected)
                std::cout << "FAILED" << std::endl;
            if (d.get_uint64() == 0 && expected == false)
                std::cout << "OK" << std::endl;
        }
    };

    void parser_test()
    {
        ParserTest rules;

        try
        {
            std::vector<std::pair<std::string, bool> > tests;
            tests.push_back(std::make_pair("cum_qty > 12", false));
            tests.push_back(std::make_pair("cum_qty && 12", true));
            tests.push_back(std::make_pair("cum_qty && 0", false));
            tests.push_back(std::make_pair("cum_qty > 12 || order_qty == 22", true));
            tests.push_back(std::make_pair("cum_qty > 12 || order_qty == 21", false));
            tests.push_back(std::make_pair("cum_qty > 12 && order_qty == 22", false));
            tests.push_back(std::make_pair("cum_qty > 12 && ord_status == ord_status.ORD_STATUS_FILLED", false));
            tests.push_back(std::make_pair("(cum_qty > 12 && ord_status == ord_status.ORD_STATUS_FILLED) && leaves_qty == 34", false));
            tests.push_back(std::make_pair("(cum_qty > 12 && ord_status == ord_status.ORD_STATUS_FILLED) || leaves_qty == 34", false));
            tests.push_back(std::make_pair("(cum_qty > 10 and ord_status == ord_status.ORD_STATUS_FILLED) or leaves_qty == 34", true));
            tests.push_back(std::make_pair("cum_qty > 10 && ((ord_status == ord_status.ORD_STATUS_FILLED) || leaves_qty == 34)", true));
            tests.push_back(std::make_pair("$ans = (cum_qty > 12 && ord_status == ord_status.ORD_STATUS_FILLED) && leaves_qty == 34, $ans == 0", true));

            for (auto& p : tests)
            {
                Machine machine;
                Result result;
                rules.init("parser test");

                result.expr = p.first;
                result.expected = p.second;
                rules.parse(p.first);

                rules.add(Call(result));

                machine.run(rules);
            }
            {
                Machine machine;
                Result result;

                // Test variable assignment
                rules.init("variable test");
                rules.parse("$ans = cum_qty > 10 && ((ord_status == ord_status.ORD_STATUS_FILLED) || leaves_qty == 34)");
                machine.run(rules);
                Data d;
                assert(machine.get_variable("ans", d));
                assert(d.is_integral());
                assert(d.get_uint64() > 0);
                if (d.get_uint64() > 0)
                    std::cout << "Variable success" << std::endl;

                d = machine.peek_data();
                assert(d.is_integral());
                assert(d.get_uint64() > 0);
                if (d.get_uint64() > 0)
                    std::cout << "TOS success" << std::endl;

                // Test variable usage
                rules.init("variable usage");
                rules.parse("$ans > 0");
                machine.run(rules);
                d = machine.peek_data();
                assert(d.is_integral());
                assert(d.get_uint64() > 0);
                if (d.get_uint64() > 0)
                    std::cout << "Variable usage success" << std::endl;
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Parser test exception: " << e.what() << std::endl;
        }
    }
}
