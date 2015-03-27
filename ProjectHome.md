Example Visual Studio project using Chromium Embedded Framework (CEF) in a Microsoft Foundation Class (MFC) application.

The key concept of project is using the MFC messaging system to pass variable between CEF and MFC.


### Steps to compile project: ###

1. Set a Windows Environment Variable "CEF\_PATH" to the path for the CEF folder. For example if the path to the CEF binary is "C:\CEF\cef\_binary\_3.1650.1562\_windows32" set the variable as follows.

**CEF\_PATH** **C:\CEF\cef\_binary\_3.1650.1562\_windows32**

2. Add the CEF project "libcef\_dll\_wrapper" found in the CEF folder to the CEFMFC project.

3. You may need to change the "libcef\_dll\_wrapper" from 64 bit to 32 bit to match the CEFMFC project for both the Debug and Release configurations.

