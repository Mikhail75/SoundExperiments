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
		// WAV-������ ���������� � RIFF-���������: �������� ������� "RIFF" � ASCII ��������� (0x52494646 � big-endian �������������)
		const std::vector<char> chunkId = { 'R','I', 'F', 'F' };

		// 36 + subchunk2Size, ��� ����� �����: 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
		// ��� ���������� ������ �������, ������� � ���� �������. ����� ������, ��� ������ ����� - 8, �� ����, ��������� ���� chunkId � chunkSize.
		uint32_t chunkSize = 0;
		// �������� ������� "WAVE" (0x57415645 � big-endian �������������)
		const std::vector<char> format = { 'W', 'A', 'V', 'E' };

		// ������ "WAVE" ������� �� ���� ����������: "fmt " � "data".
		// ���������� "fmt " ��������� ������ �������� ������:

		// �������� ������� "fmt " (0x666d7420 � big-endian �������������)
		const std::vector<char> subchunk1Id = { 'f', 'm', 't', ' ' };
		// 16 (��� 18) ��� ������� PCM. ��� ���������� ������ ����������, ������� � ���� �������.
		uint32_t subchunk1Size = 16;

		// ����� ������, ������ ������ ����� �������� ����� https://audiocoding.ru/articles/2008-05-22-wav-file-structure/wav_formats.txt
		// ���� ������ ����������� � ������� int, audioFormat = 1. ���� �� float, audioFormat = 3
		uint16_t audioFormat = 3; // ���� ������ ����������� � �������
		// ���������� �������. ���� = 1, ������ = 2 � �.�.
		uint16_t numChannels = 2;
		// ������� �������������. 8000 ��, 44100 �� � �.�.
		uint32_t sampleRate = 44100;
		// sampleRate * numChannels * bitsPerSample/8 ���������� ����, ���������� �� ������� ���������������.
		uint32_t byteRate = 352800;
		// numChannels * bitsPerSample/8 ���������� ���� ��� ������ ������, ������� ��� ������.
		uint16_t blockAlign = 8;
		// ���������� ��� � ������. ��� ���������� "��������" ��� �������� ��������. 8 ���, 16 ��� � �.�.
		uint16_t bitsPerSample = 32;

		// ���������� "data" �������� �����-������ � �� ������.

		// �������� ������� "data" (0x64617461 � big-endian �������������)
		const std::vector<char> subchunk2Id = { 'd', 'a', 't', 'a' };
		// numSamples * numChannels * bitsPerSample/8  ���������� ���� � ������� ������.
		uint32_t subchunk2Size = 0;

		// ����� ������� ��������������� Wav ������
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
