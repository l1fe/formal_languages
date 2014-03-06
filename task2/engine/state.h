#ifndef _STATE_H_INCLUDED_
#define _STATE_H_INCLUDED_

#include <string>	// std::string

#define TEMP_VAR_NAME_PATTERN "var"

class translator_state {
public:
        translator_state();

        std::string get_next_temp_var_name();
	std::string get_current_temp_var_name();
	int get_index();
	std::string generate_temp_var_name_with_index(int);

	int line_number;
private:
        int temp_var_index;
};

#endif /* _STATE_H_INCLUDED_ */
