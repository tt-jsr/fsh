count = 0;
stageOne = &[line:
    If [Find[line, "Token"] > 0
    then    
        count = count + 1;
    else
        None;
    ];
    line;
];

stageTwo = &[line2:
    Print["stage two ", line2];
    line2;
];

Try[
    file = OpenFile["common.h", "r"];
    fileOut = OpenFile["junk", "w"];
    PipeLine[file, stageOne, fileOut, stageTwo];
    Print ["Number of \"Tokens\": ", count];
catch
    Print [_exception];
];
