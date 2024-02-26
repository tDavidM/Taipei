object fTaipei: TfTaipei
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Taipei'
  ClientHeight = 310
  ClientWidth = 570
  Color = clGreen
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000F00200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000033333333333333333333000000000000
    30000000000000000003070000000000300FEFEFEFEFEFEFE003077000000000
    30F0000EF00EF0000E0307700000000030EFE000E00F000FEF03077000000000
    30FEFE00F00E00FEFE0307700000000030EFEF00E00F00EFEF03077000000000
    30FEFE00F00E00FEFE0307700000000030EF000000000000EF03077000000000
    30F00000000000000E0307700000000030E0EF00E00F00EF0F03077000000000
    30F0FE00F00E00FE0E0307700000000030E0EF00E00F00EF0F03077000000000
    30F00000000000000E0307700000000030EF000000000000EF03077000000000
    30FEFE00F00E00FEFE0307700000000030EFEF00E00F00EFEF03077000000000
    30FEFE00F00E00FEFE0307700000000030EFE000E00F000FEF03077000000000
    30F0000EF00EF0000E03077000000000300FEFEFEFEFEFEFE003077000000000
    3000000000000000000307700000000033333333333333333333077000000000
    0000000000000000000007700000000007777777777777777777707000000000
    0077777777777777777777000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000FFFF
    FFFFFFFFFFFFE000007FE000003FE000001FE000000FE000000FE000000FE000
    000FE000000FE000000FE000000FE000000FE000000FE000000FE000000FE000
    000FE000000FE000000FE000000FE000000FE000000FE000000FE000000FE000
    000FE000000FE000000FF000000FF800000FFC00000FFFFFFFFFFFFFFFFF0000
    000000000000}
  KeyPreview = True
  Menu = mMenu
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  DesignSize = (
    570
    310)
  PixelsPerInch = 96
  TextHeight = 13
  object iMainLogo: TImage
    Left = 113
    Top = 60
    Width = 347
    Height = 130
    Align = alCustom
    Center = True
    Transparent = True
  end
  object lMainTitleShadow: TLabel
    Left = 176
    Top = 193
    Width = 221
    Height = 16
    Alignment = taCenter
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Oriental Game of Skill and Chance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    Transparent = True
  end
  object lMainTitle: TLabel
    Left = 177
    Top = 194
    Width = 221
    Height = 16
    Align = alCustom
    Alignment = taCenter
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Oriental Game of Skill and Chance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    Transparent = True
  end
  object lDebug: TLabel
    Left = 8
    Top = 289
    Width = 7
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object lNbTileLayout: TLabel
    Left = 8
    Top = 8
    Width = 7
    Height = 13
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object mMenu: TMainMenu
    Left = 24
    Top = 8
    object mFile: TMenuItem
      Caption = 'File'
      Visible = False
      object mSave: TMenuItem
        Caption = 'Save Game'
        OnClick = mSaveClick
      end
      object mLoad: TMenuItem
        Caption = 'Load Game'
        OnClick = mLoadClick
      end
      object N5: TMenuItem
        Caption = '-'
        Visible = False
      end
      object mCreate: TMenuItem
        Caption = 'Create Layout'
        Visible = False
        OnClick = mCreateClick
      end
      object mPlay: TMenuItem
        Caption = 'Play Layout'
        Visible = False
        OnClick = mPlayClick
      end
    end
    object mGame: TMenuItem
      Caption = '&Game'
      object mNew: TMenuItem
        Caption = '&New'
        ShortCut = 113
        OnClick = mNewClick
      end
      object mSelect: TMenuItem
        Caption = '&Select Game...'
        OnClick = mSelectClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object mBackup: TMenuItem
        Caption = '&Backup (BkSp)'
        GroupIndex = 1
        OnClick = mBackupClick
      end
      object mHint: TMenuItem
        Caption = '&Hint (H)'
        GroupIndex = 1
        OnClick = mHintClick
      end
      object mStartOver: TMenuItem
        Caption = 'Start &Over'
        GroupIndex = 1
        OnClick = mStartOverClick
      end
      object N2: TMenuItem
        Caption = '-'
        GroupIndex = 1
      end
      object mAutoPlay: TMenuItem
        Caption = '&AutoPlay'
        GroupIndex = 2
        OnClick = mAutoPlayClick
      end
      object N3: TMenuItem
        Caption = '-'
        GroupIndex = 2
      end
      object mExit: TMenuItem
        Caption = 'E&xit'
        GroupIndex = 3
        OnClick = mExitClick
      end
    end
    object mOptions: TMenuItem
      Caption = '&Options'
      object mColor: TMenuItem
        Caption = '&Color'
        Checked = True
        OnClick = mColorClick
      end
      object mMessages: TMenuItem
        Caption = '&Messages'
        Checked = True
        OnClick = mMessagesClick
      end
      object mWatchBuilds: TMenuItem
        Caption = '&Watch Builds'
        Visible = False
        OnClick = mWatchBuildsClick
      end
      object mLighten: TMenuItem
        Caption = '&Lighten'
        Visible = False
        OnClick = mLightenClick
      end
      object mDarken: TMenuItem
        Caption = '&Darken'
        Visible = False
        OnClick = mDarkenClick
      end
      object mDifficulty: TMenuItem
        Caption = 'Difficult&y'
        Visible = False
        object mBeginner: TMenuItem
          Caption = '&Beginner'
          OnClick = mBeginnerClick
        end
        object mExpert: TMenuItem
          Caption = '&Expert'
          Checked = True
          OnClick = mExpertClick
        end
      end
      object mPeek: TMenuItem
        Caption = '&Peek'
        Visible = False
        OnClick = mPeekClick
      end
      object mZoom: TMenuItem
        Caption = 'Zoom'
        Visible = False
        object m100p: TMenuItem
          Caption = '100%'
          Checked = True
          OnClick = m100pClick
        end
        object m125p: TMenuItem
          Caption = '125%'
          OnClick = m125pClick
        end
        object m150p: TMenuItem
          Caption = '150%'
          OnClick = m150pClick
        end
        object m200p: TMenuItem
          Caption = '200%'
          OnClick = m200pClick
        end
      end
    end
    object mLayout: TMenuItem
      Caption = '&Layout'
      object mDebug: TMenuItem
        Caption = 'Debug'
        Visible = False
      end
      object mStandard: TMenuItem
        Tag = 1
        Caption = 'Standard'
        Checked = True
        OnClick = mLayoutMenuClick
      end
      object mBridge: TMenuItem
        Tag = 2
        Caption = 'Bridge'
        OnClick = mLayoutMenuClick
      end
      object mCastle: TMenuItem
        Tag = 3
        Caption = 'Castle'
        OnClick = mLayoutMenuClick
      end
      object mCube: TMenuItem
        Tag = 4
        Caption = 'Cube'
        OnClick = mLayoutMenuClick
      end
      object mGlyph: TMenuItem
        Tag = 5
        Caption = 'Glyph'
        OnClick = mLayoutMenuClick
      end
      object mPyramid: TMenuItem
        Tag = 6
        Caption = 'Pyramid'
        OnClick = mLayoutMenuClick
      end
      object mSpiral: TMenuItem
        Tag = 7
        Caption = 'Spiral'
        OnClick = mLayoutMenuClick
      end
      object mDragon: TMenuItem
        Tag = 8
        Caption = 'Dragon'
        Visible = False
        OnClick = mLayoutMenuClick
      end
      object mCustom: TMenuItem
        Tag = 99
        Caption = 'Custom'
        Visible = False
        OnClick = mLayoutMenuClick
      end
    end
    object mHelp: TMenuItem
      Caption = '&Help'
      object mHowtoPlay: TMenuItem
        Caption = '&How to Play'
        OnClick = mHowtoPlayClick
      end
      object mStrategy: TMenuItem
        Caption = '&Strategy'
        OnClick = mStrategyClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object mAbout: TMenuItem
        Caption = '&About Taipei...'
        OnClick = mAboutClick
      end
    end
  end
  object ilTiles: TImageList
    Height = 36
    Masked = False
    Width = 36
    Left = 72
    Top = 8
  end
  object XPManifest: TXPManifest
    Left = 24
    Top = 56
  end
  object tAutoPlay: TTimer
    Enabled = False
    Interval = 2500
    OnTimer = tAutoPlayTimer
    Left = 120
    Top = 8
  end
  object XMLDoc: TXMLDocument
    NodeIndentStr = '   '
    Options = [doNodeAutoCreate, doNodeAutoIndent, doAttrNull, doAutoPrefix, doNamespaceDecl]
    Left = 432
    Top = 8
    DOMVendorDesc = 'MSXML'
  end
  object GameSaveDialog: TSaveDialog
    DefaultExt = '.tpg'
    Filter = 'Taipei Game File|*.tpg'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 496
    Top = 64
  end
  object GameOpenDialog: TOpenDialog
    DefaultExt = '.tpg'
    Filter = 'Taipei Game File|*.tpg'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 496
    Top = 112
  end
  object LayoutSaveDialog: TSaveDialog
    DefaultExt = '.tpl'
    Filter = 'Taipei Game File|*.tpl'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 496
    Top = 232
  end
  object LayoutOpenDialog: TOpenDialog
    DefaultExt = '.tpl'
    Filter = 'Taipei Game File|*.tpl'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 496
    Top = 184
  end
  object mMainLayout: TMainMenu
    Left = 496
    Top = 8
    object mCreateLayout: TMenuItem
      Caption = '&File'
      object mEditLayout: TMenuItem
        Caption = '&Edit Layout'
        OnClick = mEditLayoutClick
      end
      object mSaveLayout: TMenuItem
        Caption = '&Save Layout'
        OnClick = mSaveLayoutClick
      end
      object mTestLayout: TMenuItem
        Caption = '&Play'
        OnClick = mTestLayoutClick
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object mClear: TMenuItem
        Caption = '&Clear'
        Hint = 'mClearLayout'
        OnClick = mClearClick
      end
      object mExitLayout: TMenuItem
        Caption = '&Return'
        OnClick = mExitLayoutClick
      end
    end
    object mLayer: TMenuItem
      Caption = '&Layer'
      object mLayer7: TMenuItem
        Tag = 7
        Caption = '&7 (Top)'
        OnClick = mLayerClick
      end
      object mLayer6: TMenuItem
        Tag = 6
        Caption = '&6'
        OnClick = mLayerClick
      end
      object mLayer5: TMenuItem
        Tag = 5
        Caption = '&5'
        OnClick = mLayerClick
      end
      object mLayer4: TMenuItem
        Tag = 4
        Caption = '&4'
        OnClick = mLayerClick
      end
      object mLayer3: TMenuItem
        Tag = 3
        Caption = '&3'
        OnClick = mLayerClick
      end
      object mLayer2: TMenuItem
        Tag = 2
        Caption = '&2'
        OnClick = mLayerClick
      end
      object mLayer1: TMenuItem
        Tag = 1
        Caption = '&1 (Bottom)'
        OnClick = mLayerClick
      end
    end
  end
end
