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

#define TileSize       36
#define HalfTileSize   18
#define TileXOffset    5
#define TileYOffset    3
#define MaxNumberLayer 7
#define GameHeight     17
#define GameWidth      31

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
         //this->DebugCmp = 0;
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

void __fastcall TfTaipei::mAboutClick(TObject *Sender)
{
   AnsiString DialogCap = "About " + this->Caption;
   Application->MessageBox("Taipei\n\nClone by David Morrissette\n\n2020",
                           DialogCap.c_str() ,MB_OK);
}
//--------------------------------------------------------------------------

void __fastcall TfTaipei::mStartOverClick(TObject *Sender)
{
   TTile* CurrentTile;

   if (this->StepBack > 0) {
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
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::FormShow(TObject *Sender)
{
   this->TileList     = NULL;
   this->SelectedTile = NULL;

   this->lMainTitle->Caption = "Oriental Game of Skill and Chance\nVersion 1.00\nClone by David Morrissette\n2020";
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

   TMenuItem *Current = (TMenuItem *)Sender;
   Current->Checked = true;

   this->Mode = Current->Tag;
   this->mNewClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::mNewClick(TObject *Sender)
{
   int GameNo;

   this->iMainLogo->Visible = false;
   this->lMainTitleShadow->Visible = false;
   this->lMainTitle->Visible = false;
   this->StepBack = 0;
   this->GamedDone = 0;
   this->HintLoop = 0;
   this->SelectedTile = NULL;

   Randomize();
   GameNo = Random(32767);
   fTaipei->Caption = "Taipei Game  #" + IntToStr(GameNo);

   this->BuildStructure(this->Mode);
   this->FillStructure(GameNo, this->Mode);
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
      
      this->iMainLogo->Visible = false;
      this->lMainTitleShadow->Visible = false;
      this->lMainTitle->Visible = false;
      this->StepBack = 0;
      this->HintLoop = 0;
      this->GamedDone = 0;
      this->SelectedTile = NULL;

      this->Caption = "Taipei Game  #" + IntToStr(GameNo);

      this->BuildStructure(this->Mode);
      this->FillStructure(GameNo, this->Mode);
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
         if (CurrentTile->Visible && IsTileFree(CurrentTile)) {
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
            if (CurrentTile->Visible && CurrentTile->Type == this->SelectedTile->Type && IsTileFree(CurrentTile)) {
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
   //int NextMinHint = 72;

   if (this->SelectedTile != NULL) {
      this->SelectedTile->Selected = false;
      this->SelectedTile = NULL;
   }

   //Find the lowest Hint value from all free and visible tiles and select it
   while(CurrentTile != NULL) {
      if (CurrentTile->Visible && CurrentTile->Hint < MinHint && IsTileFree(CurrentTile)) {
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
         if (CurrentTile->Visible && CurrentTile->Hint == MinHint && IsTileFree(CurrentTile)) {
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
          CurrentTile->Id != this->SelectedTile->Id && IsTileFree(CurrentTile)) {
        CurrentTile->Selected = true;
        SelectTile = CurrentTile;
      }
     CurrentTile = CurrentTile->Next;
   }

   //Select the next lowest tile available of the same type
   if (SelectTile == NULL) {
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && CurrentTile->Hint != MinHint && CurrentTile->Type == this->SelectedTile->Type &&
             /*CurrentTile->Hint < NextMinHint &&*/ IsTileFree(CurrentTile)) {
            //NextMinHint = CurrentTile->Hint;
            SelectTile = CurrentTile;
            SelectTile->Selected = true;
         }
         CurrentTile = CurrentTile->Next;
      }
   }

   if (SelectTile != NULL) {
      this->Repaint();
      Sleep(100);
      HideTileStep(SelectTile, true);
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
      lDebug->Caption = IntToStr(CurrentTile->Id) + ":" + IntToStr(CurrentTile->Hint);
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
   //this->DebugCmp++;
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
      if (CurrentTile->Visible) {
         if (!this->IsTileFree(CurrentTile)) {
            if (this->mMessages->Checked)
               Application->MessageBox("Tiles isn't free", "Taipei" , MB_OK | MB_ICONINFORMATION);
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
                     HideTileStep(CurrentTile, false);
                  } else {
                     CurrentTile->Selected = true;
                     this->Repaint();
                     Sleep(75);
                     CurrentTile->Selected = false;
                     this->Repaint();
                     if (this->mMessages->Checked)
                        Application->MessageBox("Tiles don't match", "Taipei" , MB_OK | MB_ICONINFORMATION);
                  }
               }
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

void TfTaipei::HideTileStep(TTile* pTile, bool AutoPlay)
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
   if (!Found && !AutoPlay) {
      if (this->GamedDone <= 0) {
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         MsgNo = Random(MsgCount);
         Application->MessageBox(Congrat[MsgNo].c_str(), "Winner'qus Fortune" , MB_OK | MB_ICONEXCLAMATION);
      }
      this->GamedDone = 32; // :-) if you back enough moves, you get a winning message
   } else {
      //Test for no free tiles
      Found = false;
      CurrentTile = this->TileList;
      while(CurrentTile != NULL && !Found) {
         if(CurrentTile->Visible && IsTileFree(CurrentTile)) {
            LookupTile = CurrentTile->Next;
            while(LookupTile != NULL) {
               if (LookupTile->Visible && IsTileFree(LookupTile) && LookupTile->Type == CurrentTile->Type) {
                  Found = true;
                  LookupTile = NULL;
               } else
                  LookupTile = LookupTile->Next;
            }
         }
         CurrentTile = CurrentTile->Next;
      }

      if (!Found) {
         if (!AutoPlay)
            Application->MessageBox("No free tiles", "Taipei" , MB_OK | MB_ICONSTOP);
         else {
            this->tAutoPlay->Enabled = false;
            this->mAutoPlay->Checked = false;
            PlaySound("SystemAsterisk", 0, SND_ALIAS || SND_ASYNC);
         }
      }
   }
}
//---------------------------------------------------------------------------

Byte __fastcall TfTaipei::CustomIntToByte(int i)
{
   if(i > 255) return 255;
   else if(i < 0) return 0;
   else return i;
}
//---------------------------------------------------------------------------

void __fastcall TfTaipei::Invert(Graphics::TBitmap *Bitmap)
{
   Byte *pyx;
   int r, g, b;

   Bitmap->HandleType=bmDIB;    // allows use of ScanLine
   Bitmap->PixelFormat=pf32bit; // 32bit the same as int so we can use int* for pixels pointer

   for(int y=0; y<Bitmap->Height-1; y++) {
      pyx=(Byte *)Bitmap->ScanLine[y];
      for(int x=0; x<Bitmap->Width+9; x++) {
         pyx[x*3]     = CustomIntToByte(255 -pyx[x*3]);
         pyx[x*3 + 1] = CustomIntToByte(255 -pyx[x*3 + 1]);
         pyx[x*3 + 2] = CustomIntToByte(255 -pyx[x*3 + 2]);
      }
   }
}
//---------------------------------------------------------------------------

void TfTaipei::DrawTile(int Id, bool Sel, int x, int y, int z, bool Notch, int Debug)
{
   int RealX = (x * HalfTileSize) + 8; // 8 = Global Top Offset
   int RealY = (y * HalfTileSize) + 6; // 6 = Global Left Offset
   TPoint LeftSide[4];
   TPoint LowerSide[4];
   Graphics::TBitmap * TileGraph = new Graphics::TBitmap;

   RealX = RealX + (z * TileXOffset);
   RealY = RealY - (z * TileYOffset);

   this->Canvas->Pen->Color = clBlack;
   if (Debug == 1) //Debug color mode
      this->Canvas->Brush->Color = clBlack;
   if (Debug == 2) //Debug color mode
      this->Canvas->Brush->Color = clRed;
   if (Debug == 3) //Debug color mode
      this->Canvas->Brush->Color = clYellow;
   if (Debug == 4) //Debug color mode
      this->Canvas->Brush->Color = clBlue;
      
   if (Debug == 0)
      this->Canvas->Brush->Color = clGray;

   LeftSide[0] = Point(RealX, RealY); //Upper Left Corner (Top)
   LeftSide[1] = Point(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
   LeftSide[2] = Point(RealX - TileXOffset, RealY + TileYOffset + TileSize); //Lower Left Corner (Bottom)
   LeftSide[3] = Point(RealX - TileXOffset, RealY + TileYOffset); //Upper Left Corner (Bottom)
   this->Canvas->Polygon(LeftSide, 3);

   if (Notch) {
      LowerSide[0] = Point(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(RealX + HalfTileSize - 1, RealY + TileSize - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(RealX - TileXOffset + HalfTileSize - 1, RealY + TileYOffset + TileSize); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(RealX - TileXOffset, RealY + TileYOffset + TileSize); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   } else {
      LowerSide[0] = Point(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(RealX + TileSize - 1, RealY + TileSize - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(RealX - TileXOffset + TileSize - 1, RealY + TileYOffset + TileSize); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(RealX - TileXOffset, RealY + TileYOffset + TileSize); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   }

   if (Debug > 0) { //Debug color mode
      this->Canvas->Rectangle(RealX, RealY, RealX + TileSize, RealY + TileSize); //Top Face
   } else {
      this->Canvas->Brush->Color = clWhite;
      this->Canvas->Rectangle(RealX, RealY, RealX + TileSize, RealY + TileSize); //Top Face

         this->mlTiles->GetBitmap(Id, TileGraph);
         if(Sel)
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
         //NotchTile = this->GetTile(CurrentTile->X - 2, CurrentTile->Y - 1, CurrentTile->Z, true, true);
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
            int RealX = (CurrentTile->X * HalfTileSize) + 8 + (CurrentTile->Z * TileXOffset); // 8 = Global Top Offset
            int RealY = (CurrentTile->Y * HalfTileSize) + 6 - (CurrentTile->Z * TileYOffset); // 6 = Global Left Offset
            //Left Side
            this->Canvas->MoveTo(RealX, RealY); //Upper Left Corner (Top)
            this->Canvas->LineTo(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
            this->Canvas->LineTo(RealX - TileXOffset, RealY + TileYOffset + TileSize); //Lower Left Corner (Bottom)
            this->Canvas->LineTo(RealX - TileXOffset, RealY + TileYOffset); //Upper Left Corner (Bottom)
            this->Canvas->LineTo(RealX, RealY); //Upper Left Corner (Top)
            //LowerSide
            this->Canvas->MoveTo(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
            this->Canvas->LineTo(RealX + TileSize - 1, RealY + TileSize - 1); //Lower Right Corner (Top)
            this->Canvas->LineTo(RealX - TileXOffset + TileSize - 1, RealY + TileYOffset + TileSize); //Lower Right Corner (Bottom)
            this->Canvas->LineTo(RealX - TileXOffset, RealY + TileYOffset + TileSize); //Lower Left Corner (Bottom)
            this->Canvas->LineTo(RealX , RealY + TileSize - 1); //Lower Left Corner (Top)
            //Top
            this->Canvas->MoveTo(RealX, RealY);
            this->Canvas->LineTo(RealX + TileSize, RealY);
            this->Canvas->LineTo(RealX + TileSize, RealY + TileSize);
            this->Canvas->LineTo(RealX, RealY + TileSize);
            this->Canvas->LineTo(RealX, RealY);
         }
      }

      CurrentTile = CurrentTile->Next;
   }
}
//---------------------------------------------------------------------------

TTile* TfTaipei::GetTile(int x, int y, int z, bool ExactPos, bool ExactZ)
{
   bool Found = false;
   static TTile* CurrentTile;
   CurrentTile = this->TileList;

   //Exact position match
   if (ExactPos) {
      //Exact Z position
      if (ExactZ) {
         while(CurrentTile != NULL) {
            if (CurrentTile->X == x && CurrentTile->Y == y && CurrentTile->Z == z)
               return CurrentTile;
            CurrentTile = CurrentTile->Next;
         }
         return NULL;
      } else { //First match in Z
         do{
            CurrentTile = this->GetTile(x, y, z, true, true);
            if (CurrentTile != NULL)
               return CurrentTile;
            z -= 1;
         } while(!Found && z >= 0);
         return NULL;
      }
   } else {//Any tile at that location
      do {
         CurrentTile = this->GetTile(x, y, z, true, true);
         if (CurrentTile == NULL) {
            CurrentTile = this->GetTile(x - 1, y, z, true, true);
            if (CurrentTile == NULL) {
               CurrentTile = this->GetTile(x, y - 1, z, true, true);
               if (CurrentTile == NULL) {
                  CurrentTile = this->GetTile(x - 1, y - 1, z, true, true);
                  if (CurrentTile != NULL)
                     Found = true;
            } else
               Found = true;
         } else
              Found = true;
       } else
         Found = true;

         if (ExactZ)
            return CurrentTile;
         else
            z -= 1;
     } while(!Found && z >= 0);

     return CurrentTile;
   }

}
//---------------------------------------------------------------------------

TTile* TfTaipei::GetTile(TPoint RealPos)
{
   static TTile* CurrentTile;
   int x, y;
   int z = MaxNumberLayer;
   bool Found = false;

   do {
      x = ((RealPos.x - 8) - (z * TileXOffset)) / HalfTileSize;
      y = ((RealPos.y - 6) + (z * TileYOffset)) / HalfTileSize;

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

void TfTaipei::BuildStructure(int Mode)
{
   TTile* NextTile;
   int IdCmp = 144;
   int i, j = 0;
   int x, y;
   bool Buffer[GameWidth][GameHeight];
   int Delta;
   int *CurrLayer, *CurrSize;
   int *Layer0, *Layer1, *Layer2, *Layer3, *Layer4, *Layer5, *Layer6;

   while(this->TileList != NULL) {
     NextTile = this->TileList->Next;
     delete this->TileList;
     this->TileList = NextTile;
   }

   //reset tile type array
   for (i=0; i<36; i++) {
      this->TileType[j] = i;
      j++;
      this->TileType[j] = i;
      j++;
   }
   for (i=0; i<4; i++) {
      this->SpecialGraph1[i] = i;
      this->SpecialGraph2[i] = i;
   }

   switch(Mode) {
      case 0 : CurrSize = DebugSize;
               Layer0   = DebugL0;
               Layer1   = DebugL1;
               Layer2   = NULL;
               Layer3   = NULL;
               Layer4   = NULL;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 1 : CurrSize = StandardSize;
               Layer0   = StandardL0;
               Layer1   = StandardL1;
               Layer2   = StandardL2;
               Layer3   = StandardL3;
               Layer4   = StandardL4;
               Layer5   = StandardL5;
               Layer6   = StandardL6;
         break;
      case 2 : CurrSize = BridgeSize;
               Layer0   = BridgeL0;
               Layer1   = BridgeL1;
               Layer2   = BridgeL2;
               Layer3   = BridgeL3;
               Layer4   = BridgeL4;
               Layer5   = BridgeL5;
               Layer6   = BridgeL6;
         break;
      case 3 : CurrSize = CastleSize;
               Layer0   = CastleL0;
               Layer1   = CastleL1;
               Layer2   = CastleL2;
               Layer3   = CastleL3;
               Layer4   = CastleL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 4 : CurrSize = CubeSize;
               Layer0   = CubeL0;
               Layer1   = CubeL1;
               Layer2   = CubeL2;
               Layer3   = CubeL3;
               Layer4   = CubeL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
      case 5 : CurrSize = GlyphSize;
               Layer0   = GlyphL0;
               Layer1   = GlyphL1;
               Layer2   = GlyphL2;
               Layer3   = GlyphL3;
               Layer4   = GlyphL4;
               Layer5   = GlyphL5;
               Layer6   = NULL;
         break;
      case 6 : CurrSize = PyramidSize;
               Layer0   = PyramidL0;
               Layer1   = PyramidL1;
               Layer2   = PyramidL2;
               Layer3   = PyramidL3;
               Layer4   = PyramidL4;
               Layer5   = PyramidL5;
               Layer6   = NULL;
         break;
      case 7 : CurrSize = SpiralSize;
               Layer0   = SpiralL0;
               Layer1   = SpiralL1;
               Layer2   = SpiralL2;
               Layer3   = SpiralL3;
               Layer4   = SpiralL4;
               Layer5   = NULL;
               Layer6   = NULL;
         break;
   }

   for (i = MaxNumberLayer-1; i >= 0; i--) {
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

         //int Buffer[31][17];
         for (x=0; x<GameWidth; x++) {
            for (y=0; y<GameHeight; y++) {
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

         for(j=0; j<CurrSize[i]*2; j=j+2){
            x = CurrLayer[j];
            y = CurrLayer[j+1];
            Buffer[x][y] = true;
         }

         //int Buffer[31][17];
         for (x=0; x<GameWidth; x++) {
            for (y=GameHeight-1; y>=0; y--) {
               //INSERT
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

void TfTaipei::FillStructure(int Seed, int Mode)
{
   TTile* CurrentTile;
   TTile* CandidateTileA;
   TTile* CandidateTileB;
   int Swap, Target;
   int SpecGraph1 = 0, SpecGraph2 = 0;
   int StepCmp = 0;
   int LoopCmp;
   bool StartOver = false;
   TPoint MinXY;
   TPoint MaxXY;
   MinXY.x = GameWidth;
   MinXY.y = GameHeight;
   MaxXY.x = 0;
   MaxXY.y = 0;

   RandSeed = Seed;

   //Shuffle tile type array
   for (int i=0; i<10; i++) {
      for (int j=71; j>0; j--) {
         Target = Random(j);
         Swap = this->TileType[j];
         this->TileType[j] = this->TileType[Target];
         this->TileType[Target] = Swap;
      }
   }
   for (int k=3; k>0; k--) {
      Target = Random(k);
      Swap = SpecialGraph1[k];
      SpecialGraph1[k] = SpecialGraph1[Target];
      SpecialGraph1[Target] = Swap;

      Target = Random(k);
      Swap = SpecialGraph2[k];
      SpecialGraph2[k] = SpecialGraph2[Target];
      SpecialGraph2[Target] = Swap;
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

   //Make all tiles visible
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      CurrentTile->Visible = true;
      CurrentTile = CurrentTile->Next;
   }
}
//---------------------------------------------------------------------------

TTile* TfTaipei::FindCandidate(TPoint MinXY, TPoint MaxXY)
{
   static TTile* CandidateTile;
   int x, y, z;
   bool Found;
   int GiveUpCounter = 200;
   bool StartOver;

   //May take multiple tries
   do{
      StartOver = false;
      x = MinXY.x + Random(MaxXY.x - MinXY.x + 1);
      y = MinXY.y + Random(MaxXY.y - MinXY.y + 1);
      z = MaxNumberLayer;
      Found = false;

      //Find an available tile at a position
      //CandidateTile = this->GetTile(x, y, 7, false, false);
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
            CandidateTile->Debug = 2;
            this->Repaint();
            CandidateTile->Debug = 0;
         }
         //If tile not available
         if (CandidateTile->Type != -1 || !IsTileFree(CandidateTile))
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

void TfTaipei::AssignTypeGraph(TTile* CandidateTileA, TTile* CandidateTileB, int Delta, int& SpecGraph1, int& SpecGraph2)
{
   CandidateTileA->Type  = this->TileType[Delta];
   CandidateTileA->Graph = this->TileType[Delta];

   if (this->TileType[Delta] == 34) {
      CandidateTileA->Graph = CandidateTileA->Graph + this->SpecialGraph1[SpecGraph1];
      SpecGraph1++;
   }
   if (this->TileType[Delta] == 35) {
      CandidateTileA->Graph = CandidateTileA->Graph + 3 + this->SpecialGraph2[SpecGraph2];
      SpecGraph2++;
   }

   CandidateTileB->Type  = this->TileType[Delta];
   CandidateTileB->Graph = this->TileType[Delta];

   if (this->TileType[Delta] == 34) {
      CandidateTileB->Graph = CandidateTileB->Graph + this->SpecialGraph1[SpecGraph1];
      SpecGraph1++;
   }
   if (this->TileType[Delta] == 35) {
      CandidateTileB->Graph = CandidateTileB->Graph + 3 + this->SpecialGraph2[SpecGraph2];
      SpecGraph2++;
   }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


