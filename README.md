# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## Assumptions

Assuming Client and Server are not ctrl Cd

## Spec Differ

Spec asks for ensure dimensions to ensure large enough for the grid but we instead check that the num of cols is both enough
for the grid and the header messages

Spectator is not counted as an active player so it won't keep the server alive if active players leave

We don't submit our server.log and client.log instead just submitting our testing output

We left memory leaks in testing files but not in the modules or the program itself

The formatting (tabs/spaces) are messed up. When we edited these files on both vim and VSCode, the spacing
got messed up so that tabs became 4 spaces, not 2.

## Testing

For testing we are ignoring memory leaks within the testing and are only worried about mem
leaks in the actual program

To run module tests
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` IS a comment with a hashtag!
    Then run
        `make testModuleAndMains &> moduletest.out`
    This will create the moduletest.out output for the mod test

To run server and network test
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` is NOT a comment there should not be a hashtag!
    Then run
        `make testCmdsAndNetwork &> cmdsAndNetworkTest.out`
    This will create the cmdsAndNetworkTest.out output for the server and network test

To run Client test
    First make sure that in BOTH common and main makefiles
        `DEFINES = -DTESTING` IS a comment with a hashtag!
    Then run
        `make testClient 2> clienttest.out`
    Press x to exit the test program
    Press c to move forward a step in testing
        It will exit after a couple stages of c
    If you get an ensure dimensions message make sure to resize the window to the correct size before pressing c
    This will create the clienttest.out output for the server and network test

There should be 3 .out's we have submitted all of them
clienttest.out
cmdsAndNetworkTest.out
moduletest.out
