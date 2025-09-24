#include "PDMAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "PDMAnalyzer.h"
#include "PDMAnalyzerSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

PDMAnalyzerResults::PDMAnalyzerResults( PDMAnalyzer* analyzer, PDMAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

PDMAnalyzerResults::~PDMAnalyzerResults()
{
}

void  PDMAnalyzerResults::getValue(Frame frame, DisplayBase display_base, char * number_str, size_t result_len) 
{
        U8 data = frame.mData1 + (mSettings->mSignedValues) ? (-mSettings->mBitsPerSample / 2) : 0;

        if (display_base == Decimal && mSettings->mSignedValues) {
            S64 signed_number = AnalyzerHelpers::ConvertToSignedNumber( data, 8);
            std::stringstream ss;
            ss << signed_number;
            strncpy( number_str, ss.str().c_str(), result_len); 
	    number_str[result_len-1] = '\0'; // strncpy does not \0 terminate if the result is >= n in length.
        } else {
	    AnalyzerHelpers::GetNumberString( data, display_base, 8, number_str, result_len);
        };
}

void PDMAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );
		
	char number_str[128];
	getValue(frame, display_base, number_str, sizeof(number_str));

	AddResultString( number_str );
}

void PDMAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();
	U64 num_frames = GetNumFrames();

#if 2 // Logic2 broke/retired the GenerateExport file selection options - so we take the configured value for now.
	export_type_user_id = mSettings->mExportFormat;
#endif
        if (export_type_user_id == PDMAnalyzerSettings::FORMAT_WAV) {
		// WAV file
 
		U32 file_len = 44 + num_frames;
		U32 channels = 1;
		U32 bitsPerSample = 8;

        	S64 from = GetFrame(0).mEndingSampleInclusive;
	        S64 till = GetFrame(num_frames - 1).mStartingSampleInclusive;
		double duration = (till - from) / sample_rate;
		U32 audioSampleRate = num_frames / duration;

		// Source: https://github.com/audiojs/sample-rate under the MIT license
		U32 normalRates[] = { 
			8000, //	 Hz	Adequate for human speech but without sibilance. Used in telephone/walkie-talkie.
			11025, //	 Hz	Used for lower-quality PCM, MPEG audio and for audio analysis of subwoofer bandpasses.
			16000, //	 Hz	Used in most VoIP and VVoIP, extension of telephone narrowband.
			22050, //	 Hz	Used for lower-quality PCM and MPEG audio and for audio analysis of low frequency energy.
			44100, //	 Hz	Audio CD, most commonly used rate with MPEG-1 audio (VCD, SVCD, MP3). Covers the 20 kHz bandwidth.
			48000, //	 Hz	Standard sampling rate used by professional digital video equipment, could reconstruct frequencies up to 22 kHz.
			88200, //	 Hz	Used by some professional recording equipment when the destination is CD, such as mixers, EQs, compressors, reverb, crossovers and recording devices.
			96000, //	 Hz	DVD-Audio, LPCM DVD tracks, Blu-ray audio tracks, HD DVD audio tracks.
			176400, //	 Hz	Used in HDCD recorders and other professional applications for CD production.
			192000, //	 Hz	Used with audio on professional video equipment. DVD-Audio, LPCM DVD tracks, Blu-ray audio tracks, HD DVD audio tracks.
			352800, //	 Hz	Digital eXtreme Definition. Used for recording and editing Super Audio CDs.
			384000, //	 Hz	Highest sample rate available for common software. Allows for precise peak detection.
			0
		};
		// Check if we are within 5% of any of the well known rates; and adjust to that value.
		//
		for(int i = 0; normalRates[i]; i++) {
			double delta = fabsl(normalRates[i]-audioSampleRate)/std::min(normalRates[i],audioSampleRate);
			if (delta < 0.025) {
				audioSampleRate = normalRates[i];
				break;
			};
		};

		file_stream.write("RIFF",4); // WAV/RIFF header					0,4
		{
			U32 tmp = file_len - 8; // Chunk Size
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	4,4
		};
		file_stream.write("WAVE",4);						//	8,4

		file_stream.write( "fmt ",4); // Start chunk					12,4
		{
			U32 tmp = 16; // length of this chunk
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	16,4
		};
		{
			U16 tmp = 1; // PCM format
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	20,3
		};
		{
			U16 tmp = channels; // mono / 1 channel
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	22,2
		};
		{
			U32 tmp = audioSampleRate; // sample rate
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	24,4
		};
		{
			U32 tmp = audioSampleRate * bitsPerSample * channels / 8; // byte rate
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	28,4
		};
		{
			U16 tmp = bitsPerSample * channels / 8; // bytes/block/align
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	32,2	
		};
		{
			U16 tmp = bitsPerSample; // bits per sample
			file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	34,2
		};
		file_stream.write("data",4); // start of the sample for this cunk		36,4	
		{
			U32 tmp = num_frames; // number of bytes
	                file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));//	40,4
		};

		for( U32 i=0; i < num_frames; i++ ) {
			Frame frame = GetFrame( i );
			U8 tmp = frame.mData1;
			// Centre of the values of 0..Q(inc) is at Q/2; move this
			// to 127 -- the center of 0..255; and scale to 40%
			//
			if (mSettings->mBitsPerSample < 255) {
				tmp *= 0.40 * 255/mSettings->mBitsPerSample;
				tmp += (255 - mSettings->mBitsPerSample)/2;
			};
	                file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));
			if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
			{
				file_stream.close();
				return;
			};
		};
	} else if (export_type_user_id == PDMAnalyzerSettings::FORMAT_PCM) { 
		// PCM file - as raw as raw can be - so we do not attempt
		// to recenter the bytes.
		//
		for( U32 i=0; i < num_frames; i++ ) {
			Frame frame = GetFrame( i );
			U8 tmp = frame.mData1;
	                file_stream.write(reinterpret_cast<const char*>(&tmp),sizeof(tmp));
			if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
			{
				file_stream.close();
				return;
			};
		};
        } else  // PDMAnalyzerSettings::FORMAT_CSV / text
	{
		file_stream << "Time [s],Value" << std::endl;

		for( U32 i=0; i < num_frames; i++ )
		{
			Frame frame = GetFrame( i );
			
			char time_str[128];
			AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );
	
			char number_str[128];
		        getValue(frame, display_base, number_str, sizeof(number_str));
	
			file_stream << time_str << "," << number_str << std::endl;
	
			if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
			{
				file_stream.close();
				return;
			};
		};
        }
	file_stream.close();
}

void PDMAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	getValue(frame, display_base, number_str, sizeof(number_str));

	AddTabularText( number_str );
#endif
}

void PDMAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void PDMAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}
