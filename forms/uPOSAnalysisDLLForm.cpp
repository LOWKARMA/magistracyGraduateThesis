//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Inifiles.hpp>
#include "uPOSAnalysisDLLForm.h"
#include <class_ParameterTMI_old.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TuPOSAnalysisForm *uPOSAnalysisForm;

//---------------------------------------------------------------------------
__fastcall TuPOSAnalysisForm::TuPOSAnalysisForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
int CreatePOSAnalysisForm(char* CurrentFileName)
{
    if(!FileExists(CurrentFileName))
        return 1;

    Application->CreateForm(__classid(TuPOSAnalysisForm), &uPOSAnalysisForm);

    uPOSAnalysisForm->UTMIFileName = CurrentFileName;
    uPOSAnalysisForm->LoadInfoOnForm();

    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::LoadInfoOnForm()
{

    TFileUTMI FileUTMI;
    AnsiString ErrorMessage;

    if(!FileUTMI.Open_uTMI(UTMIFileName))
    {
        FileUTMI.Get_Error(ErrorMessage);
        Application->MessageBoxA(ErrorMessage.c_str(), "Ошибка!", MB_OK|MB_ICONERROR);
        return;
    }
    FileUTMI.GetParsName((TStringList*)FileParametersCheckListBox->Items);
    FileParametersCheckListBox->MultiSelect = false;
    LoadParametersButton->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::LoadParametersButtonClick(TObject *Sender)
{
    if( !(CheckedItemsCount == 2 && FileListBoxParametersList->FileName != "") ) //Кнопка активна только при выборе 2-х параметров и
        return;

    TFileUTMI FileUTMI;
    FileUTMI.Open_uTMI(UTMIFileName);
    TParTMI *CheckhedParameters = new TParTMI [CheckedItemsCount];


    for(unsigned int i = 0, j = 0; i < FileParametersCheckListBox->Count; i++)
    {
        if(!FileParametersCheckListBox->Checked[i])
            continue;
        else
        {
            //Application->MessageBoxA(FileParametersCheckListBox->Items->Strings[i].c_str(), "", MB_OK|MB_ICONERROR);
            CheckhedParameters[j] = *FileUTMI.GetPar(i);
            j++;
        }
        if(j >= CheckedItemsCount)
            break;
    }

    unsigned int PointsCounter = 0;
    TPointTMI *ControlImpulsesArray = new TPointTMI [CheckhedParameters[0].CountPoint];
    ControlImpulsesArray = CheckhedParameters[0].GetMasPointTMI(PointsCounter);
    TParameterTMI_Old <double, double> ControlImpulses(ControlImpulsesArray, PointsCounter);

    TPointTMI *ParameterPOSArray = new TPointTMI [CheckhedParameters[1].CountPoint];
    ParameterPOSArray = CheckhedParameters[1].GetMasPointTMI(PointsCounter);
    TParameterTMI_Old <double, double> ParameterPOS(ParameterPOSArray, PointsCounter);

    //TCommonEvaluation <double, double> POS;

    TIniFile* IniFile = new TIniFile(ModellingParametersFile);
    //TStringList List;
    //IniFile->

    /*
    for(unsigned int i = 0; i < CheckhedParameters[0].CountPoint; i++)
    {     CheckhedParameters[0].Point[i].Y;
           CheckhedParameters[1].Point[i].Y;} */

    /*
    TParTMI *CheckhedParameters = new TParTMI [FileParametersCheckListBox->SelCount];
    for(unsigned short i = 0; i < FileParametersCheckListBox->SelCount; i++)
        CheckhedParameters[i] = *FileUTMI->GetPar(i);

    for(unsigned int i = 0; i < CheckhedParameters[0].CountPoint; i++)
         CheckhedParameters[0].Point[i].X;
    */
}
//---------------------------------------------------------------------------


void __fastcall TuPOSAnalysisForm::FileParametersCheckListBoxClickCheck(TObject *Sender) //Нельзя выбирать больше 2-х параметров                                                   //
{
    for(unsigned int i = 0; i < FileParametersCheckListBox->Count; i++)
    {
        if(FileParametersCheckListBox->Selected[i] && FileParametersCheckListBox->Checked[i])
        {
            FileParametersCheckListBox->Checked[i] = true;
            ++CheckedItemsCount;
            if(2 < CheckedItemsCount)
            {
                FileParametersCheckListBox->Checked[i] = false;
                --CheckedItemsCount;
            }
            break;
        }
        else if(FileParametersCheckListBox->Selected[i] && !FileParametersCheckListBox->Checked[i])
        {
            FileParametersCheckListBox->Checked[i] = false;
            --CheckedItemsCount;
            break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::FormCreate(TObject *Sender)
{
    FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\ModellingParameters";
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::ParametersTabbedNotebookChange(
      TObject *Sender, int NewTab, bool &AllowChange)
{
    switch(NewTab)
    {
        case 0:
             FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\ModellingParameters";
             break;
        case 1:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\AnalysisParameters";
            break;
        case 2:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\SepmotorsCharacteristics";
            break;
        case 3:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\TSUParameters";
            break;
        default:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\";
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::FileListBoxParametersListClick(
      TObject *Sender)
{
    switch(ParametersTabbedNotebook->PageIndex)
    {
        case 0:
            ModellingParametersFile = FileListBoxParametersList->FileName;
            break;
        case 1:
            AnalysisParametersFile = FileListBoxParametersList->FileName;
            break;
        case 2:
            StepmotorCharactristicsFile = FileListBoxParametersList->FileName;
            break;
        case 3:
            TSUparametersFile = FileListBoxParametersList->FileName;
            break;
        default:
            ModellingParametersFile = "";
            AnalysisParametersFile = "";
            StepmotorCharactristicsFile = "";
            TSUparametersFile = "";
            break;
    }
}
//---------------------------------------------------------------------------

