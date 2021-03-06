#include "String.h"

struct VariableNode {
	String name;
	int value;
};

struct Vector {
	int len;
	VariableNode* var;
};

extern Vector variableDatabase;

void makeNewVariable(String variable_name, int val) {
	variableDatabase.len += 1;
	VariableNode* p = (VariableNode*) malloc(variableDatabase.len * sizeof(VariableNode));
	VariableNode add;
	add.name = variable_name;
	add.value = val;
	for (int k = 0; k < variableDatabase.len - 1; k += 1) {
		p[k] = variableDatabase.var[k];
	}
	if(variableDatabase.var != nullptr) { free (variableDatabase.var); }
	p[variableDatabase.len - 1] = add;
	variableDatabase.var = p;
}

bool checkExistence(String variable_name) { 
	for(int i = 0; i < variableDatabase.len; i += 1) {
		if(variable_name == (variableDatabase.var[i]).name) { return true; }
	}
	return false;
}

void updateVariable(String variable_name, int val) {
	for(int i = 0; i < variableDatabase.len; i += 1) {
		if(variable_name == (variableDatabase.var[i]).name) { 
			(variableDatabase.var[i]).value = val;
			return;
		}
	}
}

int lookupSymbol(String variable_name) {
	for(int i = 0; i < variableDatabase.len; i += 1) {
		if(variable_name == (variableDatabase.var[i]).name) { 
			return (variableDatabase.var[i]).value;
		}
	}
	return 0;
}

Vector* makeNewLocal(Vector* store, String variable_name, int val) {
	store->len += 1;
	VariableNode* p = (VariableNode*) malloc(store->len * sizeof(VariableNode));
	VariableNode add;
	add.name = variable_name;
	add.value = val;
	for (int k = 0; k < store->len - 1; k += 1) {
		p[k] = store->var[k];
	}
	if(store->var != nullptr) { free (store->var); }
	p[store->len - 1] = add;
	store->var = p;
	return store;
}
