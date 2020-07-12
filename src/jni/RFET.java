/*
    RFET.java
*/
package jni;

public class RFET
{
    /* Native methods that are implemented by
     * the rfet_jni.c - java native library.
     */
    public static native void initialise ();
    public static native void clean ();

    public static native long parse (long rfet,
                                     String rfet_text,
                                     String source_name,
                                     int start_line,
                                     int start_column);

    public static native boolean evaluate (long rfet, double argument);
    public static native boolean commitReplacement (long rfet);
    public static native String getContainerText (long rfet);
    public static native String getResultString (long rfet);
    public static native String getErrorMessage ();
    public static native void remove (long rfet);

    public static native void doEval ();
    public static native void doPause ();
    public static native void doForward ();
    public static native void lowerPeriod ();
    public static native void higherPeriod ();
    public static native void setTime ();

    public static native boolean timerHandlerDo ();
    public static native void onKeyEvent (int key, boolean pressed);

    static { System.loadLibrary("rfet_jni"); }
}

