//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "uPOSModellingParametersEditorForm.h"

#include "ClassTCommonEvaluation.h"
#include <io.h>


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TModellingParametersEditorForm *ModellingParametersEditorForm;

//---------------------------------------------------------------------------
bool TModellingParametersEditorForm::TryToInt(AnsiString &Number,  int &Variable)
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
bool TModellingParametersEditorForm::TryToFloat(AnsiString &Number, double &Variable)
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
//---------------------------------------------------------------------------
void TModellingParametersEditorForm::SetStatusbarText(AnsiString Message)
{
    StatusBar->Panels->Items[0]->Text = Message;
}
//---------------------------------------------------------------------------
__fastcall TModellingParametersEditorForm::TModellingParametersEditorForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TModellingParametersEditorForm::LoadForm(AnsiString SelectedFileName)
{
    /*
    Height = 360;
    ClientHeight = 333;
    Width = 324;
    ClientWidth = 316;
    */
    FileName = SelectedFileName;

    StringGrid_ModellingParameters->Cells[0][0] = "Начальное напряжение";
    StringGrid_ModellingParameters->Cells[0][1] = "Частота точек";
    StringGrid_ModellingParameters->Cells[0][2] = "Сдвиг по шкале времени";

    StringGrid_DigitalQuantumParameters->Cells[0][0] = "Флаг использования";
    StringGrid_DigitalQuantumParameters->Cells[0][1] = "Способ округления";
    StringGrid_DigitalQuantumParameters->Cells[0][2] = "Нижняя граница сетки АЦП";
    StringGrid_DigitalQuantumParameters->Cells[0][3] = "Верхняя граница сетки АЦП";
    StringGrid_DigitalQuantumParameters->Cells[0][4] = "Количество интервалов квантования";

    LabeledEdit_FileName->Text = GetFileNameWithoutExt(ExtractFileName(FileName));

    TPOSModellingParameters <double> POSModellingParameters(SelectedFileName);
    StringGrid_ModellingParameters->Cells[1][0] = POSModellingParameters.StartVoltage;
    StringGrid_ModellingParameters->Cells[1][1] = POSModellingParameters.PointsFrequancy;
    StringGrid_ModellingParameters->Cells[1][2] = POSModellingParameters.TimeShift;

    TDigitalQuantumParameters <double> DigitalQuantumParameters(SelectedFileName);
    StringGrid_DigitalQuantumParameters->Cells[1][0] = BoolToStr(DigitalQuantumParameters.UsageFlag, true);
    StringGrid_DigitalQuantumParameters->Cells[1][1] = DigitalQuantumParameters.RoundingMethod;
    StringGrid_DigitalQuantumParameters->Cells[1][2] = DigitalQuantumParameters.QuantumLineBottomBorder;
    StringGrid_DigitalQuantumParameters->Cells[1][3] = DigitalQuantumParameters.QuantumLineTopBorder;
    StringGrid_DigitalQuantumParameters->Cells[1][4] = DigitalQuantumParameters.QuantumIntervals;
}
//---------------------------------------------------------------------------

