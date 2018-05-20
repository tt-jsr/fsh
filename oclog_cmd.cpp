#include <unordered_map>
#include <map>
#include <sstream>
#include <iostream>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"

namespace fsh
{
    /*
2018-01-11 19:24:17.983675 | INFO | 0000000 | 006 |  | ExecutionReport |  order_id=415d82f5-9826-4907-88f1-ccb513ec7436 | secondary_cl_ord_id=1515698625 | secondary_order_id= | sender_sub_id="perf_opID" order_id { lo=3924020744745185672 hi=524993269961219393 } appl_id { lo=1686536136252407172 hi=13207411707150298735 } cl_ord_id=5 account="perfAcct001" ord_type=ORD_TYPE_LIMIT price=7500 order_qty=1 leaves_qty=1 cum_qty=0 manual_order_indicator=true cust_order_capacity=CUST_ORDER_CAPACITY_ALL_OTHER order_capacity=ORDER_CAPACITY_PROPRIETARY side=SIDE_BUY exec_type=EXEC_TYPE_PENDING_NEW ord_status=ORD_STATUS_PENDING_NEW time_sent=1515698657983661418 transact_time=1515698657983661354 secondary_order_id="" secondary_cl_ord_id="1515698625" instrument_id=7253407562321969715 security_desc="6EZ9" user_id=4167 account_id=6974 connection_id=791 order_sequence=1 exch_order_assoc="R94001" source=SOURCE_INVALID correlation_cl_ord_id="1515698625" curr_user_id=4167 time_in_force=TIME_IN_FORCE_DAY order_received_oc=1515698657983593377 time_sent_exchange=1515698657983644115 market_id=TT_MARKET_ID_CME synthetic_type=SYNTHETIC_TYPE_DEFAULT exch_order_qty=1 exch_leaves_qty=0 exch_cum_qty=0 algo_type=ALGO_TYPE_UNKNOWN exec_inst=EXEC_INST_WORK contingency_type=CONTINGENCY_TYPE_NONE order_properties=0 client_time_sent=1515698657983558663 broker_id=21 sender_location_id="US,IL" calculate_epiq=false externally_created=false fix_cl_ord_id="" fix_orig_cl_ord_id="" giveup="mem"
*/


    ElementPtr ParseProtobuf(Machine& machine, StringPtr& line, StringPtr& find, StringPtr& record)
    {
        if (line->value.find(find->value) == std::string::npos)
            return MakeNone();
        FieldMap_t *pFields = machine.get_field_map(record->value);
        if (pFields == nullptr)
        {
            std::stringstream strm;
            strm << record->value << " definition does not exist";
            throw std::runtime_error(strm.str());
        }
        std::vector<ElementPtr> args;
        args.push_back(MakeString(record->value));
        ListPtr lst = CreateRecord(machine, args);

        size_t pos = 0;
        for (auto& f : *pFields)
        {
            if (f.first == "raw")
                lst->items[f.second] = line;
            else
            {
                std::string s = f.first + "=";

                pos = line->value.find(s);
                if (pos == std::string::npos)
                {
                    return MakeNone();
                }
                pos += s.size();
                const char *p = &line->value[pos];
                StringPtr v(MakeString(""));
                while (*p && *p != ' ')
                {
                    v->value.push_back(*p);
                    ++p;
                }
                lst->items[f.second] = v;
            }
        }
        return lst;
    }

    ElementPtr ParseProtobuf(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr line = GetString(machine, args, 0);
        if (!line)
            throw std::runtime_error("ParseProtobuf expects a line");

        StringPtr find = GetString(machine, args, 1);
        if (!find)
            throw std::runtime_error("ParseProtobuf expects a search string");

        StringPtr record = GetString(machine, args, 2);
        if (!record)
            throw std::runtime_error("ParseProtobuf expects a record type");

        return ParseProtobuf(machine, line, find, record);
    }

    ElementPtr OCLogFilter(Machine& machine, std::vector<ElementPtr>& args)
    {
        StringPtr line = GetString(machine, args, 0);
        if (!line)
            throw std::runtime_error("OCLogFilter expects a line");

        ListPtr rejectList = GetList(machine, args, 1);
        if (!rejectList)
            throw std::runtime_error("OCLogFilter expects a a list of reject texts");

        PipeLineActionPtr plRestart = MakePipeLineAction();
        plRestart->action = PipeLineAction::RESTART;
        
        for(ElementPtr e : rejectList->items)
        {
            if (e->IsString())
            {
                StringPtr r = e.cast<String>();
                if (line->value.find(r->value) != std::string::npos)
                {
                    return plRestart;
                }
            }
        }
        return line;
    }
}
