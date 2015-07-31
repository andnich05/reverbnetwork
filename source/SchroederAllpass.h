#ifndef SCHROEDERALLPASS_H
#define SCHROEDERALLPASS_H

class SchroederAllpass
{
public:
	SchroederAllpass(double delaySec, double decaySec);
	~SchroederAllpass();

	// Process input sample by reference => saving speed
	void doProcessing(double& sample);

	// E.g. When plugin is beeing enabled or disabled
	void createBuffers();
	void freeBuffers();

	// Change current sample rate
	inline void setSampleRate(const double& s) { sampleRate = s; freeBuffers(); createBuffers(); }; // Recreate buffers when sample rate changes
	// Set delay time
	//inline void setDelayTimeSec(const double& sec) { delayTime = sec; delaySamples = (unsigned long)(sec * sampleRate); };
	void setDelayTimeSec(const double& sec);
	// Set decay time
	void setDecayTimeSec (const double& sec);
	// Set Allpass gain
	//inline void setGain(const double& g) { gain = g; };
	inline void setGainSign(const bool& isNegative) { this->gainSignIsNegative = isNegative; calculateGain(); }

	void setModulationEnabled(const bool& enabled);
	void setModulationExcursion(const double& excursion);
	void setModulationRateMs(const double& rate);

private:
	//double* inputBuffer; // Circular buffer for input samples x(n)
	//double* outputBuffer; // Circular buffer for output samples y(n)
	double* buffer;
	long bufferSize;
	//unsigned long bufferPos; // Combined read/write index for both circular buffers
	long readPointer;
	long writePointer;
	double sampleRate; // Used for buffer creation
	double delaySamples; // Delay value in samples
	double fractDelaySamples; // Fractional part of the delay in samples, used for interpolation
	double delayTimeSec; // Delay time in seconds
	double decayTimeSec; // Decay time in seconds
	double gain; // Gain vaulue for processing
	bool gainSignIsNegative; // Gain sign

	bool modulationEnabled;
	double modulationExcursion;
	double modulationRate;

	// Temp values for processing
	//double xn; // = sample
	/*double yn;
	double xnD;
	double ynD;*/
	double nodeLeft;
	double nodeRight;

	long sampleCounter;

	void calculateGain();
};

#endif // SCHROEDERALLPASS_H