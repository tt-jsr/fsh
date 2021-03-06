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


#####################################
# Test function calling function
foo = &[arg1, arg2:
    arg1 - arg2;
];

foofoo = &[arg:
    v = foo[10, arg];
];


ans = foofoo[2];
UnitTest[6];

#######################################
# Test attributes and Bind
foo = &[line: 
    UnitTest[7];
];

foo["hello" : attr1->"cruel", attr2->"world"];

############################
#test bind
foo = &[testno, arg1, arg2, arg3:
    UnitTest[testno];
];

f2 = foo[_1, _2, _3, "world"];
f2[8, "hello", "cruel"];

f2 = foo[9, _2, _1, "world"];
f2["hello", "cruel"];

#
####################################
## Test bind with an attribute
f2 = foo[ 10, _2, _1, "world" : attr->"value"];
f2["hello", "cruel"];


###################################
# Test global


gf_global = "gf_hello";

fg1 = &[
    UnitTest[11];
];

fg1[];

fg2 = &[
    global gf_global;
    gf_global = "junk";
    UnitTest[12];
];

fg2[];

UnitTest[13];

Print["functions done"];

