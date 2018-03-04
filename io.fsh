stageOne = &[phandle, line:
    Print["stage one"];
    NextStage[phandle, line];
];

stageTwo = &[phandle, line:
    Print["stage two"];
    NextStage[phandle, line];
];

foo = &[
    Throw["Ouch"];
];

Try[
    Print ["Line one"];
    Print ["line two"];
    foo[];
    Print ["line three"];
catch
    Print[_exception];
];

