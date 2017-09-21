//---------------------------------------------------------------------------

#ifndef uPOSAnalysisResultsFormH
#define uPOSAnalysisResultsFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ClassTCommonEvaluation.h"
#include "class_FileUTMI.h"
//---------------------------------------------------------------------------
class TAnalysisResultsForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
    TGroupBox *GroupBoxAnalysisResults;
    TButton *ButtonSaveAsEtalon;
    TButton *ButtonAddToUTMI;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ButtonSaveAsEtalonClick(TObject *Sender);
    void __fastcall ButtonAddToUTMIClick(TObject *Sender);
private:	// User declarations

    AnsiString UTMIFileName;
    AnsiString FeedbackPotentiometerParameterName;
    const TCommonEvaluation<double,double> *AnalysisObject;


public:		// User declarations
    __fastcall TAnalysisResultsForm(TComponent* Owner);

    void LoadForm(int IntervalsCounter, const TIntervalCharacteristics* FailedIntervalsList,// for current form
                    const TCommonEvaluation<double,double> *AnalysisObject, AnsiString FeedbackPotentiometerParameterName,// for saveform
                    AnsiString UTMIFileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TAnalysisResultsForm *AnalysisResultsForm;
//---------------------------------------------------------------------------
#endif
