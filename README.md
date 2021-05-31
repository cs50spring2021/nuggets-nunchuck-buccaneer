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

Assuming that player names don't have any spaces in them
Specs say optionally including spaces but unclear whether player has the option or we as the designers have the option
we assumed it was us that had the option.

Assuming Client and Server are not ctrl Cd

## Spec Differ

Spec asks for ensure dimensions to ensure large enough for the grid but we instead check that the num of cols is both enough
for the grid and the header messages