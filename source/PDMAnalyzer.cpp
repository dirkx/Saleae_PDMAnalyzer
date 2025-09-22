#include "PDMAnalyzer.h"
#include "PDMAnalyzerSettings.h"
#include <AnalyzerChannelData.h>
#include <stdlib.h>

PDMAnalyzer::PDMAnalyzer()
:	Analyzer2(),
	mSettings( new PDMAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

PDMAnalyzer::~PDMAnalyzer()
{
	KillThread();
}

void PDMAnalyzer::SetupResults()
{
	mResults.reset( new PDMAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mDataChannel );
}

void PDMAnalyzer::WorkerThread()
{
	// TODO(tannewt): Why is this a member and not a local?
	mClock = GetAnalyzerChannelData( mSettings->mClockChannel );
	mData = GetAnalyzerChannelData( mSettings->mDataChannel );

	// Try to find the real start of the clock by finding a rising edge where
	// the low before it is relatively close to the same length as the following
	// high.

	// Find the start of a low.
	if( mClock->GetBitState() == BIT_HIGH ) {
		mClock->AdvanceToNextEdge();
	} 
#if 0
	else {
		mClock->AdvanceToNextEdge();
		mClock->AdvanceToNextEdge();
	}
	U64 low_start = mClock->GetSampleNumber();
	U64 low_duration = 0;
	U64 high_duration = 0;
	mClock->AdvanceToNextEdge();
	while (low_duration == 0 || std::abs((long long)(low_duration - high_duration)) > (low_duration + high_duration) / 2) {
		mClock->AdvanceToNextEdge(); // Falling edge
		low_start = mClock->GetSampleNumber();
		mClock->AdvanceToNextEdge(); // Rising edge
		low_duration = mClock->GetSampleNumber() - low_start;
		high_duration = mClock->GetSampleOfNextEdge() - mClock->GetSampleNumber();
	}
#endif

	for( ; ; )
	{
		U8 data = 0;

		U64 starting_sample = mClock->GetSampleNumber();
		mResults->AddMarker( mClock->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mDataChannel);

		for( U32 i=0; i< mSettings->mBitsPerSample; i++ )
		{
			mResults->AddMarker( mClock->GetSampleNumber(), 
				(i) ? AnalyzerResults::DownArrow : AnalyzerResults::Dot, mSettings->mClockChannel);

			mData->AdvanceToAbsPosition(mClock->GetSampleNumber());

			bool isHigh = ( mData->GetBitState() == BIT_HIGH );
			if (isHigh) data++;

			U64 s = mClock->GetSampleNumber();
			// Next falling edge.
			// TODO(tannewt): Support stereo data by reading the data here.
			mClock->AdvanceToNextEdge();

			// Place the one in the middle of this low; alternative is getting it about
			// 20-30 nanosconds after the down.
			U64 e = mClock->GetSampleNumber();
			mResults->AddMarker((s+e)/2, isHigh ? AnalyzerResults::One : AnalyzerResults::Zero, mSettings->mDataChannel );

			// Next rising edge.
			mClock->AdvanceToNextEdge();

		}


		//we have a byte to save.
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mClock->GetSampleNumber();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
}

bool PDMAnalyzer::NeedsRerun()
{
	return false;
}

U32 PDMAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 PDMAnalyzer::GetMinimumSampleRateHz()
{
	return 500000;
}

const char* PDMAnalyzer::GetAnalyzerName() const
{
	return "PDM";
}

const char* GetAnalyzerName()
{
	return "PDM";
}

Analyzer* CreateAnalyzer()
{
	return new PDMAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
