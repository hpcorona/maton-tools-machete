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

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.EGLContextFactory;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public abstract class MacheteActivity extends Activity {

	private TickGenerator tick;
	private MacheteNative engine;
	private String baseAssets;
	private boolean gameReady = false;
	private BackgroundView backView;
	private String splash;
	private int color;
	private AlertDialog.Builder builder;
	private boolean queueResume = false;
	private boolean queuePause = false;
	private boolean queueStop = false;
	private GLSurfaceView glView = null;
	private boolean contextLost = false;

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

		builder = new AlertDialog.Builder(this);
		builder.setMessage(
				"This application is not compatible with your device. Try updating to a newer version.")
				.setCancelable(false)
				.setNeutralButton("Close",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								MacheteActivity.this.finish();
							}
						});

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

	protected void startGame() {
		final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		final ConfigurationInfo configurationInfo = activityManager
				.getDeviceConfigurationInfo();
		final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;

		if (!supportsEs2) {
			MacheteActivity.this.runOnUiThread(new Runnable() {
				public void run() {
					builder.show();
				}
			});
		} else {
			glView = new GLSurfaceView(this);
			
			glView.setEGLConfigChooser(new ConfigChooser(5, 6, 5, 0, 24, 0));
			
			glView.setEGLContextFactory(new EGLContextFactory() {
				public void destroyContext(EGL10 egl, EGLDisplay display,
						EGLContext context) {
					egl.eglDestroyContext(display, context);
				}

				public EGLContext createContext(EGL10 egl, EGLDisplay display,
						EGLConfig eglConfig) {

					if (eglConfig == null) {
						throw new RuntimeException("eglConfig is null, or bad match: " + 
							Build.MANUFACTURER + ", " + Build.MODEL + ", " + Build.PRODUCT);
					}

					Log.i("machete", "Creating Context with EGL Version: "
							+ MacheteNative.GL_VERSION);
					int[] attrib_list = new int[] {
							0x3098 , // EGL10.EGL_VERSION
							MacheteNative.GL_VERSION, EGL10.EGL_NONE };

					EGLContext context = egl.eglCreateContext(display,
							eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
					return context;
				}
			});

			glView.setRenderer(new Renderer() {
				public void onSurfaceLost() {
					contextLost = true;
				}
				
				public void onSurfaceCreated(GL10 gl, EGLConfig config) {
					contextLost = true;
				}

				public void onSurfaceChanged(final GL10 gl, final int width,
						final int height) {
					if (gameReady) {
						if (contextLost) {
							engine.reconfigure();
							
							contextLost = false;
						}
						engine.resize(width, height, 1);
						glView.onResume();
					} else {
						String apkFilePath = null;
						ApplicationInfo appInfo = null;
						PackageManager packMgmr = getPackageManager();
						try {
							appInfo = packMgmr
									.getApplicationInfo(baseAssets, 0);
						} catch (NameNotFoundException e) {
							e.printStackTrace();
							throw new RuntimeException(
									"Unable to locate assets, aborting...");
						}
						apkFilePath = appInfo.sourceDir;

						File f = getFilesDir();

						engine.initialize(f.toString(), apkFilePath, width,
								height, 1, 1);

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
												fd.getStartOffset(),
												fd.getLength());

										fd.close();
									} catch (FileNotFoundException e) {
									}
								}
							}
						} catch (IOException e) {
						}

						contextLost = false;
						
						if (queueStop) {
							MacheteActivity.this.runOnUiThread(new Runnable() {
								public void run() {
									android.os.Process.killProcess(android.os.Process.myPid());
								}
							});
						} else {
							engine.startup();
							engine.start();
							
							if (queueResume) {
								engine.resume();
							}
							
							if (queuePause) {
								engine.pause();
							}
							
							MacheteActivity.this.runOnUiThread(new Runnable() {
								public void run() {
									MacheteActivity.this.backView
											.setVisibility(View.INVISIBLE);
									MacheteActivity.this.glView
											.setVisibility(View.VISIBLE);
									MacheteActivity.this.gameReady = true;
									MacheteActivity.this.backView.invalidate();
									MacheteActivity.this.glView
											.invalidate();
									MacheteActivity.this.glView.onResume();
								}
							});
						}
					}
				}

				public void onDrawFrame(GL10 gl) {
					float t = tick.nextFrame();
					engine.update(t);
					engine.render();
				}
			});

			addContentView(glView, new LayoutParams(
					LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		}

		backView = new BackgroundView(this, splash);
		addContentView(backView, new LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT));
		backView.SetColor(color);
	}

	@Override
	protected void onRestart() {
		super.onRestart();
	}

	@Override
	protected void onPause() {
		super.onPause();

		if (gameReady) {
			glView.onPause();
			engine.pause();
		} else {
			queueResume = false;
			queuePause = true;
		}
	}

	@Override
	protected void onResume() {
		super.onResume();

		if (gameReady) {
			glView.onResume();
			engine.resume();
		} else {
			if (queuePause) {
				queuePause = false;
			}
			queueResume = true;
		}
	}

	@Override
	protected void onStart() {
		super.onStart();
	}

	@Override
	protected void onStop() {
		super.onStop();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();

		if (gameReady) {
			engine.stop();
			android.os.Process.killProcess(android.os.Process.myPid());
		} else {
			queueStop = true;
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