void __fastcall TModellingParametersEditorForm::FormClose(TObject *Sender,
        TCloseAction &Action)
{
        Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TModellingParametersEditorForm::Button_SaveClick(
      TObject *Sender)
{
    SetStatusbarText("");

    if(LabeledEdit_FileName->Text.IsEmpty())
    {
        SetStatusbarText("Некорректное имя файла.");
        return;
    }

    FileName = ExtractFilePath(FileName) + LabeledEdit_FileName->Text + ".ini";


    //POS modelling
    TPOSModellingParameters <double> POSModellingParameters;

    if(!TryToFloat(StringGrid_ModellingParameters->Cells[1][0], POSModellingParameters.StartVoltage))
    {
        SetStatusbarText("Значение графы \"Начальное напряжение\" - не является float");
        return;
    }
    else
        StringGrid_ModellingParameters->Cells[1][0] = POSModellingParameters.StartVoltage;

    if(!TryToFloat(StringGrid_ModellingParameters->Cells[1][1], POSModellingParameters.PointsFrequancy))
    {
        SetStatusbarText("Значение графы \"Частота точек\" - не является float");
        return;
    }
    else
        StringGrid_ModellingParameters->Cells[1][1] = POSModellingParameters.PointsFrequancy;

    if(!TryToFloat(StringGrid_ModellingParameters->Cells[1][2], POSModellingParameters.TimeShift))
    {
        SetStatusbarText("Значение графы \"Сдвиг по шкале ...\" - не является float");
        return;
    }
    else
        StringGrid_ModellingParameters->Cells[1][2] = POSModellingParameters.TimeShift;

    //DigitalQuantum parameters
    TDigitalQuantumParameters <double> DigitalQuantumParameters;

    if(!TryStrToBool(StringGrid_DigitalQuantumParameters->Cells[1][0], DigitalQuantumParameters.UsageFlag))
    {
        SetStatusbarText("Значение графы \"Флаг использования\" - не является bool");
        return;
    }
    else
        StringGrid_DigitalQuantumParameters->Cells[1][0] = BoolToStr(DigitalQuantumParameters.UsageFlag, true);

    if(StringGrid_DigitalQuantumParameters->Cells[1][1].IsEmpty())
    {
        SetStatusbarText("Значение графы \"Способ округления\" - не является char");
        return;
    }
    else
    {
        AnsiString tmp = StringGrid_DigitalQuantumParameters->Cells[1][1];
        DigitalQuantumParameters.RoundingMethod = StringGrid_DigitalQuantumParameters->Cells[1][1].operator [](1);

        if(TryToInt(StringGrid_DigitalQuantumParameters->Cells[1][1], DigitalQuantumParameters.RoundingMethod))
        {
            SetStatusbarText("Значение графы \"Способ округления\" - не является char");
            return;
        }
        else
            StringGrid_DigitalQuantumParameters->Cells[1][1] = tmp;
    }

    if(!TryToFloat(StringGrid_DigitalQuantumParameters->Cells[1][2], DigitalQuantumParameters.QuantumLineBottomBorder))
    {
        SetStatusbarText("Значение графы \"Нижняя граница ...\" - не является float");
        return;
    }
    else
        StringGrid_DigitalQuantumParameters->Cells[1][2] = DigitalQuantumParameters.QuantumLineBottomBorder;

    if(!TryToFloat(StringGrid_DigitalQuantumParameters->Cells[1][3], DigitalQuantumParameters.QuantumLineTopBorder))
    {
        SetStatusbarText("Значение графы \"Верхняя граница ...\" - не является float");
        return;
    }
    else
        StringGrid_DigitalQuantumParameters->Cells[1][3] = DigitalQuantumParameters.QuantumLineTopBorder;

    if(!TryToInt(StringGrid_DigitalQuantumParameters->Cells[1][4], DigitalQuantumParameters.QuantumIntervals))
    {
        SetStatusbarText("Значение графы \"Количество интервалов ...\" - не является int");
        return;
    }
    else
        StringGrid_DigitalQuantumParameters->Cells[1][4] = DigitalQuantumParameters.QuantumIntervals;


    if(0 == access(FileName.c_str(), 00))
    {
        //SetStatusbarText("Файл Существует");
        if(Application->MessageBoxA("Файл с этим именем уже существует.\nВы действительно хотите его перезаписать?", "Внимание!", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        {
            POSModellingParameters.SaveToIniFile(FileName);
            DigitalQuantumParameters.SaveToIniFile(FileName);
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
        POSModellingParameters.SaveToIniFile(FileName);
        DigitalQuantumParameters.SaveToIniFile(FileName);
        SetStatusbarText("Готово!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TModellingParametersEditorForm::Button_CancelClick(
      TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

