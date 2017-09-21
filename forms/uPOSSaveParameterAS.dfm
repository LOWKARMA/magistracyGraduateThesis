object SaveAsForm: TSaveAsForm
  Left = 1544
  Top = 322
  Width = 272
  Height = 120
  Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LabelSaveAs: TLabeledEdit
    Left = 8
    Top = 24
    Width = 241
    Height = 21
    EditLabel.Width = 63
    EditLabel.Height = 13
    EditLabel.Caption = 'LabelSaveAs'
    LabelPosition = lpAbove
    LabelSpacing = 3
    TabOrder = 0
  end
  object ButtonSave: TButton
    Left = 8
    Top = 56
    Width = 75
    Height = 25
    Caption = 'ButtonSave'
    TabOrder = 1
    OnClick = ButtonSaveClick
  end
  object ButtonCancel: TButton
    Left = 96
    Top = 56
    Width = 75
    Height = 25
    Caption = 'ButtonCancel'
    TabOrder = 2
  end
end
