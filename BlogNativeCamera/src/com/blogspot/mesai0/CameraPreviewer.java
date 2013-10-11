package com.blogspot.mesai0;

import java.util.List;

import org.opencv.android.CameraBridgeViewBase.ListItemAccessor;
import org.opencv.android.NativeCameraView.OpenCvSizeAccessor;
import org.opencv.core.Size;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Display;

public class CameraPreviewer extends Activity {

    GLSurfaceView mView;
    
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        Native.loadlibs();
        VideoCapture mCamera = new VideoCapture(Highgui.CV_CAP_ANDROID);
        java.util.List<Size> sizes = mCamera.getSupportedPreviewSizes();
        mCamera.release();
 
        mView = new GLSurfaceView(getApplication()){
         @Override
         public void onPause() {
          // TODO Auto-generated method stub
          super.onPause();
           Native.releaseCamera();
         }
        };
        Size size = calculateCameraFrameSize(sizes,new OpenCvSizeAccessor());
        mView.setRenderer(new CameraRenderer(this,size));
        setContentView(mView);
    }
    
 protected Size calculateCameraFrameSize(List supportedSizes,
   ListItemAccessor accessor) {
  int calcWidth = Integer.MAX_VALUE;
  int calcHeight = Integer.MAX_VALUE;

  Display display = getWindowManager().getDefaultDisplay();

  int maxAllowedWidth = 1024;
  int maxAllowedHeight = 1024;

  for (Object size : supportedSizes) {
   int width = accessor.getWidth(size);
   int height = accessor.getHeight(size);

   if (width <= maxAllowedWidth && height <= maxAllowedHeight) {
    if ( width <= calcWidth 
      && width>=(maxAllowedWidth/2)
      &&(display.getWidth()%width==0||display.getHeight()%height==0)) {
     calcWidth = (int) width;
     calcHeight = (int) height;
    }
   }
  }

  return new Size(calcWidth, calcHeight);
 }
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
       
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
        
    }
}