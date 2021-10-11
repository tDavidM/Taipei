Taipei is an accurate 32 bits clone of the original 1990 Taipei game (16 bits) written by Dave Norris that was included in the first Microsoft Entertainment Pack.
This clone was made without any access to the original source code and is written in C++ using C++ Builder XE8 (initially Borland C++ Builder 2006).
Check Compare.png for a preview.
Remakes of Taipei were reintroduced with Windows 7 and 10 respectively as Mahjong Titans and Microsoft Mahjong.



This clone reproduces all functionalities from the version 4.0 (the one I knew as a kid) with a very high degree of fidelity, though some minor menu reorganisations and unavoidable minute visual differences exists.

The help file format from the original game is no longer supported by modern operating systems; therefore important help topics are available as system messages.

The random number generator being different, the game numbers used to “identify” and retrieve any specific tiles disposition in the game are not compatible with the original version of Taipei.

The autoplay algorithm has been improved; it is now able to find the solution every time in any state unless the user has already set the game in a state where a dead end is unavoidable.

The algorithm used to place tile pairs should be extremely similar with the original. Some decompilation attempts using a tool named Ghidra were made, however not successful enough to reveal if some special steps are taken to preferentially place matching pair of tiles within a certain distance limit when possible.
This is unlikely in the normal mode but seems to have been an option in an earlier version where two modes are available, expert (by default) and beginner.
The exact mechanism behind this adjustable difficulty is not known though it seems logical to assume the simplest way is the one describe previously where the value for that distance can be varied.
I believe that in later versions, where the difficulty cannot be dialed, the game act in expert mode and that no distance bias applied in this mode.
This is a conjecture based on hours of gameplay, I did contact the author of the original game but received no answer.
Future decompilation efforts might resolve this question.

-----------------------------------------------------------------------------------------------------------------------------

With other versions, earlier and later than 4.0, offering interesting options that are not present in 4.0, some were reintroduced in this clone.
They are hidden by default to keep the clone faithful but can be revealed using the keyboard.

An additional layout is available in the menu, the Dragon (sometime called Turtle in other games).

In the menu Options, Watch Builds can be activated to animate the tile placement when a new game is selected.

Tile's edges can be made lighter or darker in the options menu.

As discussed above, the difficulty can be set to Beginner in the options menu where the distance is set to 3, by default the game is in expert mode where no distance is set.

The Peek functionality is also in the options menu, it allows the user to temporarily look under any tile, available or not.
This is considered cheating in normal gameplay.

A new menu labeled File can be made available. In this menu, two options are shown, one where the current game state can be saved to a XML file and the other where a previously saved state file can be retrieved.
