//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <ClassTStepMotor.h>
#include "uPOSTSUParametersEditorForm.h"
//#include <SomeFunctions.h>
#include <io.h>

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
void TTSUparametersEditorForm::SetStatusbarText(AnsiString Message)
{
    StatusBar->Panels->Items[0]->Text = Message;
}
//---------------------------------------------------------------------------
void TTSUparametersEditorForm::LoadForm(AnsiString SelectedFileName)
{
    FileName = SelectedFileName;
    LabeledEdit_FileName->Text = GetFileNameWithoutExt(ExtractFileName(FileName));
    StringGrid->Cells[0][0] = "Уровень 0 ";
    StringGrid->Cells[0][1] = "Уровень 0 ";
    StringGrid->Cells[0][2] = "Уровень 1-2 ";
    StringGrid->Cells[0][3] = "Уровень 1-2 ";
    StringGrid->Cells[0][4] = "Уровень 2-3 ";
    StringGrid->Cells[0][5] = "Уровень 2-3 ";
    StringGrid->Cells[0][6] = "Уровень 3-4 ";
    StringGrid->Cells[0][7] = "Уровень 3-4 ";
    StringGrid->Cells[0][8] = "Уровень 4-1 ";
    StringGrid->Cells[0][9] = "Уровень 4-1 ";

    for(int i = 1; i <= StringGrid->RowCount; i++)
    {
        if(i % 2 != 0)//min
            StringGrid->Cells[0][i - 1] = StringGrid->Cells[0][i - 1] + "min";
        else
            StringGrid->Cells[0][i - 1] = StringGrid->Cells[0][i - 1] + "max";
    }

    TControlImpulse ControlImpulses(SelectedFileName);

    StringGrid->Cells[1][0] = ControlImpulses.Level_0Min;
    StringGrid->Cells[1][1] = ControlImpulses.Level_0Max;
    StringGrid->Cells[1][2] = ControlImpulses.Level_12Min;
    StringGrid->Cells[1][3] = ControlImpulses.Level_12Max;
    StringGrid->Cells[1][4] = ControlImpulses.Level_23Min;
    StringGrid->Cells[1][5] = ControlImpulses.Level_23Max;
    StringGrid->Cells[1][6] = ControlImpulses.Level_34Min;
    StringGrid->Cells[1][7] = ControlImpulses.Level_34Max;
    StringGrid->Cells[1][8] = ControlImpulses.Level_41Min;
    StringGrid->Cells[1][9] = ControlImpulses.Level_41Max;


    /*
    for(int i = 1; i <= StringGrid->RowCount; i++)
    {
        AnsiString Extremum;
        if(i % 2 != 0)//min
            Extremum = "min";
        else
            Extremum = "max";

        if(1 == i)  // lvl0
            StringGrid->Cells[0][i - 1] = "Уровень " + (AnsiString)(i - 1) + " " + Extremum;
        else if(2 == i)
            StringGrid->Cells[0][i - 1] = "Уровень " + (AnsiString)(i - 2) + " " + Extremum;
        else
            StringGrid->Cells[0][i - 1] = "Уровень " + (AnsiString)((i)%10) + " " + Extremum;
    }
    */
}
//---------------------------------------------------------------------------
void __fastcall TTSUparametersEditorForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TTSUparametersEditorForm::Button_SaveClick(TObject *Sender)
{
    SetStatusbarText("");

    if(LabeledEdit_FileName->Text.IsEmpty())
    {
        SetStatusbarText("Некорректное имя файла.");
        return;
    }

    FileName = ExtractFilePath(FileName) + LabeledEdit_FileName->Text + ".ini";

    TControlImpulse ControlImpulses;

    if(!TryToFloat(StringGrid->Cells[1][0], ControlImpulses.Level_0Min))
    {
        SetStatusbarText("Значение графы \"0min ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][0] = ControlImpulses.Level_0Min;

    if(!TryToFloat(StringGrid->Cells[1][1], ControlImpulses.Level_0Max))
    {
        SetStatusbarText("Значение графы \"0max ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][1] = ControlImpulses.Level_0Max;

    if(!TryToFloat(StringGrid->Cells[1][2], ControlImpulses.Level_12Min))
    {
        SetStatusbarText("Значение графы \"1-2min ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][2] = ControlImpulses.Level_12Min;

    if(!TryToFloat(StringGrid->Cells[1][3], ControlImpulses.Level_12Max))
    {
        SetStatusbarText("Значение графы \"1-2max ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][3] = ControlImpulses.Level_12Max;

    if(!TryToFloat(StringGrid->Cells[1][4], ControlImpulses.Level_23Min))
    {
        SetStatusbarText("Значение графы \"2-3min ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][4] = ControlImpulses.Level_23Min;

    if(!TryToFloat(StringGrid->Cells[1][5], ControlImpulses.Level_23Max))
    {
        SetStatusbarText("Значение графы \"2-3max ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][5] = ControlImpulses.Level_23Max;

    if(!TryToFloat(StringGrid->Cells[1][6], ControlImpulses.Level_34Min))
    {
        SetStatusbarText("Значение графы \"3-4min ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][6] = ControlImpulses.Level_34Min;

    if(!TryToFloat(StringGrid->Cells[1][7], ControlImpulses.Level_34Max))
    {
        SetStatusbarText("Значение графы \"3-4max ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][7] = ControlImpulses.Level_34Max;

    if(!TryToFloat(StringGrid->Cells[1][8], ControlImpulses.Level_41Min))
    {
        SetStatusbarText("Значение графы \"4-1min ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][8] = ControlImpulses.Level_41Min;

    if(!TryToFloat(StringGrid->Cells[1][9], ControlImpulses.Level_41Max))
    {
        SetStatusbarText("Значение графы \"4-1max ...\" - не является float");
        return;
    }
    else
        StringGrid->Cells[1][9] = ControlImpulses.Level_41Max;


    if(0 == access(FileName.c_str(), 00))
    {
        //SetStatusbarText("Файл Существует");
        if(Application->MessageBoxA("Файл с этим именем уже существует.\nВы действительно хотите его перезаписать?", "Внимание!", MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
        {
            ControlImpulses.SaveToIniFile(FileName);
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
        ControlImpulses.SaveToIniFile(FileName);
        SetStatusbarText("Готово!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TTSUparametersEditorForm::Button_CancelClick(
      TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------



