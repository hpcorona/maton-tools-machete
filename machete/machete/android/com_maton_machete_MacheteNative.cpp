
#include "com_maton_machete_MacheteNative.h"
#include "../engine.h"
#include "AndroidPlatform.h"

AndroidPlatform* platform;
static machete::IGame* game;
machete::graphics::DrawContext* renEngine;
int width, height;

/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    initialize
 * Signature: (Ljava/lang/String;II)V
 */
void Java_com_maton_machete_MacheteNative_initialize
  (JNIEnv *env, jobject, jstring name, jstring apk, jint w, jint h, jint o, jint osr) {
	const char* str;
  const char* cname;
	jboolean isCopy;
	str = env->GetStringUTFChars(apk, &isCopy);
  cname = env->GetStringUTFChars(name, &isCopy);

	platform = new AndroidPlatform(str, cname);

  machete::Start(platform);

  renEngine = CreateDrawContext(machete::graphics::TargetScreen, w, h);

  width = w;
  height = h;
}

void JNICALL Java_com_maton_machete_MacheteNative_startup(JNIEnv *, jobject) {
	game = CreateGame();
	game->Initialize(renEngine, width, height, machete::DeviceOrientationPortrait);
}

void JNICALL Java_com_maton_machete_MacheteNative_resourceOffset(JNIEnv *env, jobject, jstring resource, jlong offset, jlong length) {
  jboolean isCopy;
  const char* name = env->GetStringUTFChars(resource, &isCopy);

  platform->SetResourceData(name, offset, length);
}
/*
 * Class:     com_maton_machete_MacheteNative
 * Method:    resize
 * Signature: (III)V
 */
void Java_com_maton_machete_MacheteNative_resize
  (JNIEnv *, jobject, jint w, jint h, jint o) {
	if (game == NULL) return;
	game->Resize(w, h, machete::DeviceOrientationPortrait);
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
	if (game == NULL) return;
	game->OnStop();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {

}

void JNICALL Java_com_maton_machete_MacheteNative_touch(JNIEnv *vm, jobject, jint id, jint event, jfloat x, jfloat y) {

  machete::input::TheTouchInput->SetTouchCount(1);
  machete::input::Touch *t = machete::input::TheTouchInput->GetTouch(0);

  if (event != machete::input::TouchStart) {
    t->previous = t->current;

    t->current.x = x;
    t->current.y = y;
  } else {
    t->current.x = x;
    t->current.y = y;

    t->previous = t->current;
    t->start = t->current;

    t->owner = NULL;
  }

  t->offset = t->current - t->previous;
  t->finger = id;
  t->phase = (machete::input::TouchPhase)event;

  machete::input::TheTouchInput->MarkAvailable();
}

