#include "PDMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


PDMAnalyzerSettings::PDMAnalyzerSettings()
:	mClockChannel( UNDEFINED_CHANNEL ),
    mDataChannel( UNDEFINED_CHANNEL),
    mBitsPerSample( 64 )
{
	mClockChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mClockChannelInterface->SetTitleAndTooltip( "Clock", "Clock" );
	mClockChannelInterface->SetChannel( mClockChannel );

	mDataChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mDataChannelInterface->SetTitleAndTooltip( "Data", "PDM Clock" );
	mDataChannelInterface->SetChannel( mDataChannel );

	mBitsPerSampleInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitsPerSampleInterface->SetTitleAndTooltip( "Bits per sample",  "Specify the number of individual bits that form a sample." );
	mBitsPerSampleInterface->SetMax( 255 );
	mBitsPerSampleInterface->SetMin( 1 );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );

	AddInterface( mClockChannelInterface.get() );
	AddInterface( mDataChannelInterface.get() );
	AddInterface( mBitsPerSampleInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mClockChannel, "Clock", false );
	AddChannel( mDataChannel, "Data", false );
}

PDMAnalyzerSettings::~PDMAnalyzerSettings()
{
}

bool PDMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mClockChannel = mClockChannelInterface->GetChannel();
	mDataChannel = mDataChannelInterface->GetChannel();
	mBitsPerSample = mBitsPerSampleInterface->GetInteger();

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
	AddChannel( mDataChannel, "Data", true );

	return true;
}

void PDMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mClockChannelInterface->SetChannel( mClockChannel );
	mDataChannelInterface->SetChannel( mDataChannel );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );
}

void PDMAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mClockChannel;
    text_archive >> mDataChannel;
	text_archive >> mBitsPerSample;

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
    AddChannel( mDataChannel, "Data", true);

	UpdateInterfacesFromSettings();
}

const char* PDMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mClockChannel;
    text_archive << mDataChannel;
	text_archive << mBitsPerSample;

	return SetReturnString( text_archive.GetString() );
}
