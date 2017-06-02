#ifndef PDM_SIMULATION_DATA_GENERATOR
#define PDM_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class PDMAnalyzerSettings;

class PDMSimulationDataGenerator
{
public:
	PDMSimulationDataGenerator();
	~PDMSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, PDMAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	PDMAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //PDM_SIMULATION_DATA_GENERATOR