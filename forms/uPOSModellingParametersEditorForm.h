//---------------------------------------------------------------------------

#ifndef uPOSModellingParametersEditorFormH
#define uPOSModellingParametersEditorFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TModellingParametersEditorForm : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StringGrid_ModellingParameters;
    TStringGrid *StringGrid_DigitalQuantumParameters;
    TLabeledEdit *LabeledEdit_FileName;
    TLabel *Label_ModellingParmeters;
    TLabel *Label_DigitalQuantumParameters;
    TButton *Button_Save;
    TButton *Button_Cancel;
    TStatusBar *StatusBar;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button_SaveClick(TObject *Sender);
    void __fastcall Button_CancelClick(TObject *Sender);
private:	// User declarations
    AnsiString FileName;
    void SetStatusbarText(AnsiString Message);
public:		// User declarations
    __fastcall TModellingParametersEditorForm(TComponent* Owner);
    void LoadForm(AnsiString);
};
//---------------------------------------------------------------------------
extern PACKAGE TModellingParametersEditorForm *ModellingParametersEditorForm;
//---------------------------------------------------------------------------
#endif
