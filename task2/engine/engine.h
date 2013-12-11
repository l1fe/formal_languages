#include <map>
#include <string>
#include <vector>

enum v_type {
	vt_int,
	vt_boolean,
	vt_empty
};

enum op_type {
	op_add,
	op_mul,
	op_sub,
	op_div
};

struct value_t {
	v_type var_type;
	union {
		int val_int;
		bool val_boolean;
	};
};

struct variable_t {
	variable_t(std::string name, v_type type) {
		this->name = name;
		this->value.var_type = type;
	}
	
	variable_t(std::string name, value_t value) {
		this->name = name;
		this->value = value;
	}
	
	std::string name;
	value_t value;
};

struct code_block_t {
	code_block_t(code_block_t* parent = NULL) {
		this->parent = parent;
	}

	
	bool declare_variable(std::string name, v_type type) {
		variable_t* var = get_variable_from_block(this, name);
		if (var == NULL) {
			var = new variable_t(name, type);
			var_map.insert(std::make_pair(name, var));
			
			return true;
		}
		
		return false;
	}
	
	variable_t* get_variable_from_block(code_block_t* block, std::string name) {
		if (block == NULL) {
			return NULL;
		}
		
		std::map<std::string, variable_t*>::iterator it = block->var_map.find(name);
		if (it == block->var_map.end()) {
			return get_variable_from_block(block->parent, name);
		} 

		return it->second;
	}
	
	bool assign_variable(std::string name, value_t value) {
		variable_t* var = get_variable_from_block(this, name);
		if (var == NULL) {
			return false;
		}
		
		var->value = value;
		
		return true;
	}
	
	code_block_t* parent;
	std::map<std::string, variable_t*> var_map;	
};

struct declaration_t {
	declaration_t(std::string var_name, v_type var_type) {
		this->var_name = var_name;
		this->var_type = var_type;
	}
	
	std::string var_name;
	v_type var_type;
};

struct expression_t {
	virtual void execute(code_block_t* current_block) = 0;
};

struct statement_t {
	virtual void execute(code_block_t* current_block) = 0;
};

struct assignment_statement_t : public statement_t {
	assignment_statement_t(std::string var_name, expression_t* expr) {
		this->var_name = var_name;
		this->expr = expr;
	}
	
	virtual void execute(code_block_t* current_block) {

	}
	
	std::string var_name;
	expression_t* expr;
};

struct class_t {
	void add_declaration(declaration_t* decl) {
		decls.push_back(decl);
	}
	
	void add_statement(statement_t* stmt) {
		stmts.push_back(stmt);	
	}
	
	std::string name;
	
	std::vector<statement_t*> stmts;
	std::vector<declaration_t*> decls;
	std::vector<expression_t*> exprs;
};