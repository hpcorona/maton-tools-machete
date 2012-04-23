package com.maton.machete;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.view.View;
import android.widget.ImageView;
import android.view.ViewGroup.LayoutParams;
import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.opengl.GLSurfaceView.EGLContextFactory;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.graphics.Canvas;
import android.view.Window;
import android.view.WindowManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.content.Context;

public abstract class MacheteActivity extends Activity {

	private TickGenerator tick;
	private MacheteNative engine;
	private boolean initialized = false;
	private String baseAssets;
	private boolean gameReady = false;
	private BackgroundView backView;
	private String splash;
	private int color;

	protected MacheteActivity(String baseAssets, String splash, int color) {
		this.baseAssets = baseAssets;
		AndroidPlatform.initialize(this);
		engine = new MacheteNative();
		tick = new TickGenerator();
		gameReady = false;
		this.color = color;
		this.splash = splash;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		hideTitle();
		fullScreen();
		
		startGame();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (gameReady) {
			dispatchTouch(event);
		}

		return super.onTouchEvent(event);
	}

	public void dispatchTouch(MotionEvent event) {
		int action = event.getAction();
		int actionCode = action & MotionEvent.ACTION_MASK;

		int touchEvent = 0;
		if (actionCode == MotionEvent.ACTION_DOWN) {
			touchEvent = 1;
		} else if (actionCode == MotionEvent.ACTION_MOVE) {
			touchEvent = 2;
		} else if (actionCode == MotionEvent.ACTION_UP) {
			touchEvent = 4;
		} else if (actionCode == MotionEvent.ACTION_CANCEL) {
			touchEvent = 5;
		}

		int id = 0;
		float x = 0;
		float y = 0;

		if (event.getPointerCount() > 0) {
			id = event.getPointerId(0);

			x = event.getX(0);
			y = event.getY(0);
		}

		if (id > 0)
			return;

		engine.touch(id, touchEvent, x, y);
	}

	@Override
	public void onBackPressed() {
		if (gameReady) {
			engine.onKeyTyped(1);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		
		if (gameReady) {
			engine.resume();
		}
	}
	
	protected void startGame() {
		mGLSurfaceView = new GLSurfaceView(this);

		mGLSurfaceView.setEGLConfigChooser(new EGLConfigChooser() {
			public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
			    int[] version = new int[2];
			    egl.eglInitialize(display, version);

			    int EGL_OPENGL_ES2_BIT = 4;
			    int[] configAttribs =
			    {
			        EGL10.EGL_RED_SIZE, 4,
			        EGL10.EGL_GREEN_SIZE, 4,
			        EGL10.EGL_BLUE_SIZE, 4,
			        EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			        EGL10.EGL_NONE
			    };

			    EGLConfig[] configs = new EGLConfig[50];
			    int[] num_config = new int[1];

		    	boolean res = egl.eglChooseConfig(display, configAttribs, configs, 50, num_config);
		    	if (!res) return null;
		    	egl.eglTerminate(display);

			    return configs[num_config[0] - 1];
			}
		});

		mGLSurfaceView.setEGLContextFactory(new EGLContextFactory() {
			public void destroyContext(EGL10 egl, EGLDisplay display,
					EGLContext context) {
				egl.eglDestroyContext(display, context);
			}

			public EGLContext createContext(EGL10 egl, EGLDisplay display,
					EGLConfig eglConfig) {
				Log.i("Maton", "Creating Context with EGL Version: " + MacheteNative.GL_VERSION);
				int[] attrib_list = new int[] {
						0x3098 /* EGL10.EGL_VERSION */, MacheteNative.GL_VERSION, EGL10.EGL_NONE };

				EGLContext context = egl.eglCreateContext(display, eglConfig,
						EGL10.EGL_NO_CONTEXT, attrib_list);
				return context;
			}
		});

		mGLSurfaceView.setRenderer(new Renderer() {
			public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			}

			public void onSurfaceChanged(final GL10 gl, final int width, final int height) {
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

					File f = getFilesDir();

					engine.initialize(f.toString(), apkFilePath, width, height, 1, 1);

					AssetManager mgr = getAssets();
					String path = "";
					String list[];
					try {
						list = mgr.list(path);
						if (list != null) {
							for (int i = 0; i < list.length; ++i) {
								try {
									AssetFileDescriptor fd = mgr
											.openFd(list[i]);

									engine.resourceOffset(list[i],
											fd.getStartOffset(), fd.getLength());

									fd.close();
								} catch (FileNotFoundException e) {
								}
							}
						}
					} catch (IOException e) {
					}

					engine.startup();

					engine.resume();
					
					MacheteActivity.this.runOnUiThread(new Runnable() {
						public void run() {
							MacheteActivity.this.backView.setVisibility(View.INVISIBLE);
							MacheteActivity.this.mGLSurfaceView.setVisibility(View.VISIBLE);
							MacheteActivity.this.gameReady = true;
							MacheteActivity.this.backView.invalidate();
							MacheteActivity.this.mGLSurfaceView.invalidate();
						}
					});
				}
			}

			public void onDrawFrame(GL10 gl) {
				float t = tick.nextFrame();
				engine.update(t);
				engine.render();
			}
		});
		
		addContentView(mGLSurfaceView, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
		
		backView = new BackgroundView(this, splash);
		addContentView(backView, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
		backView.SetColor(color);
	}

	@Override
	protected void onPause() {
		super.onPause();
		
		if (gameReady) {
			engine.pause();
			engine.stop();
			android.os.Process.killProcess(android.os.Process.myPid());
		}
	}

	@Override
	protected void onStart() {
		super.onStart();
		
		if (gameReady) {
			engine.start();
		}
	}

	@Override
	protected void onStop() {
		super.onStop();
		
		if (gameReady) {
			engine.stop();
		}
	}

	protected void fullScreen() {
		hideTitle();
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
	}

	protected void hideTitle() {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
	}

	private GLSurfaceView mGLSurfaceView = null;

	class BackgroundView extends View {
		ImageView imgView;
		
		public BackgroundView(Context context, String image) {
			super(context);
			
			imgView = new ImageView(context);
			try {
				imgView.setImageBitmap(getBitmapFromAsset(image));
			} catch (IOException e) {
				Log.e("SPLASH", "Cannot find splash " + image);
			}

			addContentView(imgView, new LayoutParams(LayoutParams.FILL_PARENT,
					LayoutParams.FILL_PARENT));
		}

		@Override
		protected void onDraw(Canvas canvas) {
			if (getVisibility() == View.VISIBLE) {
				canvas.drawColor(color);
				imgView.draw(canvas);
			}
		}

		private int color = 0xff000000;

		public void SetColor(int aColor) {
			color = aColor;
			invalidate();
		}
		
    private Bitmap getBitmapFromAsset(String strName) throws IOException {
        AssetManager assetManager = getAssets();
        
        InputStream istr = assetManager.open(strName);
        Bitmap bitmap = BitmapFactory.decodeStream(istr);

        return bitmap;
    }

		@Override
		public void setVisibility(int v) {
			super.setVisibility(v);
			imgView.setVisibility(v);
			invalidate();
		}
	}

}
