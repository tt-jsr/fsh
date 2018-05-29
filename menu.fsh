DefineRecord["menurec", text, obj];

AddMenu = &[menu, txt, o:
    l = CreateRecord["menurec"];
    l.text = txt;
    l.obj = o;
    Append[menu, l];
];

DoMenu = &[menu:
    While [True
    then
        count = 1;
        For[m in menu
        then
            s = Format["{0} {1}", count, m.text];
            Print[s];
            count = count + 1;
        ];
        Print[];
        s = Input["Enter >"];
        If [s =="q"
        then
            Return None;
        else
            idx = Int[s];
            Try[
                Return menu[[idx-1]];
            catch
                Print[_exception];
            ];
        ];
    ];
];

