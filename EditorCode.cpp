//---------------------------------------------------------------------------
#include "MainTaipei.h"
//---------------------------------------------------------------------------

//Redefined from MainTaipei.cpp
#define MAXNUMBERLAYER 7   // Theres only 7 layers
#define GAMEWIDTH      62  // Horizontal width of the game grid
#define GAMEHEIGHT     34  // Vertical height of the game grid
//---------------------------------------------------------------------------

//Senses cursor mouvements in the game editor area
void __fastcall TfTaipei::FormMouseMoveEditor(TObject *Sender, TShiftState Shift,
   int X, int Y)
{
   TTile* CurrentTile;
   TPoint Pos;

   Pos.x = X;
   Pos.y = Y;

   CurrentTile = this->GetTile(Pos, this->EditLayer-1);
   if (CurrentTile != NULL) {
      if (CurrentTile != this->SelectedTile) {
         //Toggle the tile under the cursor to wireframe, only if not visible
         if(this->SelectedTile != NULL)
            this->SelectedTile->WireFrame = false;

         if (!CurrentTile->Visible) {
            CurrentTile->WireFrame = true;
            this->SelectedTile = CurrentTile;
            this->lDebug->Caption = IntToStr(CurrentTile->Id) + ":";
            this->Repaint();
         }
      }
   }
}
//---------------------------------------------------------------------------

//Senses cursor clicks in the game editor area
void __fastcall TfTaipei::FormMouseDownEditor(TObject *Sender, TMouseButton Button,
   TShiftState Shift, int X, int Y)
{
   TTile* CurrentTile;
   TPoint Pos;
   int TileCmp = 0;
   bool Found = false;

   Pos.x = X;
   Pos.y = Y;

   if (this->EditTileCmp < 144) { //Edit Layout Mode, and not already to many tiles
      //Tile clicked on toggles(Left/Right button) between visibility and wireframe
      if(Button == mbLeft) {
         if (this->SelectedTile != NULL) {
            this->SelectedTile->WireFrame = false;
            this->SelectedTile->Visible = true;
            this->SelectedTile->Selected = true;
            this->Repaint();
            Sleep(75);
            this->SelectedTile->Selected = false;
            this->SelectedTile = NULL;
         }
      } else if (Button == mbRight) {
         CurrentTile = this->GetTile(Pos);
         if (CurrentTile == NULL ) {
               //For cases where basic GetTile struggles
               Pos.x = ((Pos.x - 8) - (this->EditLayer-1 * TILEXOFFSET)) / HALFTILESIZE;
               Pos.y = ((Pos.y - 6) + (this->EditLayer-1 * TILEYOFFSET)) / HALFTILESIZE;
               CurrentTile = this->TileList;
               while(CurrentTile != NULL && !Found) {
               if (   CurrentTile->X <= Pos.x && CurrentTile->Y <= Pos.y
                   && CurrentTile->X+1 >= Pos.x && CurrentTile->Y+1 >= Pos.y
                   && CurrentTile->Z == this->EditLayer-1
                   && CurrentTile->Visible)
                  Found = true;
               else
                  CurrentTile = CurrentTile->Next;
            }
         }

         if (CurrentTile != NULL ) {
            CurrentTile->Selected = true;
            this->Repaint();
            Sleep(75);
            CurrentTile->Selected = false;
            CurrentTile->Visible = false;
         }
      }

      CurrentTile = this->TileList;
      while(CurrentTile != NULL ) {
         if (CurrentTile->Visible)
            TileCmp++;
         CurrentTile = CurrentTile->Next;
      }
      this->EditTileCmp = TileCmp;
      this->lNbTileLayout->Caption = IntToStr(TileCmp);
      this->Repaint();
   }
}
//---------------------------------------------------------------------------

