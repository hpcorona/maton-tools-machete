package com.maton.machete;

import com.google.android.apps.analytics.GoogleAnalyticsTracker;
import android.content.Context;

public class GANTracker {
	
	static Context CONTEXT = null;
	static GoogleAnalyticsTracker TRACKER = null;
	
	public static void initialize(Context c) {
		CONTEXT = c;
		
		TRACKER = GoogleAnalyticsTracker.getInstance();
	}
	
	public static void startNewSession(String accountId, int time) {
		TRACKER.startNewSession(accountId, time, CONTEXT);
	}
	
	public static void trackEvent(String s1, String s2, String s3, int v) {
		TRACKER.trackEvent(s1, s2, s3, v);
	}
	
	public static void trackPageview(String s1) {
		TRACKER.trackPageView(s1);
	}
	
	public static void dispatch() {
		TRACKER.dispatch();
	}
	
	public static void stopSession() {
		TRACKER.stopSession();
	}
	
	public static void setCustomVar(int idx, String s1, String s2) {
		TRACKER.setCustomVar(idx, s1, s2);
	}
	
	public static void setAnonymizeIp(boolean b1) {
		TRACKER.setAnonymizeIp(b1);
	}
	
	public static void setSampleRate(int r) {
		TRACKER.setSampleRate(r);
	}
	
	public static void setReferrer(String s1) {
		TRACKER.setReferrer(s1);
	}
	
}
