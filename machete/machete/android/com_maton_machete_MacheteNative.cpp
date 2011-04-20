
#include "GL2.h"
#include "com_maton_machete_MacheteNative.h"
#include "../engine.h"
#include "ResourceManager.h"

static IResourceManager* rm;
static IRenderingEngine* renderer;
static IGame* game;

static void* opengl_es_2_handle;

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    initialize
 * Signature: (Ljava/lang/String;II)V
 */
void Java_com_maton_machete_MacheteNative_initialize
  (JNIEnv *env, jobject, jstring apk, jint w, jint h, jint o, jint osr) {
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apk, &isCopy);

	rm = new AndroidResourceManager(str);

	if (opengl_es_2_handle != NULL) {
		renderer = CreateRendererAndroid1(rm);
	} else {
		renderer = CreateRendererAndroid1(rm);
	}

	game = CreateGame(renderer);
	game->Initialize(w, h, DeviceOrientationPortrait);
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    resize
 * Signature: (III)V
 */
void Java_com_maton_machete_MacheteNative_resize
  (JNIEnv *, jobject, jint w, jint h, jint o) {
	if (game == NULL) return;
	game->Resize(w, h, DeviceOrientationPortrait);
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    render
 * Signature: ()V
 */
void Java_com_maton_machete_MacheteNative_render
  (JNIEnv *, jobject) {
	if (game == NULL) return;
	game->Render();
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    pause
 * Signature: ()V
 */
void Java_com_maton_machete_MacheteNative_pause
  (JNIEnv *, jobject) {
	if (game == NULL) return;
	game->OnPause();
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    update
 * Signature: (F)V
 */
void Java_com_maton_machete_MacheteNative_update
  (JNIEnv *, jobject, jfloat time) {
	if (game == NULL) return;
	game->Update(time);
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    resume
 * Signature: ()V
 */
void Java_com_maton_machete_MacheteNative_resume
  (JNIEnv *, jobject) {
	if (game == NULL) return;
	game->OnResume();
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    start
 * Signature: ()V
 */
void Java_com_maton_machete_MacheteNative_start
  (JNIEnv *, jobject) {
	if (game == NULL) return;
	game->OnStart();
}

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    stop
 * Signature: ()V
 */
void Java_com_maton_machete_MacheteNative_stop
  (JNIEnv *, jobject) {
	if (opengl_es_2_handle != NULL) {
		dlclose(opengl_es_2_handle);
	}

	if (game == NULL) return;
	game->OnStop();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {

}
