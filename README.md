Using the token definitions from lexical-analyzer, and parser from, we can now construct an interpreter for our language.


To build an interpreter, we must add three things:
1. A class to represent values
2. Functions to do runtime checks, perform evaluations, and return any calculated values
3. A symbol table mapping identifiers to values

The Val class will contain prototypes for operator functions that has been implemented. The operator functions shall perform type checking. To perform evaluations, we must create a virtual function called Eval() and implement it for every class. Eval should take a reference to the symbol table and should return the Val thatresults from the evaluation. You will be given some notes about Eval in your startercode.

The following items describe the language. In places where a runtime type check is required, [RT] has been added as a reminder. Failing a runtime type check halts the interpreter.
1.The language contains two types: integer and string.
2.The PLUS MINUS STAR and SLASH operators are left associative.
3.The EQ operator is right associative.
4.A Repeat statement evaluates the Expr. The Expr must evaluate to an integer [RT]. If the integer is nonzero, then the Stmt is executed, otherwise it is not. The value of theExpr is decremented by 1 each repetition. In other words “repeat 5 begin println “hello”end” will print “hello” 5 times.
5.A PrintStmt evaluates the Expr and prints its value.
6.A PrintlnStmt evaluates the Expr and prints its value, followed by a newline character.
7.The type of an IDENT is the type of the value assigned to it.
8.The EQ operator assigns values to variables. It evaluates the Expr on the right hand sideand saves its value in memory associated with the left hand side (an IDENT). If the IDENT does not exist, it is created. If the IDENT already exists, its value is replaced.
9.The PLUS and MINUS operators in Expr represent addition and subtraction.
10.The STAR and SLASH operators in Prod represent multiplication and division.
11.It is an error if a variable is used before a value is assigned to it [RT].
12.Addition is defined between two integers (the result being the sum) or two strings (theresult being the concatenation).
13.Subtraction is defined between two integers (the result being the difference) or two strings (the result being removal of the first instance of the second string from the firststring, if any).
14.Multiplication is defined between two integers (the result being the product) or for an integer and a string (the result being the string repeated integer times).
15.Division is defined between two integers.
16.Performing an operation with incorrect types or type combinations is an error [RT].
17.Multiplying a string by a negative integer is an error [RT].
18.A Repeat statement whose Expr is not integer typed is an error [RT]

For runtime errors, your program should throw an exception indicating a RUNTIME ERROR.The exception should be caught and printed, and the program should stop.

The result of an unsuccessful parse is a set of error messages printed by the parse functions. Ifthe parse fails, the program should stop after the parse function returns.

If the parse succeeds, and if there are no errors on declared variables, Eval the parse tree.There may be runtime errors during the evaluation. When an error is encountered, the program should generate an error message in the format RUNTIME ERROR at N: description, where N is the line number where the error occurred (stored in the parse tree node!) and description is some descriptive text about the error.
