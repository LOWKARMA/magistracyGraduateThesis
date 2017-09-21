//---------------------------------------------------------------------------

#ifndef uPOSStepMotorParametersEditorFormH
#define uPOSStepMotorParametersEditorFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TStepMotorCharacteristicsEditorForm : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StringGrid;
    TLabeledEdit *LabeledEdit_FileName;
    TButton *Button_Save;
    TButton *Button_Cancel;
    TLabel *Label_StepMotorCharacteristicsParameters;
    TStatusBar *StatusBar;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button_SaveClick(TObject *Sender);
    void __fastcall Button_CancelClick(TObject *Sender);
private:	// User declarations
    AnsiString FileName;
    void SetStatusbarText(AnsiString Message);
public:		// User declarations
    __fastcall TStepMotorCharacteristicsEditorForm(TComponent* Owner);
    void LoadForm(AnsiString);
};
//---------------------------------------------------------------------------
extern PACKAGE TStepMotorCharacteristicsEditorForm *StepMotorCharacteristicsEditorForm;
//---------------------------------------------------------------------------
#endif
