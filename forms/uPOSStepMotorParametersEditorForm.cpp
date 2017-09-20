//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ClassTCommonEvaluation.h"
#include "uPOSStepMotorParametersEditorForm.h"
#include <io.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStepMotorCharacteristicsEditorForm *StepMotorCharacteristicsEditorForm;
//---------------------------------------------------------------------------
__fastcall TStepMotorCharacteristicsEditorForm::TStepMotorCharacteristicsEditorForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TStepMotorCharacteristicsEditorForm::SetStatusbarText(AnsiString Message)
{
    StatusBar->Panels->Items[0]->Text = Message;
}
//---------------------------------------------------------------------------
void TStepMotorCharacteristicsEditorForm::LoadForm(AnsiString SelectedFileName)
{
    Height = 231;
    ClientHeight = 204;
    Width = 324;
    ClientWidth = 316;
    FileName = SelectedFileName;
    StringGrid->Cells[0][0] = "Дельта напряжения за шаг";
    StringGrid->Cells[0][1] = "Max значение люфта";
    StringGrid->Cells[0][2] = "Текущее положение люфта";
    LabeledEdit_FileName->Text = GetFileNameWithoutExt(ExtractFileName(FileName));
    TStepMotorCharacteristics StepMotorCharacteristics(SelectedFileName);
    StringGrid->Cells[1][0] = StepMotorCharacteristics.DeltaVoltage;
    StringGrid->Cells[1][1] = StepMotorCharacteristics.MaximumLuft;
    StringGrid->Cells[1][2] = StepMotorCharacteristics.CurrentLuftPosition;
}
//---------------------------------------------------------------------------
void __fastcall TStepMotorCharacteristicsEditorForm::FormClose(
      TObject *Sender, TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TStepMotorCharacteristicsEditorForm::Button_SaveClick(
      TObject *Sender)
{
    SetStatusbarText("");

    if(LabeledEdit_FileName->Text.IsEmpty())
    {
        SetStatusbarText("Некорректное имя файла.");
        return;
    }

    FileName = ExtractFilePath(FileName) + LabeledEdit_FileName->Text + ".ini";

    TStepMotorCharacteristics StepMotorCharacteristics;

    if(!TryToFloat(StringGrid->Cells[1][0], StepMotorCharacteristics.DeltaVoltage))
    {
        SetStatusbarText("Значение графы \"Дельта напряжения ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][0] = StepMotorCharacteristics.DeltaVoltage;

    if(!TryToFloat(StringGrid->Cells[1][1], StepMotorCharacteristics.MaximumLuft))
    {
        SetStatusbarText("Значение графы \"Max значение ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][1] = StepMotorCharacteristics.MaximumLuft;

    if(!TryToFloat(StringGrid->Cells[1][2], StepMotorCharacteristics.CurrentLuftPosition))
    {
        SetStatusbarText("Значение графы \"Текущее положение ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][2] = StepMotorCharacteristics.CurrentLuftPosition;

    if(0 == access(FileName.c_str(), 00))
    {
        if(Application->MessageBoxA("Файл с этим именем уже существует.\nВы действительно хотите его перезаписать?", "Внимание!", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        {
            StepMotorCharacteristics.SaveToIniFile(FileName);
            SetStatusbarText("Готово!");
        }
        else
            return;
    }
    else if(0 == access(FileName.c_str(), 02))
    {
        Application->MessageBoxA("Нет прав на доступ к файлу с указанным именем!", "Внимание!", MB_OK|MB_ICONERROR);
        return;
    }
    else
    {
        StepMotorCharacteristics.SaveToIniFile(FileName);
        SetStatusbarText("Готово!");
    }
}
//---------------------------------------------------------------------------

