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

	mSyncChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mSyncChannelInterface->SetTitleAndTooltip( "Reset (optional)", "PDM count reset/sync (optional, resets bitcount to 0)" );
	mSyncChannelInterface->SetChannel( mSyncChannel );
	mSyncChannelInterface->SetSelectionOfNoneIsAllowed( true );

	mBitsPerSampleInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitsPerSampleInterface->SetTitleAndTooltip( "Bits per sample (Q)",  "Specify the number of individual bits that form a sample." );
	mBitsPerSampleInterface->SetMax( 255 );
	mBitsPerSampleInterface->SetMin( 1 );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );

	mSignedValuesInterface.reset( new AnalyzerSettingInterfaceBool() );
	mSignedValuesInterface->SetTitleAndTooltip( "Display as signed",  "Display the count as signed integers (e.g. Audio) rather than hex; with the 0 around Q/2");
	mSignedValuesInterface->SetValue(false );

	mFileformatInterface.reset(new AnalyzerSettingInterfaceNumberList() );
	mFileformatInterface->SetTitleAndTooltip( "Fileformat", "Fileformat on export, overwrites CSV/TEXT");
	mFileformatInterface->AddNumber(FORMAT_CSV,"Export as text/CSV file","Export as text/CSV file");
	mFileformatInterface->AddNumber(FORMAT_WAV,"Export as WAV file (mono, 8bits)","Export as WAV file, 8bits, mono and signed/unsigned depending on setting");
	mFileformatInterface->AddNumber(FORMAT_PCM,"Export as PCM file (raw values as bytes)","Export as raw bytes; i.e. a series if raw bytes; as many as Q counts.");

	AddInterface( mClockChannelInterface.get() );
	AddInterface( mDataChannelInterface.get() );
	AddInterface( mBitsPerSampleInterface.get() );
	AddInterface( mSignedValuesInterface.get() );
	AddInterface( mFileformatInterface.get() );
	AddInterface( mSyncChannelInterface.get() );


	AddExportOption(FORMAT_CSV, "Export as text/CSV file" );
	AddExportExtension(FORMAT_CSV, "text", "txt" );
	AddExportExtension(FORMAT_CSV, "csv", "csv" );

	AddExportOption(FORMAT_WAV, "Export as a WAV file" );
	AddExportExtension(FORMAT_WAV, "PCM","pcm");
	AddExportExtension(FORMAT_WAV, "U8","u8");
	AddExportExtension(FORMAT_WAV, "Raw","raw");

	AddExportOption(FORMAT_PCM, "Export as a raw PCM/bytes file" );
	AddExportExtension(FORMAT_PCM, "WAV", "wav" );

	ClearChannels();
	AddChannel( mClockChannel, "Clock", false );
	AddChannel( mDataChannel, "Data", false );
	AddChannel( mSyncChannel, "Reset", false );
}

PDMAnalyzerSettings::~PDMAnalyzerSettings()
{
}

bool PDMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mClockChannel = mClockChannelInterface->GetChannel();
	mDataChannel = mDataChannelInterface->GetChannel();
	mSyncChannel = mSyncChannelInterface->GetChannel();
	mBitsPerSample = mBitsPerSampleInterface->GetInteger();
        mSignedValues = mSignedValuesInterface->GetValue();
        mExportFormat = (format_t) mFileformatInterface->GetNumber();

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
	AddChannel( mDataChannel, "Data", true );
	AddChannel( mSyncChannel, "Reset", mSyncChannel != UNDEFINED_CHANNEL);

	return true;
}

void PDMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mClockChannelInterface->SetChannel( mClockChannel );
	mDataChannelInterface->SetChannel( mDataChannel );
	mSyncChannelInterface->SetChannel( mSyncChannel );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );
	mSignedValuesInterface->SetValue( mSignedValues );
	mFileformatInterface->SetNumber( (int)mExportFormat );
}

void PDMAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mClockChannel;
        text_archive >> mDataChannel;
        text_archive >> mSyncChannel;
	text_archive >> mBitsPerSample;
	text_archive >> mSignedValues;
	text_archive >> mExportFormat;

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
        AddChannel( mDataChannel, "Data", true);
	AddChannel( mSyncChannel, "Reset", mSyncChannel != UNDEFINED_CHANNEL);

	UpdateInterfacesFromSettings();
}

const char* PDMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mClockChannel;
        text_archive << mDataChannel;
        text_archive << mSyncChannel;
	text_archive << mBitsPerSample;
	text_archive << mSignedValues;
	text_archive << mExportFormat;

	return SetReturnString( text_archive.GetString() );
}
