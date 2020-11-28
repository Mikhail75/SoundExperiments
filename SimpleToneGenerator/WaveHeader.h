#pragma once

#include <vector>
#include <iostream>

/*
 * https://audiocoding.ru/articles/2008-05-22-wav-file-structure/
 * https://gist.github.com/alexey-detr/1664067
 * https://www.cnblogs.com/wangguchangqing/p/5957531.html
 * https://www.cnblogs.com/wangguchangqing/p/5970516.html
 */

namespace wave
{
	struct WaveHeader
	{
		// WAV-формат начинается с RIFF-заголовка: содержит символы "RIFF" в ASCII кодировке (0x52494646 в big-endian представлении)
		const std::vector<char> chunkId = { 'R','I', 'F', 'F' };

		// 36 + subchunk2Size, или более точно: 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
		// Это оставшийся размер цепочки, начиная с этой позиции. Иначе говоря, это размер файла - 8, то есть, исключены поля chunkId и chunkSize.
		uint32_t chunkSize = 0;
		// Содержит символы "WAVE" (0x57415645 в big-endian представлении)
		const std::vector<char> format = { 'W', 'A', 'V', 'E' };

		// Формат "WAVE" состоит из двух подцепочек: "fmt " и "data".
		// Подцепочка "fmt " описывает формат звуковых данных:

		// Содержит символы "fmt " (0x666d7420 в big-endian представлении)
		const std::vector<char> subchunk1Id = { 'f', 'm', 't', ' ' };
		// 16 (или 18) для формата PCM. Это оставшийся размер подцепочки, начиная с этой позиции.
		uint32_t subchunk1Size = 16;

		// Аудио формат, полный список можно получить здесь https://audiocoding.ru/articles/2008-05-22-wav-file-structure/wav_formats.txt
		// Если данные сохраняются в формате int, audioFormat = 1. Если во float, audioFormat = 3
		uint16_t audioFormat = 3; // Если данный сохраняются в формате
		// Количество каналов. Моно = 1, Стерео = 2 и т.д.
		uint16_t numChannels = 2;
		// Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
		uint32_t sampleRate = 44100;
		// sampleRate * numChannels * bitsPerSample/8 Количество байт, переданных за секунду воспроизведения.
		uint32_t byteRate = 352800;
		// numChannels * bitsPerSample/8 Количество байт для одного сэмпла, включая все каналы.
		uint16_t blockAlign = 8;
		// Количество бит в сэмпле. Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
		uint16_t bitsPerSample = 32;

		// Подцепочка "data" содержит аудио-данные и их размер.

		// Содержит символы "data" (0x64617461 в big-endian представлении)
		const std::vector<char> subchunk2Id = { 'd', 'a', 't', 'a' };
		// numSamples * numChannels * bitsPerSample/8  Количество байт в области данных.
		uint32_t subchunk2Size = 0;

		// Далее следуют непосредственно Wav данные
	};
}

std::ofstream& operator<< (std::ofstream &strm, wave::WaveHeader &wave)
{
	auto writeUint16 = [&](uint16_t &value)	{ strm.write(reinterpret_cast<char*>(&value), sizeof(uint16_t)); };
	auto writeUint32 = [&](uint32_t &value)	{ strm.write(reinterpret_cast<char*>(&value), sizeof(uint32_t)); };

	strm << wave.chunkId[0] << wave.chunkId[1] << wave.chunkId[2] << wave.chunkId[3];
	writeUint32(wave.chunkSize);

	strm << wave.format[0] << wave.format[1] << wave.format[2] << wave.format[3];

	strm << wave.subchunk1Id[0] << wave.subchunk1Id[1] << wave.subchunk1Id[2] << wave.subchunk1Id[3];
	writeUint32(wave.subchunk1Size);

	writeUint16(wave.audioFormat);
	writeUint16(wave.numChannels);
	writeUint32(wave.sampleRate);
	writeUint32(wave.byteRate);
	writeUint16(wave.blockAlign);
	writeUint16(wave.bitsPerSample);

	strm << wave.subchunk2Id[0] << wave.subchunk2Id[1] << wave.subchunk2Id[2] << wave.subchunk2Id[3];

	return strm;
}
