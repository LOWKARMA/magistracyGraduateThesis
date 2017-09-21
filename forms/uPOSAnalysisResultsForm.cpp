//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uPOSAnalysisResultsForm.h"
#include "uPOSSaveParameterAS.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAnalysisResultsForm *AnalysisResultsForm;
//---------------------------------------------------------------------------
__fastcall TAnalysisResultsForm::TAnalysisResultsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisResultsForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;
    //delete EtalonPointsArray;
}
//---------------------------------------------------------------------------
void TAnalysisResultsForm::LoadForm(int IntervalsCounter, const TIntervalCharacteristics* FailedIntervalsList,
                                    const TCommonEvaluation<double, double> *AnalysisObject, AnsiString FeedbackPotentiometerParameterName,
                                    AnsiString UTMIFileName)
{

    this->FeedbackPotentiometerParameterName = FeedbackPotentiometerParameterName;
    this->UTMIFileName = UTMIFileName;
    this->AnalysisObject = AnalysisObject;

    //EtalonPointsArray = new TPointTMI;
    //EtalonPointsArray = AnalysisObject->GetModelAsArrayOfPoints(EtalonPointsCounter);

    if(0 != IntervalsCounter)
    {
        Memo->Text = " Количество сбойных интервалов: " + (AnsiString)IntervalsCounter + ".\n";
        for(unsigned int i = 1; i <= IntervalsCounter; i++)
            Memo->Text += (AnsiString)FailedIntervalsList->IntervalStartTime + "\t" + (AnsiString)FailedIntervalsList->IntervalEndTime + ".\n";
    }
    else
    {
         Memo->Text = "В результате анализа замечаний не выявлено.";
    }
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisResultsForm::ButtonAddToUTMIClick(TObject *Sender)
{

    Application->CreateForm(__classid(TSaveAsForm), &SaveAsForm);
    SaveAsForm->LoadForm(0, FeedbackPotentiometerParameterName, AnalysisObject, UTMIFileName);

    /*
    if(!OpenUTMI())
        return;
    TParTMI FeedbackPotentiometer;
    FeedbackPotentiometer.Load(EtalonPointsArray, EtalonPointsCounter, 0);
    FeedbackPotentiometer.Name = FeedbackPotentiometerParameterName + " | эталон";
    if(!UTMIFile.LoadPar(FeedbackPotentiometer))
    {
        AnsiString ErrorMessage;
        UTMIFile.Get_Warning(ErrorMessage);
        Application->MessageBoxA(ErrorMessage.c_str(), "Ошибка!", MB_OK|MB_ICONERROR);

    }
    CloseUTMI();
    */
}
//---------------------------------------------------------------------------
void __fastcall TAnalysisResultsForm::ButtonSaveAsEtalonClick(
      TObject *Sender)
{
    Application->CreateForm(__classid(TSaveAsForm), &SaveAsForm);
    SaveAsForm->LoadForm(1, "", AnalysisObject, UTMIFileName);
    /*
    if(!OpenUTMI())
        return;
    TParTMI FeedbackPotentiometer;
    FeedbackPotentiometer.Load(EtalonPointsArray, EtalonPointsCounter, 0);
    FeedbackPotentiometer.Name = FeedbackPotentiometerParameterName + " | эталон";
    UTMIFile.LoadPar(FeedbackPotentiometer);
    CloseUTMI();
    */
}
//---------------------------------------------------------------------------
