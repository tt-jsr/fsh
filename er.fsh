
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


######################################
# Protobuf print the ER
PrettyPrint = &[er:
    PipeLine[er.raw, "|pfmt"];
];

########################################
# Format the ER
FormatEr = &[er:
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

CurrentBuild = &[pattern:
    files = Command["ls -1t", pattern];
    For [f in files
    then
         Print[f];
         r = System["zgrep \"build \"", f];
         If [r == 0
         then
            Return None;
         else
            Pass;
         ];
    ];
];

CurrentStart = &[pattern:
    files = Command["ls -1t", pattern];
    For [f in files
    then
         Print[f];
         r = System["zgrep \"S T A R T\"", f];
         If [r == 0
         then
            Return None;
         else
            Pass;
         ];
    ];
];

BuildDates = &[pattern:
    files = Command["ls -1rt", pattern];
    For [f in files
    then
        Print[f];
        System["zgrep \"build \"", f];
    ];
];

StartDates = &[pattern:
    files = Command["ls -1rt", pattern];
    For [f in files
    then
        Print[f];
        System["zgrep \"S T A R T\"", f];
    ];
];

ParseErs = &[logfile:
    lst = CreateList[];
    PipeLine[logfile+">", ParseProtobuf[_1, "ExecutionReport", "execreport"], lst];
    Return lst;
];






