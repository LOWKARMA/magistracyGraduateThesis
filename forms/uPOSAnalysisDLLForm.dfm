object uPOSAnalysisForm: TuPOSAnalysisForm
  Left = 963
  Top = 202
  Width = 935
  Height = 483
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
  object LoadParametersButton: TButton
    Left = 8
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Load'
    Enabled = False
    TabOrder = 0
    OnClick = LoadParametersButtonClick
  end
  object GroupBoxFileParameters: TGroupBox
    Left = 8
    Top = 8
    Width = 313
    Height = 313
    Caption = 'GroupBoxFileParameters'
    TabOrder = 1
    object FileParametersCheckListBox: TCheckListBox
      Left = 8
      Top = 16
      Width = 297
      Height = 289
      OnClickCheck = FileParametersCheckListBoxClickCheck
      ItemHeight = 13
      TabOrder = 0
    end
  end
  object GroupBoxModellingParameters: TGroupBox
    Left = 328
    Top = 8
    Width = 505
    Height = 313
    Caption = 'GroupBoxParameters'
    TabOrder = 2
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
    object Button1: TButton
      Left = 8
      Top = 280
      Width = 75
      Height = 25
      Caption = 'Button1'
      TabOrder = 2
    end
    object Button2: TButton
      Left = 88
      Top = 280
      Width = 75
      Height = 25
      Caption = 'Button2'
      TabOrder = 3
    end
  end
end
