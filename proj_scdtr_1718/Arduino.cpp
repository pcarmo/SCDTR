/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* Arduino.cpp                                              *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#include <iostream>
#include <cmath>
#include <string>
#include "Dynamic.cpp"

#define SEC_IN_MS 1000
#define MIN_IN_MS 60*SEC_IN_MS
#define HOUR_IN_MS 3600*SEC_IN_MS

bool findID(char);
int getTime(char);

int recv_desk = -1;
bool recebeu = false;

std::string aux1, aux2, aux3;

void ard_i(char* input){
    if(!findID(char(input[2]))){
	DataBase newDataBase;
	newDataBase.setID(char(input[2]));
	newDataBase.setSampleNr(0);
	newDataBase.setTotalSampleNr(0);
	newDataBase.setAccumulatedEnergyConsumption(0);
	newDataBase.setAccumulatedComfortError(0);
	newDataBase.setAccumulatedComfortVariance(0);
	newDataBase.setH(0);
	newDataBase.setM(0);
	newDataBase.setS(0);
	newDataBase.setMs(0);
	newDataBase.setAuxComfortError(0);
	dataBase.push_back(newDataBase);
	printf("Luminary %d just entered in the system.\n", (unsigned char)input[2]);
    }
}

void ard_p(char* input){
    float ft;
    char c_aux;

    if(findID(input[2])){
        ft=(float)input[3]+input[4]/100.0; // lum
        db->setIlluminance(db->getSampleNr(), ft);

	ft=(float)input[5]+input[6]/100.0; // dc
        db->setPWM(db->getSampleNr(), ft);

	ft=(float)input[7]+input[8]/100.0; // reference
	db->setIlluminanceControlReference(ft);
	//printf("Ref[%d]=%.2f\n", input[2],ft);

	ft=(float)input[9]+input[10]/100.0; // external lum
        db->setBackgroundIlluminance(ft);

	db->setH(input[11]);
	db->setM(input[12]);
	db->setS(input[13]);
	int ms=input[14]<<8|input[15];
	db->setMs(ms);

	float tempo = getTime(2)-getTime(1);
	float pwm = db->getPWM(db->getSampleNr())/100.0;
	if(db->getTotalSampleNr()>= 1){ // instantaneous power consumtion
	    c_aux = db->getSampleNr()-1;
	    if(c_aux<0) c_aux = 1/(SAMPLING_TIME)+c_aux;
	    ft = (db->getPWM(c_aux)/100.0)*((getTime(2)-getTime(1))/1000.0);
	    db->setInstantaneousPower(ft);
	    db->setAccumulatedEnergyConsumption(db->getAccumulatedEnergyConsumption() + ft); // accumulated energy consumption
	}

	ft=db->getIlluminanceControlReference()-db->getIlluminance(db->getSampleNr());
	if(ft<0)ft=0;
	db->setAuxComfortError(db->getAuxComfortError()+ft);
	ft = db->getAuxComfortError()/(db->getTotalSampleNr()+1);
        db->setAccumulatedComfortError(ft); // accumulated comfort error

	if(db->getTotalSampleNr()>= 2){ // accumulated comfort variance
	    c_aux = db->getSampleNr() - 2;
            if(c_aux<0)
		c_aux = 1/(SAMPLING_TIME)+c_aux;

	    ft = db->getIlluminance(c_aux);

	    c_aux = db->getSampleNr() - 1;
	    if(c_aux<0)
                c_aux = 1/(SAMPLING_TIME)+c_aux;

	    ft-= 2*db->getIlluminance(c_aux);

	    ft+= db->getIlluminance(db->getSampleNr());
	    ft = abs(ft);
	    db->setAuxComfortVariance(db->getAuxComfortVariance()+ft);

	    ft = db->getAuxComfortVariance()/((db->getTotalSampleNr()+1)*pow(SAMPLING_TIME,2));
	    db->setAccumulatedComfortVariance(ft);
	}

	// |luminance |pwm |timestamp
	aux1="| c l ";
	aux1+=std::to_string(db->getID());
	aux1+=" ";
	aux1+=float2str(db->getIlluminance(db->getSampleNr()));
        aux2="| c d ";
        aux2+=std::to_string(db->getID());
        aux2+=" ";
        aux2+=float2str(db->getPWM(db->getSampleNr()));
	aux3="| t ";
	aux3+=std::to_string(input[11]);
	aux3+=":";
	aux3+=std::to_string(input[12]);
	aux3+=":";
        aux3+=std::to_string(input[13]);
        aux3+=":";
        aux3+=std::to_string(ms);
	aux3+=" -- cTotal= ";
	aux3+=float2str(db->getAccumulatedEnergyConsumption());

	char sn = db->getSampleNr() + 1;
	if(sn==1/(SAMPLING_TIME)-1){
	    sn=0;
	    db->setOvBuffer(true);
	}

        db->setSampleNr(sn); // sample nr
	db->setTotalSampleNr(db->getTotalSampleNr()+1); //total sample nr
     	recv_desk = input[2];
	recebeu = true;
     }
}

void ard_o(char* input){
    //printf("id=%d, occ=%d", input[2], input[3]);
    if(findID(input[2])){
	if(input[3]==0)
            db->setOccupancy(false);
	else
	    db->setOccupancy(true);
    }
}

void ard_l(char* input){
    if(findID(input[2]))
        db->setIlluminanceLower(input[3]);
}

bool findID(char id){
    bool flag = false;
    for (it=dataBase.begin(); it!= dataBase.end(); ++it)
        if (it->getID() == id){
	    flag = true;
            db = &*it;
            break;
	}
    return flag;
}

int getTime(char in){
    return db->getH(in)*HOUR_IN_MS+db->getM(in)*MIN_IN_MS+db->getS(in)*SEC_IN_MS+db->getMs(in);
}
