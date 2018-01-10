/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*							   *
* DataBase.cpp						   *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#include "DataBase.h"

// sets
void DataBase::setIlluminance(char pointer, float illuminance){
    this->illuminance[pointer] = illuminance;
}

void DataBase::setPWM(char pointer, float pwm){
    this->pwm[pointer] = pwm;
}

void DataBase::setOccupancy(bool occupancy){
    this->occupancy = occupancy;
}

void DataBase::setIlluminanceLower(int illuminanceLower){
    this->illuminanceLower = illuminanceLower;
}

void DataBase::setBackgroundIlluminance(float backgroundIlluminance){
    this->backgroundIlluminance = backgroundIlluminance;
}

void DataBase::setIlluminanceControlReference(float illuminanceControlReference){
    this->illuminanceControlReference = illuminanceControlReference;
}

void DataBase::setInstantaneousPower(float instantaneousPower){
    this->instantaneousPower = instantaneousPower;
}

void DataBase::setAccumulatedEnergyConsumption(float accumulatedEnergyConsumption){
    this->accumulatedEnergyConsumption = accumulatedEnergyConsumption;
}

void DataBase::setAccumulatedComfortError(float accumulatedComfortError){
    this->accumulatedComfortError = accumulatedComfortError;
}

void DataBase::setAccumulatedComfortVariance(float accumulatedComfortVariance){
    this->accumulatedComfortVariance = accumulatedComfortVariance;
}

void DataBase::setSampleNr(char sampleNr){
    this->sampleNr = sampleNr;
}

void DataBase::setTotalSampleNr(int totalSampleNr){
    this->totalSampleNr = totalSampleNr;
}

void DataBase::setID(char id){
    this->id = id;
}

void DataBase::setH(char h){
    this->h[1] = this->h[2];
    this->h[2] = h;
}

void DataBase::setM(char m){
    this->m[1] = this->m[2];
    this->m[2] = m;
}

void DataBase::setS(char s){
    this->s[1] = this->s[2];
    this->s[2] = s;
}

void DataBase::setMs(int ms){
    this->ms[1] = this->ms[2];
    this->ms[2] = ms;
}

void DataBase::setOvBuffer(bool ovBuffer){
    this->ovBuffer = ovBuffer;
}

void DataBase::setAuxComfortError(float auxComfortError){
    this->auxComfortError = auxComfortError;
}

void DataBase::setAuxComfortVariance(float auxComfortVariance){
    this->auxComfortVariance = auxComfortVariance;
}


// gets
float DataBase::getIlluminance(char pointer){
    return this->illuminance[pointer];
}

float DataBase::getPWM(char pointer){
    return this->pwm[pointer];
}

bool DataBase::getOccupancy(void){
    return this->occupancy;
}

int DataBase::getIlluminanceLower(void){
    return this->illuminanceLower;
}

float DataBase::getBackgroundIlluminance(void){
    return this->backgroundIlluminance;
}

float DataBase::getIlluminanceControlReference(void){
    return this->illuminanceControlReference;
}

float DataBase::getInstantaneousPower(void){
    return this->instantaneousPower;
}

float DataBase::getAccumulatedEnergyConsumption(void){
    return this->accumulatedEnergyConsumption;
}

float DataBase::getAccumulatedComfortError(void){
    return this->accumulatedComfortError;
}

float DataBase::getAccumulatedComfortVariance(void){
    return this->accumulatedComfortVariance;
}

char DataBase::getSampleNr(void){
    return this->sampleNr;
}

int DataBase::getTotalSampleNr(void){
    return this->totalSampleNr;
}

char DataBase::getID(void){
    return this->id;
}

char DataBase::getH(char pointer){
    return this->h[pointer];
}

char DataBase::getM(char pointer){
    return this->m[pointer];
}

char DataBase::getS(char pointer){
    return this->s[pointer];
}

int DataBase::getMs(char pointer){
    return this->ms[pointer];
}

bool DataBase::getOvBuffer(void){
    return this->ovBuffer;
}

float DataBase::getAuxComfortError(void){
    return this->auxComfortError;
}

float DataBase::getAuxComfortVariance(void){
    return this->auxComfortVariance;
}
