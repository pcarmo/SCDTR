/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* Dynamic.cpp                                              *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#include "CMD.cpp"

// defines
#define NR_COMMANDS 25
#define ASCII_I 105
#define ASCII_P 112
#define ASCII_O 111
#define ASCII_L 108

struct funcFormat{
    std::string fncName;
    int arg1;
    int arg2;
};

// functions
funcFormat format_fnc (std::string, char);
int str2int (std::string);

// Cmd functions
extern void cmd_gl  (int* );
extern void cmd_gd  (int* );
extern void cmd_go  (int* );
extern void cmd_gL  (int* );
extern void cmd_gO  (int* );
extern void cmd_gr  (int* );
extern void cmd_gp  (int* );
extern void cmd_gpT (int* );
extern void cmd_ge  (int* );
extern void cmd_geT (int* );
extern void cmd_gc  (int* );
extern void cmd_gcT (int* );
extern void cmd_gv  (int* );
extern void cmd_gvT (int* );
extern void cmd_s   (int* );
extern void cmd_r   (int* );
extern void cmd_h   (int* );
extern void cmd_cl  (int* );
extern void cmd_cd  (int* );
extern void cmd_bl  (int* );
extern void cmd_bd  (int* );
extern void cmd_dl  (int* );
extern void cmd_dd  (int* );
extern void cmd_gt  (int* );
extern void cmd_y   (int* );

// Arduino functions
extern void ard_i (char*);
extern void ard_p (char*);
extern void ard_o (char*);
extern void ard_l (char*);

// Structure CMDs
struct STableItem{
    std::string fncName;
    int argc;
    void (*cmd_fnct)(int*);
};

STableItem cmd_table[] = {
    {"r",   0,   cmd_r},
    {"h",   0,   cmd_h},
    {"gvT", 0, cmd_gvT},
    {"geT", 0, cmd_geT},
    {"gcT", 0, cmd_gcT},
    {"gpT", 0, cmd_gpT},
    {"gl",  1,  cmd_gl},
    {"gd",  1,  cmd_gd},
    {"go",  1,  cmd_go},
    {"gL",  1,  cmd_gL},
    {"gO",  1,  cmd_gO},
    {"gr",  1,  cmd_gr},
    {"gp",  1,  cmd_gp},
    {"ge",  1,  cmd_ge},
    {"gc",  1,  cmd_gc},
    {"gv",  1,  cmd_gv},
    {"cl",  1,  cmd_cl},
    {"cd",  1,  cmd_cd},
    {"bl",  1,  cmd_bl},
    {"bd",  1,  cmd_bd},
    {"dl",  1,  cmd_dl},
    {"dd",  1,  cmd_dd},
    {"gt",  1,  cmd_gt},
    {"s",   2,   cmd_s},
    {"y",   0,   cmd_y},
};

bool flag_inv = false;

void DynamicCall (std::string func_name){
    static int argv[2];
    char argc, i;
    std::string str_aux;
    funcFormat new_func;

    flag_inv = false;
    for(i=0; i<NR_COMMANDS; i++){
 	if(func_name.length()!=1){
	    new_func = format_fnc(func_name, cmd_table[i].argc);
	    if(flag_inv)
                break;
	}else{
	    new_func.fncName = func_name;
	    new_func.arg1 = -1;
	    new_func.arg2 = -1;
	}

        if(new_func.fncName == cmd_table[i].fncName){
	    argv[0] = new_func.arg1;
	    argv[1] = new_func.arg2;
            cmd_table[i].cmd_fnct(argv);
	    break;
        }
    }
    if(i==NR_COMMANDS || flag_inv){
        str="Wrong command, use 'h' to help.";
	flag_inv = false;
    }
}

funcFormat format_fnc (std::string input, char argc){
    char count=0;
    std::string arg1, arg2;

    funcFormat new_func;

    for(std::string::size_type i = 0; i < input.size(); ++i){
        if(input[i] == ' ')
      	    count++;
	if(count > 2)
	    break;
    }

    if(count == 2){
	count = 0;
        if(argc == 0){
	    for(std::string::size_type i = 0; i < input.size(); ++i)
		if(input[i] != ' ')
                    new_func.fncName += input[i];
		new_func.arg1 = -1;
		new_func.arg2 = -1;
        }else if(argc == 1){
	    for(std::string::size_type i = 0; i < input.size(); ++i){
                if(input[i] != ' ' && count <= 1)
                    new_func.fncName += input[i];
                else if(input[i] != ' ' && count == 2)
		    arg1 += input[i];
		else if(input[i] == ' ')
                    count++;
	    }
	    new_func.arg1 = str2int(arg1);
	    new_func.arg2 = -1;

        }else if(argc == 2){
	    for(std::string::size_type i = 0; i < input.size(); ++i){
                if(input[i] != ' ' && count == 0)
                    new_func.fncName += input[i];
                else if(input[i] != ' ' && count == 1)
                    arg1 += input[i];
		else if(input[i] != ' ' && count == 2)
		    arg2 += input[i];
                else if(input[i] == ' ')
                    count++;
            }
	    new_func.arg1 = str2int(arg1);
	    new_func.arg2 = str2int(arg2);
        }
    }else
        flag_inv = true;
    return new_func;
}

int str2int(std::string input){
    int res;
    char *end;

    res = strtol(input.c_str(), &end, 10);
    if (*end != '\0'){
         res = -1;
         flag_inv = true;
    }
    return res;
}

void ArduinoCall(char* input){
    if(input[1] == ASCII_I)
        ard_i(input);
    else if (input[1] == ASCII_P)
        ard_p(input);
    else if (input[1] == ASCII_O)
        ard_o(input);
    else if (input[1] == ASCII_L)
        ard_l(input);
}
