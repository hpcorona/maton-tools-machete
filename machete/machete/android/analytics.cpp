
#include "../analytics/analytics.h"
#include "../common/log.h"
#include "utils.h"

namespace machete {
  namespace analytics {

    static jclass classGANTracker = NULL;
    static jmethodID methodGetInstance;
    static jmethodID methodStartNewSession;
    static jmethodID methodTrackEvent;
    static jmethodID methodTrackPageview;
    static jmethodID methodDispatch;
    static jmethodID methodStopTracker;
    static jmethodID methodSetCustomVar;
    static jmethodID methodSetAnonymizeIp;
    static jmethodID methodSetSampleRate;
    static jmethodID methodSetReferrer;

    GoogleTracker::GoogleTracker() {
      if (classGANTracker == NULL) {

        JNIEnv* env = GetEnv();

        classGANTracker = env->FindClass("com/maton/machete/GANTracker");
        if (!classGANTracker) {
          machete::common::Log("Cannot load class GoogleAnalyticsTracker");
          return;
        }

        methodStartNewSession = env->GetStaticMethodID(classGANTracker, "startNewSession", "(Ljava/lang/String;I)V");
        methodTrackEvent = env->GetStaticMethodID(classGANTracker, "trackEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
        methodTrackPageview = env->GetStaticMethodID(classGANTracker, "trackPageview", "(Ljava/lang/String;)V");
        methodDispatch = env->GetStaticMethodID(classGANTracker, "dispatch", "()V");
        methodStopTracker = env->GetStaticMethodID(classGANTracker, "stopSession", "()V");
        methodSetCustomVar = env->GetStaticMethodID(classGANTracker, "setCustomVar", "(ILjava/lang/String;Ljava/lang/String;)V");
        methodSetAnonymizeIp = env->GetStaticMethodID(classGANTracker, "setAnonymizeIp", "(Z)V");
        methodSetSampleRate = env->GetStaticMethodID(classGANTracker, "setSampleRate", "(I)V");
        methodSetReferrer = env->GetStaticMethodID(classGANTracker, "setReferrer", "(Ljava/lang/String;)V");
      }

      started = false;
    }

    GoogleTracker::~GoogleTracker() {
      if (started == true) {
        Stop();
      }
    }

    void GoogleTracker::Start(Str accountId, int dispatchPeriod) {
      if (started == true) {
        Stop();
      }

      char accountIdChars[20];
      accountId.GetChars(accountIdChars, 20);

      JNIEnv* env = GetEnv();

      jstring accId = env->NewStringUTF(accountIdChars);

      env->CallStaticVoidMethod(classGANTracker, methodStartNewSession, accId, dispatchPeriod, TheActivity);
      env->CallStaticVoidMethod(classGANTracker, methodSetSampleRate, 100);
      env->CallStaticVoidMethod(classGANTracker, methodSetAnonymizeIp, true);

      started = true;
    }

    void GoogleTracker::SetCustomVariable(int idx, Str name, Str value) {
      char nameChars[100];
      char valueChars[100];
      jstring nsName;
      jstring nsValue;

      name.GetChars(nameChars, 100);
      value.GetChars(valueChars, 100);

      JNIEnv* env = GetEnv();

      nsName = env->NewStringUTF(nameChars);
      nsValue = env->NewStringUTF(valueChars);

      env->CallStaticBooleanMethod(classGANTracker, methodSetCustomVar, idx, nsName, nsValue);
    }

    void GoogleTracker::TrackEvent(Str category, Str action, Str label, int value) {
      char categoryChars[100];
      char actionChars[100];
      char labelChars[100];

      jstring nsCategory;
      jstring nsAction;
      jstring nsLabel;

      category.GetChars(categoryChars, 100);
      action.GetChars(actionChars, 100);
      label.GetChars(labelChars, 100);

      JNIEnv* env = GetEnv();

      nsCategory = env->NewStringUTF(categoryChars);
      nsAction = env->NewStringUTF(actionChars);
      nsLabel = env->NewStringUTF(labelChars);

      env->CallStaticVoidMethod(classGANTracker, methodTrackEvent, nsCategory, nsAction, nsLabel, value);
    }

    void GoogleTracker::TrackPageview(Str page) {
      char pageView[100];

      jstring nsPageview;

      page.GetChars(pageView, 100);

      JNIEnv* env = GetEnv();

      nsPageview = env->NewStringUTF(pageView);

      env->CallStaticVoidMethod(classGANTracker, methodTrackPageview, nsPageview);
    }

    void GoogleTracker::Stop() {
      JNIEnv* env = GetEnv();

      env->CallStaticVoidMethod(classGANTracker, methodStopTracker);
    }

    void GoogleTracker::Dispatch() {
      JNIEnv* env = GetEnv();

      env->CallStaticBooleanMethod(classGANTracker, methodDispatch);
    }

    void GoogleTracker::SetReferrer(Str campaign, Str source, Str medium) {
      Str params(100);
      params = "utm_campaign=";
      params += campaign;
      params += "&utm_source=";
      params += source;
      params += "&utm_medium=";
      params += medium;

      char paramsChars[100];
      params.GetChars(paramsChars, 100);

      JNIEnv* env = GetEnv();

      jstring nsParams = env->NewStringUTF(paramsChars);

      env->CallStaticVoidMethod(classGANTracker, methodSetReferrer, nsParams);
    }

  }
}
