//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Inifiles.hpp>
#include "uPOSAnalysisDLLForm.h"
#include <class_ParameterTMI_old.h>
#include <system.hpp>
//paramtetersEditors forms
#include "uPOSAnalysisDLLParametersEditorForm.h"
#include "uPOSModellingParametersEditorForm.h"
#include "uPOSStepMotorParametersEditorForm.h"
#include "uPOSTSUParametersEditorForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TuPOSAnalysisForm *uPOSAnalysisForm;
//int tmp[4]= {-1,-1,-1,-1};
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
    delete SelectedItemsFlags;
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::LoadInfoOnForm()
{

    TFileUTMI FileUTMI;
    AnsiString ErrorMessage;

    if(!FileUTMI.Open_uTMI(UTMIFileName))
    {
        FileUTMI.Get_Error(ErrorMessage);
        Application->MessageBoxA(ErrorMessage.c_str(), "������!", MB_OK|MB_ICONERROR);
        return;
    }

    FileUTMI.GetParsName((TStringList*)FileParametersCheckListBox->Items);
    FileParametersCheckListBox->MultiSelect = false;
    LoadParametersButton->Enabled = true;
    SelectedItemsFlags = new int [ParametersTabbedNotebook->Pages->Count];
    for(int i = 0; i < ParametersTabbedNotebook->Pages->Count; ++i)
        SelectedItemsFlags[i] = -1;
    SetStatusbarText("", "���� �� ��� ������.");
    //MainFormStatusBar->Panels->Items[1]->Text = "���� �� ��� ������.";
    FileListBoxParametersList->Mask = "*.ini";
}
//---------------------------------------------------------------------------
void TuPOSAnalysisForm::SetStatusbarText(AnsiString FirstMsg, AnsiString SecondMsg)
{
    MainFormStatusBar->Panels->Items[0]->Text = FirstMsg;
    MainFormStatusBar->Panels->Items[1]->Text = SecondMsg;
}
//---------------------------------------------------------------------------
bool TuPOSAnalysisForm::CheckParametersFilesPath()
{
    if(ModellingParametersFile.IsEmpty())
        return false;
    else if(AnalysisParametersFile.IsEmpty())
        return false;
    else if(StepmotorCharactristicsFile.IsEmpty())
        return false;
    else if(TSUparametersFile.IsEmpty())
        return false;
    else
        return true;
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
    SetStatusbarText("", "���� �� ��� ������.");
    //if( !(CheckedItemsCount == 2 && FileListBoxParametersList->FileName != "") ) //������ ������� ������ ��� ������ 2-� ���������� �
    if( CheckedItemsCount != 2 || !CheckParametersFilesPath())
    {
        SetStatusbarText("������!", "�������� ������� ��������� � ��������� �������.");
        return;
    }
    TFileUTMI FileUTMI;
    FileUTMI.Open_uTMI(UTMIFileName);
    TParTMI *CheckhedParameters = new TParTMI [CheckedItemsCount];

    for(int i = 0, j = 0; i < FileParametersCheckListBox->Count; i++)
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
    if(!POSModellingParameters.LoadFormIniFile(ModellingParametersFile))
    {
        SetStatusbarText("������!", "������ ��������\\������ ����� ���������� �������������.");
        return;
    }

    TDigitalQuantumParameters <double> ACDParameters;
    if(!ACDParameters.LoadFormIniFile(ModellingParametersFile))
    {
        SetStatusbarText("������!", "������ ��������\\������ ����� ���������� ���.");
        return;
    }

    TCommonEvaluationParameters CommonParameters;
    if(!CommonParameters.LoadFormIniFile(AnalysisParametersFile))
    {
        SetStatusbarText("������!", "������ ��������\\������ ����� ���������� ������ �������.");
        return;
    }

    TStepMotorCharacteristics StepMotorCharacteristics;
    if(!StepMotorCharacteristics.LoadFormIniFile(StepmotorCharactristicsFile))
    {
        SetStatusbarText("������!", "������ ��������\\������ ����� ���������� ������������� ��.");
        return;
    }

    TControlImpulse TSULevels;
    if(!TSULevels.LoadFormIniFile(TSUparametersFile))
    {
        SetStatusbarText("������!", "������ ��������\\������ ����� ���������� ���.");
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TuPOSAnalysisForm::FileParametersCheckListBoxClickCheck(TObject *Sender) //������ �������� ������ 2-� ����������                                                   //
{
    for(int i = 0; i < FileParametersCheckListBox->Count; i++)
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

void __fastcall TuPOSAnalysisForm::ButtonDeleteParameterFileClick(
      TObject *Sender)
{
    if(FileListBoxParametersList->FileName.IsEmpty())
    {
        SetStatusbarText("������!", "�� ������ ���� ��� ��������.");
        return;
    }
    else if(6 == Application->MessageBox("�� ������������� ������ ������� ���� ����?\0", "��������������.\0", MB_YESNO|MB_ICONERROR))
    {
        DeleteFile(FileListBoxParametersList->FileName);
        SetStatusbarText("", "���� �����.");
        FileListBoxParametersList->Update();
    }
}
//---------------------------------------------------------------------------

void __fastcall TuPOSAnalysisForm::ButtonEditParameterFileClick(
      TObject *Sender)
{
    if(FileListBoxParametersList->FileName.IsEmpty())
    {
        SetStatusbarText("������!", "������ �������� �������������� �����.");
        return;
    }

    switch(ParametersTabbedNotebook->PageIndex)
    {
        case 0:
            //SetStatusbarText("������!", "����� ���������� ������������� ��������������");
            Application->CreateForm(__classid(TModellingParametersEditorForm), &ModellingParametersEditorForm);
            ModellingParametersEditorForm->LoadForm(FileListBoxParametersList->FileName);
            break;
        case 1:
            Application->CreateForm(__classid(TAnalysisParametersEditorForm), &AnalysisParametersEditorForm);
            AnalysisParametersEditorForm->LoadForm(FileListBoxParametersList->FileName);
            break;
        case 2:
            Application->CreateForm(__classid(TStepMotorCharacteristicsEditorForm), &StepMotorCharacteristicsEditorForm);
            StepMotorCharacteristicsEditorForm->LoadForm(FileListBoxParametersList->FileName);
            break;
        case 3:
            Application->CreateForm(__classid(TTSUparametersEditorForm), &TSUparametersEditorForm);
            TSUparametersEditorForm->LoadForm(FileListBoxParametersList->FileName);
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TuPOSAnalysisForm::ButtonAddParameterFileClick(
      TObject *Sender)
{
    switch(ParametersTabbedNotebook->PageIndex)
    {
        case 0:
            Application->CreateForm(__classid(TModellingParametersEditorForm), &ModellingParametersEditorForm);
            ModellingParametersEditorForm->LoadForm(ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\ModellingParameters\\");
            break;
        case 1:
            Application->CreateForm(__classid(TAnalysisParametersEditorForm), &AnalysisParametersEditorForm);
            AnalysisParametersEditorForm->LoadForm(ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\AnalysisParameters\\"); //NewFile.ini");
            break;
        case 2:
            Application->CreateForm(__classid(TStepMotorCharacteristicsEditorForm), &StepMotorCharacteristicsEditorForm);
            StepMotorCharacteristicsEditorForm->LoadForm(ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\SepmotorsCharacteristics\\");
            break;
        case 3:
            Application->CreateForm(__classid(TTSUparametersEditorForm), &TSUparametersEditorForm);
            TSUparametersEditorForm->LoadForm(ExtractFilePath(Application->ExeName) + "uPOSModellingParameters\\TSUParameters\\");
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------

