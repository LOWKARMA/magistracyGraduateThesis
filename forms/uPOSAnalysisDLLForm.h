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
//---------------------------------------------------------------------------
class TuPOSAnalysisForm : public TForm
{
__published:	// IDE-managed Components
    TButton *LoadParametersButton;
    TGroupBox *GroupBoxFileParameters;
    TCheckListBox *FileParametersCheckListBox;
    TGroupBox *GroupBoxModellingParameters;
    TTabbedNotebook *ParametersTabbedNotebook;
    TFileListBox *FileListBoxParametersList;
    TButton *Button1;
    TButton *Button2;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall LoadParametersButtonClick(TObject *Sender);
    void __fastcall FileParametersCheckListBoxClickCheck(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ParametersTabbedNotebookChange(TObject *Sender,
          int NewTab, bool &AllowChange);
    void __fastcall FileListBoxParametersListClick(TObject *Sender);
private:	// User declarations
    unsigned short CheckedItemsCount;
    AnsiString ModellingParametersFile, AnalysisParametersFile, StepmotorCharactristicsFile, TSUparametersFile;
public:		// User declarations
    __fastcall TuPOSAnalysisForm(TComponent* Owner);
    AnsiString UTMIFileName;
    void __fastcall LoadInfoOnForm();
    //bool LoadModellingParameters(AnsiStirng ModellingParametersFile, );

};
//---------------------------------------------------------------------------
extern PACKAGE TuPOSAnalysisForm *uPOSAnalysisForm;
//---------------------------------------------------------------------------
#endif
