# Fiesta Online Developer Tool

Welcome to the Fiesta Online Developer Tool repository! This tool is designed to aid developers and modders in working with Fiesta Online game files. Please note that this tool is provided "as is" without any warranties or guarantees, and the developer does not claim any copyright over it.

## Disclaimer

Please be aware that the Fiesta Online Developer Tool will replace a shader in the original game client. This replacement is necessary because the tool uses a newer version of the Gamebryo engine, which requires some adjustments.

As far as testing has shown, this change should not affect the game client's functionality in any way. However, please use the tool at your own risk and ensure you have backups of your original game files before making any modifications.

## Features

### SHMD Editor Submodule

- **Map Rendering**: The tool can load .shmd files and render maps based on the Fiesta Online file structure.
- **Visual Settings Editing**: Adjust settings such as Frustum, Light Colors, and Fog Values to customize the visual appearance.
- **Global Sky, Water, and Map Objects Editing**: Modify global elements of the map like the sky, water, and other map objects.
- **Non-Global Objects Editing**: Edit objects that are not globally applied. These objects can be moved and rotated using a gizmo.
- **Save Functionality**: Save the edited .shmd files with the modifications made using the tool.

### SHBD Editor Submodule

- **Loading and Editing SHBD Files**: With the new SHBD Editor submodule, you can now load and edit SHBD files as well.

### HTDG-Editor Submodule

- **Heightmap Editing**: Change the heightmap to update the visual of the map using various tools (e.g., leveling, smoothing, texturing via Perlin). Updates are shown in real-time.

### Texture-Editor Submodule

- **Texture Editing**: Update textures by drawing directly onto the map with different brushes (smoothing, Perlin, regular color). Updates are shown in real-time.

### Map-Creator Submodule

- **Map Generation**: Generate new maps using algorithms like Perlin, Worley, or DiamondSquare. Customize parameters to create endless map variations. The generated maps can be instantly edited with all other tools, but need to be added to the .shn files manually. Apply erosion to smooth the entire map.

### General Features

- **File Selectors**: File selectors that choose a .nif file now preview those files, and the same applies to textures in the .dds format.
- **Scene Overview**: A general scene overview has been added where objects can be selected, and the camera gets teleported there. This feature can be toggled on or off.
- **Updated Graphics**: The tool's graphics have been updated to more closely resemble the actual game.
- **Shortcuts**: A variety of shortcuts have been added for easier use. Check the help window for a full list of shortcuts.
- **New Settings**: Includes options for auto-save timing, full-screen mode, and toggling SHMD objects visibility.

## Screenshots

### SHMD Editor

1. Middle Mouse Button Menu<br></br>
   ![Middle Mouse Button Menu](./Images/PopUp.jpg)

2. Folder Dialog<br></br>
   ![Folder Dialog](./Images/AddPickable.jpg)

3. Start Screen<br></br>
   ![Start Screen](./Images/StartScreen.jpg)

4. Translate Gizmo<br></br>
   ![Translate Gizmo](./Images/Translate.jpg)

5. Rotate Gizmo<br></br>
   ![Rotate Gizmo](./Images/Rotate.jpg)

### SHBD Editor

1. SHBD Editor Tool Part<br></br>
   ![SHBD Editor](./Images/SHBD.jpg)

### HTDG-Editor

1. Heightmap Editing<br></br>
   *(Add screenshots or videos here)* To be done

### Texture-Editor

1. Texture Editing<br></br>
   *(Add screenshots or videos here)* To be done

### Map-Creator

1. Map Generation<br></br>
   *(Add screenshots or videos here)* To be done

## Usage

To get started with the Fiesta Online Developer Tool, simply download the latest release from the Releases section of this repository. Once downloaded, run the executable file to launch the tool. You can then load .shmd or .shbd files, make edits, and save your changes as needed.

## License

This tool is provided under an open-source license. Please refer to the LICENSE file in this repository for more information.

## Acknowledgements

At this point, I want to shout out Maki for helping me a lot with understanding the fundamentals and providing me with some coding help for various parts of the code. I also want to shout out Spiike for doing a lot of alpha testing.

A big thank you to everyone else who contributed to developing and improving the tool. Your suggestions, bug reports, and testing have been invaluable. All feedback is welcome and greatly appreciated.

---

*Note: Some parts of this README were generated with a little help from an AI assistant.*

