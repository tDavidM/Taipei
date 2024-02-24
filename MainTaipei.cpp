//---------------------------------------------------------------------------

#include <vcl.h>
#include <mmsystem.h>
#include <algorithm>
#include <exception>
#include <registry.hpp>
#pragma hdrstop

#include "MainTaipei.h"
#include "SelGame.h"

#include "EditorCode.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfTaipei *fTaipei;

//Sizes at zoom factor 100
#define TILESIZE_100       36  // Horizontal pixel width of a tile
#define HALFTILESIZE_100   18  // Since a tile covers 2 by 2 units of game grid, an half value is sometime required
#define TILEXOFFSET_100    5   // Pixel horizontal offset for the high-angle isometric perspective
#define TILEYOFFSET_100    3   // Pixel vertical offset for the high-angle isometric perspective

//Sizes at zoom factor 150
#define TILESIZE_150       54  // Horizontal pixel width of a tile
#define HALFTILESIZE_150   27  // Since a tile covers 2 by 2 units of game grid, an half value is sometime required
#define TILEXOFFSET_150    7   // Pixel horizontal offset for the high-angle isometric perspective
#define TILEYOFFSET_150    4   // Pixel vertical offset for the high-angle isometric perspective

//Sizes at zoom factor 200
#define TILESIZE_200       72  // Horizontal pixel width of a tile
#define HALFTILESIZE_200   36  // Since a tile covers 2 by 2 units of game grid, an half value is sometime required
#define TILEXOFFSET_200    10   // Pixel horizontal offset for the high-angle isometric perspective
#define TILEYOFFSET_200    6   // Pixel vertical offset for the high-angle isometric perspective


//Those don't scale (check Redefined in EditorCode.cpp)
#define MAXNUMBERLAYER 7   // Theres only 7 layers
#define GAMEWIDTH      31  // Horizontal width of the game grid
#define GAMEHEIGHT     17  // Vertical height of the game grid

//---------------------------------------------------------------------------

//Constructor for a Tile
TTile::TTile(int pId, int pX, int pY, int pZ)
{
   this->Id   = pId; // Unique ID for each tiles, 0 to number of tiles
                     //   used by the AutoPlay algo and the Hint functionality as well as Save/Load game functionality
   this->Next = NULL;// Pointer to the next entry in the Linked List used to store tiles

                     // X, Y and Z coordinates in the game grid (not Canvas X/Y) for tiles draw and cursor interactions
                     // NOTE: a tile is a square with 2 units of width and height !!!
   this->X = pX;     // X starts on the left side and increases horizontaly toward the right
   this->Y = pY;     // Y starts on the upper portion and increases verticaly toward the lower portion
   this->Z = pZ;     // Z starts on the bottom layer and increases verticaly toward the top layer
                     // drawing is done win and isometric perspective with a "high-angle shot" with a lateral offset
                     //
                     //   |\¯¯¯¯\    //
                     //   \ \____\   //
                     //    \|____|   //

   this->Type  = -1; // Value used to match tiles of the same type
   this->Graph = -1; // Image list index for graph used to draw tile
   this->Step  = -1; // Indicate the order tiles were removed: starts at 0 and increases by 1 for each pair of tiles removed
   this->Hint  = -1; // Indicate the order tiles were laid, starts at number of pairs and decreases by 1 to 0
                     //   used by the AutoPlay algo and the Hint functionality
   this->Debug = 0;  // Debug indicator to toggle a tile visiable as a solid color: 1=blue 2=black

   this->Selected  = false; // Selected tiles are drawn with an inverted color graph
   this->Visible   = false; // Visible tiles are drawn with their corresponding Graph
   this->WireFrame = false; // WireFrame tiles are drawn in ..well, wireframe :-)
}

//---------------------------------------------------------------------------

