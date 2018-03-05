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
    PipeLine[file, stageOne, stageTwo];
catch
    Print [_exception];
];
