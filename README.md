# TrophySystem
Demo of a simple trophy system based of an event listener of a transactional database.

## Structure

|__ dbmanager.h

|__ dbmanager.cpp

Class that deals with interfacing with the database, make queries and translate them into program usable data structures, also used to write to the database.
It also generates events that listeners registered to it can react to.

|__ dblistener.h

|__ dblistener.cpp

Listener abstract class that all dbmanager listener must implement

|__ trophysystem.h

|__ trophysystem.cpp

Main files that describes the trophy system, how it avaliates and how it awards users their trophies
It implements the dblistener abstract class.

|__ trophy.h

|__ trophy.cpp

The trophy data structure

|__ constants.h

A helper namespace to congregate constants into a single place for easy refactoring

|__ globalsystemmanager.h

|__ globalsystemmanager.cpp

Static storage for static objects to ease access to them

--- Other files are visual and user interaction

### Executable

The executables folder contains an executable for win64 computers

## Brief description

Every time a new transaction is added into the database an event is triggered.  Then, the trophy system check to see if any award condition is met. If so a new registry is made into the trophy table with the user id, trophy id (identifies which type of trophy it is), trophy level and a timestamp of when the user received the award.



