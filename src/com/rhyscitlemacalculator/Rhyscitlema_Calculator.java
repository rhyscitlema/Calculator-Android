/*
    Rhyscitlema_Calculator.java
*/
package com.rhyscitlemacalculator;

import jni.MFET;

import android.app.Activity;
import android.os.Bundle;

import android.os.Handler;
import java.lang.Runnable;

import android.view.View;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.view.View.OnTouchListener;
import android.content.Context;
import android.os.Message;
import android.os.Looper;

import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;


public class Rhyscitlema_Calculator extends Activity
{
    // the order below MUST follow that of userinterface.h
    public static final int UI_main_text       = 0;
    public static final int UI_mesg_text       = 1;
    public static final int UI_path_text       = 2;
    public static final int UI_time_text       = 3;
    public static final int UI_pause_button    = 4;
    public static final int UI_forward_button  = 5;

    private static EditText main_text;
    private static EditText mesg_text;
    private static EditText time_text;
    private static Button pause_button;
    private static Button forward_button;

    private static Context context;


    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        main_text = (EditText)findViewById(R.id.gui_main_text);
        mesg_text = (EditText)findViewById(R.id.gui_mesg_text);
        time_text = (EditText)findViewById(R.id.gui_time_text);
        pause_button = (Button)findViewById(R.id.gui_pause_button);
        forward_button = (Button)findViewById(R.id.gui_forward_button);

