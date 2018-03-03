printError = &[err:
    If[IsError[$err] 
        then
            Print[ErrorString[$err]]
        else
            None
    ]
]

foo = &[line, writeFunc:
    
err = ReadFile[
        &[line: 
            Print[$line]
            ,True]
        , "common.h"]
printError[$err]

Foo = &[line, pl: 
   Print[$line],
   NextPipe[&pl, $line]
]

PipeLine["cat common.h", "grep ExecutionReport", "cut -c -24", $Foo]

hash = 0
buildHash = &[line, write: 
    parts = Split[$line, " "],
    hash = Part[parts, 1]
]

PipeLine["grep \"build hash\" OC_cme.log", $buildHash]
System["git checkout $hash"]

PipeLine["grep \"build hash\" OC_cme.log", ">junk"]
