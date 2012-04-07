package com.maton.machete;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class AndroidPlatform {
	
	static Context CONTEXT = null;
	
	public static void initialize(Context c) {
		CONTEXT = c;
		GANTracker.initialize(c);
	}
	
	public static void launchUrl(String url) {
		Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
		CONTEXT.startActivity(intent);
	}
	
}
