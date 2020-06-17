//---------------------------------------------------------------------------

#ifndef MainTaipeiH
#define MainTaipeiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <XPMan.hpp>

#define CONGRATSIZE 39

//---------------------------------------------------------------------------

const AnsiString gCongrat[CONGRATSIZE] = {
   "You will attend a party where strange customs prevail.",
   "A visit with friends will prove an enjoyable occasion.",
   "Strange new experiences will add to your joy of living.",
   "Another's expression of appreciation will delight you.",
   "Memorable moments will make your trip delightful.",
   "A joyful reunion awaits your arrival.",
   "Listen to your heart and proceed with confidence.",
   "Whatever you do, make it fun.",
   "A secret goal is in sight.  Hang in there.",
   "Through eyes of love all things will take on a new meaning.",
   "You will relax in the lap of luxury.",
   "Know yourself so that you might understand others.",
   "New experiences and new friends will enrich your life.",
   "What you do with sincerity pays the greatest reward.",
   "An unexpected gift will add to your pleasure.",
   "Your trust in a friend will prove well-founded.",
   "The concern of others will make your trip a delight.",
   "Unexpected offer deserves serious consideration.",
   "A new friendship will help cast a spell of enchantment.",
   "Unseen forces are working in your favor.",
   "Unusual offer will enhance your future.",
   "Meet a new challenge with calm assurance.",
   "Within you lies the power for good -- use it.",
   "All the little things will add to a happy journey.",
   "Welcome the chance to learn about others.",
   "Concern for a friend's happiness will enhance your own.",
   "Be patient, and the answer will be revealed.",
   "Travel with a light heart and happy expectations.",
   "You will be showered by attention.",
   "That fleeting thought is worth pursuing.",
   "A helping hand brings you closer to a secret goal.",
   "Stay calm, cool, and collected, and all things will fall into place.",
   "Accept the next proposition you hear.",
   "Congratulations on winning Taipei!",
   "Have you found the ancient Taipei secret?",
   "Why not try another fine Windows game?",
   "Wherever you go, there you are.",
   "Bouncy ball is the source of all goodness and light",
   "Congratulations on winning Taipei!"
};
//---------------------------------------------------------------------------

//number of tiles per layers { L0, L1, L2, L3, L4, L5, L6 }
const int DEBUGSIZE[7] = {29, 9, 0, 0, 0, 0, 0};

//Layer L6 down to L0 {X,Y,  X,Y,  X,Y,  X,Y,  X,Y}
//Where X low to high -> and Y low to high \/
const int DEBUGL0[29*2] = {                      17,4,
            4,6,  6,6,  8,6,  10,6,  12,6,    16,6, 18,6,     22,6,
                                                         20,7,
            4,8,  6,8,  8,8,  10,8,  12,8,    16,8, 18,8,     22,8,
                                                 17,10,
            4,10, 6,10, 8,10, 10,10, 12,10,
            4,12, 6,12, 8,12, 10,12, 12,12 };

const int DEBUGL1[9*2] = {                       17,7,
               5,8,  7,8,   9,8,  11,8,
               5,10, 7,10,  9,10, 11,10 };
//---------------------------------------------------------------------------

const int STANDARDSIZE[7] = {63, 46, 19, 10, 3, 2, 1}; //144

const int STANDARDL0[63*2] = {      10,1,  12,1,  14,1,  16,1,  18,1,
                      5,2,  7,2,                                         21,2,  23,2,
                                 9,3,  11,3, 13,3,   15,3,  17,3,  19,3,
                      5,4,  7,4,                                         21,4,  23,4,
                                    10,5,  12,5,  14,5,  16,5,  18,5,
                        6,6,  8,6,                                     20,6,  22,6,
                   4,7,             10,7,  12,7,  14,7,  16,7,  18,7,              24,7,
                        6,8,  8,8,                                     20,8,  22,8,
                                    10,9,  12,9,  14,9,  16,9,  18,9,
                      5,10, 7,10,                                        21,10, 23,10,
                                 9,11, 11,11, 13,11, 15,11, 17,11, 19,11,
                      5,12, 7,12,                                        21,12, 23,12,
                                    10,13, 12,13, 14,13, 16,13, 18,13 };

