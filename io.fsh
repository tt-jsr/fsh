stageOne = &[line:
    Print["stage one ", line];
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
catch
    Print [_exception];
];
