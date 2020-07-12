/*
    jni_rfet.c

    Info at:
    (accessed 2017-06-06) http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html
    (accessed 2017-06-06) http://www.rgagnon.com/javadetails/java-0286.html
    (accessed 2017-06-06) https://developer.android.com/studio/command-line/logcat.html
                          <path-to-android-sdk-directory>/platform-tools/adb logcat
*/
#include <jni.h>
#include <_string.h>

#include <rfet.h>
#include <mouse.h>
#include <timer.h>
#include <tools.h>
#include <userinterface.h>


//--------------------------------------------------------------------------------------------

static wchar* buffer =  NULL;

void userinterface_clean () { wchar_free(buffer); buffer=NULL; }


static const wchar* fromJString (JNIEnv* env, jstring jstr)
{
    if(jstr==NULL) return NULL;
    const jchar* nativeString = (*env)->GetStringChars(env, jstr, NULL);
    long length = (*env)->GetStringLength(env, jstr); // important step
    astrcpy22S(&buffer, (const wchar*)nativeString, length);
    (*env)->ReleaseStringChars(env, jstr, nativeString);
    return buffer;
}

static jstring toJString (JNIEnv* env, const wchar* str)
{
    if(!str) str = (const wchar*)"\0\0";
    return (*env)->NewString(env, (const jchar*)str, strlen2(str));
}


//--------------------------------------------------------------------------------------------

static JNIEnv *g_env = NULL;

void Java_jni_RFET_initialise ( JNIEnv* env, jobject thiz)
{
    g_env = env;
    timer_install_do();
    tools_init(100000, NULL);
    userinterface_update();
}

void Java_jni_RFET_clean ( JNIEnv* env, jobject thiz)
{
    g_env = env;
    tools_clean();
    g_env = NULL;
}


//--------------------------------------------------------------------------------------------

#define toRFET(rfet) ((RFET)(long)(rfet))

jlong Java_jni_RFET_parse (
    JNIEnv* env,
    jobject thiz,
    jlong rfet,
    jstring rfet_text,
    jstring source_name,
    jint start_line,
    jint start_column )
{
    const wchar* mstr;

    mstr = fromJString(env, rfet_text);
    lchar* text=NULL; astrcpy32(&text, mstr);

    mstr = fromJString(env, source_name);
    set_line_coln_source(text, start_line, start_column, mstr);

    RFET trfet = toRFET(rfet);
    trfet = rfet_parse(trfet, text); text=NULL;
    return (jlong)(long)trfet;
}


jboolean Java_jni_RFET_evaluate ( JNIEnv* env, jobject thiz, jlong rfet, jdouble argument )
{ return rfet_evaluate(toRFET(rfet), NULL, NULL); }

jboolean Java_jni_RFET_commitReplacement ( JNIEnv* env, jobject thiz, jlong rfet )
{ return rfet_commit_replacement(toRFET(rfet)); }

jstring Java_jni_RFET_getContainerText ( JNIEnv* env, jobject thiz, jlong rfet )
{ return toJString(env, rfet_get_container_text(toRFET(rfet))); }

jstring Java_jni_RFET_getResultString ( JNIEnv* env, jobject thiz, jlong rfet )
{
    VstToStr(mainStack(), errorMessage(), 4, -1, -1, -1);
    return toJString(env, errorMessage());
}

jstring Java_jni_RFET_getErrorMessage ( JNIEnv* env, jobject thiz )
{ return toJString(env, errorMessage()); }

void Java_jni_RFET_remove ( JNIEnv* env, jobject thiz, jlong rfet )
{ rfet_remove(toRFET(rfet)); }


//-------------------------------------------------------------------------------------------

void Java_jni_RFET_doEval       ( JNIEnv* env, jobject thiz ) { tools_do_eval(NULL); }
void Java_jni_RFET_doPause      ( JNIEnv* env, jobject thiz ) { tools_do_pause(!timer_paused()); }
void Java_jni_RFET_doForward    ( JNIEnv* env, jobject thiz ) { tools_go_forward(timer_get_period()<0); }
void Java_jni_RFET_lowerPeriod  ( JNIEnv* env, jobject thiz ) { tools_lower_period(); }
void Java_jni_RFET_higherPeriod ( JNIEnv* env, jobject thiz ) { tools_higher_period(); }
void Java_jni_RFET_setTime      ( JNIEnv* env, jobject thiz ) { tools_set_time(NULL); }

jboolean Java_jni_RFET_timerHandlerDo( JNIEnv* env, jobject thiz )
{ return timer_handler_do(); }

void Java_jni_RFET_onKeyEvent ( JNIEnv* env, jobject thiz, jint key, jboolean pressed )
{ on_key_event(key, pressed); }


//-------------------------------------------------------------------------------------------

//---- not used ----
int  main_window_width = 0;
int  main_window_height = 0;
bool main_window_resize () { return false; }
bool file_exists_get () { return false; }
const wchar* file_name_get () { return NULL; }
//---- not used ----


#define CLAZZ_NAME "com/rhyscitlemacalculator/Rhyscitlema_Calculator"

const wchar* userinterface_get_text (enum UI_ITEM ui_item)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "ui_get_text", "(I)Ljava/lang/String;");
    jstring jstr = (jstring) (*env)->CallStaticObjectMethod(env, clazz, method, ui_item);
    return fromJString(env, jstr);
}

void userinterface_set_text (enum UI_ITEM ui_item, const wchar* text)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "ui_set_text", "(ILjava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, clazz, method, ui_item, toJString(env, text));
}


void wait_for_user_first   (const wchar* title, const wchar* message)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "wait_for_user_first", "(Ljava/lang/String;Ljava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, clazz, method, toJString(env, title), toJString(env, message));
}

bool wait_for_confirmation (const wchar* title, const wchar* message)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "wait_for_confirmation", "(Ljava/lang/String;Ljava/lang/String;)Z");
    return (*env)->CallStaticBooleanMethod(env, clazz, method, toJString(env, title), toJString(env, message));
}


void timer_pause_do ()
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "timer_pause_do", "()V");
    (*env)->CallStaticVoidMethod(env, clazz, method);
}

void timer_set_period_do (int period)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "timer_set_period_do", "(I)V");
    (*env)->CallStaticVoidMethod(env, clazz, method, period);
}

