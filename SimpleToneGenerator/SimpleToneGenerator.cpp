#include <iostream>
#include <vector>
#include <cmath>

/*
 * Простой генератор звуковой волны
 *
 * Теория https://en.wikibooks.org/wiki/Sound_Synthesis_Theory/Oscillators_and_Wavetables
 */

int main()
{
	const float PI = 3.141593;

	const unsigned samplingRate = 44100;	// Частота дискретизации (Hz)
	const float toneFrequency = 440;		// Частота тона (Hz)
	const float	amplitude = 1;				// Амплитуда (громкость) звуковой волны (0..1)
	const unsigned sampleDuration = 2;		// Длительность звучания семпла (сек)

	const float stepChangeInRadians = 2 * PI * toneFrequency / samplingRate;
	const unsigned bufferLength = samplingRate * sampleDuration;

	std::vector<float> buffer(bufferLength);

	for (unsigned int i = 0; i < bufferLength; ++i)
	{
		buffer[i] = amplitude * std::sin(stepChangeInRadians * static_cast<float>(i));
	}
}