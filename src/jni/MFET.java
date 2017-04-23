/*
    MFET.java
*/
package jni;

public class MFET
{
    /* Native methods that are implemented by
     * the mfet_jni.c - java native library.
     */
    public static native void initialise ();
    public static native void clean ();

    public static native long parse (long mfet,
                                     String mfet_text,
                                     String source_name,
                                     int start_line,
                                     int start_column);

    public static native boolean evaluate (long mfet, double argument);
    public static native boolean commitReplacement (long mfet);
    public static native String getContainerText (long mfet);
    public static native String getResultString (long mfet);
    public static native String getErrorMessage ();
    public static native void remove (long mfet);

    public static native void doEval ();
    public static native void doPause ();
    public static native void doForward ();
    public static native void lowerPeriod ();
    public static native void higherPeriod ();
    public static native void setTime ();

    public static native boolean timerHandlerDo ();
    public static native void onKeyEvent (int key, boolean pressed);

    static { System.loadLibrary("mfet_jni"); }
}

