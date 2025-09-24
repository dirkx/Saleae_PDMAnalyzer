#ifndef PDM_ANALYZER_SETTINGS
#define PDM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class PDMAnalyzerSettings : public AnalyzerSettings
{
public:
	PDMAnalyzerSettings();
	virtual ~PDMAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel mClockChannel;
	Channel mDataChannel;
	Channel mSyncChannel;
	U32 mBitsPerSample;
	bool mSignedValues;
        
	typedef enum { FORMAT_CSV = 0, FORMAT_WAV = 1, FORMAT_PCM = 2 } format_t;
	double mExportFormat;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mClockChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mDataChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mSyncChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitsPerSampleInterface;
	std::auto_ptr< AnalyzerSettingInterfaceBool >		mSignedValuesInterface;
	std::auto_ptr< AnalyzerSettingInterfaceNumberList>	mFileformatInterface;

};

#endif //PDM_ANALYZER_SETTINGS
