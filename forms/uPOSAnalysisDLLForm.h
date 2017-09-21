//---------------------------------------------------------------------------

#ifndef uPOSAnalysisDLLFormH
#define uPOSAnalysisDLLFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <class_FileUTMI.h>
#include <Tabs.hpp>
#include <ComCtrls.hpp>
#include <TabNotBk.hpp>
#include <FileCtrl.hpp>
#include <Menus.hpp>


//---------------------------------------------------------------------------
class TuPOSAnalysisForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBoxFileParameters;
    TGroupBox *GroupBoxModellingParameters;
    TTabbedNotebook *ParametersTabbedNotebook;
    TFileListBox *FileListBoxParametersList;
    TButton *ButtonEditParameterFile;
    TButton *ButtonDeleteParameterFile;
    TStatusBar *MainFormStatusBar;
    TButton *ButtonAddParameterFile;
    TLabeledEdit *LabeledEditModellingTimeStart;
    TLabeledEdit *LabeledEditModellingTimeEnd;
    TPopupMenu *PopupMenuMarkParameters;
    TMenuItem *MarkAsControlImpulses;
    TMenuItem *MarkAsFeedbackPotentiometer;
    TListBox *FileParametersListBox;
    TLabel *LabelFirstAnalyseItem;
    TLabel *LabelSecondAnalyseItem;
    TPageControl *WorkModePageControl;
    TTabSheet *TabSheetWorkModeWithModelling;
    TTabSheet *TabSheetWorkModeWithEtalon;
    TFileListBox *EtalonFileListBox;
    TButton *AnalyseButton;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    //void __fastcall LoadParametersButtonClick(TObject *Sender);
    //void __fastcall FileParametersCheckListBoxClickCheck(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ParametersTabbedNotebookChange(TObject *Sender,
          int NewTab, bool &AllowChange);
    void __fastcall FileListBoxParametersListClick(TObject *Sender);
    void __fastcall ButtonDeleteParameterFileClick(TObject *Sender);
    void __fastcall ButtonEditParameterFileClick(TObject *Sender);
    void __fastcall ButtonAddParameterFileClick(TObject *Sender);
    void __fastcall FileParametersListBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall MarkAsControlImpulsesClick(TObject *Sender);
    void __fastcall MarkAsFeedbackPotentiometerClick(TObject *Sender);
    void __fastcall WorkModePageControlChange(TObject *Sender);
    void __fastcall AnalyseButtonClick(TObject *Sender);
    void __fastcall EtalonFileListBoxClick(TObject *Sender);
private:	// User declarations
    unsigned short CheckedItemsCount;
    AnsiString ModellingParametersFile, AnalysisParametersFile, StepmotorCharactristicsFile, TSUparametersFile, FeedbackPotentiometerEtalonFileName;
    int ControlImpulsesParameterIndex, FeedbackPotentiometerParameterIndex;
    short WorkMode; // 0 - Work with modelling; 1 - work with etalon
    int *SelectedItemsFlags;
    bool CheckParametersFilesPath();
    void SetStatusbarText(AnsiString, AnsiString);
    TFileUTMI FileUTMI;

    void SetSelectedItemFlag(TFileListBox *TabbedNotebook, int TabIndex);
    bool OpenUTMI();
    void CloseUTMI();

public:		// User declarations
    __fastcall TuPOSAnalysisForm(TComponent* Owner);
    void __fastcall LoadInfoOnForm();
    AnsiString UTMIFileName;
    //bool LoadModellingParameters(AnsiStirng ModellingParametersFile, );

};
//---------------------------------------------------------------------------
extern PACKAGE TuPOSAnalysisForm *uPOSAnalysisForm;
//---------------------------------------------------------------------------
#endif