        main_text.setOnEditorActionListener(new OnEditorActionListener(){
            @Override
            public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
                if(actionId == EditorInfo.IME_ACTION_DONE)
                    onButton_Eval(view);
                return false;
            }
        });

        time_text.setOnEditorActionListener(new OnEditorActionListener(){
            @Override
            public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
                if(actionId == EditorInfo.IME_ACTION_DONE)
                    MFET.setTime();
                return false;
            }
        });

        context = this;
        main_text.requestFocus();
        MFET.initialise();
    }

    @Override
    public void onDestroy() {
        MFET.clean();
        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        moveTaskToBack(true);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int action = event.getAction();
        boolean pressed = (action == KeyEvent.ACTION_DOWN);
        if(pressed || action == KeyEvent.ACTION_UP)
        {
            int key = ANDROID_KEY_TO_MFET_KEY(event.getKeyCode(), event.isShiftPressed());
            if(key!=0) MFET.onKeyEvent(key, pressed);
        }
        return super.dispatchKeyEvent(event);
    }

    public void onButton_Eval (View view) { MFET.doEval(); }
    public void onButton_Lock (View view) { findViewById(R.id.gui_main_view).requestFocus(); }
    public void onButton_Pause (View view) { MFET.doPause(); }
    public void onButton_Forward (View view) { MFET.doForward(); }
    public void onButton_LowerP (View view) { MFET.lowerPeriod(); }
    public void onButton_HigherP (View view) { MFET.higherPeriod(); }


    public static String ui_get_text (int ui_item) {
        switch(ui_item) {
            case UI_main_text: return main_text.getText().toString();
            case UI_time_text: return time_text.getText().toString();
            default: return "";
        }
    }
    public static void ui_set_text (int ui_item, String text) {
        switch(ui_item) {
            case UI_main_text: main_text.setText(text); break;
            case UI_mesg_text: mesg_text.setText(text); break;
            case UI_time_text: time_text.setText(text); break;
            case UI_pause_button: pause_button.setText(text); break;
            case UI_forward_button: forward_button.setText(text); break;
        }
    }


    private static int interval;
    private static final Handler handler = new Handler();
    private static final Runnable runnable = new Runnable() {
        public void run() {
            handler.postDelayed(this, interval);
            if(!MFET.timerHandlerDo()) timer_pause_do();
        }
    };
    public static void timer_pause_do () {
        handler.removeCallbacks(runnable);
    }
    public static void timer_set_period_do (int period) {
        interval = period;
        handler.postDelayed(runnable, interval);
    }


    public static void wait_for_user_first (String title, String message)
    {
        AlertDialog alertDialog = new AlertDialog.Builder(context).create();
        alertDialog.setTitle(title);
        alertDialog.setMessage(message);
        alertDialog.setButton("OK", new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog, int which) {}
        });
        alertDialog.show();
    }

    static boolean response;
    public static boolean wait_for_confirmation (String title, String message)
    {
        // http://stackoverflow.com/a/10358260
        // make a handler that throws a runtime exception when a message is received
        final Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) { throw new RuntimeException(); }
        };
        response = false;
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setCancelable(false);
        builder.setTitle(title);
        builder.setMessage(message);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog, int which) {
                response=true; handler.sendMessage(handler.obtainMessage());
            }});
        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog, int which) {
                response=false; handler.sendMessage(handler.obtainMessage());
            }});
        builder.create().show();

        // loop till a runtime exception is triggered.
        try { Looper.loop(); }
        catch(RuntimeException e2) {}

        return response;
    }


    public static final int ANDROID_KEY_TO_MFET_KEY (int keyCode, boolean shift)
    {
        switch(keyCode)
        {
      //case KeyEvent.KEYCODE_SPACE: return ' ';
      //case KeyEvent.KEYCODE_ENTER: return '\n';
      //case KeyEvent.KEYCODE_DEL:   return 0x8; // backspace

      /*case KeyEvent.KEYCODE_0: return '0';
        case KeyEvent.KEYCODE_1: return '1';
        case KeyEvent.KEYCODE_2: return '2';
        case KeyEvent.KEYCODE_3: return '3';
        case KeyEvent.KEYCODE_4: return '4';
        case KeyEvent.KEYCODE_5: return '5';
        case KeyEvent.KEYCODE_6: return '6';
        case KeyEvent.KEYCODE_7: return '7';
        case KeyEvent.KEYCODE_8: return '8';
        case KeyEvent.KEYCODE_9: return '9';*/

        case KeyEvent.KEYCODE_A: return shift ? 'A' : 'a';
        case KeyEvent.KEYCODE_B: return shift ? 'B' : 'b';
        case KeyEvent.KEYCODE_C: return shift ? 'C' : 'c';
        case KeyEvent.KEYCODE_D: return shift ? 'D' : 'd';
        case KeyEvent.KEYCODE_E: return shift ? 'E' : 'e';
        case KeyEvent.KEYCODE_F: return shift ? 'F' : 'f';
        case KeyEvent.KEYCODE_G: return shift ? 'G' : 'g';
        case KeyEvent.KEYCODE_H: return shift ? 'H' : 'h';
        case KeyEvent.KEYCODE_I: return shift ? 'I' : 'i';
        case KeyEvent.KEYCODE_J: return shift ? 'J' : 'j';
        case KeyEvent.KEYCODE_K: return shift ? 'K' : 'k';
        case KeyEvent.KEYCODE_L: return shift ? 'L' : 'l';
        case KeyEvent.KEYCODE_M: return shift ? 'M' : 'm';
        case KeyEvent.KEYCODE_N: return shift ? 'N' : 'n';
        case KeyEvent.KEYCODE_O: return shift ? 'O' : 'o';
        case KeyEvent.KEYCODE_P: return shift ? 'P' : 'p';
        case KeyEvent.KEYCODE_Q: return shift ? 'Q' : 'q';
        case KeyEvent.KEYCODE_R: return shift ? 'R' : 'r';
        case KeyEvent.KEYCODE_S: return shift ? 'S' : 's';
        case KeyEvent.KEYCODE_T: return shift ? 'T' : 't';
        case KeyEvent.KEYCODE_U: return shift ? 'U' : 'u';
        case KeyEvent.KEYCODE_V: return shift ? 'V' : 'v';
        case KeyEvent.KEYCODE_W: return shift ? 'W' : 'w';
        case KeyEvent.KEYCODE_X: return shift ? 'X' : 'x';
        case KeyEvent.KEYCODE_Y: return shift ? 'Y' : 'y';
        case KeyEvent.KEYCODE_Z: return shift ? 'Z' : 'z';

        default: return 0;
        }
    }
}

