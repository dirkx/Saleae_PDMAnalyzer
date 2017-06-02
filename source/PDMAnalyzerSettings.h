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
	U32 mBitsPerSample;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mClockChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mDataChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitsPerSampleInterface;
};

#endif //PDM_ANALYZER_SETTINGS
