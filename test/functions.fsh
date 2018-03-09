Print["Testing Functions"];

foo = &[
    True;
];

result = foo[];
UnitTest[1];

foo = &[arg1:
    # Make sure arg1 is visible in this scope
    UnitTest[2];
];
foo["hello"];

#make sure arg1 is not visible in ths scope
UnitTest[3];

# test try catch
foo = &[
    var = 6 * 7;

   Throw["test throw"];
];

Try[
    foo[];
catch
    UnitTest[4]; # catch the string and make sure we can't see 'var'
];
UnitTest[4]; # catch the string and make sure we can't see 'var'

# Test lambda
foo = &[func:
    func["lambda"];
];

var = foo[&[msg:
    msg;
    ]
];

UnitTest[5];

foo = &[arg1, arg2:
    arg1 - arg2;
];

fooBind = &[arg:
    v = foo[10, arg];
];

ans = fooBind[2];
UnitTest[6];
