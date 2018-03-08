Print["running test1.fsh"];
str = "hello";
int = 1234;
flt = 12.27;
bt= True;
bf= False;
none = None;

__str2_3 = "__str2_3";

#test assignment of an idntifier
s = "hello world";
s2 = s;

#test negation of an int variable
#n = -int;    # This does not work!!

# test relational operators
Print ["Testing relational operators"];
v1 =  7 == 7;
v2 = 3 != 4;
v3 = 3!=3;
v4 = 7 < 12;
v5 = 12 < 7;
v6 = 12 <= 7;
v7 = 12 <= 15;
v8 = 12 <= 12;
v9 = 8 > 4;
v10 = 4 > 8;
v11 = 4>=8;
v12 = 8>=4;
v13 = 8>= 8;

# Test logical
Print["test logical operators"];
op1 = True and False;
op2 = True and True;
op3 = True or False;
op4 = False or False;

# Test precedence

m1 = 3 + 4 * 12;
m2 = (3 + 4) * 12;
m3 = 3 * -6;

