/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* CMD.cpp                                                  *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iomanip>
#include "DataBase.cpp"
#include "Serial.cpp"

// FUNCTIONS
/********************************************************************************************************************/

extern void DynamicCall(std::string);
extern void call_print(std::string);

bool listFindID(char);
bool verify_pointer(char);
void removeAll(void);
std::string float2str(float);

bool sys_on = 1;

// GLOBALS
/********************************************************************************************************************/

std::list<DataBase> dataBase;
std::list<DataBase>::iterator it;

DataBase *db;

std::string str;

// COMMANDS
/********************************************************************************************************************/

void cmd_gl(int* argv){
    if(listFindID(argv[0])){
	str="l ";
        str+=std::to_string(argv[0]);
        str+=" ";
	if(verify_pointer(0)){
	    str+=float2str(db->getIlluminance(db->getSampleNr()-1));
	}
    }
}

void cmd_gd(int* argv){
    if(listFindID(argv[0])){
        str="d ";
        str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
            str+=float2str(db->getPWM(db->getSampleNr()-1));
    }
}

void cmd_go(int* argv){
    if(listFindID(argv[0])){
        str="o ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
            str+=std::to_string(db->getOccupancy());
    }
}

void cmd_gL(int* argv){
    if(listFindID(argv[0])){
        str="L ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
            str+=std::to_string(db->getIlluminanceLower());
    }
}

void cmd_gO(int* argv){
    if(listFindID(argv[0])){
	str="O ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
            str+=float2str(db->getBackgroundIlluminance());
    }
}

void cmd_gr(int* argv){
    if(listFindID(argv[0])){
	str="r ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
            str+=float2str(db->getIlluminanceControlReference());
    }
}

void cmd_gp(int* argv){
    if(listFindID(argv[0])){
	str="p ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(1))
            str+=float2str(db->getInstantaneousPower());
    }
}

void cmd_gpT (int* argv){
    float ft=0;

    str="p T ";

    for (it=dataBase.begin(); it!= dataBase.end(); ++it){
            db = &*it;
            ft+=db->getInstantaneousPower();
    }
    str+=float2str(ft);
}

void cmd_ge(int* argv){
    if(listFindID(argv[0])){
	str="e ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(1))
            str+=float2str(db->getAccumulatedEnergyConsumption());
    }
}

void cmd_geT(int* argv){
    float ft = 0;

    str="e T ";

    for (it=dataBase.begin(); it!= dataBase.end(); ++it){
            db = &*it;
            ft+=db->getAccumulatedEnergyConsumption();
    }
    str+=float2str(ft);
}

void cmd_gc(int* argv){
    if(listFindID(argv[0])){
        str="c ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0))
	    str+=float2str(db->getAccumulatedComfortError());
    }
}

void cmd_gcT(int* argv){
    float ft = 0;

    str="c T ";

    for (it=dataBase.begin(); it!= dataBase.end(); ++it){
            db = &*it;
            ft+=db->getAccumulatedComfortError();
    }
    str+=float2str(ft);

}

void cmd_gv(int* argv){
    if(listFindID(argv[0])){
        str="v ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(2))
	    str+=float2str(db->getAccumulatedComfortVariance());
    }
}

void cmd_gvT(int* argv){
    float ft = 0;

    str="v T ";

    for (it=dataBase.begin(); it!= dataBase.end(); ++it){
            db = &*it;
            ft+=db->getAccumulatedComfortVariance();
    }
    str+=float2str(ft);
}

void cmd_s(int* argv){
    std::string aux;

    if(listFindID(argv[0])){
        str="ack";

	aux="s";
    	aux+=std::to_string(argv[0]);
    	aux+=std::to_string(argv[1]);
	aux+="#";
    }
    async_write(sp, buffer(aux), serial_write_handler);
}

void cmd_r(int* argv){
    removeAll();
    str="r3#";
    async_write(sp, buffer(str), serial_write_handler);
    str="ack";
}

void cmd_bl(int* argv){
    if(listFindID(argv[0])){
	str="b l ";
	str+=std::to_string(argv[0]);
	str+=" ";
	if(verify_pointer(0)){
	    char i;

	    if(db->getOvBuffer()){
		for(i=db->getSampleNr()-1; i<1/(SAMPLING_TIME); i++)
		    str+=float2str(db->getIlluminance(i))+", ";
		for(i=0; i<db->getSampleNr()-3; i++)
		    str+=float2str(db->getIlluminance(i))+", ";
		str+=float2str(db->getIlluminance(i));
	    }else{
		for(i=0; i<db->getSampleNr()-2; i++)
		    str+=float2str(db->getIlluminance(i))+", ";
		str+=float2str(db->getIlluminance(i));
	    }
	}
    }
}

