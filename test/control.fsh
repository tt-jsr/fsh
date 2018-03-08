Print["Testing control statements"];

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
        False;
];

UnitTest[2];


count = 25;
result = While [val > 5 and count > 0
    then
        count = count - 1;
    else
        False;
];

UnitTest[3];


