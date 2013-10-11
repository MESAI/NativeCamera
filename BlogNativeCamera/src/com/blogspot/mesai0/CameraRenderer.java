package com.blogspot.mesai0;
 
 
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
 
import org.opencv.core.Size;
 
import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;
 
 
public class CameraRenderer implements Renderer {
 
 private Size size;
 private Context context;
 public CameraRenderer(Context c,Size size) {
  super();
  context = c;
  this.size = size;
 }
  
 public void onSurfaceCreated(GL10 gl, EGLConfig config) {
  Thread.currentThread().setPriority(Thread.MAX_PRIORITY);
  Native.initCamera((int)size.width,(int)size.height);
 }
 
 public void onDrawFrame(GL10 gl) {
//  long startTime   = System.currentTimeMillis();
  Native.renderBackground();
//  long endTime   = System.currentTimeMillis();
//  if(30-(endTime-startTime)>0){
//   try {
//    Thread.sleep(30-(endTime-startTime));
//   } catch (InterruptedException e) {}
//  }
//  endTime   = System.currentTimeMillis();
  //System.out.println(endTime-startTime+" ms");
 }
  
 public void onSurfaceChanged(GL10 gl, int width, int height) {
  Native.surfaceChanged(width,height,context.getResources().getConfiguration().orientation);
 }
} 