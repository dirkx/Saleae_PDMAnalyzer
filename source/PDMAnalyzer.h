#ifndef PDM_ANALYZER_H
#define PDM_ANALYZER_H

#include <Analyzer.h>
#include "PDMAnalyzerResults.h"
#include "PDMSimulationDataGenerator.h"

class PDMAnalyzerSettings;
class ANALYZER_EXPORT PDMAnalyzer : public Analyzer2
{
public:
	PDMAnalyzer();
	virtual ~PDMAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< PDMAnalyzerSettings > mSettings;
	std::auto_ptr< PDMAnalyzerResults > mResults;
	AnalyzerChannelData* mClock;
	AnalyzerChannelData* mData;
	AnalyzerChannelData* mSync;

	PDMSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //PDM_ANALYZER_H