//Saves a current game/layout state to a file
void __fastcall TfTaipei::mSaveClick(TObject *Sender)
{
   TTile* CurrentTile;
   String GameType = this->mLayout->Items[this->Mode]->Caption;

   GameType = StringReplace(GameType, "&", "", TReplaceFlags() << rfReplaceAll);
   this->GameSaveDialog->Title = GameType + " #" + IntToStr(this->GameNumber);
   this->GameSaveDialog->FileName = "Taipei_Game_" + GameType + "_" + IntToStr(this->GameNumber);

   if (this->TileList != NULL && this->GameSaveDialog->Execute() == true)
   {
      _di_IXMLDocument XmlRoot = NewXMLDocument();
      XmlRoot->Options = TXMLDocOptions() << doNodeAutoIndent;

	  _di_IXMLNode Game = XmlRoot->CreateElement("Game", "");
      XmlRoot->ChildNodes->Add(Game);
	  Game->SetAttribute("Name", (String)"Taipei");

        _di_IXMLNode Param = XmlRoot->CreateElement("Param", "");
        Game->ChildNodes->Add(Param);
		Param->SetAttribute("LayoutType", (String)GameType);
        Param->SetAttribute("GameNumber", (int)this->GameNumber);
        Param->SetAttribute("StepBack", (int)this->StepBack);
        Param->SetAttribute("Radius", (int)this->Radius);

          _di_IXMLNode SelTile = XmlRoot->CreateElement("SelectedTile", "");
          Param->ChildNodes->Add(SelTile);
          if (this->SelectedTile != NULL)
              SelTile->Text = IntToStr(this->SelectedTile->Id);

        _di_IXMLNode TileState = XmlRoot->CreateElement("TileState", "");
        Game->ChildNodes->Add(TileState);
        TileState->SetAttribute("TileCount", (int)this->TileList->Id);

        _di_IXMLNode Tile;
        _di_IXMLNode VisTile;
        CurrentTile = this->TileList;
        while(CurrentTile != NULL) {
           Tile = XmlRoot->CreateElement("Tile", "");
           TileState->ChildNodes->Add(Tile);
           Tile->SetAttribute("ID", (int)CurrentTile->Id);
           Tile->SetAttribute("Step", (int)CurrentTile->Step);

             VisTile = XmlRoot->CreateElement("Visible", "");
             Tile->ChildNodes->Add(VisTile);
             if (CurrentTile->Visible)
                VisTile->Text = "True";
             else
                VisTile->Text = "False";

           CurrentTile = CurrentTile->Next;
        }
      XmlRoot->SaveToFile(this->GameSaveDialog->FileName);
   }
}
//---------------------------------------------------------------------------
/*
<?xml version="1.0"?>
<Game Name="Taipei">
  <Param LayoutType="Cube" GameNumber="18215" StepBack="31" Radius="0">
    <SelectedTile>34</SelectedTile>
  </Param>
  <TileState TileCount="124">
    <Tile ID="124" Step="-1">
      <Visible>True</Visible>
    </Tile>
<!--[...]-->
  </TileState>
</Game>
*/
//---------------------------------------------------------------------------

