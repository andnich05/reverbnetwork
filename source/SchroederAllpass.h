#ifndef SCHROEDERALLPASS_H
#define SCHROEDERALLPASS_H

class SchroederAllpass
{
public:
	SchroederAllpass(double sampleRate, double delay, double decay);
	~SchroederAllpass();

	// Process input sample by reference => saving speed
	void doProcessing(double& sample);

	// E.g. When plugin is beeing enabled or disabled
	void createBuffers();
	void freeBuffers();

	// Change current sample rate
	inline void setSampleRate(const unsigned long& s) { sampleRate = s; freeBuffers(); createBuffers(); }; // Recreate buffers when sample rate changes
	// Set delay time
	//inline void setDelayTimeSec(const double& sec) { delayTime = sec; delaySamples = (unsigned long)(sec * sampleRate); };
	void setDelayTimeMsec(const double& ms);
	// Set decay time
	void setDecayTime(const double& sec);
	// Set Allpass gain
	//inline void setGain(const double& g) { gain = g; };

private:
	//double* inputBuffer; // Circular buffer for input samples x(n)
	//double* outputBuffer; // Circular buffer for output samples y(n)
	double* buffer;
	//unsigned long bufferPos; // Combined read/write index for both circular buffers
	long readPointer;
	long writePointer;
	double sampleRate; // Used for buffer creation
	long delaySamples; // Delay value in samples for processing
	double delayTime; // Delay time in seconds
	double decayTime; // Decay time in seconds
	double gain; // Gain vaulue for processing

	// Temp values for processing
	//double xn; // = sample
	/*double yn;
	double xnD;
	double ynD;*/
	double nodeLeft;
	double nodeRight;

	void calculateGain();
};

#endif // SCHROEDERALLPASS_H