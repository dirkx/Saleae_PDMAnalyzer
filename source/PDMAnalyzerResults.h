#ifndef PDM_ANALYZER_RESULTS
#define PDM_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class PDMAnalyzer;
class PDMAnalyzerSettings;

class PDMAnalyzerResults : public AnalyzerResults
{
public:
	PDMAnalyzerResults( PDMAnalyzer* analyzer, PDMAnalyzerSettings* settings );
	virtual ~PDMAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	PDMAnalyzerSettings* mSettings;
	PDMAnalyzer* mAnalyzer;

private:
        virtual void getValue(Frame frame, DisplayBase display_base, char * result_str, size_t result_len);
};

#endif //PDM_ANALYZER_RESULTS
