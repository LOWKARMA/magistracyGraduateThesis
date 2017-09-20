object TSUparametersEditorForm: TTSUparametersEditorForm
  Left = 1315
  Top = 370
  BorderIcons = [biSystemMenu, biMaximize]
  BorderStyle = bsSingle
  Caption = 'TSUparametersEditorForm'
  ClientHeight = 340
  ClientWidth = 323
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
  PixelsPerInch = 96
  TextHeight = 13
  object Label_DigitalQuantumParameters: TLabel
    Left = 8
    Top = 56
    Width = 203
    Height = 13
    Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1084#1086#1076#1077#1083#1080#1088#1086#1074#1072#1085#1080#1103'  '#1089#1077#1090#1082#1080' '#1040#1062#1055
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 321
    Width = 323
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object LabeledEdit_FileName: TLabeledEdit
    Left = 8
    Top = 24
    Width = 153
    Height = 21
    EditLabel.Width = 57
    EditLabel.Height = 13
    EditLabel.Caption = #1048#1084#1103' '#1092#1072#1081#1083#1072
    LabelPosition = lpAbove
    LabelSpacing = 3
    TabOrder = 1
  end
  object StringGrid: TStringGrid
    Left = 8
    Top = 72
    Width = 305
    Height = 214
    ColCount = 2
    DefaultColWidth = 150
    DefaultRowHeight = 20
    RowCount = 10
    FixedRows = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    ParentFont = False
    TabOrder = 2
    RowHeights = (
      20
      20
      20
      20
      20
      20
      20
      20
      20
      20)
  end
  object Button_Save: TButton
    Left = 8
    Top = 292
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 3
  end
  object Button_Cancel: TButton
    Left = 96
    Top = 292
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 4
  end
end