__fastcall TfTaipei::TfTaipei(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

//Called to close the application
void __fastcall TfTaipei::mExitClick(TObject *Sender)
{
   this->Close();
}
//---------------------------------------------------------------------------

//Initialises form's default values and read command line call parameters
void __fastcall TfTaipei::FormCreate(TObject *Sender)
{
   this->mlTiles->SetSize(36,36);
   this->mlTiles->ResourceLoad(rtBitmap, "TILE_100_ALL", clWhite);
   this->ZoomFactor = 100;
   this->TILESIZE     = TILESIZE_100;
   this->HALFTILESIZE = HALFTILESIZE_100;
   this->TILEXOFFSET  = TILEXOFFSET_100;
   this->TILEYOFFSET  = TILEYOFFSET_100;

   bool OpenResult;
   TRegistry* Reg = new TRegistry(KEY_READ);

   this->DoubleBuffered = true;
   Application->Icon = this->Icon;

   this->lMainTitle->Caption = "Oriental Game of Skill and Chance\nVersion 6.00\nClone by David Morrissette\n2020-2023";
   this->lMainTitleShadow->Caption = this->lMainTitle->Caption;

   this->Mode = 1;
   this->DebugDraw = false;
   this->Shade = 4;
   this->Radius = 0;
   this->Peek = false;
   this->EditLayer = 0;
   this->AdvancedUserMode = 0;
   this->MsgNo = 0;

   this->TileList     = NULL;
   this->SelectedTile = NULL;

   this->ClientWidth = 570;
   this->ClientHeight = 310;

   OutputDebugString("Taipei.exe\nParams: \n  -d debug mode\n  -m no messages\n");
   OutputDebugString("Press T in main window for Dragon layout and WatchBuild option");
   OutputDebugString("  Ctrl+T  for Difficulty and Peak options");
   OutputDebugString("  Shift+T for Save Game and Load Game functionalities");
   OutputDebugString("  Shift+Alt+T for experimental custom layout editor");

   Reg->RootKey = HKEY_CURRENT_USER;
   OpenResult = Reg->OpenKey("Software\\tDavidM\\Taipei\\", true);
   if(OpenResult)
      this->mMessages->Checked = Reg->ReadString("MessagesVisible") == "True";
	Reg->CloseKey();
	Reg->Free();

   for (int i=1;i<=ParamCount();i++) {
      //DEBUG mode
      if (LowerCase(ParamStr(i)) == "-d") {
         Beep();
         this->DebugDraw = true;
         this->Mode = 0;
         lDebug->Visible = true;
      //No Messages
      } else if (LowerCase(ParamStr(i)) == "-m") {
         this->mMessages->Checked = false;
      }
   }
}
//---------------------------------------------------------------------------

//Toggles contextual help/reminder messages about gameplay
void __fastcall TfTaipei::mMessagesClick(TObject *Sender)
{
   this->mMessages->Checked = !this->mMessages->Checked;

   //If Ctrl+T was pressed
   if (this->AdvancedUserMode > 0) {
      bool OpenResult;
	   TRegistry* Reg = new TRegistry(KEY_WRITE);

	   Reg->RootKey = HKEY_CURRENT_USER;
      OpenResult = Reg->OpenKey("Software\\tDavidM\\Taipei\\", true);
	   if(OpenResult)
         Reg->WriteString("MessagesVisible", this->mMessages->Checked ? "True" : "False");
	   Reg->CloseKey();
	   Reg->Free();
   }
}
//---------------------------------------------------------------------------

//Shows a system MessageBox with infos about Taipei
void __fastcall TfTaipei::mAboutClick(TObject *Sender)
{
   String DialogText = "Taipei !\n\nOriginal by Dave Norris\n"
					   "Clone by David Morrissette\n\n2020-2023\n\n\nPress T in main window for more options";
   String DialogCap  = "About " + this->Caption;

   if (this->AdvancedUserMode > 0) {
      DialogText = DialogText + "\n or Ctrl+T";
      if (this->AdvancedUserMode > 1)
         DialogText = DialogText + "\n or Shift+T";
      DialogText = DialogText + "\n these options were available in lesser known versions of Taipei.";
   }
   Application->MessageBox(DialogText.w_str(), DialogCap.w_str(), MB_OK);
}
//--------------------------------------------------------------------------

//Shows a system MessageBox with help to play the game
void __fastcall TfTaipei::mHowtoPlayClick(TObject *Sender)
{
   String DialogText = "Taipei is a modern, solitaire version of the ancient oriental game, Mah-Jongg.\n\n"
                       "Playing Taipei is simple.\n"
                       "The object of Taipei is to remove all of the tiles from the board.\n\n"
                       "Tiles are removed from the board in matching pairs.\n"
                       "Tiles can only be removed if they are \"free\".\n"
                       "A tile is \"free\" if:\n"
                       "  1) it has no tiles on top of it, and\n"
                       "  2) you can \"slide\" the tile out to the right or left.";

   Application->MessageBox(DialogText.w_str(), L"What is Taipei?", MB_OK);
}
//---------------------------------------------------------------------------

//Shows a system MessageBox with strategies to play the game
void __fastcall TfTaipei::mStrategyClick(TObject *Sender)
{
   String DialogText = "Winning a game is much like solving an intricate puzzle.\n"
                       "One false move at the beginning could ruin your chances at victory.\n"
                       "And, while every game has a solution, finding it often takes hours of diligent play.\n\n"
                       "Here are some hints if you are having difficulty solving the Taipei puzzles:\n"
                       "1.  Remove the end tiles as soon as you can, "
                       "especially those tiles that block more than one tile.\n"
                       "2.  If all four tiles in a matching set are free, you can remove all four safely.\n"
                       "3.  Work from the outside in.";

   Application->MessageBox(DialogText.w_str(), L"Strategy", MB_OK); // MB_HELP
}
//---------------------------------------------------------------------------

//Change zoom to 100%
void __fastcall TfTaipei::m_100pClick(TObject *Sender)
{
   this->m_100p->Checked = true;
   this->m_150p->Checked = false;
   this->m_200p->Checked = false;

   this->mlTiles->Clear();
   this->mlTiles->SetSize(36,36);
   if (this->mColor->Checked)
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_100_ALL", clWhite);
   else
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_100_ALL_B", clWhite);
   this->ZoomFactor   = 100;
   this->ClientWidth  = 570;
   this->ClientHeight = 310;

   this->TILESIZE     = TILESIZE_100;
   this->HALFTILESIZE = HALFTILESIZE_100;
   this->TILEXOFFSET  = TILEXOFFSET_100;
   this->TILEYOFFSET  = TILEYOFFSET_100;

   this->Invalidate();
}
//---------------------------------------------------------------------------

//Change zoom to 150%
void __fastcall TfTaipei::m_150pClick(TObject *Sender)
{
   this->m_100p->Checked = false;
   this->m_150p->Checked = true;
   this->m_200p->Checked = false;

   this->mlTiles->Clear();
   this->mlTiles->SetSize(54,54);
   if (this->mColor->Checked)
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_150_ALL", clWhite);
   else
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_150_ALL_B", clWhite);
   this->ZoomFactor   = 150;
   this->ClientWidth  = 855;
   this->ClientHeight = 465;

   this->TILESIZE     = TILESIZE_150;
   this->HALFTILESIZE = HALFTILESIZE_150;
   this->TILEXOFFSET  = TILEXOFFSET_150;
   this->TILEYOFFSET  = TILEYOFFSET_150;

   this->Invalidate();
}
//---------------------------------------------------------------------------

//Change zoom to 200%
void __fastcall TfTaipei::m_200pClick(TObject *Sender)
{
   this->m_100p->Checked = false;
   this->m_150p->Checked = false;
   this->m_200p->Checked = true;

   this->mlTiles->Clear();
   this->mlTiles->SetSize(72,72);
   if (this->mColor->Checked)
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_200_ALL", clWhite);
   else
      this->mlTiles->ResourceLoad(rtBitmap, "TILE_200_ALL_B", clWhite);
   this->ZoomFactor   = 200;
   this->ClientWidth  = 1140;
   this->ClientHeight = 620;

   this->TILESIZE     = TILESIZE_200;
   this->HALFTILESIZE = HALFTILESIZE_200;
   this->TILEXOFFSET  = TILEXOFFSET_200;
   this->TILEYOFFSET  = TILEYOFFSET_200;

   this->Invalidate();
}
//---------------------------------------------------------------------------

//Toggles between color and black&white tile graphs
void __fastcall TfTaipei::mColorClick(TObject *Sender)
{
   this->mColor->Checked = !this->mColor->Checked;

   if (this->m_100p->Checked)
      this->m_100p->Click();

   if (this->m_150p->Checked)
      this->m_150p->Click();

   if (this->m_200p->Checked)
      this->m_200p->Click();

   this->Repaint();
}
//---------------------------------------------------------------------------

//Toggles tile heap build animation
void __fastcall TfTaipei::mWatchBuildsClick(TObject *Sender)
{
   this->mWatchBuilds->Checked = !this->mWatchBuilds->Checked;
}
//---------------------------------------------------------------------------

//Lowers the tile's eges brightness
void __fastcall TfTaipei::mDarkenClick(TObject *Sender)
{
   if (this->Shade > 1)
      this->Shade--;

   this->Repaint();
}
//---------------------------------------------------------------------------

//Increases the tile's eges brightness
void __fastcall TfTaipei::mLightenClick(TObject *Sender)
{
   if (this->Shade < 8)
      this->Shade++;

   this->Repaint();
}
//---------------------------------------------------------------------------

//Toggles the Peek Mode used to temporarily remove any tile to have a look under
void __fastcall TfTaipei::mPeekClick(TObject *Sender)
{
   this->Peek = !this->Peek;
   this->mPeek->Checked = !this->mPeek->Checked;
}
//---------------------------------------------------------------------------

//Activates Beginner Mode where a pair of tiles will preferencialy be located within a limited distance when possible
void __fastcall TfTaipei::mBeginnerClick(TObject *Sender)
{
   this->Radius = 3;
   this->mExpert->Checked   = false;
   this->mBeginner->Checked = true;
}
//---------------------------------------------------------------------------

//Activates Expert Mode where a pair of tiles have no bias on their relative random location
void __fastcall TfTaipei::mExpertClick(TObject *Sender)
{
   this->Radius = 0;
   this->mExpert->Checked   = true;
   this->mBeginner->Checked = false;
}
//---------------------------------------------------------------------------

//Called by every entries in the Layour menu, sets the game Mode using the caller's Tag
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
   this->mCustom->Checked   = false;

   TMenuItem *Current = (TMenuItem *)Sender;
   Current->Checked = true;

   this->Mode = Current->Tag;
   this->mNewClick(NULL);
}
//---------------------------------------------------------------------------