//Loads a file to retrive previously saved game/layout state
void __fastcall TfTaipei::mLoadClick(TObject *Sender)
{
   TTile* CurrentTile;
   String LayoutType, GameName, Visible;
   int GameNo, StepBack, SelTile, Radius, TileCount, ID, Step;

   if (this->GameOpenDialog->Execute() == true) {
      try
      {
         this->XMLDoc->FileName = this->GameOpenDialog->FileName;
         this->XMLDoc->Active = true;
         _di_IXMLNode XmlRoot = XMLDoc->DocumentElement;
         //_di_IXMLNode Param = XmlRoot->ChildNodes->Nodes[0];
         GameName = XmlRoot->GetAttribute("Name");
         if (GameName == "Taipei") {
            _di_IXMLNode Param = XmlRoot->ChildNodes->Nodes[0];
            LayoutType = Param->GetAttribute("LayoutType");
            GameNo     = Param->GetAttribute("GameNumber");
            StepBack   = Param->GetAttribute("StepBack");
            Radius     = Param->GetAttribute("Radius");

            this->Mode = this->mLayout->Find(LayoutType)->Tag;
            this->Radius   = Radius;
            this->InitGame(GameNo);
            this->StepBack = StepBack;

            if (Param->ChildNodes->Nodes[0]->IsTextElement)
              SelTile = StrToInt(Param->ChildNodes->Nodes[0]->Text);
            else
              SelTile = -1;

            _di_IXMLNode TileState = XmlRoot->ChildNodes->Nodes[1];
            TileCount = StrToInt(TileState->GetAttribute("TileCount"));

            CurrentTile = this->TileList;
            _di_IXMLNodeList TileList = TileState->ChildNodes;
            if (TileCount != TileList->Count)
               throw;

            for(int i = 0; i < TileList->Count; i++) {
               ID      = TileList->Get(i)->GetAttribute("ID");
               Step    = TileList->Get(i)->GetAttribute("Step");
               Visible = TileList->Get(i)->ChildNodes->Nodes[0]->Text;

               if (CurrentTile != NULL && CurrentTile->Id != ID) {
                  CurrentTile = this->TileList;
                  while(CurrentTile != NULL && CurrentTile->Id != ID) {
                     CurrentTile = CurrentTile->Next;
                  }
               }
               if (CurrentTile == NULL || CurrentTile->Id != ID)
                  throw;

               if (CurrentTile->Id == SelTile) {
                  CurrentTile->Selected = true;
                  this->SelectedTile = CurrentTile;
               }
               CurrentTile->Visible = UpperCase(Visible) == "TRUE";
               CurrentTile->Step    = Step;
               CurrentTile = CurrentTile->Next;
            }
         }
         this->XMLDoc->Active = false;

         this->mStandard->Checked = false;
         this->mBridge->Checked   = false;
         this->mCastle->Checked   = false;
         this->mCube->Checked     = false;
         this->mGlyph->Checked    = false;
         this->mPyramid->Checked  = false;
         this->mSpiral->Checked   = false;
         this->mDragon->Checked   = false;
         this->mCustom->Checked   = false;
         this->mLayout->Find(LayoutType)->Checked = true;
      }
	  catch (Exception &E)
	  {
		 Application->MessageBox(L"Invalid File", L"Taipei", MB_OK | MB_ICONINFORMATION);
      }

      this->Repaint();
   }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

//Enters Edit Layout Mode where an user may create or edit a custom layout
void __fastcall TfTaipei::mCreateClick(TObject *Sender)
{
   TTile* NextTile;
   int z, IdCmp = 1;
   int x, y;

   this->tAutoPlay->Enabled = false;
   this->mAutoPlay->Checked = false;
   this->iMainLogo->Visible = false;
   this->lMainTitleShadow->Visible = false;
   this->lMainTitle->Visible = false;
   this->StepBack = 0;
   this->GamedDone = 0;
   this->HintLoopMain = NULL;
   this->SelectedTile = NULL;
   this->GameNumber = 0;
   this->Caption = "Taipei Create/Edit Layout";

   //Empty previous tile list
   while(this->TileList != NULL) {
      NextTile = this->TileList->Next;
      delete this->TileList;
      this->TileList = NextTile;
   }

   //Fill every possible space with invisible tiles
   for (z = MAXNUMBERLAYER-1; z >= 0; z--) {
      for (x=0; x<GAMEWIDTH-1; x++) {
         for (y=GAMEHEIGHT-2; y>=0; y--) {
         //Insert into tile list
            NextTile = new TTile(IdCmp, x, y, z);
            NextTile->Next = this->TileList;
            this->TileList = NextTile;
            NextTile->Type = 99;
            NextTile->Graph = 42;
            IdCmp++;
         }
      }
   }

   this->Repaint();

   this->Menu = this->mMainLayout;
   this->lNbTileLayout->Visible = true;
   this->mLayerClick(this->mLayer1);
   this->EditLayer = 1;
   this->EditTileCmp = 0;
   this->lNbTileLayout->Caption = "0";



   this->OnMouseMove = FormMouseMoveEditor;
   this->OnMouseDown = FormMouseDownEditor;
   //fTaipei->OnMouseMove
}
//---------------------------------------------------------------------------

//Exits Edit Layout Mode without saving and returns to an empty game area
void __fastcall TfTaipei::mExitLayoutClick(TObject *Sender)
{
   TTile* NextTile;

   this->SelectedTile = NULL;
   this->Caption = "Taipei Game";
   this->EditLayer = 0;
   this->lNbTileLayout->Visible = false;
   this->Menu = mMenu;

   this->OnMouseMove = FormMouseMove;
   this->OnMouseDown = FormMouseDown;
   //fTaipei->OnMouseMove

   //Empty previous tile list
   while(this->TileList != NULL) {
      NextTile = this->TileList->Next;
      delete this->TileList;
      this->TileList = NextTile;
   }

   this->Repaint();

   this->iMainLogo->Visible = true;
   this->lMainTitleShadow->Visible = true;
   this->lMainTitle->Visible = true;
}
//---------------------------------------------------------------------------

//Called by every entries in the Layer menu, sets the EditLayer using the caller's Tag
void __fastcall TfTaipei::mLayerClick(TObject *Sender)
{
   this->mLayer7->Checked = false;
   this->mLayer6->Checked = false;
   this->mLayer5->Checked = false;
   this->mLayer4->Checked = false;
   this->mLayer3->Checked = false;
   this->mLayer2->Checked = false;
   this->mLayer1->Checked = false;

   TMenuItem *Current = (TMenuItem *)Sender;
   Current->Checked = true;

   this->EditLayer = Current->Tag;
}
//---------------------------------------------------------------------------

//Saves the current custom layout being edited/created to a file
void __fastcall TfTaipei::mSaveLayoutClick(TObject *Sender)
{
   TTile* CurrentTile;

   this->LayoutSaveDialog->Title = "Taipei Custom Layout";
   this->LayoutSaveDialog->FileName = "Taipei_CustomLayout_";
   if (this->TileList != NULL && this->LayoutSaveDialog->Execute() == true)
   {
      _di_IXMLDocument XmlRoot = NewXMLDocument();
      XmlRoot->Options = TXMLDocOptions() << doNodeAutoIndent;

	   _di_IXMLNode Game = XmlRoot->CreateElement("Game", "");
      XmlRoot->ChildNodes->Add(Game);
	   Game->SetAttribute("Name", (String)"Taipei");

      _di_IXMLNode Layout = XmlRoot->CreateElement("Layout", "");
      Game->ChildNodes->Add(Layout);
      Layout->SetAttribute("TileCount", (int)this->EditTileCmp);

      _di_IXMLNode Tile;
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible) {
            Tile = XmlRoot->CreateElement("Tile", "");
            Layout->ChildNodes->Add(Tile);
            Tile->SetAttribute("ID", (int)CurrentTile->Id);
            Tile->SetAttribute("X", (int)CurrentTile->X);
            Tile->SetAttribute("Y", (int)CurrentTile->Y);
            Tile->SetAttribute("Layer", (int)CurrentTile->Z);
         }
         CurrentTile = CurrentTile->Next;
      }
      XmlRoot->SaveToFile(this->LayoutSaveDialog->FileName);
   }
}
//---------------------------------------------------------------------------
/*
<?xml version="1.0"?>
<Game Name="Taipei">
  <Layout TileCount="124">
    <Tile ID="124" X="12" Y="8" Layer="1" />
<!--[...]-->
  </Layout>
</Game>
*/
//---------------------------------------------------------------------------

