//---------------------------------------------------------------------------

#ifndef uPOSAnalysisDLLParametersEditorFormH
#define uPOSAnalysisDLLParametersEditorFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <SomeFunctions.h>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TuPOSAnalysisParametersEditorForm : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StringGrid;
    TButton *ButtonSave;
    TButton *ButtonCancel;
    TLabeledEdit *LabeledEditFileName;
    TLabel *Label_CommonEvaluationParameters;
    TStatusBar *StatusBar;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ButtonSaveClick(TObject *Sender);
    void __fastcall ButtonCancelClick(TObject *Sender);
private:	// User declarations
    AnsiString FileName;
    //int ParametersPageIndex;
    //void LoadInfoOnForm();
    void SetStatusbarText(AnsiString Message);
    bool TryToFloat(AnsiString &Number, double &Variable);
    bool TryToInt(AnsiString &Number,  int &Variable);
public:		// User declarations
    __fastcall TuPOSAnalysisParametersEditorForm(TComponent* Owner);
    void LoadForm(AnsiString);//, int);

};
//---------------------------------------------------------------------------
extern PACKAGE TuPOSAnalysisParametersEditorForm *uPOSAnalysisParametersEditorForm;
//---------------------------------------------------------------------------
#endif
