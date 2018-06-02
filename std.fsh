
Edit = &[
    System["vim junk.fsh"];
    LazyImport["junk.fsh"];
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

Command = &[
    Attribute["asString", False];
    cmd = "";
    For [c in _args_
    then
        cmd = cmd + " " + If[c then ToString[c]; else "";];
    ];
    If [asString == True
    then
        r = "";
        PipeLine[cmd + "|", r];
    else
        r = CreateList[];
        PipeLine[cmd + "|", r];
    ];
    Return r;
];

import er.fsh
