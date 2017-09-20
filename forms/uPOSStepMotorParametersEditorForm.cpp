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
    StringGrid->Cells[0][0] = "������ ���������� �� ���";
    StringGrid->Cells[0][1] = "Max �������� �����";
    StringGrid->Cells[0][2] = "������� ��������� �����";
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
        SetStatusbarText("������������ ��� �����.");
        return;
    }

    FileName = ExtractFilePath(FileName) + LabeledEdit_FileName->Text + ".ini";

    TStepMotorCharacteristics StepMotorCharacteristics;

    if(!TryToFloat(StringGrid->Cells[1][0], StepMotorCharacteristics.DeltaVoltage))
    {
        SetStatusbarText("�������� ����� \"������ ���������� ...\" - �� �������� float");
        return;
    }
    else
        StringGrid->Cells[1][0] = StepMotorCharacteristics.DeltaVoltage;

    if(!TryToFloat(StringGrid->Cells[1][1], StepMotorCharacteristics.MaximumLuft))
    {
        SetStatusbarText("�������� ����� \"Max �������� ...\" - �� �������� float");
        return;
    }
    else
        StringGrid->Cells[1][1] = StepMotorCharacteristics.MaximumLuft;

    if(!TryToFloat(StringGrid->Cells[1][2], StepMotorCharacteristics.CurrentLuftPosition))
    {
        SetStatusbarText("�������� ����� \"������� ��������� ...\" - �� �������� float");
        return;
    }
    else
        StringGrid->Cells[1][2] = StepMotorCharacteristics.CurrentLuftPosition;

    if(0 == access(FileName.c_str(), 00))
    {
        if(Application->MessageBoxA("���� � ���� ������ ��� ����������.\n�� ������������� ������ ��� ������������?", "��������!", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        {
            StepMotorCharacteristics.SaveToIniFile(FileName);
            SetStatusbarText("������!");
        }
        else
            return;
    }
    else if(0 == access(FileName.c_str(), 02))
    {
        Application->MessageBoxA("��� ���� �� ������ � ����� � ��������� ������!", "��������!", MB_OK|MB_ICONERROR);
        return;
    }
    else
    {
        StepMotorCharacteristics.SaveToIniFile(FileName);
        SetStatusbarText("������!");
    }
}
//---------------------------------------------------------------------------

