# Installation Guide
 
This guide will walk you through the necessary steps for setting up your environment and to build the Fiesta Online Developer Tool.
 
## Prerequisites
 
### Visual Studio 2022
 
To build the Fiesta Online Developer Tool, you need to have Visual Studio 2022 installed with the following features:
 
1. **C++ Development Tools**: Ensure that you install the C++ development tools during the Visual Studio setup process.
2. **MSVC v143**: The tool requires the MSVC v143 compiler to be installed. This can be selected during the Visual Studio installation or added later through the Visual Studio Installer.
 
Everything else should be included in the repository and all paths should adjust on their own for the linker.
 
### Debug Builds
 
If you wish to create a debug build of the tool, you will also need to install **Visual Studio 2005**. You can find the installer for Visual Studio 2005 in this repository under the following path:
 
- [Visual Studio 2005 Installer](./Images/en_vc_2005_express.iso)
 
## Building the Tool
 
1. **Clone the Repository**: Clone the repository to your local machine using Git.
 
    ```bash
    git clone https://github.com/Setnr/Fiesta-Online-Developer-Tool.git
    cd fiesta-online-dev-tool
    ```
 
2. **Open the Solution File**: Navigate to the directory where the repository was cloned and open the solution file (`.sln`) in Visual Studio 2022.
 
3. **Set Build Configuration**: Choose the desired build configuration (e.g., `ShippingLib` or `DebugLib` for static linking or `Shipping` or `Debug` for dynamic linking) from the drop-down menu in Visual Studio.
 
4. **Build the Solution**: Build the solution by selecting **Build** > **Build Solution** from the top menu in Visual Studio, or by pressing `Ctrl + Shift + B`.
 
5. **Start the Tool** You should now see a build folder containing a subfolder called the same as the build configuration you have chosen. To start the tool, just run the executable inside it. If you want to move the tool to a different folder you must also copy the `FiestaOnlineTool` folder.
 
## Notes
 
- Ensure that all necessary dependencies and libraries are installed for the tool to build successfully.
- For debug builds, make sure that Visual Studio 2005 is properly installed and configured before attempting to build.
 
With these steps, you should be able to set up your environment and build the Fiesta Online Developer Tool successfully. If you encounter any issues, feel free to check the [README](./README.md) or seek help from the community.