//Resets the current game back to start
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
      this->HintLoopMain = NULL;
      this->GamedDone = 0;
      this->Repaint();
   }
}
//---------------------------------------------------------------------------

//Revers the last move, can be called sequentially to cancel all moves back to start, cannot be undo
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
      this->HintLoopMain = NULL;
      this->Repaint();
   }
}
//---------------------------------------------------------------------------

//Calls to Initialize a new game (by calling InitGame() ) with a random seed
void __fastcall TfTaipei::mNewClick(TObject *Sender)
{
   int GameNo;

   Randomize();
   GameNo = Random(32767);

   this->InitGame(GameNo);
}
//---------------------------------------------------------------------------

//Calls to Initialize a new game (by calling InitGame() ) with a seed provided by the user
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
      //The original had a stange "bug" replicated here,
      //  I belive the seed was stored in a short unsigned int while the prompt used a short signed int
      //  out of bounds values where set to zero instead of wrapping around
      if (GameNo > 32767)
         GameNo = 0;
      if (GameNo < -32767)
         GameNo = 0;
      if (GameNo < 0)
         GameNo = (65536 + GameNo);

      this->InitGame(GameNo);
   }
   delete fSelectGame;
}
//---------------------------------------------------------------------------

//Initializes a new game using a seed given in parameter and the currently selected game mode (layout)
void TfTaipei::InitGame(int pGameNo)
{
   this->tAutoPlay->Enabled = false;
   this->mAutoPlay->Checked = false;
   this->iMainLogo->Visible = false;
   this->lMainTitleShadow->Visible = false;
   this->lMainTitle->Visible = false;
   this->StepBack = 0;
   this->GamedDone = 0;
   this->HintLoopMain = NULL;
   this->SelectedTile = NULL;
   this->GameNumber = pGameNo;
   this->Caption = "Taipei Game  #" + IntToStr(pGameNo);

   this->BuildStructure(this->Mode);
   this->FillStructure(pGameNo);
   this->Repaint();
}
//---------------------------------------------------------------------------

