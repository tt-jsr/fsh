Print["Testing string functions"];

s = "   trim this   ";
tl = TrimLeft[s];
tr = TrimRight[s];
t = Trim[s];

UnitTest[1];

s = "col1 col2 col3 col4";
parts = Split[s, " "];

s = "  col1   col2   ";
parts2 = Split[s, " "];
UnitTest[2];


s = "Hello World";
s1 = Part[s, 1];
s2 = Part[s, 2:4];
s3 = Part[s, :3];
s4 = Part[s, 3:];
UnitTest[3];

