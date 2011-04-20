package com.maton.machete;

public class MacheteNative {
	static {
		System.loadLibrary("machete");
	}
	
	public native void initialize(String apkpath, int w, int h, int o, int opengles);
	public native void resize(int w, int h, int o);
	public native void render();
	public native void pause();
	public native void update(float time);
	public native void resume();
	public native void start();
	public native void stop();
}