//Get next available tile for hint
TTile* TfTaipei::GetNextHintTile(TTile* pStartingTile, int pType)
{
   bool ItsFound = false;

   while(pStartingTile != NULL && !ItsFound) {
      if (pStartingTile->Visible &&
          (pType == -1 || pStartingTile->Type == pType) &&
          this->IsTileFree(pStartingTile))
         ItsFound = true;
      else
         pStartingTile = pStartingTile->Next;
   }
   return pStartingTile;
}

//---------------------------------------------------------------------------

//suggests a pair of matching tiles from the available tiles at the current game state, can be called sequentially to show all matchs
void __fastcall TfTaipei::mHintClick(TObject *Sender)
{
   TTile* StartingTile = NULL;
   TTile* FollowingTile = NULL;
   bool TryNext;
   int LoopCmp = 0;

   this->tAutoPlay->Enabled = false;
   this->mAutoPlay->Checked = false;

   //Cancel any selection
   if (this->SelectedTile != NULL) {
      this->SelectedTile->Selected = false;
      this->SelectedTile = NULL;
   }

   //If HintLoopMain not yet initialized (at the beginning of the game and after every step)
   if (this->HintLoopMain == NULL) {
      //Find the first available tile
      this->HintLoopMain   = GetNextHintTile(this->TileList);
      if (this->HintLoopMain != NULL)
         this->HintLoopSecond = GetNextHintTile(this->HintLoopMain->Next, this->HintLoopMain->Type);
   }
   StartingTile  = this->HintLoopMain;
   FollowingTile = this->HintLoopSecond;

   //Just a quick test in case
   if (StartingTile != NULL) {
      do {
         //FailSafe counter
         LoopCmp++;

         //If match exists
         if (FollowingTile != NULL) {
            //Show hint
            StartingTile->Selected = true;
            FollowingTile->Selected = true;
            this->Repaint();
            Sleep(100);
            StartingTile->Selected = false;
            FollowingTile->Selected = false;
            this->Repaint();

            //Select the next secondary matching available tile of the same type
            FollowingTile = GetNextHintTile(FollowingTile->Next, StartingTile->Type);
            TryNext = false;
         } else {
            //Select the next available tile and the next matching available tile of the same type
            StartingTile  = GetNextHintTile(StartingTile->Next);
            TryNext = true;
            //If end of main loop is reach, skip one round
            if (StartingTile == NULL)
               TryNext = false;
            else
               FollowingTile = GetNextHintTile(StartingTile->Next, StartingTile->Type);
         }
      } while (TryNext && LoopCmp <= 72);

      this->HintLoopMain   = StartingTile;
      this->HintLoopSecond = FollowingTile;
   }
}
//---------------------------------------------------------------------------

//Toggles the AutoPlay mode where the games plays a move every few seconds (see tAutoPlayTimer)
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

