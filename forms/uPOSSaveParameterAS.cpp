//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uPOSSaveParameterAS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSaveAsForm *SaveAsForm;
//---------------------------------------------------------------------------
__fastcall TSaveAsForm::TSaveAsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TSaveAsForm::OpenUTMI()
{
    AnsiString ErrorMessage;

    if(!UTMIFile.Open_uTMI(UTMIFileName))
    {
        UTMIFile.Get_Error(ErrorMessage);
        Application->MessageBoxA(ErrorMessage.c_str(), "Ошибка!", MB_OK|MB_ICONERROR);
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TSaveAsForm::CloseUTMI()
{
    UTMIFile.Close_uTMI();
}
//---------------------------------------------------------------------------
void TSaveAsForm::LoadForm(short WorkMode, AnsiString ParameterName, const TCommonEvaluation<double,double> *AnalysisObject,
                            AnsiString UTMIFileName)
{
    this->WorkMode = WorkMode;
    this->UTMIFileName = UTMIFileName;
    this->FeedbackPotentiometerParameterName = ParameterName;

    EtalonPointsArray = new TPointTMI;
    EtalonPointsArray = AnalysisObject->GetModelAsArrayOfPoints(EtalonPointsCounter);

    this->UTMIFile;
    if(0 == this->WorkMode)
    {
        SaveAsForm->Caption = "Добавить параметр в UTMI";
        LabelSaveAs->EditLabel->Caption = "Имя параметра:";
        LabelSaveAs->Text = ParameterName;
    }
    else
    {
        SaveAsForm->Caption = "Сохранить эталон в файл";
        LabelSaveAs->EditLabel->Caption = "Имя файла:";
    }
    SaveAsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TSaveAsForm::ButtonSaveClick(TObject *Sender)
{
    if(!OpenUTMI())
        return;

    TParTMI FeedbackPotentiometer;
    FeedbackPotentiometer.Load(EtalonPointsArray, EtalonPointsCounter, 0);
    FeedbackPotentiometer.Name = LabelSaveAs->Text;

    if(0 == this->WorkMode)
    {
        if(!UTMIFile.LoadPar(FeedbackPotentiometer))
        {
            AnsiString ErrorMessage;
            UTMIFile.Get_Warning(ErrorMessage);
            Application->MessageBoxA(ErrorMessage.c_str(), "Ошибка!", MB_OK|MB_ICONERROR);
            CloseUTMI();
            return;
        }
        CloseUTMI();
        SaveAsForm->Close();
    }
    else
    {

        CloseUTMI();
    }
}
//---------------------------------------------------------------------------

