//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <ClassTCommonEvaluation.h>
#include "uPOSAnalysisDLLParametersEditorForm.h"
#include <io.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAnalysisParametersEditorForm *AnalysisParametersEditorForm;
//---------------------------------------------------------------------------
/*
bool TAnalysisParametersEditorForm::TryToInt(AnsiString &Number,  int &Variable)
{
    if(!TryStrToInt(Number, Variable))
    {
        Number = ChangeDecimalSeparator(Number);
        if(!TryStrToInt(Number, Variable))
            return false;
        else
            return true;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TAnalysisParametersEditorForm::TryToFloat(AnsiString &Number, double &Variable)
{
    if(!TryStrToFloat(Number, Variable))
    {
        Number = ChangeDecimalSeparator(Number);
        if(!TryStrToFloat(Number, Variable))
            return false;
        else
            return true;
    }
    return true;
}
*/
//---------------------------------------------------------------------------
void TAnalysisParametersEditorForm::SetStatusbarText(AnsiString Message)
{
    StatusBar->Panels->Items[0]->Text = Message;
}
//---------------------------------------------------------------------------
__fastcall TAnalysisParametersEditorForm::TAnalysisParametersEditorForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisParametersEditorForm::FormClose(
      TObject *Sender, TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------
void TAnalysisParametersEditorForm::LoadForm(AnsiString SelectedFileName)
{
    /*
    Height = 231;
    ClientHeight = 204;
    Width = 324;
    ClientWidth = 316;
    */
    AnalysisParametersEditorForm->FileName = SelectedFileName;
    StringGrid->Cells[0][0] = "Шаг квантования";
    StringGrid->Cells[0][1] = "Допуск на расхождение";
    StringGrid->Cells[0][2] = "Количество точек";
    LabeledEditFileName->Text = GetFileNameWithoutExt(ExtractFileName(FileName));
    TCommonEvaluationParameters EvaluationParameters(SelectedFileName);
    StringGrid->Cells[1][0] = EvaluationParameters.QantumStep;
    StringGrid->Cells[1][1] = EvaluationParameters.Measure;
    StringGrid->Cells[1][2] = EvaluationParameters.MinimumFailedPoints;
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisParametersEditorForm::ButtonSaveClick(
      TObject *Sender)
{
    SetStatusbarText("");

    if(LabeledEditFileName->Text.IsEmpty())
    {
        SetStatusbarText("Некорректное имя файла.");
        return;
    }

    FileName = ExtractFilePath(FileName) + LabeledEditFileName->Text + ".ini";
    TCommonEvaluationParameters EvaluationParameters;

    if(!TryToFloat(StringGrid->Cells[1][0], EvaluationParameters.QantumStep))
    {
        SetStatusbarText(/*(AnsiString)StringGrid->Cells[1][0] +*/ "Значение графы \"Шаг ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][0] = EvaluationParameters.QantumStep;

    if(!TryToFloat(StringGrid->Cells[1][1], EvaluationParameters.Measure))
    {
        SetStatusbarText(/*(AnsiString)StringGrid->Cells[1][1] +*/  "Значение графы \"Допуск ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][1] = EvaluationParameters.Measure;

    if(!TryToInt(StringGrid->Cells[1][2], EvaluationParameters.MinimumFailedPoints))
    {
        SetStatusbarText(/*(AnsiString)StringGrid->Cells[1][2] +*/  "Значение графы \"Кол-во точек ...\" - не является int");
        return;
    }
    else
        StringGrid->Cells[1][2] = EvaluationParameters.MinimumFailedPoints;

    if(0 == access(FileName.c_str(), 00))
    {
        //SetStatusbarText("Файл Существует");
        if(Application->MessageBoxA("Файл с этим именем уже существует.\nВы действительно хотите его перезаписать?", "Внимание!", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        {
            EvaluationParameters.SaveToIniFile(FileName);
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
        EvaluationParameters.SaveToIniFile(FileName);
        SetStatusbarText("Готово!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisParametersEditorForm::ButtonCancelClick(
      TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

