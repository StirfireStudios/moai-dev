//
//  OggAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "OpusAudioSource.h"


OpusAudioSource::OpusAudioSource()
{
    opusFile = NULL;
	mInFile = 0;
	localBuffer = NULL;
}

OpusAudioSource::~OpusAudioSource()
{
	close();
}

bool OpusAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;

	int error = 0;
	// Try opening the given file
	opusFile = op_open_file(path.c_str(), &error);
	
	if(error != 0)
	{
		std::cerr << "Error opening " << path.c_str() << " for decoding..." << std::endl;
		return false;
	}

	// Get some information about the Opus file
	total_samples = op_pcm_total(opusFile, -1);
	channels = op_channel_count(opusFile, -1);
	
	return BufferedAudioSource::init(path, loadIntoMemory);
}

void OpusAudioSource::close()
{
    BufferedAudioSource::close();

	if(localBuffer != NULL)
	{
		free(localBuffer);
	}
    
	if(opusFile != NULL)
	{
		op_free(opusFile);
		opusFile = NULL;
	}
}

Int64 OpusAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	if (opusFile == NULL)
		return startFrame;
	
	int status = op_pcm_seek(opusFile, startFrame);
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;

	Int64 pos = op_pcm_tell(opusFile);

	//return startFrame;
	return pos;
}

double OpusAudioSource::getLength() 
{ 
	if (opusFile != NULL)
		return total_samples / 48000.0;
	else
		return 0;
}

double OpusAudioSource::getSampleRate() 
{
	if (opusFile != NULL)
		return 48000.0;
	else
		return 0;
}

UInt32 OpusAudioSource::getNumChannels()
{
	if (opusFile != NULL)
		return channels;
	else
		return 0;
}

UInt32 OpusAudioSource::getBitsPerSample()
{
	// Opus samples are always floating point.
	if (opusFile != NULL)
		return 32;
	else
		return 0;
}

Int64 OpusAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	RScopedLock l(&mDecodeLock);

	if (!opusFile) {
		return 0;
	}
	
	if (localBuffer == NULL) {
		localBuffer = (float*)malloc(sizeof(float) * numFrames * channels);
		localBufferSize = numFrames * channels;
	} else if (localBufferSize < (numFrames * channels)) {
		localBuffer = (float*)realloc(localBuffer, sizeof(float) * numFrames * channels);
		localBufferSize = numFrames * channels;
	}

	int framesRead = op_read_float(opusFile, localBuffer, numFrames * channels, NULL);
	if(framesRead > 0)
	{
		for(int i = 0; i < channels; ++i)
		{
			float *pTemp = &buffer[i];
			for(int j = 0; j < framesRead; ++j)
			{
				*pTemp = localBuffer[j * channels + i];
				pTemp += channels;
			}
		}
	}
	else
		mEOF = true;

	return framesRead;
}
