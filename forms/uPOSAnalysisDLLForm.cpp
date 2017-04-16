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



int tmp[4]= {-1,-1,-1,-1};
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
    for(int i = 0; i < 4; ++i)
        SelectedItemsFlags[i] = -1;

}
//---------------------------------------------------------------------------
void TuPOSAnalysisForm::SetSelectedItemFlag(TFileListBox *TabbedNotebook, int TabIndex)
{
    for(int i = 0; i < TabbedNotebook->Items->Count; ++i)
    {
        if(TabbedNotebook->Selected[i])
        {
            SelectedItemsFlags[TabIndex] = TabbedNotebook->ItemIndex;
            break;
        }
    }
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

    TPOSModellingParameters <double> POSModellingParameters;
    POSModellingParameters.LoadFormIniFile(ModellingParametersFile);

    TDigitalQuantumParameters <double> ACDParameters;
    ACDParameters.LoadFormIniFile(ModellingParametersFile);

    TCommonEvaluationParameters CommonParameters;
    CommonParameters.LoadFormIniFile(AnalysisParametersFile);

    TStepMotorCharacteristics StepMotorCharacteristics;
    StepMotorCharacteristics.LoadFormIniFile(StepmotorCharactristicsFile);
    //TCommonEvaluation <double, double> POS;
    /*
    TIniFile* IniFile = new TIniFile(ModellingParametersFile);
    TStringList *List = new TStringList;
    IniFile->ReadSection("ModellingParmeters", List);
    IniFile->ReadSectionValues("ModellingParmeters", List);
    int tmp;
    for(unsigned int i = 0; i < List->Count; ++i)
    {
        //if(List->Names[i] == "test0")
        //StrToInt(List->Values[i]);
        List->Names[i];
        ShowMessage(List->Values[ List->Names[i]]);
        List->Strings[i];
        //List->Objects[i];
        //List->Text[i];
    }
    //List->Names
    */

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
             if(SelectedItemsFlags[NewTab] != -1)
                FileListBoxParametersList->Selected[SelectedItemsFlags[NewTab]] = true;
             break;
        case 1:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\AnalysisParameters";
            if(SelectedItemsFlags[NewTab] != -1)
                FileListBoxParametersList->Selected[SelectedItemsFlags[NewTab]] = true;
            break;
        case 2:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\SepmotorsCharacteristics";
            if(SelectedItemsFlags[NewTab] != -1)
                FileListBoxParametersList->Selected[SelectedItemsFlags[NewTab]] = true;
            break;
        case 3:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\TSUParameters";
            if(SelectedItemsFlags[NewTab] != -1)
                FileListBoxParametersList->Selected[SelectedItemsFlags[NewTab]] = true;
            break;
        default:
            FileListBoxParametersList->Directory = ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\";
            break;
    }
    //FileListBoxParametersList->
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::FileListBoxParametersListClick(
      TObject *Sender)
{
    switch(ParametersTabbedNotebook->PageIndex)
    {
        case 0:
            ModellingParametersFile = FileListBoxParametersList->FileName;
            /*
            for(int i = 0; i < FileListBoxParametersList->Items->Count; ++i)
                if(FileListBoxParametersList->Selected[i])
                    tmp[0] = FileListBoxParametersList->ItemIndex;
            */
            SetSelectedItemFlag(FileListBoxParametersList, ParametersTabbedNotebook->PageIndex);
            break;
        case 1:
            AnalysisParametersFile = FileListBoxParametersList->FileName;
            SetSelectedItemFlag(FileListBoxParametersList, ParametersTabbedNotebook->PageIndex);
            break;
        case 2:
            StepmotorCharactristicsFile = FileListBoxParametersList->FileName;
            SetSelectedItemFlag(FileListBoxParametersList, ParametersTabbedNotebook->PageIndex);
            break;
        case 3:
            TSUparametersFile = FileListBoxParametersList->FileName;
            SetSelectedItemFlag(FileListBoxParametersList, ParametersTabbedNotebook->PageIndex);
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