//Plays automaticly the best move from a given game state
void __fastcall TfTaipei::tAutoPlayTimer(TObject *Sender)
{
   TTile* CurrentTile = this->TileList;
   TTile* SelectTile = NULL;
   int MinHint   = 72;
   int FloorHint = 0;
   int MaxLoop   = 72;

   if (this->SelectedTile != NULL) {
      this->SelectedTile->Selected = false;
      this->SelectedTile = NULL;
   }

   //May take multiple tries
   do {
      //Find the lowest Hint value from all free and visible tiles and select it
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && CurrentTile->Hint <= MinHint && CurrentTile->Hint > FloorHint &&
              this->IsTileFree(CurrentTile)) {
            MinHint = CurrentTile->Hint;
            this->SelectedTile = CurrentTile;
         }
         CurrentTile = CurrentTile->Next;
      }
      if (this->SelectedTile != NULL)
         this->SelectedTile->Selected = true;

      //Select the matching tile with lowest Hint value if possible
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible && CurrentTile->Hint == MinHint &&
              CurrentTile->Id != this->SelectedTile->Id && this->IsTileFree(CurrentTile)) {
            SelectTile = CurrentTile;
            SelectTile->Selected = true;
         }
        CurrentTile = CurrentTile->Next;
      }

      //Select the other tile available of the same type if possible
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

      //If no match found, try again with the the next "lowest" that is greater than this one
      if (SelectTile == NULL) {
         FloorHint = MinHint;
         MinHint = 72;
         MaxLoop--;
         this->SelectedTile->Selected = false;
         this->SelectedTile = NULL;
         CurrentTile = this->TileList;
      }
   } while(MaxLoop > 0 && SelectTile == NULL) ;

   if (SelectTile != NULL) {
      this->Repaint();
      Sleep(100);
      this->HideTileStep(SelectTile, true);
   } else {
      //AutoPlay is stuck and cannot finish the game as it was
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

//Senses keystrokes used as shortcuts
void __fastcall TfTaipei::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   //If not in Edit mode
   if (this->EditLayer == 0) {
      if(Key == 0x8 /*BackSpace*/)
         this->mBackup->Click();

      if (Key == 0x48 /*VK_KEY_H*/)
         this->mHint->Click();

      if (Key == 0x54 /*VK_KEY_T*/) {
         this->mDragon->Visible      = true;
         this->m_Zoom->Visible       = true;
         this->mWatchBuilds->Visible = true;
         this->AdvancedUserMode = this->AdvancedUserMode | 1;
         if (Shift.Contains(ssCtrl)) {
            this->mLighten->Visible    = true;
            this->mDarken->Visible     = true;
            this->mDifficulty->Visible = true;
            this->mPeek->Visible       = true;
            this->AdvancedUserMode = this->AdvancedUserMode | 2;
         }
         if (Shift.Contains(ssShift)) {
            this->mFile->Visible = true;
            this->AdvancedUserMode = this->AdvancedUserMode | 4;
         }
         if (Shift.Contains(ssAlt)) {
            this->N5->Visible      = true;
            this->mCreate->Visible = true;
            this->mPlay->Visible   = true;
            this->AdvancedUserMode = this->AdvancedUserMode | 8;
         }
      }
   }
}
//---------------------------------------------------------------------------

//Senses cursor mouvements in the game area, primarily cursor over tile events
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

      if (this->Peek && CurrentTile->Z > 0 ) {
         Screen->Cursor = crHelp;  //crHandPoint; crDrag;
      } else {
         if (this->IsTileFree(CurrentTile))
            Screen->Cursor = crCross;
         else
            Screen->Cursor = crDefault;
      }
   } else
      Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