//Loads a custom layout from a file to be edited
void __fastcall TfTaipei::mEditLayoutClick(TObject *Sender)
{
   TTile* NextTile;
   TTile* CurrentTile = NULL;
   int i, IdCmp = 1;
   int TileCount, ID, x, y, Layer;
   String GameName;

   this->mClearClick(Sender);

   if (this->LayoutOpenDialog->Execute() == true) {
      try
      {
         this->XMLDoc->FileName = this->LayoutOpenDialog->FileName;
         this->XMLDoc->Active = true;
         _di_IXMLNode XmlRoot = XMLDoc->DocumentElement;
         //_di_IXMLNode Param = XmlRoot->ChildNodes->Nodes[0];
         GameName = XmlRoot->GetAttribute("Name");
         if (GameName == "Taipei") {
            _di_IXMLNode Layout = XmlRoot->ChildNodes->Nodes[0];
            TileCount = Layout->GetAttribute("TileCount");

            CurrentTile = this->TileList;
            _di_IXMLNodeList TileList = Layout->ChildNodes;
            if (TileCount != TileList->Count)
               throw;

            for(i = 0; i < TileCount; i++) {
               ID    = TileList->Get(i)->GetAttribute("ID");
               x     = TileList->Get(i)->GetAttribute("X");
               y     = TileList->Get(i)->GetAttribute("Y");
               Layer = TileList->Get(i)->GetAttribute("Layer");

               //NextTile->Type = 99;
               //NextTile->Graph = 42;
               CurrentTile = this->TileList;
               while(CurrentTile != NULL) {
                  if (CurrentTile->X == x && CurrentTile->Y == y && CurrentTile->Z == Layer) {
                    CurrentTile->Visible = true;
                  }
                 CurrentTile = CurrentTile->Next;
               }

               IdCmp++;
            }
         }
         this->XMLDoc->Active = false;

         this->EditTileCmp = TileCount;
         this->lNbTileLayout->Caption = IntToStr(TileCount);
      }
	   catch (Exception &E)
	   {
		   Application->MessageBox(L"Invalid File", L"Taipei", MB_OK | MB_ICONINFORMATION);
      }

      this->Repaint();
   }
}
//---------------------------------------------------------------------------

