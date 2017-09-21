object TSUparametersEditorForm: TTSUparametersEditorForm
  Left = 1504
  Top = 161
  BorderIcons = [biSystemMenu, biMaximize]
  BorderStyle = bsSingle
  Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1058#1057#1059
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
    Width = 106
    Height = 13
    Caption = #1059#1088#1086#1074#1085#1080' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103':'
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
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 3
    OnClick = Button_SaveClick
  end
  object Button_Cancel: TButton
    Left = 96
    Top = 292
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 4
    OnClick = Button_CancelClick
  end
end
