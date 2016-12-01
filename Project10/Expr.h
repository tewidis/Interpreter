/*
 * Expr.h
 *
 *  Created on: Jul 24, 2014
 *      Author: chase
 */
#ifndef _Expr_h
#define _Expr_h 1

#include <stdint.h>
#include <vector>
#include "String.h"

/* these are all the types of expressions I used.
 * do not change this enum, or you may break the library
 */
enum ExprType { // DO NOT EDIT
	CONST,
	VAR,

	ADD,
	SUB,
	MUL,
	DIV,
	MOD,

	LOGICAL_AND,
	LOGICAL_OR,

	LESS,
	GREATER,
	LTE,
	GTE,
	EQUAL,
	NOT_EQUAL,

	NEG,
	LOGICAL_NOT,

	CALL,

	UNKNOWN
/* DO NOT EDIT THIS ENUM!!!! */
};

/* this is not really the struct that I used.
 * my struct had a lot more components in it than this one.
 */
struct Expr { // do not edit
	ExprType type;
	/* if you're using my library, then you should not change this struct */
};

/* these are the three functions I provide to you
 * parseExpr reads tokens from the input stream and returns an expression parse tree
 * evalExpr has one param which must be a pointer to the root of a parse tree
 *     evalExpr then evaluates the expression and returns the integer result
 * destroyExpr is optional. if called, it deallocates all the nodes in an expression parse tree
 * Depending on your implementation of the CallExpr struct, destroyExpr may break
 */
Expr* parseExpr(void);
int32_t evalExpr(Expr* e);
void destroyExpr(Expr* e); // not required. USE WITH CAUTION!

/*
 * you will need to add components to this struct. The ExprType
 * component must not be changed and must be the first component in your
 * CallExpr struct. Your struct only has to work for call expressions...
 * it will never be used for +, *, -, / or anything else (it will
 * only be used for call). So, you don't need to put any components
 * in your struct unless you use them for your implementation of
 * functions.
 */
struct CallExpr {
	ExprType type;

	/* you can add additional components to this struct, as long as
	 * the ExprType remains the first component.
	 */
};



/* You must write this function -- it is not provided as part of the solution.
 * When evaluating expressions, the solution may have to evaluate a function call.
 * e.g., the expression could be "+ x call doit args sgra"
 * In order to evaluate that expression, evalExpr needs to know what the value of x is
 * (you must write the function lookupSymol for that) and evalExpr must call the doit function
 * (you must write the function evalFunCall to do that).
 */

/* These are the functions that you must write.
 * * lookupSymbol must return the current value for the variable "var_name".
 * * parseCallExpr must return a pointer to an expression tree. The expression tree must be on the heap
 * and the root of the tree must be a CallExpr node with the ExprType set to CALL
 * * evalFunCall takes a pointer to a CallExpr node and must call the function and then return the value
 * that the function returned.
 */
int32_t lookupSymbol(String var_name);
Expr* parseCallExpr(void);
int32_t evalFunCall(CallExpr* e);

#endif /* _Expr_h */
