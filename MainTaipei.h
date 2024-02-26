//---------------------------------------------------------------------------

#ifndef MainTaipeiH
#define MainTaipeiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <XPMan.hpp>
#include <Dialogs.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <System.ImageList.hpp>

#define CONGRATSIZE 39
#define TILESIZE_100       36  // Horizontal pixel width of a tile

//---------------------------------------------------------------------------

const String gCongrat[CONGRATSIZE] = {
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

//Hard coded layouts, one array per layer L0 to L6 plus Size array

//number of tiles per layers { L0, L1, L2, L3, L4, L5, L6 }
const int DEBUGSIZE[7] = {29, 9, 0, 0, 0, 0, 0};

//Layer L0 up to L6 {X,Y,  X,Y,  X,Y,  X,Y,  X,Y}
//  where X low to high -> and Y low to high \/
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

const int STANDARDSIZE[7] = {63, 46, 19, 10, 3, 2, 1}; //144 tiles in total

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

const int BRIDGESIZE[7] = {24, 20, 24, 32, 16, 14, 2}; //132 tiles in total

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

const int CASTLESIZE[7] = {34, 34, 36, 28, 12, 0, 0}; //144 tiles in total

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

const int CUBESIZE[7] = {25, 25, 24, 25, 25, 0, 0}; //124 tiles in total

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

const int GLYPHSIZE[7] = {43, 43, 43, 9, 3, 1, 0}; //142 tiles in total

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

const int PYRAMIDSIZE[7] = {49, 36, 25, 16, 8, 4, 0}; //138 tiles in total
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

const int SPIRALSIZE[7] = {54, 34, 18, 4, 4, 0, 0}; //114 tiles in total
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

const int DRAGONSIZE[7] = {87, 36, 16, 4, 1, 0, 0}; //144 tiles in total
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

//Object for a Tile
class TTile
{
private:
  int Dummy;   // Not used
public:
  int Id;      // Unique ID for each tiles, 0 to number of tiles
               //   used by the AutoPlay algo and the Hint functionality as well as Save/Load game functionality
  TTile *Next; // Pointer to the next entry in the chained list used to store tiles

               // X, Y and Z coordinates for tiles draw and cursor interactions
  int X, Y, Z; // X starts on the left side and increases horizontaly toward the right
               // Y starts on the upper portion and increases verticaly toward the lower portion
               // Z starts on the bottom layer and increases verticaly toward the top layer
               // drawing is done win and isometric perspective with a "high-angle shot" plus a lateral offset
               //
               //   |\¯¯¯¯\    //
               //   \ \____\   //
               //    \|____|   //

  int Type;    // Value used to match tiles of the same type
  int Graph;   // Image list index for graph used to draw tile
  int Step;    // Indicate the order tiles were removed: starts at 0 and increases by 1 for each pair of tiles removed
  int Hint;    // Indicate the order tiles were laid, starts at number of pairs and decreases by 1 to 0
               //   used by the AutoPlay algo and the Hint functionality
  int Debug;   // Debug indicator to toggle a tile visiable as a solid color: 1=blue 2=black

  bool Selected;  // Selected tiles are drawn with an inverted color graph
  bool Visible;   // Visible tiles are drawn with their corresponding Graph
  bool WireFrame; // WireFrame tiles are drawn in ..well, wireframe :-)

  TTile(int pId, int pX, int pY, int pZ);
};

//---------------------------------------------------------------------------

class TfTaipei : public TForm
{
__published:
   TImageList *ilTiles;            //Stores all tile's graph while loaded to be resize
   TTimer *tAutoPlay;              //Timer to get a delay between AutoPlay moves
   TImage *iMainLogo;              //Main Taipei Logo image object
   TLabel *lMainTitle;             //Main Logo/title text in white transparent
   TLabel *lMainTitleShadow;       //Main Logo/title text in black with a few pixel offset to simulate a shadow
   TLabel *lNbTileLayout;          //In Edit Mode, displays the current number of tile
   TLabel *lDebug;                 //When Debug Mode is activated, displays the current tile ID and Hint value (tile under the cursor)
   TXPManifest *XPManifest;        //Improve the look of the form
   TXMLDocument *XMLDoc;           //Used to read XML files
   TSaveDialog *GameSaveDialog;    //Dialog used to save the current gamestate
   TOpenDialog *GameOpenDialog;    //Dialog used to load a previously saved gamestate
   TSaveDialog *LayoutSaveDialog;  //Dialog used to save the current edited/created custom game layout
   TOpenDialog *LayoutOpenDialog;  //Dialog used to load and play a custom game layout

   TMainMenu *mMenu;       //Menus items, wow, that's too long of a list to comment and it's fairly self-explanatory
   TMenuItem *mGame;
   TMenuItem *mOptions;
   TMenuItem *mMessages;
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
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *N3;
   TMenuItem *mColor;
   TMenuItem *mDragon;
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
   TMenuItem *mFile;
   TMenuItem *mSave;
   TMenuItem *mLoad;
   TMenuItem *mCreate;
   TMenuItem *mPlay;
   TMenuItem *N5;
   TMainMenu *mMainLayout;
   TMenuItem *mCreateLayout;
   TMenuItem *mSaveLayout;
   TMenuItem *mExitLayout;
   TMenuItem *mLayer;
   TMenuItem *mLayer7;
   TMenuItem *mLayer6;
   TMenuItem *mLayer5;
   TMenuItem *mLayer4;
   TMenuItem *mLayer3;
   TMenuItem *mLayer2;
   TMenuItem *mLayer1;
   TMenuItem *mClear;
   TMenuItem *N6;
   TMenuItem *mTestLayout;
   TMenuItem *mCustom;
   TMenuItem *mEditLayout;
   TMenuItem *mDebug;
   TMenuItem *mZoom;
   TMenuItem *m150p;
   TMenuItem *m200p;
   TMenuItem *m100p;
   TMenuItem *m125p;

   void __fastcall mExitClick(TObject *Sender);        //Called to close the application
   void __fastcall FormCreate(TObject *Sender);        //Initialises form's default values and read command line call parameters
   void __fastcall mMessagesClick(TObject *Sender);    //Toggles contextual help/reminder messages about gameplay
   void __fastcall mAboutClick(TObject *Sender);       //Shows a system MessageBox with infos about Taipei
   void __fastcall mHowtoPlayClick(TObject *Sender);   //Shows a system MessageBox with help to play the game
   void __fastcall mStrategyClick(TObject *Sender);    //Shows a system MessageBox with strategies to play the game
   void __fastcall m100pClick(TObject *Sender);        //Change zoom to 100%
   void __fastcall m125pClick(TObject *Sender);        //Change zoom to 125%
   void __fastcall m150pClick(TObject *Sender);        //Change zoom to 150%
   void __fastcall m200pClick(TObject *Sender);        //Change zoom to 200%
   void __fastcall mColorClick(TObject *Sender);       //Toggles between color and black&white tile graphs
   void __fastcall mWatchBuildsClick(TObject *Sender); //Toggles tile heap build animation
   void __fastcall mDarkenClick(TObject *Sender);      //Lowers the tile's eges brightness
   void __fastcall mLightenClick(TObject *Sender);     //Increases the tile's eges brightness
   void __fastcall mPeekClick(TObject *Sender);        //Toggles the Peek Mode used to temporarily remove any tile to have a look under
   void __fastcall mBeginnerClick(TObject *Sender);    //Activates Beginner Mode where a pair of tiles will preferencialy be located within a limited distance when possible
   void __fastcall mExpertClick(TObject *Sender);      //Activates Expert Mode where a pair of tiles have no bias on their relative random location
   void __fastcall mLayoutMenuClick(TObject *Sender);  //Called by every entries in the Layout menu, sets the game Mode using the caller's Tag
   void __fastcall mStartOverClick(TObject *Sender);   //Resets the current game back to start
   void __fastcall mBackupClick(TObject *Sender);      //Revers the last move, can be called sequentially to cancel all moves back to start, cannot be undo
   void __fastcall mNewClick(TObject *Sender);         //Calls to Initialize a new game (by calling InitGame() ) with a random seed
   void __fastcall mSelectClick(TObject *Sender);      //Calls to Initialize a new game (by calling InitGame() ) with a seed provided by the user

   void __fastcall mHintClick(TObject *Sender);        //Suggests a pair of matching tiles from the available tiles at the current game state, can be called sequentially to show all matchs
   void __fastcall mAutoPlayClick(TObject *Sender);    //Toggles the AutoPlay mode where the games plays a move every few seconds (see tAutoPlayTimer)
   void __fastcall tAutoPlayTimer(TObject *Sender);    //Plays automaticly the best move from a given game state
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);  //Senses keystrokes used as shortcuts
   void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,     
          int Y);                                      //Senses cursor mouvements in the game area, primarily cursor over tile events
   void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);            //Senses cursor clicks in the game area, primarily tile selection events
   void __fastcall FormPaint(TObject *Sender);         //Calls tiles drawing procedures

   void __fastcall mSaveClick(TObject *Sender);        //Saves a current game/layout state to a file
   void __fastcall mLoadClick(TObject *Sender);        //Loads a file to retrive previously saved game/layout state

   void __fastcall mCreateClick(TObject *Sender);      //Enters Edit Layout Mode where an user may create or edit a custom layout
   void __fastcall mExitLayoutClick(TObject *Sender);  //Exits Edit Layout Mode without saving
   void __fastcall mLayerClick(TObject *Sender);       //Called by every entries in the Layer menu, sets the EditLayer using the caller's Tag

   void __fastcall mSaveLayoutClick(TObject *Sender);  //Saves the current custom layout being edited/created to a file
   void __fastcall mEditLayoutClick(TObject *Sender);  //Loads a custom layout from a file to be edited
   void __fastcall mPlayClick(TObject *Sender);        //Loads a custom layout from a file to be played
   void __fastcall mTestLayoutClick(TObject *Sender);  //Exits Edit Layout Mode and loads the current custom layout to be played
   void __fastcall mClearClick(TObject *Sender);       //Clears the current custom layout being edited/created

   void __fastcall FormMouseMoveEditor(TObject *Sender, TShiftState Shift, int X, int Y); //Senses cursor mouvements in the game editor area
   void __fastcall FormMouseDownEditor(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //Senses cursor clicks in the game editor area


private:
   int Mode;              //Indicated tile layout selected, 0=debug, 1t o 8= hard code layout and 99=custom
   int GameNumber;        //Seed used to generate the game, 0 to 32766 ~ish
                          //  the original had a stange "bug" replicated here, see mSelectClick
   int StepBack;          //Number of steps taken, used to back/cancel a move
   int GamedDone;         //Cheating prevention messure, a player must back at least 32 moves
                          //  from a finished game to get a new "Winner'qus Fortune" message
   int TileType[72];      //Array used to generate 72 pairs for the 35 tiles types
   int SpecialGraph1[4];  //Array used to generate 2 pairs of 4 Seasons tiles
   int SpecialGraph2[4];  //Array used to generate 2 pairs of 4 Flowers tiles
   int Shade;             //Shade used for the tiles edges Default=4(Gray) take value from 0 to 8 (9 position)
   int Radius;            //Preferencial search distance (not circular tho) for tiles pairs placement
                          //  Default=0(Expert)[no limit] and only one other possible value: 3=Beginner
   int AdvancedUserMode;  //Bitmask for options made available only after pressing T or Ctrl+T or Shift+T
   int EditLayer;         //Indicate which layer is been edited, Default=0(not in edit mode)[Normal Mode]
                          //  and 1 to 7 for layer 0 to 6, ...yeah, I know D:
   int EditTileCmp;       //Counter used in Edit Mode for the number of tiles in a layout
   int MsgNo;             //End of game message index generated at the start of the game

   bool Peek;             //Flag for Peek Mode where any visible tile can be "lifted" to reveal the tile(s) under
   bool DebugDraw;        //Flag to make every invisible tiles drawn as wirefram and tiles pairs placement algo "verbose"

   int  ZoomFactor;       //Zoom factor used to display the game: 100, 150 or 200 (in %)
   int  TILESIZE;         // Horizontal pixel width of a tile
   int  HALFTILESIZE;     // Since a tile covers 2 by 2 units of game grid, an half value is sometime required
   int  TILEXOFFSET;      // Pixel horizontal offset for the high-angle isometric perspective
   int  TILEYOFFSET;      // Pixel vertical offset for the high-angle isometric perspective

   TTile* TileList;       //Pointer to the first element/entry in the Linked List used to store tiles
   TTile* SelectedTile;   //Pointer to the selected element/entry in the Linked List used to store tiles
   TTile* HintLoopMain;   //Both used by the Hint functionality to keep track of which hints have been shown
   TTile* HintLoopSecond; //  when calling Hint multiple time in a row

   void InitGame(int pGameNo); //Initializes a new game using a seed given in parameter and the currently selected game mode (layout)

   TTile* GetNextHintTile(TTile* pStartingTile,             //Get next available tile for hint
                          int pType = -1);
   void HideTileStep(TTile* pTile, bool pAutoPlay = false); //Called when a tile is selected, if valid, it will select a tile or match with a selected tile and test if game is done

   float CubicHermite (float A, float B, float C, float D, float t);                          //Used to resize tile graphs when loaded from Ressource, Compute Dest value
   Byte* SampleBicubic(Graphics::TBitmap *image, float u, float v, Byte pSrcRowCache[4][TILESIZE_100*44*3]);       //Used to resize tile graphs when loaded from Ressource, Scan source, Generate Dest pixel
   void  ResizeImage(Graphics::TBitmap *srcImage, Graphics::TBitmap *destImage, float scale); //Used to resize tile graphs when loaded from Ressource, Entry point
   void __fastcall Invert(Graphics::TBitmap *pBitmap);      //Inverts the RGB color values of a bitmap given in parameter

   void DrawTile(int pId, bool pSel, int pRealX, int pRealY,
                 bool pNotch = false, int pDebug = 0);      //Draws a tile with a graphic according to the given parameters (Called by DrawAllTiles )
   void DrawAllTiles(void);                                 //Draws all visibles tiles on the game area as graphic or wireframe depending on requirements
   TTile* GetTile(int pX, int pY, int pZ, bool pExactPos = true,
                  bool pExactZ = true);                     //Locates a tile at a given grid/game coordinates with exact match or optional first match
   TTile* GetTile(TPoint pRealPos, int pExactZ = -1);       //Locates a tile at a given form/pixel coordinates with exact match or optional first match
   bool IsTileFree(TTile* pTile);                           //Tests if a tile can be played, not covered anywhere on top or locked on both sides

   void BuildStructure(int pMode);                          //Build the linked list used to store a game layout from the hard coded arrays or cleanup a custom game layout
   void FillStructure(int pSeed);                           //Sets the Type and Graphic in pairs on every tiles of the layout in a way that guarantees at least one solution
   TTile* FindCandidate(TPoint pMinXY, TPoint pMaxXY);      //Finds a random tile in the layout that can be played/remove, it may fails to find any
   void AssignTypeGraph(TTile* pCandidateTileA, TTile* pCandidateTileB,
                        int pDelta, int& pSpecGraph1, int& pSpecGraph2); //Takes the basic Type and Graph values from FillStructure() to a tile and handles specials cases/tiles

   bool TestIfValid(int Mode);                              //Tests if a custom layout can produce a valid game

public:
   __fastcall TfTaipei(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfTaipei *fTaipei;
//---------------------------------------------------------------------------
#endif
