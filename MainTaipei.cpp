//---------------------------------------------------------------------------

#include <vcl.h>
#include <mmsystem.h>
#pragma hdrstop

#include "MainTaipei.h"
#include "SelGame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTaipei *fTaipei;

#define TILESIZE       36
#define HALFTILESIZE   18
#define TILEXOFFSET    5
#define TILEYOFFSET    3
#define MAXNUMBERLAYER 7
#define GAMEHEIGHT     17
#define GAMEWIDTH      31

//---------------------------------------------------------------------------

//Constructor for a Tile
TTile::TTile(int pId, int pX, int pY, int pZ)
{
   this->Id   = pId;
   this->Next = NULL;

   this->X = pX;
   this->Y = pY;
   this->Z = pZ;

   this->Type  = -1;
   this->Graph = -1;
   this->Step  = -1;
   this->Hint  = -1;
   this->Debug = 0;

   this->Selected = false;
   this->Visible  = false;
}

//---------------------------------------------------------------------------

__fastcall TfTaipei::TfTaipei(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mExitClick(TObject *Sender)
{
   this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormCreate(TObject *Sender)
{
   this->DoubleBuffered = true;
   Application->Icon = this->Icon;
   this->Mode = 1;
   this->DebugDraw = false;

   for (int i=1;i<=ParamCount();i++) {
      if (LowerCase(ParamStr(i)) != "") {
         Beep();
         this->DebugDraw = true;
         this->Mode = 0;
         lDebug->Visible = true;
      }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mMessagesClick(TObject *Sender)
{
   this->mMessages->Checked = !this->mMessages->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mColorClick(TObject *Sender)
{
   this->mColor->Checked = !this->mColor->Checked;
   this->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mWatchBuildsClick(TObject *Sender)
{
   this->mWatchBuilds->Checked = !this->mWatchBuilds->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mAboutClick(TObject *Sender)
{
   AnsiString DialogCap = "About " + this->Caption;
   Application->MessageBox("Taipei !\n\nClone by David Morrissette\n\nPress T in main window for more options\n\n2020",
                           DialogCap.c_str(), MB_OK);
}
//--------------------------------------------------------------------------

void __fastcall TfTaipei::mHowtoPlayClick(TObject *Sender)
{
   AnsiString DialogText = "Taipei is a modern, solitaire version of the ancient oriental game, Mah-Jongg.\n\n"
                           "Playing Taipei is simple.\n"
                           "The object of Taipei is to remove all of the tiles from the board.\n\n"
                           "Tiles are removed from the board in matching pairs.\n"
                           "Tiles can only be removed if they are \"free\".\n"
                           "A tile is \"free\" if:\n"
                           "  1) it has no tiles on top of it, and\n"
                           "  2) you can \"slide\" the tile out to the right or left.";

   Application->MessageBox(DialogText.c_str(), "What is Taipei?", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mStrategyClick(TObject *Sender)
{
   AnsiString DialogText = "Winning a game is much like solving an intricate puzzle.\n"
                           "One false move at the beginning could ruin your chances at victory.\n"
                           "And, while every game has a solution, finding it often takes hours of diligent play.\n\n"
                           "Here are some hints if you are having difficulty solving the Taipei puzzles:\n"
                           "1.  Remove the end tiles as soon as you can, "
                           "especially those tiles that block more than one tile.\n"
                           "2.  If all four tiles in a matching set are free, you can remove all four safely.\n"
                           "3.  Work from the outside in.";

   Application->MessageBox(DialogText.c_str(), "Strategy", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mStartOverClick(TObject *Sender)
{
   TTile* CurrentTile;

   if (this->StepBack > 0) {
      this->tAutoPlay->Enabled = false;
      this->mAutoPlay->Checked = false;
      
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Step >= 0) {
            CurrentTile->Step = -1;
            CurrentTile->Visible = true;
         }
         CurrentTile = CurrentTile->Next;
      }
      this->StepBack = 0;
      this->HintLoop = 0;
      this->GamedDone = 0;
      this->Repaint();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mBackupClick(TObject *Sender)
{
   TTile* CurrentTile;

   if (this->StepBack > 0) {
      this->tAutoPlay->Enabled = false;
      this->mAutoPlay->Checked = false;

      this->StepBack--;
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Step == this->StepBack) {
            CurrentTile->Step = -1;
            CurrentTile->Visible = true;
            if (this->GamedDone > 0)
               this->GamedDone--;
         }
         CurrentTile = CurrentTile->Next;
      }
      this->HintLoop = 0;
      this->Repaint();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if(Key == 0x8 /*BackSpace*/)
      this->mBackup->Click();

   if (Key == 0x48 /*VK_KEY_H*/)
      this->mHint->Click();

   if (Key == 0x54 /*VK_KEY_H*/) {
      this->mDragon->Visible = true;
      this->mWatchBuilds->Visible = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormShow(TObject *Sender)
{
   this->TileList     = NULL;
   this->SelectedTile = NULL;

   this->lMainTitle->Caption = "Oriental Game of Skill and Chance\nVersion 6.00\nClone by David Morrissette\n2020";
   this->lMainTitleShadow->Caption = this->lMainTitle->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mLayoutMenuClick(TObject *Sender)
{
   this->mStandard->Checked = false;
   this->mBridge->Checked   = false;
   this->mCastle->Checked   = false;
   this->mCube->Checked     = false;
   this->mGlyph->Checked    = false;
   this->mPyramid->Checked  = false;
   this->mSpiral->Checked   = false;
   this->mDragon->Checked   = false;

   TMenuItem *Current = (TMenuItem *)Sender;
   Current->Checked = true;

   this->Mode = Current->Tag;
   this->mNewClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mNewClick(TObject *Sender)
{
   int GameNo;

   this->tAutoPlay->Enabled = false;
   this->mAutoPlay->Checked = false;
   this->iMainLogo->Visible = false;
   this->lMainTitleShadow->Visible = false;
   this->lMainTitle->Visible = false;
   this->StepBack = 0;
   this->GamedDone = 0;
   this->HintLoop = 0;
   this->SelectedTile = NULL;

   Randomize();
   GameNo = Random(32767);
   this->Caption = "Taipei Game  #" + IntToStr(GameNo);

   this->BuildStructure(this->Mode);
   this->FillStructure(GameNo);
   this->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mSelectClick(TObject *Sender)
{
   int GameNo;
   TfSelectGame *fSelectGame = new TfSelectGame( this );

   if (fSelectGame->ShowModal() == mrOk) {
      try {
         GameNo = StrToInt(fSelectGame->eGameNo->Text);
      } catch(Exception& e) {
         GameNo = 0;
      }
      if (GameNo > 32767)
         GameNo = 0;
      if (GameNo < -32767)
         GameNo = 0;
      if (GameNo < 0)
         GameNo = (65536 + GameNo);
      
      this->tAutoPlay->Enabled = false;
      this->mAutoPlay->Checked = false;
      this->iMainLogo->Visible = false;
      this->lMainTitleShadow->Visible = false;
      this->lMainTitle->Visible = false;
      this->StepBack = 0;
      this->HintLoop = 0;
      this->GamedDone = 0;
      this->SelectedTile = NULL;

      this->Caption = "Taipei Game  #" + IntToStr(GameNo);

      this->BuildStructure(this->Mode);
      this->FillStructure(GameNo);
      this->Repaint();
   }
   delete fSelectGame;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mHintClick(TObject *Sender)
{
   TTile* CurrentTile;
   TTile* StartingTile = NULL;
   TTile* SelectTile = NULL;
   bool GiveUp = false;
   bool TryNext = false;
   int LoopCmp = 144;

   this->tAutoPlay->Enabled = false;
   this->mAutoPlay->Checked = false;

   //Cancel any selection
   if (this->SelectedTile != NULL) {
      this->SelectedTile->Selected = false;
      this->SelectedTile = NULL;
   }

   if (this->HintLoop == 0)
      this->HintLoop = this->TileList->Id;

   //Just a quick test in case
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      if (CurrentTile->Id == this->HintLoop) {
         StartingTile = CurrentTile;
         CurrentTile = NULL;
      } else
         CurrentTile = CurrentTile->Next;
   }

   if (StartingTile == NULL)
      StartingTile = this->TileList;

   //Might need more than one try
   do {
      //prevent infinite loop
      if (StartingTile == this->TileList)
         GiveUp = true;

      TryNext = false;
      //Find the next available tile
      CurrentTile = StartingTile;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && this->IsTileFree(CurrentTile)) {
            this->HintLoop = CurrentTile->Id;
            this->SelectedTile = CurrentTile;
            CurrentTile = NULL;
         } else
            CurrentTile = CurrentTile->Next;
      }
      if (this->SelectedTile != NULL) {
         GiveUp = false;
         this->SelectedTile->Selected = true;
         SelectTile = NULL;
         //Select the matching available tile of the same type
         CurrentTile = this->SelectedTile->Next;
         while(CurrentTile != NULL) {
            if (CurrentTile->Visible && CurrentTile->Type == this->SelectedTile->Type &&
                 this->IsTileFree(CurrentTile)) {
               SelectTile = CurrentTile;
               SelectTile->Selected = true;
               CurrentTile = NULL;
            } else
               CurrentTile = CurrentTile->Next;
         }
         if (SelectTile == NULL) {
            this->SelectedTile->Selected = false;
            StartingTile = this->SelectedTile->Next;
            this->SelectedTile = NULL;
            TryNext = true;
         } else {
            this->Repaint();
            Sleep(100);
            this->HintLoop = this->SelectedTile->Next->Id;
            SelectTile->Selected = false;
            this->SelectedTile->Selected = false;
            this->SelectedTile = NULL;
            this->Repaint();
         }

      } else {
         StartingTile = this->TileList;
         TryNext = true;
      }
      LoopCmp--;
   } while(TryNext && !GiveUp && LoopCmp >= 0);

   if (GiveUp || LoopCmp < 0) {
      this->HintLoop = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mAutoPlayClick(TObject *Sender)
{
   this->mAutoPlay->Checked = !this->mAutoPlay->Checked;
   this->tAutoPlay->Enabled = this->mAutoPlay->Checked;
   if (this->mAutoPlay->Checked) {
      Sleep(200);
      this->tAutoPlayTimer(Sender);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::tAutoPlayTimer(TObject *Sender)
{
   TTile* CurrentTile = this->TileList;
   TTile* SelectTile = NULL;
   int MinHint = 72;

   if (this->SelectedTile != NULL) {
      this->SelectedTile->Selected = false;
      this->SelectedTile = NULL;
   }

   //Find the lowest Hint value from all free and visible tiles and select it
   while(CurrentTile != NULL) {
      if (CurrentTile->Visible && CurrentTile->Hint < MinHint && this->IsTileFree(CurrentTile)) {
         MinHint = CurrentTile->Hint;
         this->SelectedTile = CurrentTile;
      }
      CurrentTile = CurrentTile->Next;
   }
   if (this->SelectedTile != NULL)
      this->SelectedTile->Selected = true;
   else {
      //Rare Case
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && CurrentTile->Hint == MinHint && this->IsTileFree(CurrentTile)) {
            MinHint = CurrentTile->Hint;
            this->SelectedTile = CurrentTile;
            this->SelectedTile->Selected = true;
            CurrentTile = NULL;
         } else
            CurrentTile = CurrentTile->Next;
      }
   }

   //Select the matching tile with lowest Hint value if possible
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      if (CurrentTile->Visible && CurrentTile->Hint == MinHint &&
           CurrentTile->Id != this->SelectedTile->Id && this->IsTileFree(CurrentTile)) {
        CurrentTile->Selected = true;
        SelectTile = CurrentTile;
      }
     CurrentTile = CurrentTile->Next;
   }

   //Select the next lowest tile available of the same type
   if (SelectTile == NULL) {
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && CurrentTile->Hint != MinHint &&
              CurrentTile->Type == this->SelectedTile->Type && this->IsTileFree(CurrentTile)) {
            SelectTile = CurrentTile;
            SelectTile->Selected = true;
         }
         CurrentTile = CurrentTile->Next;
      }
   }

   if (SelectTile != NULL) {
      this->Repaint();
      Sleep(100);
      this->HideTileStep(SelectTile, true);
   } else {
      if (this->SelectedTile != NULL) {
         this->SelectedTile->Selected = false;
         this->SelectedTile = NULL;
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         PlaySound("SystemAsterisk", 0, SND_ALIAS || SND_ASYNC);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormMouseMove(TObject *Sender, TShiftState Shift,
   int X, int Y)
{
   TTile* CurrentTile;
   TPoint Pos;

   Pos.x = X;
   Pos.y = Y;

   CurrentTile = this->GetTile(Pos);
   if (CurrentTile != NULL) {
      this->lDebug->Caption = IntToStr(CurrentTile->Id) + ":" + IntToStr(CurrentTile->Hint);
      if (CurrentTile->Visible && this->IsTileFree(CurrentTile))
         Screen->Cursor = crCross;
      else
         Screen->Cursor = crDefault;
   } else
      Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormPaint(TObject *Sender)
{
   this->DrawAllTiles();
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormMouseDown(TObject *Sender, TMouseButton Button,
   TShiftState Shift, int X, int Y)
{
   TTile* CurrentTile;
   TPoint Pos;

   Pos.x = X;
   Pos.y = Y;

   CurrentTile = this->GetTile(Pos);
   if (CurrentTile != NULL ) {
      this->tAutoPlay->Enabled = false;
      this->mAutoPlay->Checked = false;
      
      if (CurrentTile->Visible) {
         if (!this->IsTileFree(CurrentTile)) {
            if (this->mMessages->Checked)
               Application->MessageBox("Tiles isn't free", "Taipei", MB_OK | MB_ICONINFORMATION);
         } else {
            //Select 1 tile
            if (this->SelectedTile == NULL) {
               CurrentTile->Selected = true;
               this->SelectedTile = CurrentTile;
               this->Repaint();
            } else {
               //Same tile = Deselect
               if(CurrentTile == this->SelectedTile) {
                  this->SelectedTile->Selected = false;
                  this->SelectedTile = NULL;
                  this->Repaint();
               } else {
                  //Match tile with selection
                  if (CurrentTile->Type == this->SelectedTile->Type){
                     this->HideTileStep(CurrentTile, false);
                  } else {
                     CurrentTile->Selected = true;
                     this->Repaint();
                     Sleep(75);
                     CurrentTile->Selected = false;
                     this->Repaint();
                     if (this->mMessages->Checked)
                        Application->MessageBox("Tiles don't match", "Taipei", MB_OK | MB_ICONINFORMATION);
                  }
               }
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

void TfTaipei::HideTileStep(TTile* pTile, bool pAutoPlay)
{
   TTile* CurrentTile;
   TTile* LookupTile;
   bool Found = false;
   int MsgNo;

   pTile->Visible = false;
   pTile->Selected = false;
   pTile->Step = this->StepBack;

   this->SelectedTile->Visible = false;
   this->SelectedTile->Selected = false;
   this->SelectedTile->Step = this->StepBack;

   this->SelectedTile = NULL;
   this->StepBack++;
   this->HintLoop = 0;
   this->Repaint();

   //Test if endgame
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      if(CurrentTile->Visible)
         Found = true;
      CurrentTile = CurrentTile->Next;
   }
   if (!Found && !pAutoPlay) {
      if (this->GamedDone <= 0) {
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         
         MsgNo = Random(CONGRATSIZE-1);
         Application->MessageBox(gCongrat[MsgNo].c_str(), "Winner'qus Fortune", MB_OK | MB_ICONEXCLAMATION);
      }
      this->GamedDone = 32; // :-) if you back enough moves, you get a winning message
   } else {
      //Test for no free tiles
      Found = false;
      CurrentTile = this->TileList;
      while(CurrentTile != NULL && !Found) {
         if(CurrentTile->Visible && this->IsTileFree(CurrentTile)) {
            LookupTile = CurrentTile->Next;
            while(LookupTile != NULL) {
               if (LookupTile->Visible && this->IsTileFree(LookupTile) && LookupTile->Type == CurrentTile->Type) {
                  Found = true;
                  LookupTile = NULL;
               } else
                  LookupTile = LookupTile->Next;
            }
         }
         CurrentTile = CurrentTile->Next;
      }

      if (!Found) {
         if (!pAutoPlay)
            Application->MessageBox("No free tiles", "Taipei", MB_OK | MB_ICONSTOP);
         else {
            this->tAutoPlay->Enabled = false;
            this->mAutoPlay->Checked = false;
            PlaySound("SystemAsterisk", 0, SND_ALIAS || SND_ASYNC);
         }
      }
   }
}
//---------------------------------------------------------------------------

Byte __fastcall TfTaipei::CustomIntToByte(int pByte)
{
   if(pByte > 255) return 255;
   else if(pByte < 0) return 0;
   else return pByte;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::Invert(Graphics::TBitmap *pBitmap)
{
   Byte *pyx;
   int r, g, b;

   pBitmap->HandleType=bmDIB;    // allows use of ScanLine
   pBitmap->PixelFormat=pf32bit; // 32bit the same as int so we can use int* for pixels pointer

   for(int y=0; y<pBitmap->Height-1; y++) {
      pyx=(Byte *)pBitmap->ScanLine[y];
      for(int x=0; x<pBitmap->Width+9; x++) {
         pyx[x*3]     = this->CustomIntToByte(255 -pyx[x*3]);
         pyx[x*3 + 1] = this->CustomIntToByte(255 -pyx[x*3 + 1]);
         pyx[x*3 + 2] = this->CustomIntToByte(255 -pyx[x*3 + 2]);
      }
   }
}
//---------------------------------------------------------------------------

void TfTaipei::DrawTile(int pId, bool pSel, int pX, int pY, int pZ, bool pNotch, int pDebug)
{
   int RealX = (pX * HALFTILESIZE) + 8; // 8 = Global Top Offset
   int RealY = (pY * HALFTILESIZE) + 6; // 6 = Global Left Offset
   TPoint LeftSide[4];
   TPoint LowerSide[4];
   Graphics::TBitmap * TileGraph = new Graphics::TBitmap;

   RealX = RealX + (pZ * TILEXOFFSET);
   RealY = RealY - (pZ * TILEYOFFSET);

   this->Canvas->Pen->Color = clBlack;

//clLtGray
//clDkGray

   switch(pDebug) { 
      case 0 : this->Canvas->Brush->Color = clGray; //Normal Mode
         break;     
      case 1 : this->Canvas->Brush->Color = clBlue; //Debug color mode
         break;     
      case 2 : this->Canvas->Brush->Color = clBlack;
         break;
   }

   LeftSide[0] = Point(RealX, RealY); //Upper Left Corner (Top)
   LeftSide[1] = Point(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
   LeftSide[2] = Point(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
   LeftSide[3] = Point(RealX - TILEXOFFSET, RealY + TILEYOFFSET); //Upper Left Corner (Bottom)
   this->Canvas->Polygon(LeftSide, 3);

   if (pNotch) {
   LowerSide[0] = Point(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(RealX + HALFTILESIZE - 1, RealY + TILESIZE - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(RealX - TILEXOFFSET + HALFTILESIZE - 1, RealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   } else {
      LowerSide[0] = Point(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(RealX + TILESIZE - 1, RealY + TILESIZE - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(RealX - TILEXOFFSET + TILESIZE - 1, RealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   }

   if (pDebug > 0) { //Debug color mode
      this->Canvas->Rectangle(RealX, RealY, RealX + TILESIZE, RealY + TILESIZE); //Top Face
   } else {
      this->Canvas->Brush->Color = clWhite;
      this->Canvas->Rectangle(RealX, RealY, RealX + TILESIZE, RealY + TILESIZE); //Top Face

      if (this->mColor->Checked)
         this->mlTiles->GetBitmap(pId, TileGraph);
      else
         this->mlTilesBW->GetBitmap(pId, TileGraph);

      if(pSel)
         this->Invert(TileGraph);

      this->Canvas->Draw(RealX, RealY, TileGraph);

      delete TileGraph;
   }
}
//---------------------------------------------------------------------------

void TfTaipei::DrawAllTiles(void)
{
   //Draw from the Bottom to the Top Layer (z 0 to 7)
   //Draw from the Upper to the Lower Row (y 0 to 17)
   //Draw from the Right to the Left Column (x 31 to 0)

   TTile* CurrentTile;
   TTile* NotchTile;
   bool IndNotch;
   CurrentTile = this->TileList;

   while(CurrentTile != NULL) {
      if (CurrentTile->Visible || CurrentTile->Debug) {
         NotchTile = this->GetTile(CurrentTile->X + 1, CurrentTile->Y + 2, CurrentTile->Z, true, true);
         if (NotchTile != NULL)
            IndNotch = NotchTile->Visible;
         else
            IndNotch = false;

         this->DrawTile(CurrentTile->Graph, CurrentTile->Selected, CurrentTile->X, CurrentTile->Y, CurrentTile->Z,
                        IndNotch, CurrentTile->Debug);
      } else {
         //Debug WireFrame Mode
         if (this->DebugDraw) {
            int RealX = (CurrentTile->X * HALFTILESIZE) + 8 + (CurrentTile->Z * TILEXOFFSET); // 8 = Global Top Offset
            int RealY = (CurrentTile->Y * HALFTILESIZE) + 6 - (CurrentTile->Z * TILEYOFFSET); // 6 = Global Left Offset
            //Left Side
            this->Canvas->MoveTo(RealX, RealY); //Upper Left Corner (Top)
            this->Canvas->LineTo(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
            this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
            this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET); //Upper Left Corner (Bottom)
            this->Canvas->LineTo(RealX, RealY); //Upper Left Corner (Top)
            //LowerSide
            this->Canvas->MoveTo(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
            this->Canvas->LineTo(RealX + TILESIZE - 1, RealY + TILESIZE - 1); //Lower Right Corner (Top)
            this->Canvas->LineTo(RealX - TILEXOFFSET + TILESIZE - 1, RealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
            this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
            this->Canvas->LineTo(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
            //Top
            this->Canvas->MoveTo(RealX, RealY);
            this->Canvas->LineTo(RealX + TILESIZE, RealY);
            this->Canvas->LineTo(RealX + TILESIZE, RealY + TILESIZE);
            this->Canvas->LineTo(RealX, RealY + TILESIZE);
            this->Canvas->LineTo(RealX, RealY);
         }
      }

      CurrentTile = CurrentTile->Next;
   }
}
//---------------------------------------------------------------------------

TTile* TfTaipei::GetTile(int pX, int pY, int pZ, bool pExactPos, bool pExactZ)
{
   bool Found = false;
   static TTile* CurrentTile;
   CurrentTile = this->TileList;

   //Exact position match
   if (pExactPos) {
      //Exact Z position
      if (pExactZ) {
         while(CurrentTile != NULL) {
            if (CurrentTile->X == pX && CurrentTile->Y == pY && CurrentTile->Z == pZ)
               return CurrentTile;
            CurrentTile = CurrentTile->Next;
         }
         return NULL;
      } else { //First match in Z
         do{
            CurrentTile = this->GetTile(pX, pY, pZ, true, true);
            if (CurrentTile != NULL)
               return CurrentTile;
            pZ -= 1;
         } while(!Found && pZ >= 0);
         return NULL;
      }
   } else {//Any tile at that location
      do {
         CurrentTile = this->GetTile(pX, pY, pZ, true, true);
         if (CurrentTile == NULL) {
            CurrentTile = this->GetTile(pX - 1, pY, pZ, true, true);
            if (CurrentTile == NULL) {
               CurrentTile = this->GetTile(pX, pY - 1, pZ, true, true);
               if (CurrentTile == NULL) {
                  CurrentTile = this->GetTile(pX - 1, pY - 1, pZ, true, true);
                  if (CurrentTile != NULL)
                     Found = true;
            } else
               Found = true;
         } else
              Found = true;
       } else
         Found = true;

         if (pExactZ)
            return CurrentTile;
         else
            pZ -= 1;
     } while(!Found && pZ >= 0);

     return CurrentTile;
   }

}
//---------------------------------------------------------------------------

TTile* TfTaipei::GetTile(TPoint pRealPos)
{
   static TTile* CurrentTile;
   int x, y;
   int z = MAXNUMBERLAYER;
   bool Found = false;

   do {
      x = ((pRealPos.x - 8) - (z * TILEXOFFSET)) / HALFTILESIZE;
      y = ((pRealPos.y - 6) + (z * TILEYOFFSET)) / HALFTILESIZE;

      CurrentTile = this->GetTile(x, y, z, false, true);
      if (CurrentTile != NULL)
         Found = CurrentTile->Visible;
      z -= 1;
   } while(!Found && z >= 0);

   return CurrentTile;
}
//---------------------------------------------------------------------------

bool TfTaipei::IsTileFree(TTile* pTile)
{
   TTile* TestTile;
   bool LeftSide = false, RightSide = false, TopSide = false;

   // Upper Left Side
   TestTile =    this->GetTile(pTile->X - 1, pTile->Y    , pTile->Z    , false, true);
   if (TestTile != NULL)
      LeftSide = TestTile->Visible;

   // Lower Left Side
   if (!LeftSide) {
      TestTile = this->GetTile(pTile->X - 1, pTile->Y + 1, pTile->Z    , false, true);
      if (TestTile != NULL)
         LeftSide = TestTile->Visible;
   }

   // Upper Right Side
   TestTile =    this->GetTile(pTile->X + 2, pTile->Y    , pTile->Z    , false, true);
   if (TestTile != NULL)
      RightSide = TestTile->Visible;

   // Lower Right Side
   if (!RightSide) {
      TestTile = this->GetTile(pTile->X + 2, pTile->Y + 1, pTile->Z    , false, true);
      if (TestTile != NULL)
         RightSide = TestTile->Visible;
   }

   if (LeftSide && RightSide)
      return false;

   // Top Upper Left
   TestTile =    this->GetTile(pTile->X    , pTile->Y    , pTile->Z + 1, false, true);
   if (TestTile != NULL)
      TopSide = TestTile->Visible;

   // Top Upper Right
   if (!TopSide) {
      TestTile = this->GetTile(pTile->X + 1, pTile->Y    , pTile->Z + 1, false, true);
      if (TestTile != NULL)
         TopSide = TestTile->Visible;
   }

   // Top Lower Left
   if (!TopSide) {
      TestTile = this->GetTile(pTile->X    , pTile->Y + 1, pTile->Z + 1, false, true);
      if (TestTile != NULL)
         TopSide = TestTile->Visible;
   }

   // Top Lower  Right
   if (!TopSide) {
      TestTile = this->GetTile(pTile->X + 1, pTile->Y + 1, pTile->Z + 1, false, true);
      if (TestTile != NULL)
         TopSide = TestTile->Visible;
   }

   return !TopSide;
}
//---------------------------------------------------------------------------

void TfTaipei::BuildStructure(int pMode)
{
   TTile* NextTile;
   int IdCmp = 144;
   int i, j = 0;
   int x, y;
   bool Buffer[GAMEWIDTH][GAMEHEIGHT];
   int Delta;
   const int *CurrLayer, *CurrSize;
   const int *Layer0, *Layer1, *Layer2, *Layer3, *Layer4, *Layer5, *Layer6;

   //Empty previous tile list
   while(this->TileList != NULL) {
     NextTile = this->TileList->Next;
     delete this->TileList;
     this->TileList = NextTile;
   }

   //Reset tile type array
   for (i=0; i<36; i++) {
      this->TileType[j] = i;
      j++;
      this->TileType[j] = i;
      j++;
   }
   //Reset special graph arrays
   for (i=0; i<4; i++) {
      this->SpecialGraph1[i] = i;
      this->SpecialGraph2[i] = i;
   }

   switch(pMode) {
      case 0 : CurrSize = DEBUGSIZE;
               Layer0   = DEBUGL0;
               Layer1   = DEBUGL1;
               Layer2   = NULL;
               Layer3   = NULL;
               Layer4   = NULL;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 1 : CurrSize = STANDARDSIZE;
               Layer0   = STANDARDL0;
               Layer1   = STANDARDL1;
               Layer2   = STANDARDL2;
               Layer3   = STANDARDL3;
               Layer4   = STANDARDL4;
               Layer5   = STANDARDL5;
               Layer6   = STANDARDL6;
         break;
      case 2 : CurrSize = BRIDGESIZE;
               Layer0   = BRIDGEL0;
               Layer1   = BRIDGEL1;
               Layer2   = BRIDGEL2;
               Layer3   = BRIDGEL3;
               Layer4   = BRIDGEL4;
               Layer5   = BRIDGEL5;
               Layer6   = BRIDGEL6;
         break;
      case 3 : CurrSize = CASTLESIZE;
               Layer0   = CASTLEL0;
               Layer1   = CASTLEL1;
               Layer2   = CASTLEL2;
               Layer3   = CASTLEL3;
               Layer4   = CASTLEL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 4 : CurrSize = CUBESIZE;
               Layer0   = CUBEL0;
               Layer1   = CUBEL1;
               Layer2   = CUBEL2;
               Layer3   = CUBEL3;
               Layer4   = CUBEL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 5 : CurrSize = GLYPHSIZE;
               Layer0   = GLYPHL0;
               Layer1   = GLYPHL1;
               Layer2   = GLYPHL2;
               Layer3   = GLYPHL3;
               Layer4   = GLYPHL4;
               Layer5   = GLYPHL5;
               Layer6   = NULL;
         break;
      case 6 : CurrSize = PYRAMIDSIZE;
               Layer0   = PYRAMIDL0;
               Layer1   = PYRAMIDL1;
               Layer2   = PYRAMIDL2;
               Layer3   = PYRAMIDL3;
               Layer4   = PYRAMIDL4;
               Layer5   = PYRAMIDL5;
               Layer6   = NULL;
         break;
      case 7 : CurrSize = SPIRALSIZE;
               Layer0   = SPIRALL0;
               Layer1   = SPIRALL1;
               Layer2   = SPIRALL2;
               Layer3   = SPIRALL3;
               Layer4   = SPIRALL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 8 : CurrSize = DRAGONSIZE;
               Layer0   = DRAGONL0;
               Layer1   = DRAGONL1;
               Layer2   = DRAGONL2;
               Layer3   = DRAGONL3;
               Layer4   = DRAGONL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
   }

   for (i = MAXNUMBERLAYER-1; i >= 0; i--) {
      if (CurrSize[i] > 0) {
         switch(i) {
            case 0 : CurrLayer = Layer0;
               break;
            case 1 : CurrLayer = Layer1;
               break;
            case 2 : CurrLayer = Layer2;
               break;
            case 3 : CurrLayer = Layer3;
               break;
            case 4 : CurrLayer = Layer4;
               break;
            case 5 : CurrLayer = Layer5;
               break;
            case 6 : CurrLayer = Layer6;
               break;
            }

         //Init layer Buffer
         //Buffer[31][17];
         for (x=0; x<GAMEWIDTH; x++) {
            for (y=0; y<GAMEHEIGHT; y++) {
               Buffer[x][y] = false;
            }
         }

  //NOTE, INSERT IS BACKWARD
  //Draw from the Bottom to the Top Layer (z 0 to 7)
  //Draw from the Upper to the Lower Row (y 0 to 17)
  //Draw from the Right to the Left Column (x 31 to 0)

// Array Definition Before Rotate               \/ Finish Here
/*int DebugL0[40] = {4,6,  6,6,  8,6,  10,6,  12,6,            /\
                     4,8,  6,8,  8,8,  10,8,  12,8,            |
                     4,10, 6,10, 8,10, 10,10, 12,10,           |
                     4,12, 6,12, 8,12, 10,12, 12,12 };        Y--  then  X++ ---->
       Read From Here /\

// Array After Rotate   X++ ---->
//Read From Here ----> {4,12,  4,10,  4,8,  4,6,   Y--
                        6,12,  6,10,  6,8,  6,6,    |
                        8,12,  8,10,  8,8,  8,6,    |
                        10,12, 10,10, 10,8, 10,6,   \/
                        12,12, 12,10, 12,8, 12,6} <---- Finish Here */

         //Set layer Buffer
         for(j=0; j<CurrSize[i]*2; j=j+2){
            x = CurrLayer[j];
            y = CurrLayer[j+1];
            Buffer[x][y] = true;
         }

         //Read layer Buffer 90 degree rotated
         //Buffer[31][17];
         for (x=0; x<GAMEWIDTH; x++) {
            for (y=GAMEHEIGHT-1; y>=0; y--) {
               //Insert into tile list
               if (Buffer[x][y]) {
                  NextTile = new TTile(IdCmp, x, y, i);
                  NextTile->Next = this->TileList;
                  this->TileList = NextTile;
                  IdCmp--;
               }
            }
         }
      }
   }
   this->Repaint();
}
//---------------------------------------------------------------------------

void TfTaipei::FillStructure(int pSeed)
{
   TTile* CurrentTile;
   TTile* CandidateTileA;
   TTile* CandidateTileB;
   int Swap, Target;
   int SpecGraph1 = 0, SpecGraph2 = 0;
   int LoopCmp, StepCmp = 0;
   bool StartOver = false;
   TPoint MinXY;
   TPoint MaxXY;
   MinXY.x = GAMEWIDTH;
   MinXY.y = GAMEHEIGHT;
   MaxXY.x = 0;
   MaxXY.y = 0;

   RandSeed = pSeed;

   //Shuffle tile types array
   for (int i=0; i<10; i++) {
      for (int j=71; j>0; j--) {
         Target = Random(j);
         Swap = this->TileType[j];
         this->TileType[j] = this->TileType[Target];
         this->TileType[Target] = Swap;
      }
   }
   //Shuffle special types arrays
   for (int k=3; k>0; k--) {
      Target = Random(k);
      Swap = this->SpecialGraph1[k];
      this->SpecialGraph1[k] = this->SpecialGraph1[Target];
      this->SpecialGraph1[Target] = Swap;

      Target = Random(k);
      Swap = this->SpecialGraph2[k];
      this->SpecialGraph2[k] = this->SpecialGraph2[Target];
      this->SpecialGraph2[Target] = Swap;
   }

   //Find min/max to limit search
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      StepCmp++;
      if (CurrentTile->X > MaxXY.x)
         MaxXY.x = CurrentTile->X + 1;
      if (CurrentTile->X < MinXY.x)
         MinXY.x = CurrentTile->X;
      if (CurrentTile->Y > MaxXY.y)
         MaxXY.y = CurrentTile->Y + 1;
      if (CurrentTile->Y < MinXY.y)
         MinXY.y = CurrentTile->Y;
     CurrentTile = CurrentTile->Next;
   }
   //Number of tile pairs/loop steps
   if (StepCmp % 2 == 1)
      StepCmp--;
   StepCmp = StepCmp / 2;
   LoopCmp = StepCmp;

   //Init
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      CurrentTile->Type  = -1;
      CurrentTile->Graph = 42;
      CurrentTile->Hint  = -1;
      CurrentTile->Visible = true;

      CurrentTile = CurrentTile->Next;
   }

   //May take multiple tries
   do {
      CandidateTileA = NULL;
      CandidateTileB = NULL;

      //Find a first tile
      CandidateTileA = this->FindCandidate(MinXY, MaxXY);

      if (CandidateTileA != NULL) {
         CandidateTileA->Type = 99;
         if (this->DebugDraw) {
            this->Repaint();
            Sleep(10);
         }
         //Find a second tile
         CandidateTileB = this->FindCandidate(MinXY, MaxXY);
         if (CandidateTileB != NULL) {
            CandidateTileB->Type = 99;
            if (this->DebugDraw) {
               this->Repaint();
               Sleep(10);
            }
         } else
            StartOver = true;
      } else
         StartOver = true;

      //If ok
      if ( !StartOver ) {
         //Assign Type and Graphics to both
         CandidateTileA->Visible = false;
         CandidateTileB->Visible = false;
         this->AssignTypeGraph(CandidateTileA, CandidateTileB, StepCmp, SpecGraph1, SpecGraph2);
         CandidateTileA->Hint = LoopCmp-StepCmp;
         CandidateTileB->Hint = LoopCmp-StepCmp;
         StepCmp--;
      } else { //If stuck, start over
         StepCmp = LoopCmp;
         StartOver = false;
         SpecGraph1 = 0;
         SpecGraph2 = 0;

         CurrentTile = this->TileList;
         while(CurrentTile != NULL) {
            CurrentTile->Type  = -1;
            CurrentTile->Graph = 42;
            CurrentTile->Hint  = -1;
            CurrentTile->Visible = true;

            CurrentTile = CurrentTile->Next;
         }
      }
      
      Application->ProcessMessages();
   } while(StepCmp > 0);

   //Make all tiles visible, with or without animation
   if (this->mWatchBuilds->Checked) {

      while(LoopCmp >= 0) {
         CurrentTile = this->TileList;
         while(CurrentTile != NULL) {
            if (CurrentTile->Hint >= LoopCmp)
               CurrentTile->Visible = true;
            CurrentTile = CurrentTile->Next;
         }

         this->Repaint();
         Sleep(10);
         LoopCmp--;
      }
   } else {
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         CurrentTile->Visible = true;
         CurrentTile = CurrentTile->Next;
      }
   }
}
//---------------------------------------------------------------------------

TTile* TfTaipei::FindCandidate(TPoint pMinXY, TPoint pMaxXY)
{
   static TTile* CandidateTile;
   int x, y, z;
   bool Found;
   int GiveUpCounter = 200;
   bool StartOver;

   //May take multiple tries
   do{
      StartOver = false;
      x = pMinXY.x + Random(pMaxXY.x - pMinXY.x + 1);
      y = pMinXY.y + Random(pMaxXY.y - pMinXY.y + 1);
      z = MAXNUMBERLAYER;
      Found = false;

      //Find an available tile at a position
      do {
         CandidateTile = this->GetTile(x, y, z, false, true);
         if (CandidateTile != NULL)
            Found = CandidateTile->Visible;
         z -= 1;
      } while(!Found && z >= 0);

      if(CandidateTile == NULL)
         StartOver = true;
      else {
         if (this->DebugDraw) {
            CandidateTile->Debug = 1;
            this->Repaint();
            CandidateTile->Debug = 0;
         }
         //If tile not available
         if (CandidateTile->Type != -1 || !this->IsTileFree(CandidateTile))
            StartOver = true;
      }
      GiveUpCounter--;
   } while(StartOver && GiveUpCounter > 0);

   if (GiveUpCounter > 0)
      return CandidateTile;
   else
      return NULL;
}
//---------------------------------------------------------------------------

void TfTaipei::AssignTypeGraph(TTile* pCandidateTileA, TTile* pCandidateTileB, int pDelta, int& pSpecGraph1, int& pSpecGraph2)
{
   pCandidateTileA->Type  = this->TileType[pDelta];
   pCandidateTileA->Graph = this->TileType[pDelta];

   //If tile A is Season or Plant, take care of the special graph
   if (this->TileType[pDelta] == 34) {
      pCandidateTileA->Graph = pCandidateTileA->Graph + this->SpecialGraph1[pSpecGraph1];
      pSpecGraph1++;
   }
   if (this->TileType[pDelta] == 35) {
      pCandidateTileA->Graph = pCandidateTileA->Graph + 3 + this->SpecialGraph2[pSpecGraph2];
      pSpecGraph2++;
   }
   int SpecialTile = Random(2048);
   if (this->TileType[pDelta] == 33 && SpecialTile == 0) {
      pCandidateTileA->Graph = pCandidateTileA->Graph + 10;
      pSpecGraph1++;
   }

   pCandidateTileB->Type  = this->TileType[pDelta];
   pCandidateTileB->Graph = this->TileType[pDelta];

   //If tile B is Season or Plant, take care of the special graph
   if (this->TileType[pDelta] == 34) {
      pCandidateTileB->Graph = pCandidateTileB->Graph + this->SpecialGraph1[pSpecGraph1];
      pSpecGraph1++;
   }
   if (this->TileType[pDelta] == 35) {
      pCandidateTileB->Graph = pCandidateTileB->Graph + 3 + this->SpecialGraph2[pSpecGraph2];
      pSpecGraph2++;
   }
   if (this->TileType[pDelta] == 33 && SpecialTile == 0) {
      pCandidateTileB->Graph = pCandidateTileB->Graph + 10;
      pSpecGraph2++;
   }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