//Loads a custom layout from a file to be played
void __fastcall TfTaipei::mPlayClick(TObject *Sender)
{
   TTile* NextTile;
   TTile* CurrentTile = NULL;
   int i, IdCmp = 1;
   int TileCount, x, y, Layer;
   String GameName;
   int GameNo;

   Randomize();
   GameNo = Random(32767);

   //Empty previous tile list
   while(this->TileList != NULL) {
      NextTile = this->TileList->Next;
      delete this->TileList;
      this->TileList = NextTile;
   }

   if (this->LayoutOpenDialog->Execute() == true) {
      try
      {
         this->XMLDoc->FileName = this->LayoutOpenDialog->FileName;
         this->XMLDoc->Active = true;
         _di_IXMLNode XmlRoot = XMLDoc->DocumentElement;
         //_di_IXMLNode Param = XmlRoot->ChildNodes->Nodes[0];
         GameName = XmlRoot->GetAttribute("Name");
         if (GameName == "Taipei") {
            _di_IXMLNode Layout = XmlRoot->ChildNodes->Nodes[0];
            TileCount = Layout->GetAttribute("TileCount");

            CurrentTile = this->TileList;
            _di_IXMLNodeList TileList = Layout->ChildNodes;
            if (TileCount != TileList->Count)
               throw;

            for(i = 0; i < TileCount; i++) {
               //ID    = TileList->Get(i)->GetAttribute("ID");
               x     = TileList->Get(i)->GetAttribute("X");
               y     = TileList->Get(i)->GetAttribute("Y");
               Layer = TileList->Get(i)->GetAttribute("Layer");

               NextTile = new TTile(IdCmp, x, y, Layer);
               NextTile->Visible = true;
               NextTile->Type = 99;
               NextTile->Graph = 42;

               if (IdCmp == 1) {
                  this->TileList = NextTile;
                  CurrentTile = NextTile;
               } else {
                  CurrentTile->Next = NextTile;
                  CurrentTile = CurrentTile->Next; //NextTile
               }
               //NextTile->Next = this->TileList;
               //this->TileList = NextTile;

               IdCmp++;
            }
         }
         this->XMLDoc->Active = false;

         if (TestIfValid(0))
           this->mLayoutMenuClick(mCustom);
         else
           throw;
      }
	   catch (Exception &E)
	   {
		   Application->MessageBox(L"Invalid File", L"Taipei", MB_OK | MB_ICONINFORMATION);
      }

      this->Repaint();
   }

}
//---------------------------------------------------------------------------

