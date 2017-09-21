//---------------------------------------------------------------------------

#ifndef uPOSSaveParameterASH
#define uPOSSaveParameterASH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "ClassTCommonEvaluation.h"
#include "class_FileUTMI.h"
//---------------------------------------------------------------------------
class TSaveAsForm : public TForm
{
__published:	// IDE-managed Components
    TLabeledEdit *LabelSaveAs;
    TButton *ButtonSave;
    TButton *ButtonCancel;
    void __fastcall ButtonSaveClick(TObject *Sender);
private:	// User declarations
    short WorkMode;// 1 - Save as stand-alone file (etalon); 0 - add to utmi  
    TFileUTMI UTMIFile;
    TPointTMI *EtalonPointsArray;
    long EtalonPointsCounter;
    AnsiString UTMIFileName;
    AnsiString FeedbackPotentiometerParameterName;
    TCommonEvaluation<double,double> *AnalysisObject;

    bool OpenUTMI();
    void CloseUTMI();


public:		// User declarations
    __fastcall TSaveAsForm(TComponent* Owner);
    void LoadForm(short WorkMode, AnsiString ParameterName, const TCommonEvaluation<double,double> *AnalysisObject, AnsiString UTMIFileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TSaveAsForm *SaveAsForm;
//---------------------------------------------------------------------------
#endif
