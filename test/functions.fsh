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
