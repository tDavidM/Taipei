object fSelectGame: TfSelectGame
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Select Game Number:'
  ClientHeight = 76
  ClientWidth = 179
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object eGameNo: TEdit
    Left = 16
    Top = 11
    Width = 149
    Height = 21
    TabOrder = 0
    Text = '1'
  end
  object bOK: TButton
    Left = 16
    Top = 42
    Width = 65
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = bOKClick
  end
  object bCancel: TButton
    Left = 99
    Top = 42
    Width = 66
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    OnClick = bCancelClick
  end
end
