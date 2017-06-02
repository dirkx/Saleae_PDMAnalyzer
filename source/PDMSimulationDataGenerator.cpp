#include "PDMSimulationDataGenerator.h"
#include "PDMAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

PDMSimulationDataGenerator::PDMSimulationDataGenerator()
:	mSerialText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
{
}

PDMSimulationDataGenerator::~PDMSimulationDataGenerator()
{
}

void PDMSimulationDataGenerator::Initialize( U32 simulation_sample_rate, PDMAnalyzerSettings* settings )
{

}

U32 PDMSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{

	return 1;
}

void PDMSimulationDataGenerator::CreateSerialByte()
{

}
