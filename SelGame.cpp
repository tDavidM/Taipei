//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelGame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSelectGame *fSelectGame;
//---------------------------------------------------------------------------
__fastcall TfSelectGame::TfSelectGame(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfSelectGame::bOKClick(TObject *Sender)
{
   this->ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfSelectGame::bCancelClick(TObject *Sender)
{
   this->ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

