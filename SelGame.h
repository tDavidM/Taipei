//---------------------------------------------------------------------------

#ifndef SelGameH
#define SelGameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfSelectGame : public TForm
{
__published:	// Composants gérés par l'EDI
    TEdit *eGameNo;
    TButton *bOK;
    TButton *bCancel;
   void __fastcall bOKClick(TObject *Sender);
   void __fastcall bCancelClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
    __fastcall TfSelectGame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfSelectGame *fSelectGame;
//---------------------------------------------------------------------------
#endif
