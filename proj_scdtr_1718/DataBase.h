/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* DataBase.h                                               *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#ifndef DataBase_H
#define DataBase_H

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

#define SAMPLING_TIME 20*pow(10,-3)

class DataBase{

    // methods
    public:
	DataBase() : illuminance(1/(SAMPLING_TIME)), pwm(1/(SAMPLING_TIME)), h(2), m(2), s(2), ms(2) {}

	void setID(char);
	void setIlluminance(char, float);
	void setPWM(char, float);
	void setOccupancy(bool);
	void setIlluminanceLower(int);
	void setBackgroundIlluminance(float);
	void setIlluminanceControlReference(float);
	void setInstantaneousPower(float);
	void setAccumulatedEnergyConsumption(float);
	void setAccumulatedComfortError(float);
	void setAccumulatedComfortVariance(float);
	void setSampleNr(char);
	void setTotalSampleNr(int);
	void setTime(float);
	void setH(char);
        void setM(char);
        void setS(char);
        void setMs(int);
	void setOvBuffer(bool);
	void setAuxComfortError(float);
	void setAuxComfortVariance(float);

	char getID(void);
	float getIlluminance(char);
	float getPWM(char);
	bool getOccupancy(void);
	int getIlluminanceLower(void);
	float getBackgroundIlluminance(void);
	float getIlluminanceControlReference(void);
	float getInstantaneousPower(void);
	float getAccumulatedEnergyConsumption(void);
	float getAccumulatedComfortError(void);
	float getAccumulatedComfortVariance(void);
	char getSampleNr(void);
	int getTotalSampleNr(void);
	float getTime(void);
	char getH(char);
	char getM(char);
	char getS(char);
	int getMs(char);
	bool getOvBuffer(void);
	float getAuxComfortError(void);
	float getAuxComfortVariance(void);

    // attributes
    private:
	char id;
	std::vector<float> illuminance;
	std::vector<float> pwm;
	float illuminanceLower;
	float backgroundIlluminance;
	float illuminanceControlReference;
	float instantaneousPower;
	float accumulatedEnergyConsumption;
	float accumulatedComfortError;
	float accumulatedComfortVariance;
	bool occupancy;
	char sampleNr;
	int totalSampleNr;
	float time;
	std::vector<char> h;
	std::vector<char> m;
	std::vector<char> s;
	std::vector<int> ms;
	bool ovBuffer;
	float auxComfortError;
	float auxComfortVariance;
};

#endif
