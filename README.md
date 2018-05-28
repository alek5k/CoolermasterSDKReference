# Coolermaster SDK Reference Project

This project is a reconfiguration of the Coolermaster SDK sample project which has been updated to work with Visual Studio 2017. 

This reference project should let you get up and running with the Coolermaster SDK very quickly in VS2017 - all you have to do is open the project and hit build.


## Changes from reference SDK:

- Updated to use Visual Studio 2017 (Tested on Professional and Community)
- Added in lib files (SDKDLL.lib and SDKDLL64.lib) for proper linker configuration
- Added Coolermaster SDK external DLLs (SDKDLL.dll and SDKDLL64.dll) into the build process, and added post-build instructions to automatically copy the DLLs to the Debug/Release directories
- Reconfigured the project to use Windows 10 SDK
- Trimmed out all comments that weren't in English causing strange characters to appear in Visual Studio


# Sharing Programs

If you find this reference project configuration helpful, consider making a pull request to this repo with a link to your project in the readme file.