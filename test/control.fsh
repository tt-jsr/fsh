Print["Testing control statements"];

#############################
# Test If

val = 10;
result = If [val > 8
    then
        True;
    else
        False;
];

UnitTest[1];

result = If [val < 10
    then
        True;
    else
        ifval = "boo";
        False;
];

UnitTest[2];
#########################################
# Test While

count = 25;
result = While [val > 5 and count > 0
    then
        count = count - 1;
];

UnitTest[3];

##########################################
# Test For

DefineRecord["fruit", apple, banana, kiwi, orange];
lst = CreateRecord["fruit"];

count = 0;
For[item in lst
then
    count = count + 1;
    rtn = count;
];

UnitTest[4];

