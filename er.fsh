
LogCleanList = CreateList[];
Append[LogCleanList, "INSTRUMENT_STORE"];
Append[LogCleanList, "INSTRUMENT_STORE"];
Append[LogCleanList, "darwin."];
Append[LogCleanList, "CassandraMessageStore"];
Append[LogCleanList, "| LBM |"];
Append[LogCleanList, "| lbm |"];
Append[LogCleanList, "| ump:"];
Append[LogCleanList, "lbm_cpp.h"];
Append[LogCleanList, "JSONtopb.h"];
Append[LogCleanList, "Cassandra upload"];
Append[LogCleanList, "TTUSApiHandler"];
Append[LogCleanList, "ttus_api\.cpp"];
Append[LogCleanList, "openssl_initializer.cpp:57"];
Append[LogCleanList, "LBM channel source (PENDING) created for connection_id"];
Append[LogCleanList, "LBM channel source created for connection_id"];
Append[LogCleanList, "Deleting pending topic="];
Append[LogCleanList, "TTUS WSS.*heartbeat"];
#Append[LogCleanList, "TTUS WSS: recv"];
Append[LogCleanList, "Throttle rate for the primary"];
Append[LogCleanList, "Deleting source topic="];
Append[LogCleanList, "CONNECTION-[0-9]* | Ignoring"];
Append[LogCleanList, "CONNECTION-[0-9]* | This does not look like"];
Append[LogCleanList, "session_manager_inl"];


DefineRecord["execreport", order_id, exec_type, ord_status, user_id, account_id, raw];

ordersById = CreateMap[];
orders = CreateList[];
ordersByAccount = CreateMap[];
ordersByExecType = CreateMap[];


######################################
# Protobuf print the ER
prettyPrint = &[er:
    PipeLine[er.raw, "|pfmt"];
];

########################################
# Format the ER
formatEr = &[er:
    If [er != None
    then
        s = "";
        For[f in er[[:-1]]
        then
            s = s + Format["{0}, ", f];
        ];
        Return s;
    else
        Return PIPELINE_RESTART;
    ];
];

# Given a record, create a Pair (field, record)
RecordPair = &[record, field:
    Return CreatePair[record[[field]], record];
];

# Given an a Record, fieldid and value, reject records that match
RecordFilter = &[record, field, value:
    Try [
        If [record[[field]] == value
        then
            Return PIPELINE_RESTART;
        else
            Return record;
        ];
    catch
        Return PIPELINE_RESTART;
    ];
];

# Given an a Record, fieldid and value, select records that match
RecordSelect = &[record, field, value:
    Try [
        If [record[[field]] == value
        then
            Return record;
        else
            Return PIPELINE_RESTART;
        ];
    catch
        Return PIPELINE_RESTART;
    ];
];


# Parse execution reports
parseEr = &[line:
    er = ParseProtobuf[line, "ExecutionReport", "execreport"];
    If [er != None
    then
        Return er;
    else
        Return PIPELINE_RESTART;
    ];
];

BuildDates = &[pattern:
    PipeLine[Format["ls -1rt {0}", pattern], Format["|zgrep -H \"build \" {0}", _1] ];
];

StartDates = &[pattern:
    files = Format["ls -1rt {0}", pattern];
    grepFunc = Format["|zgrep -H \"S T A R T\" {0}", _1];
    PipeLine[files, grepFunc];
];

ParseErs = &[logfile:
    lst = CreateList[];
    PipeLine[logfile+">", parseEr, lst];
    Return lst;
];





