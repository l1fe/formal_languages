#ifndef _TYPES_H_INCLUDED_
#define _TYPES_H_INCLUDED_

#include <string>	// std::string

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

class variable_t {
public:
	variable_t(std::string, v_type);
private:
	v_type var_type;
	std::string name;
};

#endif /* _TYPES_H_INCLUDED_ */
