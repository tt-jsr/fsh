stageOne = &[line:
    If [Find[line, "Token"] > 0
    then    
        Print ["***", line, "***"];
    else
        Print[line];
    ];
    True;
];

stageTwo = &[line2:
    #Print["stage two ", line2];
    line2;
];

#Try[
    #file = OpenFile["common.h", "r"];
    #fileOut = OpenFile["junk", "w"];
    #PipeLine[file, stageOne, fileOut, stageTwo];
    #Print ["Number of \"Tokens\": ", count];
#catch
    #Print [_exception];
#];

Try [
    ReadProcess[stageOne, "cat common.h | grep Token"];
catch
    Print["Exception: ", _exception];
];