//Senses cursor clicks in the game area, primarily tile selection events
void __fastcall TfTaipei::FormMouseDown(TObject *Sender, TMouseButton Button,
   TShiftState Shift, int X, int Y)
{
   TTile* CurrentTile;
   TPoint Pos;
   int TileCmp = 0;
   bool Found = false;

   Pos.x = X;
   Pos.y = Y;

   CurrentTile = this->GetTile(Pos);
   if (CurrentTile != NULL ) {
      this->tAutoPlay->Enabled = false;
      this->mAutoPlay->Checked = false;

      //Peek Mode
      if (this->Peek) {
         //Tile clicked on toggles visibility for 750ms
         CurrentTile->Visible = false;
         this->Repaint();
         this->Peek = false;
         this->mPeek->Checked = false;
         Sleep(750);
         CurrentTile->Visible = true;
         this->Repaint();
      } else { //Normal Mode
         //Tile clicked on get selected or removed
         if (!this->IsTileFree(CurrentTile)) {
            if (this->mMessages->Checked)
               Application->MessageBox(L"Tiles isn't free", L"Taipei", MB_OK | MB_ICONINFORMATION);
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
                        Application->MessageBox(L"Tiles don't match", L"Taipei", MB_OK | MB_ICONINFORMATION);

                     //There's a strange rare bug where the SelectedTile is no longer visible (seen once)
                     //  or not free yet (uncertain) and that jams the game logics, the player can't recover
                     if (!this->SelectedTile->Visible || !this->IsTileFree(this->SelectedTile)) {
                        //So when this situation is detected, simply deselect the invalid selection
                        this->SelectedTile->Selected = false;
                        this->SelectedTile = NULL;
                        this->Repaint();
                        //Testers did not repport using Autoplay, just regular click when the bug occurs
                     }
                  }
               }
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

//Calls tiles drawing procedures
void __fastcall TfTaipei::FormPaint(TObject *Sender)
{
   this->DrawAllTiles();
}
//---------------------------------------------------------------------------

//Called when a tile is selected, if valid, it will select a tile or match with a selected tile and test if game is done
void TfTaipei::HideTileStep(TTile* pTile, bool pAutoPlay)
{
   TTile* CurrentTile;
   TTile* LookupTile;
   bool Found = false;

   pTile->Visible = false;
   pTile->Selected = false;
   pTile->Step = this->StepBack;

   this->SelectedTile->Visible = false;
   this->SelectedTile->Selected = false;
   this->SelectedTile->Step = this->StepBack;

   this->SelectedTile = NULL;
   this->StepBack++;
   this->HintLoopMain = NULL;
   this->Repaint();

   //Test if endgame
   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      if(CurrentTile->Visible)
         Found = true;
      CurrentTile = CurrentTile->Next;
   }
   if (!Found) {
      if (!pAutoPlay) {
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         if (this->GamedDone <= 0) {
            Application->MessageBox(gCongrat[this->MsgNo].w_str(), L"Winner'qus Fortune", MB_OK | MB_ICONEXCLAMATION);
         }
      } else {
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         PlaySound("SystemAsterisk", 0, SND_ALIAS || SND_ASYNC);
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
         this->tAutoPlay->Enabled = false;
         this->mAutoPlay->Checked = false;
         if (!pAutoPlay)
            Application->MessageBox(L"No free tiles", L"Taipei", MB_OK | MB_ICONSTOP);
         else
            PlaySound("SystemAsterisk", 0, SND_ALIAS || SND_ASYNC);
      }
   }
}
//---------------------------------------------------------------------------

//Inverts the RGB color values of a bitmap given in parameter
void __fastcall TfTaipei::Invert(Graphics::TBitmap *pBitmap)
{
   Byte *pyx;

   pBitmap->HandleType = bmDIB; // allows use of ScanLine
   pBitmap->PixelFormat = pf24bit;

   for(int y=0; y<pBitmap->Height-1; y++) {
      pyx=(Byte *)pBitmap->ScanLine[y];
      for(int x=0; x<pBitmap->Width-1; x++) {
         pyx[x*3]     = (255 -pyx[x*3]);
         pyx[x*3 + 1] = (255 -pyx[x*3 + 1]);
         pyx[x*3 + 2] = (255 -pyx[x*3 + 2]);
      }
   }
}
//---------------------------------------------------------------------------

//Draws a tile with a graphic according to the given parameters (Called by DrawAllTiles )
void TfTaipei::DrawTile(int pId, bool pSel, int pRealX, int pRealY, bool pNotch, int pDebug)
{
   TPoint LeftSide[4];
   TPoint LowerSide[4];
   Graphics::TBitmap * TileGraph;

   this->Canvas->Pen->Color = clBlack;

   if (pDebug > 0) {
      //Debug color mode
      switch(pDebug) {
         case 1 : this->Canvas->Brush->Color = clBlue; 
            break;
         case 2 : this->Canvas->Brush->Color = clBlack;
            break;
      }
   } else {
      //Normal Mode
      if (this->Shade == 4)
         this->Canvas->Brush->Color = clGray; //clDkGray or 0x808080 or 8421504
      else {
         this->Canvas->Brush->Color = (TColor)(((this->Shade * 0x1F)) +         //R
                                               ((this->Shade * 0x1F) * 256 ) +  //V
                                               ((this->Shade * 0x1F) * 65536)); //B  //0x1F = 31
      }
   }

   LeftSide[0] = Point(pRealX, pRealY); //Upper Left Corner (Top)
   LeftSide[1] = Point(pRealX , pRealY + TILESIZE - 1); //Lower Left Corner (Top)
   LeftSide[2] = Point(pRealX - TILEXOFFSET, pRealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
   LeftSide[3] = Point(pRealX - TILEXOFFSET, pRealY + TILEYOFFSET); //Upper Left Corner (Bottom)
   this->Canvas->Polygon(LeftSide, 3);

   if (pNotch) {
      LowerSide[0] = Point(pRealX , pRealY + TILESIZE - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(pRealX + HALFTILESIZE - 1, pRealY + TILESIZE - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(pRealX - TILEXOFFSET + HALFTILESIZE - 1, pRealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(pRealX - TILEXOFFSET, pRealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   } else {
      LowerSide[0] = Point(pRealX , pRealY + TILESIZE - 1); //Lower Left Corner (Top)
      LowerSide[1] = Point(pRealX + TILESIZE - 1, pRealY + TILESIZE - 1); //Lower Right Corner (Top)
      LowerSide[2] = Point(pRealX - TILEXOFFSET + TILESIZE - 1, pRealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
      LowerSide[3] = Point(pRealX - TILEXOFFSET, pRealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
      this->Canvas->Polygon(LowerSide, 3);
   }

   if (pDebug > 0) { //Debug color mode
      this->Canvas->Rectangle(pRealX, pRealY, pRealX + TILESIZE, pRealY + TILESIZE); //Top Face
   } else {
      TileGraph = new Graphics::TBitmap;

      this->Canvas->Brush->Color = clWhite;
      this->Canvas->Rectangle(pRealX, pRealY, pRealX + TILESIZE, pRealY + TILESIZE); //Top Face

      this->mlTiles->GetBitmap(pId, TileGraph);

      if(pSel)
         this->Invert(TileGraph);

      this->Canvas->Draw(pRealX, pRealY, TileGraph);

      delete TileGraph;
   }
}
//---------------------------------------------------------------------------

//Draws all visibles tiles on the game area as graphic or wireframe depending on requirements
void TfTaipei::DrawAllTiles(void)
{
   //Draw from the Bottom to the Top Layer (z 0 to 7)
   //Draw from the Upper to the Lower Row (y 0 to 17)
   //Draw from the Right to the Left Column (x 31 to 0)

   TTile* CurrentTile;
   TTile* NotchTile;
   bool IndNotch;
   int RealX, RealY;
   CurrentTile = this->TileList;

   while(CurrentTile != NULL) {
      RealX = (CurrentTile->X * HALFTILESIZE) + 8 + (CurrentTile->Z * TILEXOFFSET); // 8 = Global Top Offset
      RealY = (CurrentTile->Y * HALFTILESIZE) + 6 - (CurrentTile->Z * TILEYOFFSET); // 6 = Global Left Offset

      if (CurrentTile->Visible || CurrentTile->Debug) {
         //Solide Mode (Polygon)
         NotchTile = this->GetTile(CurrentTile->X + 1, CurrentTile->Y + 2, CurrentTile->Z, true, true);
         if (NotchTile != NULL)
            IndNotch = NotchTile->Visible;
         else
            IndNotch = false;

         this->DrawTile(CurrentTile->Graph, CurrentTile->Selected, RealX, RealY, IndNotch, CurrentTile->Debug);

         if (this->DebugDraw)
            this->Canvas->TextOutA(RealX+2, RealY+2, IntToStr(CurrentTile->Hint));
      } else if (CurrentTile->WireFrame || this->DebugDraw) {
         //WireFrame Mode (LineTo)
         if (CurrentTile->WireFrame)
            this->Canvas->Pen->Color = clPurple;
         else
            this->Canvas->Pen->Color = clBlack;

         //Left Side
         this->Canvas->MoveTo(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
         this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
         this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET); //Upper Left Corner (Bottom)
         this->Canvas->LineTo(RealX, RealY); //Upper Left Corner (Top)
         //LowerSide
         this->Canvas->MoveTo(RealX + TILESIZE - 1, RealY + TILESIZE - 1); //Lower Right Corner (Top)
         this->Canvas->LineTo(RealX - TILEXOFFSET + TILESIZE - 1, RealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
         this->Canvas->LineTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET + TILESIZE); //Lower Left Corner (Bottom)
         this->Canvas->LineTo(RealX , RealY + TILESIZE - 1); //Lower Left Corner (Top)
         //Top
         this->Canvas->MoveTo(RealX, RealY);
         this->Canvas->LineTo(RealX + TILESIZE, RealY);
         this->Canvas->LineTo(RealX + TILESIZE, RealY + TILESIZE);
         this->Canvas->LineTo(RealX, RealY + TILESIZE);
         this->Canvas->LineTo(RealX, RealY);

         //UnderSide (back and right)
         if (CurrentTile->WireFrame) {
            this->Canvas->MoveTo(RealX - TILEXOFFSET, RealY + TILEYOFFSET); //Upper Left Corner (Bottom)
            this->Canvas->LineTo(RealX - TILEXOFFSET + TILESIZE, RealY + TILEYOFFSET); //Upper Right Corner (Bottom)
            this->Canvas->LineTo(RealX + TILESIZE, RealY); //Upper Right Corner (Top)
            this->Canvas->MoveTo(RealX - TILEXOFFSET + TILESIZE, RealY + TILEYOFFSET); //Upper Right Corner (Bottom)
            this->Canvas->LineTo(RealX - TILEXOFFSET + TILESIZE - 1, RealY + TILEYOFFSET + TILESIZE); //Lower Right Corner (Bottom)
         }
      }

      CurrentTile = CurrentTile->Next;
   }
}
//---------------------------------------------------------------------------

//Locates a tile at a given grid/game coordinates with exact match or optional first match
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

//Locates a tile at a given form/pixel coordinates with exact match or optional first match
TTile* TfTaipei::GetTile(TPoint pRealPos, int pExactZ)
{
   static TTile* CurrentTile;
   int x, y;
   int z = MAXNUMBERLAYER;
   bool Found = false;

   //If used in normal mode, ExactZ is only used for Design/Edit Layout Mode
   if (pExactZ < 0) {
      do {
         x = ((pRealPos.x - 8) - (z * TILEXOFFSET)) / HALFTILESIZE;
         y = ((pRealPos.y - 6) + (z * TILEYOFFSET)) / HALFTILESIZE;

         CurrentTile = this->GetTile(x, y, z, false, true);
         if (CurrentTile != NULL)
            Found = CurrentTile->Visible || CurrentTile->WireFrame ;
         z -= 1;
      } while(!Found && z >= 0);

      if (Found)
         return CurrentTile;
   }
   else
   {
      //This mode is only used in Design/Edit Layout Mode
      x = ((pRealPos.x - 8) - (pExactZ * TILEXOFFSET)) / HALFTILESIZE;
      y = ((pRealPos.y - 6) + (pExactZ * TILEYOFFSET)) / HALFTILESIZE;

      //Find any tile that could interfere
      CurrentTile = this->TileList;
      while(CurrentTile != NULL && !Found) {
         if (   CurrentTile->X >= x-1 && CurrentTile->Y >= y-1
             && CurrentTile->X <= x+1 && CurrentTile->Y <= y+1
             && CurrentTile->Z == pExactZ
             && CurrentTile->Visible)
               return CurrentTile;
             else
               CurrentTile = CurrentTile->Next;
      }

      CurrentTile = this->GetTile(x, y, pExactZ, true, true);
      if (CurrentTile != NULL)
         return CurrentTile;

   }
   return NULL;
}
//---------------------------------------------------------------------------

//Tests if a tile can be played, not covered anywhere on top or locked on both sides
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

//Build the linked list used to store a game layout from the hard coded arrays or cleanup a custom game layout
void TfTaipei::BuildStructure(int pMode)
{
   TTile* NextTile;
   int IdCmp = 1;
   int i, j = 0;
   int x, y;
   bool Buffer[GAMEWIDTH][GAMEHEIGHT];
   int Delta;
   const int *CurrLayer, *CurrSize;
   const int *Layer0, *Layer1, *Layer2, *Layer3, *Layer4, *Layer5, *Layer6;

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

   //If not custom layout, reset everything
   if (this->Mode != 99) {
      //Empty previous tile list
      while(this->TileList != NULL) {
        NextTile = this->TileList->Next;
        delete this->TileList;
        this->TileList = NextTile;
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
                     IdCmp++;
                  }
               }
            }
         }
      }
   } else {
      // If custom layout, just reset value in memory
      NextTile = this->TileList;
      while(NextTile != NULL) {
         if (NextTile->Step >= 0) {
            NextTile->Step = -1;
            NextTile->Visible = true;
            NextTile->Selected  = false;
         }
         NextTile = NextTile->Next;
      }
   }
   this->Repaint();
}
//---------------------------------------------------------------------------

//Sets the Type and Graphic in pairs on every tiles of the layout in a way that guarantees at least one solution
void TfTaipei::FillStructure(int pSeed)
{
   TTile* CurrentTile;
   TTile* CandidateTileA;
   TTile* CandidateTileB;
   int Swap, Target;
   int SpecGraph1 = 0, SpecGraph2 = 0;
   int LoopCmp, StepCmp = 0;
   bool StartOver = false;
   TPoint MinXY, MaxXY, LocalMin, LocalMax;
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
         if (this->Radius > 0) {
            LocalMin.x = std::max(CandidateTileA->X - this->Radius, (int)MinXY.x);
            LocalMin.y = std::max(CandidateTileA->Y - this->Radius, (int)MinXY.y);
            LocalMax.x = std::min(CandidateTileA->X + this->Radius, (int)MaxXY.x);
            LocalMax.y = std::min(CandidateTileA->Y + this->Radius, (int)MaxXY.y);
            CandidateTileB = this->FindCandidate(LocalMin, LocalMax);
            if (CandidateTileB == NULL)
               CandidateTileB = this->FindCandidate(MinXY, MaxXY);
         } else {
            CandidateTileB = this->FindCandidate(MinXY, MaxXY);
         }
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
         this->AssignTypeGraph(CandidateTileA, CandidateTileB, StepCmp-1, SpecGraph1, SpecGraph2);
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

   //Set the end of game message
   this->MsgNo = Random(CONGRATSIZE-1);

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

//Finds a random tile in the layout that can be played/remove, it may fails to find any
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

//Takes the basic Type and Graph values from FillStructure() to a tile and handles specials cases/tiles
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

//---------------------------------------------------------------------------


