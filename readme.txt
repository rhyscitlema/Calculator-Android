
Software name:
    Rhyscitlema Calculator

Software description:
    Evaluate complex mathematical
    expressions using the RFET language.


To build the project using Linux Command Line Interface:

1- Get-everything-ready, by following the steps at:
   http://rhyscitlema.com/applications/on-android-os

2- Open the Command Line Interface Terminal
3- Go to ./jni folder to get "librfet_jni".
   Refer to the readme.txt file there.

4- Change directory to this directory
5- Go into file 'local.properties' and set
   sdk.dir=<path-to-android-sdk-directory>

6- Execute: ant debug


To load to device, execute:
  <path-to-android-sdk-directory>/platform-tools/adb \
  install -r ./bin/Rhyscitlema_Calculator-debug.apk


Provided by Rhyscitlema
@ http://rhyscitlema.com

USE AT YOUR OWN RISK!
