Platformer
==========

A simple platformer written in C++ using Unreal Engine 4

Be sure to compile the C++ from either the Unreal editor or Visual Studio/XCode

###Adding gtest framework to XCode Project
* 1. Right click Frameworks and click "add files to platformer". Add the gtest.framework.
* 2. Create a new Command Line Target named "UnitTests", add the gtest.framework as a dependency
* 3. Copy the main.cpp of UnitTests into the main.cpp of this new target
* 4. Compile and Run, You may get an error similar to 
* "dyld: Library not loaded: /Library/Frameworks/gtest.framework/Versions/A/gtest
*   Referenced from: /Users/adamwyeth/Documents/cs220/Platformer/Intermediate/Build/DebugGame/UnitTests
*   Reason: image not found"
* If you add gtest to the location where the Library could not be loaded, "Library/Frameworks", then 
* it should run