const int STANDARDL1[46*2] = {         11,1,                17,1,
                                             13,2,   15,2,
                        6,3,  8,3,  10,3,                       18,3,  20,3,  22,3,
                                           12,4,  14,4,  16,4,
                            7,5, 9,5,                              19,5, 21,5,
                                       11,6, 13,6,   15,6,  17,6,
                            7,7, 9,7,                              19,7, 21,7,
                                       11,8, 13,8,   15,8,  17,8,
                            7,9, 9,9,                              19,9, 21,9,
                                           12,10, 14,10, 16,10,
                        6,11, 8,11, 10,11,                      18,11, 20,11, 22,11,
                                             13,12,  15,12,
                                       11,13,               17,13 };

const int STANDARDL2[19*2] = {   9,5,  11,5, 13,5,   15,5,  17,5,  19,5,
                              8,7,  10,7,  12,7,  14,7,  16,7,  18,7,  20,7,
                                 9,9,  11,9, 13,9,   15,9,  17,9,  19,9  };

const int STANDARDL3[10*2] = {         11,6, 13,6,   15,6,  17,6,
                                 9,7,                              19,7,
                                       11,8, 13,8,   15,8,  17,8 };
const int STANDARDL4[3*2]  = {             12,7,  14,7,  16,7 };
const int STANDARDL5[2*2]  = {               13,7,   15,7   };
const int STANDARDL6[1*2]  = {                    14,7        };
//---------------------------------------------------------------------------

const int BRIDGESIZE[7] = {24, 20, 24, 32, 16, 14, 2}; //132

const int BRIDGEL0[24*2] = {3,4,  5,4,  7,4,                                           21,4,  23,4,  25,4,
                            3,6,  5,6,  7,6,                                           21,6,  23,6,  25,6,
                            3,8,  5,8,  7,8,                                           21,8,  23,8,  25,8,
                            3,10, 5,10, 7,10,                                          21,10, 23,10, 25,10};

const int BRIDGEL1[20*2] = {3,4,  5,4,  7,4,                                           21,4,  23,4,  25,4,
                                  5,6,  7,6,                                           21,6,  23,6,
                                  5,8,  7,8,                                           21,8,  23,8,
                            3,10, 5,10, 7,10,                                          21,10, 23,10, 25,10};

const int BRIDGEL2[24*2] = {3,4,  5,4,  7,4,  9,4,                              19,4,  21,4,  23,4,  25,4,
                                        7,6,  9,6,                              19,6,  21,6,
                                        7,8,  9,8,                              19,8,  21,8,
                            3,10, 5,10, 7,10, 9,10,                             19,10, 21,10, 23,10, 25,10};

const int BRIDGEL3[32*2] = {3,4,        7,4,  9,4,  11,4,  13,4,  15,4,  17,4,  19,4,  21,4,         25,4,
                                              9,6,  11,6,  13,6,  15,6,  17,6,  19,6,
                                              9,8,  11,8,  13,8,  15,8,  17,8,  19,8,
                            3,10,       7,10, 9,10, 11,10, 13,10, 15,10, 17,10, 19,10, 21,10,        25,10};

const int BRIDGEL4[16*2] = {3,4,              9,4,  11,4,  13,4,  15,4,  17,4,  19,4,                25,4,


                            3,10,             9,10, 11,10, 13,10, 15,10, 17,10, 19,10,               25,10};

const int BRIDGEL5[14*2] = {3,4,              9,4,             14,4,            19,4,                25,4,
                            3,6,                                                                     25,6,
                            3,8,                                                                     25,8,
                            3,10,             9,10,            14,10,           19,10,               25,10};

const int BRIDGEL6[2*2]  = {                                   14,4,


                                                               14,10};
//---------------------------------------------------------------------------

const int CASTLESIZE[7] = {34, 34, 36, 28, 12, 0, 0}; //144

const int CASTLEL0[34*2] = {6,1,  8,1,  10,1,  12,1,         16,1,  18,1,  20,1,  22,1,
                            6,3,                                                  22,3,
                            6,5,               12,5,  14,5,  16,5,                22,5,
                            6,7,               12,7,         16,7,                22,7,
                            6,9,               12,9,  14,9,  16,9,                22,9,
                            6,11,                                                 22,11,
                            6,13, 8,13, 10,13, 12,13,        16,13, 18,13, 20,13, 22,13};

