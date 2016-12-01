#include "VariableVector.h"

struct Expr {
	Expr* left;
	Expr* right;
	String value;
};

extern Vector variableDatabase;

int isdigit (const char* c) {
        return((*c >= '0') && (*c <= '9'));
}

int toInt(const char* str) {
	int res = 0;
	int sign = 1;
	int i = 0;
	if(str[0] == '-') { 
		sign = -1;
		i += 1;
	}
	while(str[i] != '\0') {
        	res = res * 10 + str[i] - '0';
		i += 1;
	}
	return sign * res;
}

String toChar(int num) {
	int sign = 1;
	int temp = num;
	int length = 0;
	int length2 = 0;
	while(temp) {
		length += 1;
		temp /= 10;
	}
	if(num < 0) { 
		sign = -1;
		num *= -1;
		int length2 = length + 1;
	}
	char* p = (char*) malloc(length + 1);
	p[length] = '\0';
	while(length) {
 		p[length - 1] = ((num % 10) + '0');
		num /= 10;
		length -= 1;
	}
	if(sign == -1) {
		char* q = (char*) malloc (length2 + 1);
		q[0] = '-';
		int j = 0;
		while(p[j] != '\0') {
			q[j + 1] = p[j];
			j += 1;
		}
		q[j + 1] = '\0';
		const char* ret = q;
		return String(ret);
	}
	const char* ret = p;
	return String(ret);
}

Expr* parseExpr(void) {
	read_next_token();
	String token = next_token();
	if(next_token_type == NUMBER || next_token_type == NAME) {
		Expr* ret = (Expr*) malloc(sizeof(Expr));
		ret->left = nullptr;
		ret->right = nullptr;
		ret->value = token;
		return ret;
	}
	Expr* root = (Expr*) malloc(sizeof(Expr));
	root->value = token;
	if(token == "!" || token == "~") {
		root->left = parseExpr();
		root->right = nullptr;
		return root;
	}
	else {
		root->left = parseExpr();
		root->right = parseExpr();
		return root;
	}
}

String evalExpr(Expr* node) {
	if(node->left == nullptr && node->right == nullptr) { 
		if(isdigit(node->value.c_str())) { return node->value; }
		else { return toChar(lookupSymbol(node->value)); }
	}
	int l = toInt(evalExpr(node->left).c_str());
	if(node->right == nullptr) { 
		if(node->value == "~"){ return toChar(-1 * l); }
		if(node->value == "!"){
			if(l == 0) { return toChar(1); }
			else { return toChar(0); }
		}
	}
	int r = toInt(evalExpr(node->right).c_str());
	if(node->value == "+") { return toChar(l + r); }
	if(node->value == "-") { return toChar(l - r); }
	if(node->value == "*") { return toChar(l * r); }
	if(node->value == "/") { return toChar(l / r); }
	if(node->value == "%") { return toChar(l % r); }
	if(node->value == "<=") {
		if(l <= r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == ">=") {
		if(l >= r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == "<") {
		if(l < r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == ">") {
		if(l > r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == "==") {
		if(l == r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == "!=") {
		if(l != r) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == "&&") {
		if((l != 0) && (r != 0)) { return toChar(1); }
		else { return toChar(0); }
	}
	if(node->value == "||") {
		if((l != 0) || (r != 0)) { return toChar(1); }
		else { return toChar(0); }
	}
}
