object AnalysisResultsForm: TAnalysisResultsForm
  Left = 1259
  Top = 218
  Width = 580
  Height = 402
  Caption = 'AnalysisResultsForm'
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
  object GroupBoxAnalysisResults: TGroupBox
    Left = 8
    Top = 8
    Width = 553
    Height = 329
    Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090#1099' '#1072#1085#1072#1083#1080#1079#1072
    TabOrder = 0
    object Memo: TMemo
      Left = 8
      Top = 16
      Width = 537
      Height = 305
      Lines.Strings = (
        '')
      TabOrder = 0
    end
  end
  object ButtonSaveAsEtalon: TButton
    Left = 144
    Top = 344
    Width = 121
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1072#1082' '#1101#1090#1072#1083#1086#1085
    TabOrder = 1
    OnClick = ButtonSaveAsEtalonClick
  end
  object ButtonAddToUTMI: TButton
    Left = 8
    Top = 344
    Width = 121
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1074' UTMI'
    TabOrder = 2
    OnClick = ButtonAddToUTMIClick
  end
end