const int CASTLEL1[34*2] = {6,1,  8,1,  10,1,  12,1,         16,1,  18,1,  20,1,  22,1,
                            6,3,                                                  22,3,
                            6,5,               12,5,  14,5,  16,5,                22,5,
                            6,7,               12,7,         16,7,                22,7,
                            6,9,               12,9,  14,9,  16,9,                22,9,
                            6,11,                                                 22,11,
                            6,13, 8,13, 10,13, 12,13,        16,13, 18,13, 20,13, 22,13};
                                                
const int CASTLEL2[36*2] = {6,1,  8,1,  10,1,  12,1,  14,1,  16,1,  18,1,  20,1,  22,1,
                            6,3,                                                  22,3,
                            6,5,               12,5,  14,5,  16,5,                22,5,
                            6,7,               12,7,         16,7,                22,7,
                            6,9,               12,9,  14,9,  16,9,                22,9,
                            6,11,                                                 22,11,
                            6,13, 8,13, 10,13, 12,13, 14,13, 16,13, 18,13, 20,13, 22,13};

const int CASTLEL3[28*2] = {6,1,  8,1,         12,1,  14,1,  16,1,         20,1,  22,1,
                            6,3,                                                  22,3,
                                            11,4,     14,5,     17,4,
                            6,7,               12,7,         16,7,                22,7,
                                            11,10,    14,9,     17,10,
                            6,11,                                                 22,11,
                            6,13, 8,13,        12,13, 14,13, 16,13,        20,13, 22,13};

const int CASTLEL4[12*2] = {6,1,                      14,1,                       22,1,

                                            11,4,               17,4,
                            6,7,                                                  22,7,
                                            11,10,              17,10,

                            6,13,                     14,13,                      22,13};
//---------------------------------------------------------------------------

const int CUBESIZE[7] = {25, 25, 24, 25, 25, 0, 0}; //124

const int CUBEL0[25*2] = {10,3,  12,3,  14,3,  16,3,  18,3,
                          10,5,  12,5,  14,5,  16,5,  18,5,
                          10,7,  12,7,  14,7,  16,7,  18,7,
                          10,9,  12,9,  14,9,  16,9,  18,9,
                          10,11, 12,11, 14,11, 16,11, 18,11};
const int CUBEL1[25*2] = {10,3,  12,3,  14,3,  16,3,  18,3,
                          10,5,  12,5,  14,5,  16,5,  18,5,
                          10,7,  12,7,  14,7,  16,7,  18,7,
                          10,9,  12,9,  14,9,  16,9,  18,9,
                          10,11, 12,11, 14,11, 16,11, 18,11};
const int CUBEL2[24*2] = {10,3,  12,3,  14,3,  16,3,  18,3,
                          10,5,  12,5,  14,5,  16,5,  18,5,
                          10,7,  12,7,         16,7,  18,7,
                          10,9,  12,9,  14,9,  16,9,  18,9,
                          10,11, 12,11, 14,11, 16,11, 18,11};
const int CUBEL3[25*2] = {10,3,  12,3,  14,3,  16,3,  18,3,
                          10,5,  12,5,  14,5,  16,5,  18,5,
                          10,7,  12,7,  14,7,  16,7,  18,7,
                          10,9,  12,9,  14,9,  16,9,  18,9,
                          10,11, 12,11, 14,11, 16,11, 18,11};
const int CUBEL4[25*2] = {10,3,  12,3,  14,3,  16,3,  18,3,
                          10,5,  12,5,  14,5,  16,5,  18,5,
                          10,7,  12,7,  14,7,  16,7,  18,7,
                          10,9,  12,9,  14,9,  16,9,  18,9,
                          10,11, 12,11, 14,11, 16,11, 18,11};
//---------------------------------------------------------------------------

const int GLYPHSIZE[7] = {43, 43, 43, 9, 3, 1, 0}; //142