void cmd_bd(int* argv){
    if(listFindID(argv[0])){
        str="b d ";
	str+=std::to_string(argv[0]);
	str+=" ";
        if(verify_pointer(0)){
            char i;

            if(db->getOvBuffer()){
                for(i=db->getSampleNr()-1; i<1/(SAMPLING_TIME); i++)
                    str+=float2str(db->getPWM(i))+", ";
                for(i=0; i<db->getSampleNr()-3; i++)
                    str+=float2str(db->getPWM(i))+", ";
                str+=float2str(db->getPWM(i));
            }else{
                for(i=0; i<db->getSampleNr()-2; i++)
                    str+=float2str(db->getPWM(i))+", ";
                str+=float2str(db->getPWM(i));
            }
        }
    }
}

void cmd_cl(int* argv){
    if(listFindID(argv[0])){
        str="";
    }
}

void cmd_cd(int* argv){
    if(listFindID(argv[0])){
        str="";
    }
}

void cmd_dl(int* argv){
    if(listFindID(argv[0])){
        str="ack";
    }
}

void cmd_dd(int* argv){
    if(listFindID(argv[0])){
        str="ack";
    }
}

void cmd_gt(int* argv){
    char i;
    if(listFindID(argv[0])){
	str="t ";
	str+=std::to_string(argv[0]);
	str+=" ";
	if(verify_pointer(0)){
	    str+=std::to_string(db->getH(2));
	    str+=":";
	    str+=std::to_string(db->getM(2));
            str+=":";
	    str+=std::to_string(db->getS(2));
	    str+=":";
	    str+=std::to_string(db->getMs(2));
	}
    }
}

void cmd_y(int* argv){
    str="y3";
    str+=std::to_string(sys_on);
    str+="#";
    if(sys_on)
	sys_on = false;
    else
	sys_on = true;
    async_write(sp, buffer(str), serial_write_handler);
    str = "ack";
}

void cmd_h(int* argv){
    str="+---------------+--------------------------------------------------------------------------------+\r\n";
    str+="| Command       | Function                                                                       |\r\n";
    str+="+---------------+--------------------------------------------------------------------------------+\r\n";
    str+="| \"g l <i>\"     | Get current measured illuminance at desk <i>.                                  |\r\n";
    str+="| \"g d <i>\"     | Get current duty cycle at luminaire <i>.                                       |\r\n";
    str+="| \"g o <i>\"     | Get current occupancy state at desk <i>.                                       |\r\n";
    str+="| \"g L <i>\"     | Get current illuminance lower bound at desk <i>.                               |\r\n";
    str+="| \"g O <i>\"     | Get current external illuminance at desk <i>.                                  |\r\n";
    str+="| \"g r <i>\"     | Get current illuminance control reference at desk <i>.                         |\r\n";
    str+="| \"g p <i>\"     | Get instantaneous power consumtion at desk <i>.                                |\r\n";
    str+="| \"g p T\"       | Get instantaneous total power consumption in the system.                       |\r\n";
    str+="| \"g e <i>\"     | Get accumulated energy consumption at desk <i> since the last system restart.  |\r\n";
    str+="| \"g e T\"       | Get total accumulated energy consumption since last system restart.            |\r\n";
    str+="| \"g c <i>\"     | Get accumulated comfort error at desk <i> since last system restart.           |\r\n";
    str+="| \"g c T\"       | Get total comfort error since last system restart.                             |\r\n";
    str+="| \"g v <i>\"     | Get accumulated comfort variance at desk <i> since last system restart.        |\r\n";
    str+="| \"g v T\"       | Get total comfort variance since last system restart.                          |\r\n";
    str+="| \"s <i> <val>\" | Set occupancy state at desk <i>. <val> can be \"0\" or \"1\"                       |\r\n";
    str+="| \"r\"           | Restart system.                                                                |\r\n";
    str+="| \"b <x> <i>\"   | Get last minute buffer of variable <x> of desk <i>. <x> can be \"l\" or \"d\".     |\r\n";
    str+="| \"c <x> <i>\"   | Start stream of realtime variable <x> of desk <i>. <x> can be \"l\" or \"d\".      |\r\n";
    str+="| \"d <x> <i>\"   | Stop stream of realtime variable <x> of desk <i>. <x> can be \"l\" or \"d\"        |\r\n";
    str+="| \"g t <i>\"     | Get elapsed time since last restart at desk <i>                                |\r\n";
    str+="| \"y\"           | Turn on/off the decentralized coordinated control                              |\r\n";
    str+="+---------------+--------------------------------------------------------------------------------+";
}

// AUX FUNCTIONS
/********************************************************************************************************************/

bool listFindID(char id){
    bool flag = false;

    str.erase();

    for (it=dataBase.begin(); it!= dataBase.end(); ++it){
        if (it->getID() == id){
            flag = true;
	    db = &*it;
	    break;
        }
    }
    if(!flag)
	str="Invalid input argument. Check the desk number.";
    return flag;
}

bool verify_pointer(char nr){
    if(db->getTotalSampleNr()>nr)
	return true;
    else{
        str+="NaN";
	return false;
    }
}

void removeAll(void){
    for (it=dataBase.begin(); it!= dataBase.end(); ++it)
	it=dataBase.erase(it);
}

std::string float2str(float ft){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << ft;
    std::string s = stream.str();
    return s;
}