//Exits Edit Layout Mode and loads the current custom layout to be played
void __fastcall TfTaipei::mTestLayoutClick(TObject *Sender)
{
   TTile* CurrentTile = NULL;
   TTile* PreviousTile = NULL;
   TTile* NewTileList;
   TTile* NextTile;
   int IdCmp = 1;
   int GameNo;

   //test if even number of tiles

   if (TestIfValid(1)) {
      //Add all visible tiles to a new list
      CurrentTile = this->TileList;
      while(CurrentTile != NULL) {
         if (CurrentTile->Visible) {
            //Insert into tile list
            NextTile = new TTile(IdCmp, CurrentTile->X, CurrentTile->Y, CurrentTile->Z);

            //Add at the end of the list
            NextTile->Next = NULL;
            if (PreviousTile == NULL)
               NewTileList = NextTile;
            else
               PreviousTile->Next = NextTile;
            PreviousTile = NextTile;
            IdCmp++;
         }
         CurrentTile = CurrentTile->Next;
      }

      //Exit Create/Edit Mode
      if (this->SelectedTile != NULL) {
         this->SelectedTile->WireFrame = false;
         this->SelectedTile = NULL;
      }
      this->Caption = "Taipei Game";
      this->EditLayer = 0;
      this->lNbTileLayout->Visible = false;
      this->Menu = mMenu;

      this->OnMouseMove = FormMouseMove;
      this->OnMouseDown = FormMouseDown;
      //fTaipei->OnMouseMove

      //Empty previous tile list
      while(this->TileList != NULL) {
         NextTile = this->TileList->Next;
         delete this->TileList;
         this->TileList = NextTile;
      }

      //Make new Layout playable
      this->TileList = NewTileList;
      this->mLayoutMenuClick(mCustom);
   }
}
//---------------------------------------------------------------------------

//Clears the current custom layout being edited/created
void __fastcall TfTaipei::mClearClick(TObject *Sender)
{
   TTile* CurrentTile;

   //Clear all tiles in Create/Edit Layout Mode
   if (this->SelectedTile != NULL) {
      this->SelectedTile->WireFrame = false;
      this->SelectedTile = NULL;
   }

   CurrentTile = this->TileList;
   while(CurrentTile != NULL) {
      if (CurrentTile->Visible)
         CurrentTile->Visible = false;
      CurrentTile = CurrentTile->Next;
   }
   this->SelectedTile = NULL;

   this->EditTileCmp = 0;
   this->lNbTileLayout->Caption = "0";
}
//---------------------------------------------------------------------------

//Tests if a custom layout can produce an valid game
bool TfTaipei::TestIfValid(int Mode)
{
   if(Mode == 1) {
      //Mode 0 where the game tested is in game mode
      return true;
   }
   else
   {
      //Mode 1 where the game tested is in edit mode
      if (this->EditTileCmp%2 != 0) {
        Application->MessageBox(L"Tile count is not even", L"Taipei", MB_OK | MB_ICONINFORMATION);
        return false;
      } else
        return true;
   }
}
//---------------------------------------------------------------------------