const int GLYPHL0[43*2] = {6,1,  8,1,  10,1,        14,1,         18,1,  20,1,  22,1,
                                       10,3,        14,3,         18,3,
                           6,5,  8,5,  10,5,  12,5, 14,5,  16,5,  18,5,  20,5,  22,5,
                           6,7,        10,7,        14,7,         18,7,         22,7,
                           6,9,  8,9,  10,9,  12,9, 14,9,  16,9,  18,9,  20,9,  22,9,
                                       10,11,       14,11,        18,11,
                           6,13, 8,13, 10,13,       14,13,        18,13, 20,13, 22,13};

const int GLYPHL1[43*2] = {6,1,  8,1,  10,1,        14,1,         18,1,  20,1,  22,1,
                                       10,3,        14,3,         18,3,
                           6,5,  8,5,  10,5,  12,5, 14,5,  16,5,  18,5,  20,5,  22,5,
                           6,7,        10,7,        14,7,         18,7,         22,7,
                           6,9,  8,9,  10,9,  12,9, 14,9,  16,9,  18,9,  20,9,  22,9,
                                       10,11,       14,11,        18,11,
                           6,13, 8,13, 10,13,       14,13,        18,13, 20,13, 22,13};

const int GLYPHL2[43*2] = {6,1,  8,1,  10,1,        14,1,         18,1,  20,1,  22,1,
                                       10,3,        14,3,         18,3,
                           6,5,  8,5,  10,5,  12,5, 14,5,  16,5,  18,5,  20,5,  22,5,
                           6,7,        10,7,        14,7,         18,7,         22,7,
                           6,9,  8,9,  10,9,  12,9, 14,9,  16,9,  18,9,  20,9,  22,9,
                                       10,11,       14,11,        18,11,
                           6,13, 8,13, 10,13,       14,13,        18,13, 20,13, 22,13};

const int GLYPHL3[9*2]  = {                         14,3,
                                              12,5, 14,5,  16,5,
                                                    14,7,
                                              12,9, 14,9,  16,9,
                                                    14,11  };

const int GLYPHL4[3*2]  = {                         14,5,
                                                    14,7,
                                                    14,9  };

const int GLYPHL5[1*2]  = {                         14,7  };
//---------------------------------------------------------------------------

const int PYRAMIDSIZE[7] = {49, 36, 25, 16, 8, 4, 0}; //138
const int PYRAMIDL0[49*2] = {8,1,  10,1,  12,1,  14,1,  16,1,  18,1,  20,1,
                             8,3,  10,3,  12,3,  14,3,  16,3,  18,3,  20,3,
                             8,5,  10,5,  12,5,  14,5,  16,5,  18,5,  20,5,
                             8,7,  10,7,  12,7,  14,7,  16,7,  18,7,  20,7,
                             8,9,  10,9,  12,9,  14,9,  16,9,  18,9,  20,9,
                             8,11, 10,11, 12,11, 14,11, 16,11, 18,11, 20,11,
                             8,13, 10,13, 12,13, 14,13, 16,13, 18,13, 20,13};
const int PYRAMIDL1[36*2] = {   9,2,  11,2,  13,2,  15,2,  17,2,  19,2,
                                9,4,  11,4,  13,4,  15,4,  17,4,  19,4,
                                9,6,  11,6,  13,6,  15,6,  17,6,  19,6,
                                9,8,  11,8,  13,8,  15,8,  17,8,  19,8,
                                9,10, 11,10, 13,10, 15,10, 17,10, 19,10,
                                9,12, 11,12, 13,12, 15,12, 17,12, 19,12};
const int PYRAMIDL2[25*2] = {     10,3,  12,3,  14,3,  16,3,  18,3,
                                  10,5,  12,5,  14,5,  16,5,  18,5,
                                  10,7,  12,7,  14,7,  16,7,  18,7,
                                  10,9,  12,9,  14,9,  16,9,  18,9,
                                  10,11, 12,11, 14,11, 16,11, 18,11};
const int PYRAMIDL3[16*2] = {         11,4,  13,4,  15,4,  17,4,
                                      11,6,  13,6,  15,6,  17,6,
                                      11,8,  13,8,  15,8,  17,8,
                                      11,10, 13,10, 15,10, 17,10};
const int PYRAMIDL4[8*2]  = {             12,5,  14,5,  16,5,
                                          12,7,         16,7,
                                          12,9,  14,9,  16,9};
const int PYRAMIDL5[4*2]  = {                 13,6,  15,6,
                                              13,8,  15,8};
