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

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //PDM_ANALYZER_SETTINGS
