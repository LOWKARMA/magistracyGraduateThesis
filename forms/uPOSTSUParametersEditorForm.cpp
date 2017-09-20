//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uPOSTSUParametersEditorForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTSUparametersEditorForm *TSUparametersEditorForm;
//---------------------------------------------------------------------------
__fastcall TTSUparametersEditorForm::TTSUparametersEditorForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TTSUparametersEditorForm::LoadForm(AnsiString SelectedFileName)
{
    FileName = SelectedFileName;
}
//---------------------------------------------------------------------------
void __fastcall TTSUparametersEditorForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        Action = caFree;
}
//---------------------------------------------------------------------------