//---------------------------------------------------------------------------

const int SPIRALSIZE[7] = {54, 34, 18, 4, 4, 0, 0}; //114
const int SPIRALL0[54*2] = {  9,1,
                       7,2,                        15,2,  17,2,
                5,3,                        13,3,
  1,4,   3,4,          7,4,                        15,4,  17,4,  19,4,
                5,5,          9,5,   11,5,  13,5,
      2,6,             7,6,                        15,6,  17,6,  19,6,  21,6,
             4,7,             9,7,   11,7,  13,7,                              23,7,
                    6,8,                               16,8,  18,8,  20,8,            25,8,
                           8,9,   10,9,  12,9,  14,9,                       22,9,            27,9,
                                                       16,10, 18,10, 20,10,        24,10,
                                  10,11, 12,11, 14,11,                      22,11,        26,11, 28,11,
                                                       16,12,                      24,12,
                                         12,13, 14,13,                      22,13,
                                                                     20,14  };

const int SPIRALL1[34*2] = {                           16,3,
             4,4,   6,4,                        14,4,
      2,5,                                             16,5,  18,5,
             4,6,   6,6,   8,6,   10,6,  12,6,  14,6,
                                                       16,7,  18,7,  20,7,  22,7,
                       7,8,   9,8,   11,8,  13,8,
                                                   15,9,  17,9,  19,9,  21,9,  23,9,  25,9,
                                     11,10, 13,10,                                           27,10,
                                                   15,11,                      23,11, 25,11,
                                            13,12  };

const int SPIRALL2[18*2] = {                           16,4,
         3,5,   5,5,
                                                   15,6,  17,6,
                       7,7,   9,7,   11,7,  13,7,
                                                       16,8,  18,8,  20,8,  22,8,
                                         12,9,  14,9,
                                                                                   24,10, 26,10,
                                            13,11  };

const int SPIRALL3[4*2]  = {                           16,5,
                                                       16,7,
                                            13,8,
                                            13,10  };
const int SPIRALL4[4*2]  = {                       15,6,
                                            13,7,
                                                       16,8,
                                                14,9  };
//---------------------------------------------------------------------------

const int DRAGONSIZE[7] = {87, 36, 16, 4, 1, 0, 0}; //144
const int DRAGONL0[87*2] = {
    2,0,  4,0,  6,0,  8,0,  10,0,  12,0,  14,0,  16,0,  18,0,  20,0,  22,0,  24,0,
                6,2,  8,2,  10,2,  12,2,  14,2,  16,2,  18,2,  20,2,
          4,4,  6,4,  8,4,  10,4,  12,4,  14,4,  16,4,  18,4,  20,4,  22,4,
    2,6,  4,6,  6,6,  8,6,  10,6,  12,6,  14,6,  16,6,  18,6,  20,6,  22,6,  24,6,
0,7,                                                                              26,7, 28,7,
    2,8,  4,8,  6,8,  8,8,  10,8,  12,8,  14,8,  16,8,  18,8,  20,8,  22,8,  24,8,
          4,10, 6,10, 8,10, 10,10, 12,10, 14,10, 16,10, 18,10, 20,10, 22,10,
                6,12, 8,12, 10,12, 12,12, 14,12, 16,12, 18,12, 20,12,
    2,14, 4,14, 6,14, 8,14, 10,14, 12,14, 14,14, 16,14, 18,14, 20,14, 22,14, 24,14};

const int DRAGONL1[36*2] = {
                      8,2,  10,2,  12,2,  14,2,  16,2,  18,2,
                      8,4,  10,4,  12,4,  14,4,  16,4,  18,4,
                      8,6,  10,6,  12,6,  14,6,  16,6,  18,6,
                      8,8,  10,8,  12,8,  14,8,  16,8,  18,8,
                      8,10, 10,10, 12,10, 14,10, 16,10, 18,10,
                      8,12, 10,12, 12,12, 14,12, 16,12, 18,12 };
const int DRAGONL2[16*2] = {10,4,  12,4,  14,4,  16,4,
                            10,6,  12,6,  14,6,  16,6,
                            10,8,  12,8,  14,8,  16,8,
                            10,10, 12,10, 14,10, 16,10 };
