#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/log.h>
#include <pthread.h>
#include <time.h>
#include <Math.h>
#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"

// Utility for logging:
#define LOG_TAG    "CAMERA_RENDERER"
#define LOG(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

GLuint texture;
cv::VideoCapture capture;
cv::Mat buffer[30];
cv::Mat rgbFrame;
cv::Mat inframe;
cv::Mat outframe;
int bufferIndex;
int rgbIndex;
int frameWidth;
int frameHeight;
int screenWidth;
int screenHeight;
int orientation;
pthread_mutex_t FGmutex;
pthread_t frameGrabber;
pthread_attr_t attr;
struct sched_param param;

GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f, // V1 - bottom left
      -1.0f,  1.0f, 0.0f, // V2 - top left
       1.0f, -1.0f, 0.0f, // V3 - bottom right
       1.0f,  1.0f, 0.0f // V4 - top right
       };

GLfloat textures[8];

extern "C" {

void drawBackground();
void createTexture();
void destroyTexture();
void *frameRetriever(void*);

JNIEXPORT void JNICALL Java_com_blogspot_mesai0_Native_initCamera(JNIEnv*, jobject,jint width,jint height)
{
 LOG("Camera Created");
 capture.open(CV_CAP_ANDROID + 0);
 capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
 capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
 frameWidth =width;
 frameHeight = height;
 LOG("frameWidth = %d",frameWidth);
 LOG("frameHeight = %d",frameHeight);
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 glShadeModel(GL_SMOOTH);
 glClearDepthf(1.0f);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 pthread_attr_t attr;
 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
 pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
 memset(&param, 0, sizeof(param));
 param.sched_priority = 100;
 pthread_attr_setschedparam(&attr, &param);
 pthread_create(&frameGrabber, &attr, frameRetriever, NULL);
 pthread_attr_destroy(&attr);

}

JNIEXPORT void JNICALL Java_com_blogspot_mesai0_Native_surfaceChanged(JNIEnv*, jobject,jint width,jint height,jint orien)
{
 LOG("Surface Changed");
 glViewport(0, 0, width,height);
 if(orien==1) {
   screenWidth = width;
   screenHeight = height;
   orientation = 1;
  } else {
   screenWidth = height;
   screenHeight = width;
   orientation = 2;
  }


 LOG("screenWidth = %d",screenWidth);
 LOG("screenHeight = %d",screenHeight);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 float aspect = screenWidth / screenHeight;
 float bt = (float) tan(45 / 2);
 float lr = bt * aspect;
 glFrustumf(-lr * 0.1f, lr * 0.1f, -bt * 0.1f, bt * 0.1f, 0.1f,
   100.0f);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glEnable(GL_TEXTURE_2D);
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 glClearDepthf(1.0f);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);
 createTexture();
}

JNIEXPORT void JNICALL Java_com_blogspot_mesai0_Native_releaseCamera(JNIEnv*, jobject)
{
 LOG("Camera Released");
 capture.release();
 destroyTexture();

}

void createTexture() {
  textures[0] = ((1024.0f-frameWidth*1.0f)/2.0f)/1024.0f;
  textures[1] = ((1024.0f-frameHeight*1.0f)/2.0f)/1024.0f + (frameHeight*1.0f/1024.0f);
  textures[2] = ((1024.0f-frameWidth*1.0f)/2.0f)/1024.0f + (frameWidth*1.0f/1024.0f);
  textures[3] = ((1024.0f-frameHeight*1.0f)/2.0f)/1024.0f + (frameHeight*1.0f/1024.0f);
  textures[4] = ((1024.0f-frameWidth*1.0f)/2.0f)/1024.0f;
  textures[5] = ((1024.0f-frameHeight*1.0f)/2.0f)/1024.0f;
  textures[6] = ((1024.0f-frameWidth*1.0f)/2.0f)/1024.0f + (frameWidth*1.0f/1024.0f);
  textures[7] = ((1024.0f-frameHeight*1.0f)/2.0f)/1024.0f;
 LOG("Texture Created");
 glGenTextures(1, &texture);
 glBindTexture(GL_TEXTURE_2D, texture);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024,1024, 0, GL_RGB,
   GL_UNSIGNED_SHORT_5_6_5, NULL);
 glBindTexture(GL_TEXTURE_2D, 0);
}

void destroyTexture() {
 LOG("Texture destroyed");
 glDeleteTextures(1, &texture);
}

JNIEXPORT void JNICALL Java_com_blogspot_mesai0_Native_renderBackground(
  JNIEnv*, jobject) {
 drawBackground();
}

void drawBackground() {
 glClear (GL_COLOR_BUFFER_BIT);
 glBindTexture(GL_TEXTURE_2D, texture);
 if(bufferIndex>0){
 pthread_mutex_lock(&FGmutex);
 cvtColor(buffer[(bufferIndex - 1) % 30], outframe, CV_BGR2BGR565);
 pthread_mutex_unlock(&FGmutex);
 cv::flip(outframe, rgbFrame, 1);
 if (texture != 0)
   glTexSubImage2D(GL_TEXTURE_2D, 0, (1024-frameWidth)/2, (1024-frameHeight)/2, frameWidth, frameHeight,
   GL_RGB, GL_UNSIGNED_SHORT_5_6_5, rgbFrame.ptr());
 }
 glEnableClientState (GL_VERTEX_ARRAY);
 glEnableClientState (GL_TEXTURE_COORD_ARRAY);
 glLoadIdentity();
 if(orientation!=1){
  glRotatef( 90,0,0,1);
 }
 // Set the face rotation
 glFrontFace (GL_CW);
 // Point to our vertex buffer
 glVertexPointer(3, GL_FLOAT, 0, vertices);
 glTexCoordPointer(2, GL_FLOAT, 0, textures);
 // Draw the vertices as triangle strip
 glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 //Disable the client state before leaving
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void *frameRetriever(void*) {
 while (capture.isOpened()) {
  capture.read(inframe);
  if (!inframe.empty()) {
   pthread_mutex_lock(&FGmutex);
   inframe.copyTo(buffer[(bufferIndex++) % 30]);
   pthread_mutex_unlock(&FGmutex);
  }
 }
 LOG("Camera Closed");
 pthread_exit (NULL);
}


}
