# Functionality

## PgWin

Interface Functionality
PgWin is inteded to be the MainClass for Interface Windows within the Gamebryo-Engine

To Create a new Interface Object just derive from PgWin
The SubConstructor needs to contain the Creation of All Objects within the Interfacewindow.
Currently Supported:
- Regular Sprite Objects <code> CreatePanel or  CreateFrame</code>
    - Only those are currently existing to define the Whole Frame of the Object because the Game Splits the "Main"Object in Panels and Frames (Currently only Panel01 can be used)
- Sprites that change when they get Hovered above <code> No Function to Create them Directly</code>
- Sprites that are clickable <code> CreateButton or CreateHiddenButton</code> 
    - As the Name indicates CreateHiddenButton just removes all Button related TriShapes from the SceneRoot 

All given Functions need to get the Name of the TriShapeData-Object within the .nif representing the Object

Updates of PgWin
-   All Visible PgWin-Objects get update when a MouseClick-Event appears (press or release) and every Frame
    - Check if this leads to Problems when Window gets hidden between those events
- The SceneRoot-Obj gets updated With every MouseClick-Event (press or release) and every Frame

<font color="red">For FPS gain this could be changed to only Update for Events (maybe xD)!</font>

Clicks are only viable for 1 Frame because the Obj Gets added into a ResetClick Array which gets cleared every Frame or every MouseClick-Event