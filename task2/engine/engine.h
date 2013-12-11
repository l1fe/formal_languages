#include <map>
#include <string>
#include <vector>
#include <iostream>

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

struct expression_t {
	virtual value_t evaluate() = 0;
};



struct literal_expression_t : public expression_t {
	literal_expression_t(value_t value) {
		this->value = value;
	}
	
	virtual value_t evaluate() {
		switch (value.var_type) {
			case vt_int:
				std::cout << value.val_int;
				break;
			case vt_boolean:
				std::cout << value.val_boolean;
				break;
		}
		
		return value;	
	}
	
	value_t value;
};

struct binary_expression_t : public expression_t {
	binary_expression_t(op_type operation_type, expression_t* arg1, expression_t* arg2) {
		this->operation_type = operation_type;
		this->arg1 = arg1;
		this->arg2 = arg2;
	}
	
	virtual value_t evaluate() {
		switch (operation_type) {
			case op_add:
				std::cout << "add ";
				break;
			case op_sub:
				std::cout << "sub ";
				break;
			case op_mul:
				std::cout << "mul ";
				break;
			case op_div:
				std::cout << "div ";
				break;
		}
		
		value_t arg1_val = arg1->evaluate();
		
		std::cout << " ";
		
		value_t arg2_val = arg2->evaluate();
		
		value_t x;
		return x;
	}
	
	op_type operation_type;
	expression_t* arg1;
	expression_t* arg2;
};

struct var_ref_expression_t: public expression_t {
	var_ref_expression_t(std::string name) {
		this->name = name;
	}
	
	virtual value_t evaluate() {
		//TODO
		
		value_t x;
		return x;
	}
	
	std::string name;
};

struct statement_t {
	virtual void execute(code_block_t* current_block) = 0;
};

struct declaration_statement_t : public statement_t {
	declaration_statement_t(std::string var_name, v_type var_type) {
		this->var_name = var_name;
		this->var_type = var_type;
	}
	
	virtual void execute(code_block_t* current_block) {
		std::cout << "let " << var_name << " " << 0 << std::endl;
	}
	
	std::string var_name;
	v_type var_type;
};


struct assignment_statement_t : public statement_t {
	assignment_statement_t(std::string var_name, expression_t* expr) {
		this->var_name = var_name;
		this->expr = expr;
	}
	
	virtual void execute(code_block_t* current_block) {
		expr->evaluate();
		
		std::cout << " " << var_name << std::endl;
	}
	
	std::string var_name;
	expression_t* expr;
};

struct class_t {
	class_t() {
		main_block = new code_block_t(NULL);
	}
	
	void add_statement(statement_t* stmt) {
		stmts.push_back(stmt);	
	}
	
	void run() {
		for (int i = 0; i < stmts.size(); ++i) {
			stmts[i]->execute(main_block);
		}
	}
	
	code_block_t* main_block;
	
	std::string name;
	
	std::vector<statement_t*> stmts;
};