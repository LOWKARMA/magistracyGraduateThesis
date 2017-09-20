object uPOSAnalysisParametersEditorForm: TuPOSAnalysisParametersEditorForm
  Left = 1553
  Top = 181
  Width = 324
  Height = 256
  Caption = 'uPOSAnalysisParametersEditorForm'
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
  PixelsPerInch = 96
  TextHeight = 13
  object Label_CommonEvaluationParameters: TLabel
    Left = 8
    Top = 56
    Width = 145
    Height = 13
    Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1086#1073#1097#1077#1075#1086' '#1072#1085#1072#1083#1080#1079#1072
  end
  object StringGrid: TStringGrid
    Left = 8
    Top = 72
    Width = 305
    Height = 67
    ColCount = 2
    DefaultColWidth = 150
    DefaultRowHeight = 20
    RowCount = 3
    FixedRows = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    ParentFont = False
    TabOrder = 0
  end
  object ButtonSave: TButton
    Left = 8
    Top = 160
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 1
    OnClick = ButtonSaveClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 160
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = ButtonCancelClick
  end
  object LabeledEditFileName: TLabeledEdit
    Left = 8
    Top = 24
    Width = 153
    Height = 21
    EditLabel.Width = 57
    EditLabel.Height = 13
    EditLabel.Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
    LabelPosition = lpAbove
    LabelSpacing = 3
    TabOrder = 3
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 210
    Width = 316
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = False
  end
end
