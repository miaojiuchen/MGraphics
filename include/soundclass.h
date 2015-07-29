////////////////////////////////////
// Filename:soundclass.h
// The SoundClass encapsulates the DirectSound functionality 
// as well as the .wav audio loading and playing capabilities.
////////////////////////////////////

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

//linking instructions
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

//includes
#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<stdio.h>

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSound;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass &);
	~SoundClass();

	bool Initialize(HWND hwnd);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND hwnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char *, IDirectSoundBuffer8 **);
	void ShutdownWaveFile(IDirectSoundBuffer8 **);

	bool PlayWaveFile();

private:
	IDirectSound8 *m_DirectSound;
	IDirectSoundBuffer *m_primaryBuffer;
	IDirectSoundBuffer8 *m_secondaryBuffer1;
	IDirectSoundBuffer8 *m_secondaryBuffer2;

};

#endif

