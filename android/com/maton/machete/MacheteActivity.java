package com.maton.machete;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.opengl.GLSurfaceView.EGLContextFactory;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public abstract class MacheteActivity extends Activity {

	private TickGenerator tick;
	private MacheteNative engine;
	private boolean initialized = false;
	private String baseAssets;

	protected MacheteActivity(String baseAssets) {
		this.baseAssets = baseAssets;
		engine = new MacheteNative();
		tick = new TickGenerator();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		fullScreen();
		hideTitle();

		mGLSurfaceView = new GLSurfaceView(this);

		mGLSurfaceView.setEGLConfigChooser(new EGLConfigChooser() {
			public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
				EGLConfig[] configs = new EGLConfig[1];
				int[] num_config = new int[1];

				boolean check = false;

				int[] configSpec = { EGL10.EGL_DEPTH_SIZE, 16, EGL10.EGL_NONE };

				check = egl.eglInitialize(display, new int[] { 2, 0 });

				if (!check)
					return null;
				check = false;

				check = egl.eglChooseConfig(display, configSpec, configs, 1,
						num_config);
				if (!check)
					return null;

				return configs[0];
			}
		});

		mGLSurfaceView.setEGLContextFactory(new EGLContextFactory() {
			public void destroyContext(EGL10 egl, EGLDisplay display,
					EGLContext context) {
				egl.eglDestroyContext(display, context);
			}

			public EGLContext createContext(EGL10 egl, EGLDisplay display,
					EGLConfig eglConfig) {
				int[] attrib_list = new int[] { 0x3098 /*EGL10.EGL_VERSION*/, 2,
						EGL10.EGL_NONE };

				EGLContext context = egl.eglCreateContext(display, eglConfig,
						EGL10.EGL_NO_CONTEXT, attrib_list);
				return context;
			}
		});

		mGLSurfaceView.setRenderer(new Renderer() {
			public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			}

			public void onSurfaceChanged(GL10 gl, int width, int height) {
				if (initialized) {
					engine.resize(width, height, 1);
				} else {
					String apkFilePath = null;
					ApplicationInfo appInfo = null;
					PackageManager packMgmr = getPackageManager();
					try {
						appInfo = packMgmr.getApplicationInfo(baseAssets, 0);
					} catch (NameNotFoundException e) {
						e.printStackTrace();
						throw new RuntimeException(
								"Unable to locate assets, aborting...");
					}
					apkFilePath = appInfo.sourceDir;

					engine.initialize(apkFilePath, width, height, 1, 1);
				}
			}

			public void onDrawFrame(GL10 gl) {
				float t = tick.nextFrame();
				engine.update(t);
				engine.render();
			}
		});
		setContentView(mGLSurfaceView);
	}

	@Override
	protected void onResume() {
		super.onResume();
		engine.resume();
	}

	@Override
	protected void onPause() {
		super.onPause();
		engine.pause();
	}

	@Override
	protected void onStart() {
		super.onStart();
		engine.start();
	}

	@Override
	protected void onStop() {
		super.onStop();
		engine.stop();
	}

	protected void fullScreen() {
		hideTitle();
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
	}

	protected void hideTitle() {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
	}

	private GLSurfaceView mGLSurfaceView;

}