const int DRAGONL3[4*2] = {        12,6,  14,6,
                                   12,8,  14,8 };
const int DRAGONL4[1*2] = {            13,7 };

//---------------------------------------------------------------------------

class TTile
{
private:
  int Dummy;
public:
  int Id;
  TTile *Next;

  int X, Y, Z;
  int Type;
  int Graph;
  int Step;
  int Hint;
  int Debug;

  bool Selected;
  bool Visible;

  TTile(int pId, int pX, int pY, int pZ);
};

//---------------------------------------------------------------------------

class TfTaipei : public TForm
{
__published:
   TMainMenu *mMenu;
   TMenuItem *mGame;
   TMenuItem *mLayout;
   TMenuItem *mHelp;
   TMenuItem *mAbout;
   TMenuItem *mNew;
   TMenuItem *mSelect;
   TMenuItem *mBackup;
   TMenuItem *mHint;
   TMenuItem *mStartOver;
   TMenuItem *mAutoPlay;
   TMenuItem *mExit;
   TMenuItem *mStandard;
   TMenuItem *mBridge;
   TMenuItem *mCastle;
   TMenuItem *mCube;
   TMenuItem *mGlyph;
   TMenuItem *mPyramid;
   TMenuItem *mSpiral;
   TImageList *mlTiles;
   TImage *iMainLogo;
   TLabel *lMainTitleShadow;
   TLabel *lMainTitle;
   TMenuItem *mOptions;
   TMenuItem *mMessages;
   TXPManifest *XPManifest1;
   TTimer *tAutoPlay;
   TLabel *lDebug;
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *N3;
   TMenuItem *mColor;
   TMenuItem *mDragon;
   TImageList *mlTilesBW;
   TMenuItem *mWatchBuilds;
   TMenuItem *N4;
   TMenuItem *mStrategy;
   TMenuItem *mLighten;
   TMenuItem *mDarken;
   TMenuItem *mDifficulty;
   TMenuItem *mBeginner;
   TMenuItem *mExpert;
   TMenuItem *mPeek;
   TMenuItem *mHowtoPlay;
   void __fastcall mExitClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall mMessagesClick(TObject *Sender);
   void __fastcall mAboutClick(TObject *Sender);
   void __fastcall mStartOverClick(TObject *Sender);
   void __fastcall mBackupClick(TObject *Sender);
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall mLayoutMenuClick(TObject *Sender);
   void __fastcall mNewClick(TObject *Sender);
   void __fastcall mSelectClick(TObject *Sender);
   void __fastcall mHintClick(TObject *Sender);
   void __fastcall mAutoPlayClick(TObject *Sender);
   void __fastcall tAutoPlayTimer(TObject *Sender);
   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall mColorClick(TObject *Sender);
   void __fastcall mWatchBuildsClick(TObject *Sender);
   void __fastcall mStrategyClick(TObject *Sender);
   void __fastcall mHowtoPlayClick(TObject *Sender);
private:
   int Mode;
   int StepBack;
   int GamedDone;
   int HintLoop;
   int TileType[72];
   int SpecialGraph1[4];
   int SpecialGraph2[4];

   bool DebugDraw;

   TTile* TileList;
   TTile* SelectedTile;

   void HideTileStep(TTile* pTile, bool pAutoPlay = false);

   Byte __fastcall TfTaipei::CustomIntToByte(int pByte);
   void __fastcall Invert(Graphics::TBitmap *pBitmap);
   void DrawTile(int pId, bool pSel, int pX, int pY, int pZ, bool pNotch = false, int pDebug = 0);
   void DrawAllTiles(void);

   TTile* GetTile(int pX, int pY, int pZ, bool pExactPos = true, bool pExactZ = true);
   TTile* GetTile(TPoint pRealPos);
   bool IsTileFree(TTile* pTile);

   void BuildStructure(int pMode);
   void FillStructure(int pSeed);
   TTile* FindCandidate(TPoint pMinXY, TPoint pMaxXY);
   void AssignTypeGraph(TTile* pCandidateTileA, TTile* pCandidateTileB, int pDelta, int& pSpecGraph1, int& pSpecGraph2);


public:
   __fastcall TfTaipei(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTaipei *fTaipei;
//---------------------------------------------------------------------------
#endif
