object uPOSAnalysisForm: TuPOSAnalysisForm
  Left = 751
  Top = 615
  Width = 914
  Height = 410
  Caption = #1040#1085#1072#1083#1080#1079' '#1055#1054#1057
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefaultPosOnly
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxFileParameters: TGroupBox
    Left = 8
    Top = 8
    Width = 313
    Height = 353
    Caption = #1057#1087#1080#1089#1086#1082' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074
    TabOrder = 0
    object LabelFirstAnalyseItem: TLabel
      Left = 8
      Top = 316
      Width = 139
      Height = 13
      BiDiMode = bdRightToLeft
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088' '#1059#1064#1052' '#1085#1077' '#1074#1099#1073#1088#1072#1085'.'
      ParentBiDiMode = False
    end
    object LabelSecondAnalyseItem: TLabel
      Left = 8
      Top = 332
      Width = 136
      Height = 13
      BiDiMode = bdRightToLeft
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088' '#1055#1054#1057' '#1085#1077' '#1074#1099#1073#1088#1072#1085'.'
      ParentBiDiMode = False
    end
    object FileParametersListBox: TListBox
      Left = 8
      Top = 16
      Width = 297
      Height = 265
      ItemHeight = 13
      TabOrder = 0
      OnMouseDown = FileParametersListBoxMouseDown
    end
    object AnalyseButton: TButton
      Left = 8
      Top = 288
      Width = 81
      Height = 25
      Caption = #1040#1085#1072#1083#1080#1079
      TabOrder = 1
      OnClick = AnalyseButtonClick
    end
  end
  object GroupBoxModellingParameters: TGroupBox
    Left = 336
    Top = 8
    Width = 561
    Height = 353
    Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099' '#1080' '#1087#1072#1088#1072#1084#1077#1090#1088#1099' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1103
    TabOrder = 1
    object WorkModePageControl: TPageControl
      Left = 8
      Top = 16
      Width = 545
      Height = 329
      ActivePage = TabSheetWorkModeWithModelling
      TabIndex = 0
      TabOrder = 0
      OnChange = WorkModePageControlChange
      object TabSheetWorkModeWithModelling: TTabSheet
        Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099' '#1089' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1077#1084' '#1055#1054#1057
        object ParametersTabbedNotebook: TTabbedNotebook
          Left = 0
          Top = 4
          Width = 489
          Height = 261
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
            object LabeledEditModellingTimeStart: TLabeledEdit
              Left = 0
              Top = 208
              Width = 121
              Height = 21
              EditLabel.Width = 154
              EditLabel.Height = 13
              EditLabel.Caption = #1042#1088#1077#1084#1103' '#1085#1072#1095#1072#1083#1072' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1103
              LabelPosition = lpAbove
              LabelSpacing = 3
              TabOrder = 0
            end
            object LabeledEditModellingTimeEnd: TLabeledEdit
              Left = 160
              Top = 208
              Width = 121
              Height = 21
              EditLabel.Width = 172
              EditLabel.Height = 13
              EditLabel.Caption = #1042#1088#1077#1084#1103' '#1086#1082#1086#1085#1095#1072#1085#1080#1103' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1103
              LabelPosition = lpAbove
              LabelSpacing = 3
              TabOrder = 1
            end
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
          Left = 8
          Top = 31
          Width = 473
          Height = 190
          ItemHeight = 13
          TabOrder = 1
          OnClick = FileListBoxParametersListClick
        end
        object ButtonEditParameterFile: TButton
          Left = 0
          Top = 272
          Width = 89
          Height = 25
          Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100
          TabOrder = 2
          OnClick = ButtonEditParameterFileClick
        end
        object ButtonAddParameterFile: TButton
          Left = 96
          Top = 272
          Width = 75
          Height = 25
          Caption = #1044#1086#1073#1072#1074#1080#1090#1100
          TabOrder = 3
          OnClick = ButtonAddParameterFileClick
        end
        object ButtonDeleteParameterFile: TButton
          Left = 176
          Top = 272
          Width = 75
          Height = 25
          Caption = #1059#1076#1072#1083#1080#1090#1100
          TabOrder = 4
          OnClick = ButtonDeleteParameterFileClick
        end
      end
      object TabSheetWorkModeWithEtalon: TTabSheet
        Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099' '#1089' '#1101#1090#1072#1083#1086#1085#1086#1084' '#1055#1054#1057
        ImageIndex = 1
        object EtalonFileListBox: TFileListBox
          Left = 8
          Top = 4
          Width = 521
          Height = 293
          ItemHeight = 13
          TabOrder = 0
          OnClick = EtalonFileListBoxClick
        end
      end
    end
  end
  object MainFormStatusBar: TStatusBar
    Left = 0
    Top = 364
    Width = 906
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
  object PopupMenuMarkParameters: TPopupMenu
    Left = 24
    Top = 32
    object MarkAsControlImpulses: TMenuItem
      Caption = #1054#1090#1084#1077#1090#1080#1090#1100' '#1082#1072#1082' '#1087#1072#1088#1072#1084#1077#1090#1088' '#1059#1064#1052
      OnClick = MarkAsControlImpulsesClick
    end
    object MarkAsFeedbackPotentiometer: TMenuItem
      Caption = #1054#1087#1084#1077#1090#1080#1090#1100' '#1082#1072#1082' '#1087#1072#1088#1072#1084#1077#1090#1088' '#1055#1054#1057
      OnClick = MarkAsFeedbackPotentiometerClick
    end
  end
end
