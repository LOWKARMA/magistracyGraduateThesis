object uPOSAnalysisForm: TuPOSAnalysisForm
  Left = 264
  Top = 141
  Width = 1283
  Height = 775
  Caption = 'uPOSAnalysisForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxFileParameters: TGroupBox
    Left = 8
    Top = 8
    Width = 313
    Height = 313
    Caption = 'GroupBoxFileParameters'
    TabOrder = 0
    object FileParametersCheckListBox: TCheckListBox
      Left = 8
      Top = 16
      Width = 297
      Height = 257
      OnClickCheck = FileParametersCheckListBoxClickCheck
      ItemHeight = 13
      TabOrder = 0
    end
    object LoadParametersButton: TButton
      Left = 8
      Top = 280
      Width = 75
      Height = 25
      Caption = 'Load'
      Enabled = False
      TabOrder = 1
      OnClick = LoadParametersButtonClick
    end
  end
  object GroupBoxModellingParameters: TGroupBox
    Left = 328
    Top = 8
    Width = 505
    Height = 313
    Caption = 'GroupBoxParameters'
    TabOrder = 1
    object ParametersTabbedNotebook: TTabbedNotebook
      Left = 8
      Top = 16
      Width = 489
      Height = 257
      TabFont.Charset = DEFAULT_CHARSET
      TabFont.Color = clBtnText
      TabFont.Height = -11
      TabFont.Name = 'MS Sans Serif'
      TabFont.Style = []
      TabOrder = 0
      OnChange = ParametersTabbedNotebookChange
      object TTabPage
        Left = 4
        Top = 24
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1103
      end
      object TTabPage
        Left = 4
        Top = 24
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1072#1085#1072#1083#1080#1079#1072
      end
      object TTabPage
        Left = 4
        Top = 24
        Caption = #1061#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1080' '#1064#1052
      end
      object TTabPage
        Left = 4
        Top = 24
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1058#1057#1059
      end
    end
    object FileListBoxParametersList: TFileListBox
      Left = 16
      Top = 40
      Width = 473
      Height = 225
      ItemHeight = 13
      TabOrder = 1
      OnClick = FileListBoxParametersListClick
    end
    object ButtonEditParameterFile: TButton
      Left = 8
      Top = 280
      Width = 89
      Height = 25
      Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100
      TabOrder = 2
      OnClick = ButtonEditParameterFileClick
    end
    object ButtonDeleteParameterFile: TButton
      Left = 184
      Top = 280
      Width = 75
      Height = 25
      Caption = #1059#1076#1072#1083#1080#1090#1100
      TabOrder = 3
      OnClick = ButtonDeleteParameterFileClick
    end
    object ButtonAddParameterFile: TButton
      Left = 104
      Top = 280
      Width = 75
      Height = 25
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100
      TabOrder = 4
      OnClick = ButtonAddParameterFileClick
    end
  end
  object MainFormStatusBar: TStatusBar
    Left = 0
    Top = 729
    Width = 1275
    Height = 19
    Panels = <
      item
        Width = 50
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
end
