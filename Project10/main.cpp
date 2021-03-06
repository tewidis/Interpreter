/* Name: Tanner Widis
 * TA: Karim Serhan
 * Date 12/5/2014
 * Project 10
 */

#include <stdio.h>
#include <stdlib.h>
#include "Parse.h"
#include "ParseTree.h"

struct statement;
struct StatementVector;
struct function;
struct funcVec;
struct ExpressionVector;

int executeFunction(String);
StatementVector* makeStructure(StatementVector*);
int evaluateStructure(StatementVector*);
void addFunction(function*);

struct statement {
	String command;
	String var_name;
	String funcname;
	ExpressionVector* expression;
	StatementVector* body1;
	StatementVector* body2;
};

struct ExpressionVector {
	int len;
	Expr* exp;
};

struct StatementVector {
	int len;
	statement* stmt;
};

struct function {
	String name;
	Vector* parameters;
	StatementVector* body;
};

struct funcVec {
	int len;
	function* func;
};

Vector variableDatabase;
funcVec FunctionDatabase;

ExpressionVector* addExpression(ExpressionVector* vec, Expr* exp) {
	vec->len += 1;
	Expr* p = (Expr*) malloc(vec->len * sizeof(Expr));
	for (int k = 0; k < vec->len - 1; k += 1) {
		p[k] = vec->exp[k];
	}
	if(vec->exp != nullptr) { free (vec->exp); }
	p[vec->len - 1] = *exp;
	vec->exp = p;
	return vec;
}

StatementVector* makeStructure(StatementVector* prog) {
	read_next_token();
	String state = next_token();
	if(state == "od" || next_token_type == END || state == "fi" || state == "else" || state == "nufed") { return prog; }
	if(state == "//") {
		skip_line();
		return makeStructure(prog);
	}
	if(state == "defun") {
		read_next_token();
		function* newfunc = (function*) malloc(sizeof(function));
		newfunc->name = next_token();
		read_next_token();//params
		String temp = peek_next_token();
		int v = 0;
		while(temp != "smarap") {
			Vector* locals = (Vector*) malloc(sizeof(Vector));
			locals->len = 0;
			locals->var = nullptr;
			locals = makeNewLocal(locals, temp, 0);
			v += 1;
			read_next_token();
			temp = peek_next_token();
		}
		read_next_token();//smarap
		StatementVector* j = (StatementVector*) malloc(sizeof(StatementVector));
		j->len = 0;
		j->stmt = nullptr;
		newfunc->body = makeStructure(j);
		addFunction(newfunc);
		return makeStructure(prog);
	}
	statement* newstmt = (statement*) malloc(sizeof(statement));
	newstmt->command = state;
	newstmt->var_name = "";
	newstmt->funcname = "";
	newstmt->expression = nullptr;
	newstmt->body1 = nullptr;
	newstmt->body2 = nullptr;
	ExpressionVector* exps = (ExpressionVector*) malloc(sizeof(ExpressionVector));
	exps->len = 0;
	exps->exp = nullptr;
	Expr* newexpr = (Expr*) malloc(sizeof(Expr));
	if(state == "text") {
		read_next_token();
		newstmt->var_name = next_token();
	}
	else if(state == "output") {
		String callfunc = peek_next_token();
		if(callfunc == "call") {
			read_next_token();//call
			read_next_token();
			newstmt->funcname = next_token();
			read_next_token();//args
			String temp = peek_next_token();
			int v = 0;
			while(temp != "sgra") {
				exps = addExpression(exps, parseExpr());
				newstmt->expression = exps;
				v += 1;
				temp = peek_next_token();
			}
			read_next_token();//sgra
		}
		else { 
			newexpr = parseExpr();
			exps->exp = newexpr;
			newstmt->expression = exps;
		}
	}
	else if(state == "var") {
		read_next_token();
		newstmt->var_name = next_token();
		String callfunc = peek_next_token();
		if(callfunc == "call") {
			read_next_token();//call
			read_next_token();
			newstmt->funcname = next_token();
			read_next_token();//args
			String temp = peek_next_token();
			int v = 0;
			while(temp != "sgra") {
				exps = addExpression(exps, parseExpr());
				newstmt->expression = exps;
				v += 1;
				temp = peek_next_token();
			}
			read_next_token();//sgra
		}
		else { 
			newexpr = parseExpr();
			exps->exp = newexpr;
			newstmt->expression = exps;
		}
	}
	else if(state == "set") {
		read_next_token();
		newstmt->var_name = next_token();
		String callfunc = peek_next_token();
		if(callfunc == "call") {
			read_next_token();
			read_next_token();
			newstmt->funcname = next_token();
			read_next_token();//args
			String temp = peek_next_token();
			int v = 0;
			while(temp != "sgra") {
				exps = addExpression(exps, parseExpr());
				newstmt->expression = exps;
				v += 1;
				temp = peek_next_token();
			}
			read_next_token();//sgra
		}
		else { 
			newexpr = parseExpr();
			exps->exp = newexpr;
			newstmt->expression = exps;
		}
	}
	else if(state == "do") {
		newexpr = parseExpr();
		exps->exp = newexpr;
		newstmt->expression = exps;
		StatementVector* j = (StatementVector*) malloc(sizeof(StatementVector));
		j->len = 0;
		j->stmt = nullptr;
		newstmt->body1 = makeStructure(j);
	}
	else if(state == "if") {
		newexpr = parseExpr();
		exps->exp = newexpr;
		newstmt->expression = exps;
		StatementVector* j = (StatementVector*) malloc(sizeof(StatementVector));
		j->len = 0;
		j->stmt = nullptr;
		newstmt->body1 = makeStructure(j);
		String token = next_token();
		if(token == "else") {
			StatementVector* k = (StatementVector*) malloc(sizeof(StatementVector));
			k->len = 0;
			k->stmt = nullptr;
			newstmt->body2 = makeStructure(k);
		}
	}
	else if(state == "return") {
		newexpr = parseExpr();
		exps->exp = newexpr;
		newstmt->expression = exps;
	}
	prog->len += 1;
	statement* p = (statement*) malloc(prog->len * sizeof(statement));
	for (int l = 0; l < prog->len - 1; l += 1) {
		p[l] = prog->stmt[l];
	}
	p[prog->len - 1] = *newstmt;
	prog->stmt = p;
	return makeStructure(prog);
}

