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
- we assume that any map passed to the server has more open spaces than the max-goldpiles constant specified in server
- we assume that there is always an open spot '.' in the current gameGrid when a new player joins
- we assume that Client and Server exit normally, ie. are not ctrl C'd

## Spec Differ

Spec asks for ensure dimensions to ensure large enough for the grid but we instead check that the num of cols is both enough
for the grid and the header messages

Spectator is not counted as an active player so it won't keep the server alive if active players leave

We don't submit our server.log and client.log instead just submitting our testing output

We left memory leaks in testing files but not in the modules or the program itself

The formatting (tabs/spaces) are messed up. When we edited these files on both vim and VSCode, the spacing
got messed up so that tabs became 4 spaces, not 2.