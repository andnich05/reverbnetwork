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
	void setDelayTimeSec(const double& sec);
	// Set decay time
	void setDecayTimeSec (const double& sec);
	// Set Allpass gain (unused because the gain is set by setting delay and decay)
	//inline void setGain(const double& g) { gain = g; };
	// Set the sign of the gain
	inline void setGainSign(const bool& isNegative) { this->gainSignIsNegative = isNegative; calculateGain(); }

	// ---Modulation setter functions
	// Enable or disable modulation
	void setModulationEnabled(const bool& enabled);
	// Set the time range which is modulated (e.g. delay = 20 ms, excursion = 5 ms => Range: from 15 ms to 25 ms)
	void setModulationExcursion(const double& excursion);
	// Set the modulation speed in Hertz (frequency of the modulation signal e.g. sine)
	void setModulationRateMs(const double& rate);

private:
	//double* inputBuffer; // Circular buffer for input samples x(n)
	//double* outputBuffer; // Circular buffer for output samples y(n)
	double* buffer; // Circular buffer, size is the maximum possible delay in samples
	long bufferSize; // Maximum possible delay in samples
	//unsigned long bufferPos; // Combined read/write index for both circular buffers
	long readPointer; // Read index
	long writePointer; // Write index
	double sampleRate; // Used for buffer creation
	double delaySamples; // Delay value in samples
	double fractDelaySamples; // Fractional part of the delay in samples, used for interpolation
	double delayTimeSec; // Delay time in seconds
	double decayTimeSec; // Decay time in seconds
	double gain; // Gain vaulue for processing
	bool gainSignIsNegative; // Gain sign

	// Modulation parameters
	bool modulationEnabled;
	double modulationExcursion;
	double modulationRate;

	// Temp values for processing
	//double xn; // = sample
	/*double yn;
	double xnD;
	double ynD;*/
	double nodeLeft; // Left node value in the signal flow diagram
	double nodeRight; // Right node value in the signal flow diagram

	long sampleCounter; // Circular counter used for modulation, counts from 1 to sampleRate / modulationRate (one periode)

	void calculateGain(); // Calculate the loop gain
};

#endif // SCHROEDERALLPASS_H