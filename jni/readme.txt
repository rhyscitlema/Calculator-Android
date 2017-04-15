
Library name:
    Rhyscitlema MFET library for Android OS

Library description:
    Evaluate any mathematical expression.


To compile using Linux Command Line Interface:

1- First download the library repositories:
   * lib_std
   * libmfet
   * librodt
   * algorithms

   which are also found at:
   https://github.com/rhyscitlema

   Put them in the parent directory of
   this "MFET-Calculator-Android" project folder,
   so they will become sibling folders.

2- Open the Command Line Interface Terminal.
3- Change directory to this directory.
4- Execute: <path-to-android-ndk-directory>/ndk-build


Example compiler output:
[x86] Compile        : mfet_jni <= mfet_jni.c
[x86] Compile        : mfet_jni <= lib_std.c
[x86] Compile        : mfet_jni <= libmfet.c
[x86] Compile        : mfet_jni <= librodt.c
[x86] Compile        : mfet_jni <= algorithms.c
[x86] SharedLibrary  : libmfet_jni.so
[x86] Install        : libmfet_jni.so => libs/x86/libmfet_jni.so


Provided by Rhyscitlema
@ http://rhyscitlema.com

USE AT YOUR OWN RISK!

