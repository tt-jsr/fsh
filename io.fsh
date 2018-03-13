example1 = &[
    stageOne = &[line:
        If [Find[line, "Token"] > 0
        then    
            line = "**** Token ***";
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
        file = OpenProcess["cat common.h", "r"];
        fileOut = OpenProcess["cut -c -20", "w"];
        PipeLine[file, stageOne, fileOut, stageTwo];
    catch
        Print [_exception];
    ];
];

example2= &[
    parse = &[line:
        parts = Split[line, " "];
        parts;
    ];
    
    print = &[lst:
        Print[lst];
        lst;
    ];
    ls = OpenProcess["ls -l", "r"];
    PipeLine[ls, parse, print];
];

example2[];

