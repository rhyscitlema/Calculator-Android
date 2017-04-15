/*
    jni_mfet.c

    Info at:
    (accessed 2017-06-06) http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html
    (accessed 2017-06-06) http://www.rgagnon.com/javadetails/java-0286.html
    (accessed 2017-06-06) https://developer.android.com/studio/command-line/logcat.html
*/
#include <jni.h>
#include <_string.h>

#include <mfet.h>
#include <timer.h>
#include <tools.h>
#include <userinterface.h>


//--------------------------------------------------------------------------------------------

static mchar* buffer =  NULL;

void userinterface_clean () { mchar_free(buffer); buffer=NULL; }


static const mchar* fromJString (JNIEnv* env, jstring jstr)
{
    if(jstr==NULL) return NULL;
    const jchar* nativeString = (*env)->GetStringChars(env, jstr, NULL);
    long length = (*env)->GetStringLength(env, jstr); // important step
    astrcpy22S(&buffer, nativeString, length);
    (*env)->ReleaseStringChars(env, jstr, nativeString);
    return buffer;
}

static jstring toJString (JNIEnv* env, const mchar* str)
{
    if(!str) str = (const mchar*)"\0\0";
    return (*env)->NewString(env, str, strlen2(str));
}


//--------------------------------------------------------------------------------------------

static JNIEnv *g_env = NULL;

void Java_jni_MFET_initialise ( JNIEnv* env, jobject thiz)
{
    g_env = env;
    timer_install_do();
    tools_init(100000, NULL);
    userinterface_update();
}

void Java_jni_MFET_clean ( JNIEnv* env, jobject thiz)
{
    g_env = env;
    tools_clean();
    g_env = NULL;
}


//--------------------------------------------------------------------------------------------

#define toMFET(mfet) ((MFET)(long)(mfet))

jlong Java_jni_MFET_parse (
    JNIEnv* env,
    jobject thiz,
    jlong mfet,
    jstring mfet_text,
    jstring source_name,
    jint start_line,
    jint start_column )
{
    const mchar* mstr;

    mstr = fromJString(env, mfet_text);
    lchar* text=NULL; astrcpy32(&text, mstr);

    mstr = fromJString(env, source_name);
    set_line_coln_file(text, start_line, start_column, mstr);

    MFET tmfet = toMFET(mfet);
    tmfet = mfet_parse(tmfet, text); text=NULL;
    return (jlong)(long)tmfet;
}


jboolean Java_jni_MFET_evaluate ( JNIEnv* env, jobject thiz, jlong mfet, jdouble argument )
{ return mfet_evaluate(toMFET(mfet), NULL, NULL); }

jboolean Java_jni_MFET_commitReplacement ( JNIEnv* env, jobject thiz, jlong mfet )
{ return mfet_commit_replacement(toMFET(mfet)); }

jstring Java_jni_MFET_getContainerText ( JNIEnv* env, jobject thiz, jlong mfet )
{ return toJString(env, mfet_get_container_text(toMFET(mfet))); }

jstring Java_jni_MFET_getResultString ( JNIEnv* env, jobject thiz, jlong mfet )
{
    VstToStr(mainStack(), errorMessage(), 4, -1, -1, -1);
    return toJString(env, errorMessage());
}

jstring Java_jni_MFET_getErrorMessage ( JNIEnv* env, jobject thiz )
{ return toJString(env, errorMessage()); }

void Java_jni_MFET_remove ( JNIEnv* env, jobject thiz, jlong mfet )
{ mfet_remove(toMFET(mfet)); }


//-------------------------------------------------------------------------------------------

void Java_jni_MFET_doEval       ( JNIEnv* env, jobject thiz ) { tools_do_eval(NULL); }
void Java_jni_MFET_doPause      ( JNIEnv* env, jobject thiz ) { tools_do_pause(!timer_paused()); }
void Java_jni_MFET_doForward    ( JNIEnv* env, jobject thiz ) { tools_go_forward(timer_get_period()<0); }
void Java_jni_MFET_lowerPeriod  ( JNIEnv* env, jobject thiz ) { tools_lower_period(); }
void Java_jni_MFET_higherPeriod ( JNIEnv* env, jobject thiz ) { tools_higher_period(); }
void Java_jni_MFET_setTime      ( JNIEnv* env, jobject thiz ) { tools_set_time(NULL); }

jboolean Java_jni_MFET_timerHandlerDo( JNIEnv* env, jobject thiz )
{ return timer_handler_do(); }

void Java_jni_MFET_onKeyEvent ( JNIEnv* env, jobject thiz, jint key, jboolean pressed )
{ on_key_event(key, pressed); }


//-------------------------------------------------------------------------------------------

//---- not used ----
int  main_window_width = 0;
int  main_window_height = 0;
bool main_window_resize () {}
bool file_exists_get () { return false; }
const mchar* file_name_get () { return NULL; }
//---- not used ----


#define CLAZZ_NAME "com/rhyscitlemacalculator/Rhyscitlema_Calculator"

const mchar* userinterface_get_text (enum UI_ITEM ui_item)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "ui_get_text", "(I)Ljava/lang/String;");
    jstring jstr = (jstring) (*env)->CallStaticObjectMethod(env, clazz, method, ui_item);
    return fromJString(env, jstr);
}

void userinterface_set_text (enum UI_ITEM ui_item, const mchar* text)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "ui_set_text", "(ILjava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, clazz, method, ui_item, toJString(env, text));
}


void wait_for_user_first   (const mchar* title, const mchar* message)
{
    JNIEnv *env = g_env;
    jclass clazz = (*env)->FindClass(env, CLAZZ_NAME);
    jmethodID method = (*env)->GetStaticMethodID(env, clazz, "wait_for_user_first", "(Ljava/lang/String;Ljava/lang/String;)V");
    (*env)->CallStaticVoidMethod(env, clazz, method, toJString(env, title), toJString(env, message));
}

bool wait_for_confirmation (const mchar* title, const mchar* message)
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

