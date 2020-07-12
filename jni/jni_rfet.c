/*
	jni_rfet.c

	Info at:
	(accessed 2017-06-06) http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html
	(accessed 2017-06-06) http://www.rgagnon.com/javadetails/java-0286.html
	(accessed 2017-06-06) https://developer.android.com/studio/command-line/logcat.html
	--------------------- <path-to-android-sdk-directory>/platform-tools/adb logcat
*/
#include <jni.h>
#include <_string.h>

#include <rfet.h>
#include <mouse.h>
#include <timer.h>
#include <tools.h>
#include <userinterface.h>

#define stack stackArray()

//--------------------------------------------------------------------------------------------

static wchar* buffer = NULL;

void userinterface_clean () { buffer = str2_free(buffer); }


static const wchar* fromJString (JNIEnv* env, jstring jstr)
{
	if(jstr==NULL) return NULL;
	const jchar* str = (*env)->GetStringChars(env, jstr, NULL);
	long i, len = (*env)->GetStringLength(env, jstr); // important step
	buffer = str2_alloc(buffer, len);
	if(buffer)
	{
		for(i=0; i<len; i++) buffer[i] = str[i];
		buffer[len]=0;
	}
	(*env)->ReleaseStringChars(env, jstr, str);
	return buffer;
}

static jstring toJString (JNIEnv* env, const wchar* str)
{
	if(!str) str = L"";
	long i, len = strlen2(str);
	jchar jstr[len+1];
	for(i=0; i<len; i++) jstr[i] = str[i];
	jstr[len]=0;
	return (*env)->NewString(env, jstr, len);
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
	jstring source_name)
{
	const wchar* input = fromJString(env, rfet_text);
	const wchar* source = fromJString(env, source_name);
	Str3 text = astrcpy32(C37(NULL), input, source);

	RFET trfet = toRFET(rfet);
	trfet = rfet_parse(stack, trfet, text); text=C37(NULL);
	return (jlong)(long)trfet;
}


jboolean Java_jni_RFET_evaluate ( JNIEnv* env, jobject thiz, jlong rfet, jdouble argument )
{ return !VERROR(rfet_evaluate(stack, toRFET(rfet), NULL)); }

jboolean Java_jni_RFET_commitReplacement ( JNIEnv* env, jobject thiz, jlong rfet )
{ return rfet_commit_replacement(stack, toRFET(rfet)); }

jstring Java_jni_RFET_getContainerText ( JNIEnv* env, jobject thiz, jlong rfet )
{
	value v = rfet_get_container_text(stack, toRFET(rfet));
	return toJString(env, getStr2(vGetPrev(v)));
}

jstring Java_jni_RFET_getResultString ( JNIEnv* env, jobject thiz, jlong rfet )
{
	value v = VstToStr(vnext(stack), PUT_NEWLINE|0, -1, -1);
	return toJString(env, getStr2(vGetPrev(v)));
}

jstring Java_jni_RFET_getErrorMessage ( JNIEnv* env, jobject thiz )
{ return toJString(env, getMessage(vGet(stack))); }

void Java_jni_RFET_remove ( JNIEnv* env, jobject thiz, jlong rfet )
{ rfet_remove(stack, toRFET(rfet)); }


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
bool main_window_resize() { return false; }
const wchar* get_file_name() { return NULL; }
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

