#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "WaveHeader.h"

/*
 * Простой генератор звуковой волны
 *
 * Теория
 * https://en.wikipedia.org/wiki/Sine_wave
 * https://en.wikibooks.org/wiki/Sound_Synthesis_Theory/Oscillators_and_Wavetables
 *
 * Пример
 * https://stackoverrun.com/ru/q/13071619
 */


using namespace std;
using namespace wave;

int main()
{
	const float PI = 3.141593;

	const unsigned samplingRate = 44100;	// Частота дискретизации (Hz)
	const float toneFrequency = 440;		// Частота тона (Hz)
	const float amplitude = 1;				// Амплитуда (громкость) звуковой волны (0..1)
	const unsigned sampleDuration = 1;		// Длительность звучания семпла (сек)

	const float stepChangeInRadians = 2 * PI * toneFrequency / static_cast<float>(samplingRate);
	const unsigned bufferLength = samplingRate * sampleDuration;

	vector<float> buffer(bufferLength);

	for (unsigned int i = 0; i < bufferLength; ++i)
	{
		buffer[i] = amplitude * sin(stepChangeInRadians * static_cast<float>(i));
	}


	WaveHeader waveHeader;

	waveHeader.subchunk2Size = bufferLength * 2 * 4;
	waveHeader.chunkSize = 36 + waveHeader.subchunk2Size;
	
	ofstream ofs("sample.wav", ios::trunc | ios::binary);
	ofs << waveHeader;

	for (unsigned int i = 0; i < bufferLength; ++i)
	{
		ofs.write(reinterpret_cast<char*>(&buffer[i]), sizeof(float));
		ofs.write(reinterpret_cast<char*>(&buffer[i]), sizeof(float));
	}

	/*
	// Если waveHeader.audioFormat = 1, float нужно конвертировать в int32
	for (unsigned int i = 0; i < bufferLength; ++i)
	{
		float value = buffer[i];
		int32_t converted_value;

		if (value >= 0)
		{
			converted_value = static_cast<int32_t>(2147483647. * value);
		}
		else
		{
			converted_value = static_cast<int32_t>(-2147483648. * (value * -1));
		}
		
		ofs.write(reinterpret_cast<char*>(&converted_value), sizeof(int32_t));
		ofs.write(reinterpret_cast<char*>(&converted_value), sizeof(int32_t));
	}
	*/

	ofs.close();
}