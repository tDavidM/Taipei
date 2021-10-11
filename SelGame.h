//---------------------------------------------------------------------------

#ifndef SelGameH
#define SelGameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TfSelectGame : public TForm
{
__published:	// Composants g�r�s par l'EDI
    TEdit *eGameNo;
    TButton *bOK;
    TButton *bCancel;
   void __fastcall bOKClick(TObject *Sender);
   void __fastcall bCancelClick(TObject *Sender);
private:	// D�clarations de l'utilisateur
public:		// D�clarations de l'utilisateur
    __fastcall TfSelectGame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfSelectGame *fSelectGame;
//---------------------------------------------------------------------------
#endif