int evaluateStructure(StatementVector* vec) {
	for(int i = 0; i < vec->len; i += 1) {
		if(vec->stmt[i].command == "text") { printf("%s", (vec->stmt[i].var_name).c_str()); }
		else if(vec->stmt[i].command == "output") { 
			if(vec->stmt[i].funcname != "") { printf("%d", executeFunction(vec->stmt[i].funcname)); }
			else { printf("%d", toInt(evalExpr(vec->stmt[i].expression->exp).c_str())); }
		}
		else if(vec->stmt[i].command == "var") {
			if(!checkExistence(vec->stmt[i].var_name)) { 
				if(vec->stmt[i].funcname != "") { 
					makeNewVariable(vec->stmt[i].var_name, executeFunction(vec->stmt[i].funcname)); 
				}
				else { makeNewVariable(vec->stmt[i].var_name, toInt(evalExpr(vec->stmt[i].expression->exp).c_str())); }
			}
			else { printf("variable %s incorrectly re-initialized\n", (vec->stmt[i].var_name).c_str()); }
		}
		else if(vec->stmt[i].command == "set") {
			if(checkExistence(vec->stmt[i].var_name)) { 
				if(vec->stmt[i].funcname != "") { 
					updateVariable(vec->stmt[i].var_name, executeFunction(vec->stmt[i].funcname));
				}
				else { updateVariable(vec->stmt[i].var_name, toInt(evalExpr(vec->stmt[i].expression->exp).c_str())); }
			}
			else { 
				printf("variable %s not declared\n", (vec->stmt[i].var_name).c_str());
				if(vec->stmt[i].funcname != "") { 
					makeNewVariable(vec->stmt[i].var_name, executeFunction(vec->stmt[i].funcname)); 
				}
				else { makeNewVariable(vec->stmt[i].var_name, toInt(evalExpr(vec->stmt[i].expression->exp).c_str())); }
			}
		}
		else if(vec->stmt[i].command == "do") {
			while(toInt(evalExpr(vec->stmt[i].expression->exp).c_str())) {
				evaluateStructure(vec->stmt[i].body1);
			}
		}
		else if(vec->stmt[i].command == "if") {
			if(toInt(evalExpr(vec->stmt[i].expression->exp).c_str())) { evaluateStructure(vec->stmt[i].body1); }
			else if(vec->stmt[i].body2 != nullptr) { evaluateStructure(vec->stmt[i].body2); }
		}
		else if(vec->stmt[i].command == "return") {
			return toInt(evalExpr(vec->stmt[i].expression->exp).c_str());
		}
	}
	return 0;
}

void addFunction(function* newfunc) {
	FunctionDatabase.len += 1;
	function* p = (function*) malloc(FunctionDatabase.len * sizeof(function));
	for (int k = 0; k < FunctionDatabase.len - 1; k += 1) {
		p[k] = FunctionDatabase.func[k];
	}
	if(FunctionDatabase.func != nullptr) { free(FunctionDatabase.func); }
	p[FunctionDatabase.len - 1] = *newfunc;
	FunctionDatabase.func = p;
}

int executeFunction(String funcname) {
	for(int i = 0; i < FunctionDatabase.len; i += 1) {
		if(funcname == FunctionDatabase.func[i].name) {
			return evaluateStructure(FunctionDatabase.func[i].body);
		}
	}
	return 0;
}

int main(void) {
	//printf("TEST\n");
	set_input("Alisha.blip");
	StatementVector* program = (StatementVector*) malloc(sizeof(StatementVector));
	program->len = 0;
	program->stmt = nullptr;
	program = makeStructure(program);
	evaluateStructure(program);
}
