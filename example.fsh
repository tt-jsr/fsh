Print ["Hello ", "world"];
foo = &[var : 
    Print["Var: ", var + 1];
];

ans = 5;
junk = If[ans > 4 
    then 
        r = While[ans > 2
            then
                Print[ans];
                ans=ans-1;
            else
                foo[3];
                Print["test"];
                Print[True];
                "complete";
        ];
        Print["r:", r];
        r;
    else 
        Print[False];
        "no dice";
];
Print[junk];
lst = {1,2,3,4};
Print [lst];



