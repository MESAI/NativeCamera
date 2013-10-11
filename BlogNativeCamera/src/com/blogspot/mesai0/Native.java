package com.blogspot.mesai0;

public class Native {
	public static void loadlibs() {
		System.loadLibrary("opencv_java");
		System.loadLibrary("NativeCamera");
	}

	public static native void initCamera(int width, int height);

	public static native void releaseCamera();

	public static native void renderBackground();

	public static native void surfaceChanged(int width, int height,
			int orientation);
}