#include <jni.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <string>
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"
#include <android/log.h>
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"


using namespace std;
using namespace cv;

void detect(Mat& frame);

extern "C"
{
void JNICALL Java_in_errorlabs_spotface_MainActivity_salt(JNIEnv *env, jobject instance,
                                                                           jlong matAddrGray,
                                                                           jint nbrElem) {
    __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "fun_1");
    Mat &mGr = *(Mat *) matAddrGray;
    for (int k = 0; k < nbrElem; k++) {
        int i = rand() % mGr.cols;
        int j = rand() % mGr.rows;
        mGr.at<uchar>(j, i) = 255;
    }
}

void JNICALL Java_in_errorlabs_spotface_MainActivity_faceDetection(JNIEnv *env, jobject  instance, jlong addrRgba){

    __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "fun2");

    Mat& frame = *(Mat*)addrRgba;

    //detect(frame);
//    detect(frame){
        //String face_cascade_name = "/storage/emulator/0/opencv/haarcascade_frontalface_alt.xml";
        String face_cascade_name = "/storage/emulated/0/data/haarcascade_frontalface_alt.xml";
        String eyes_cascade_name = "/storage/emulated/0/data/haarcascade_eye_tree_eyeglasses.xml";
        CascadeClassifier face_cascade;
        CascadeClassifier eyes_cascade;
        if( !face_cascade.load( face_cascade_name) ){
            printf("--(!)Error loading\n");
            __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "failed to load xml1");
            //return ;
        };
        if( !eyes_cascade.load( eyes_cascade_name ) ){
            printf("--(!)Error loading\n");
            __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "failed to load xml2");
            //return ;
        };

        std::vector<Rect> faces;
        Mat frame_gray;

        cvtColor( frame, frame_gray, CV_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );

        //-- Detect faces
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for( size_t i = 0; i < faces.size(); i++ )
        {
            Point center((int) (faces[i].x + faces[i].width * 0.5),
                         (int) (faces[i].y + faces[i].height * 0.5));
            ellipse(frame, center, Size((int) (faces[i].width * 0.5), (int) (faces[i].height * 0.5)), 0, 0, 360, Scalar(255, 0, 255 ), 4, 8, 0 );

            Mat faceROI = frame_gray( faces[i] );
            std::vector<Rect> eyes;

            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

            for( size_t j = 0; j < eyes.size(); j++ )
            {
                __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "fun2_loop");
                Point cen((int) (faces[i].x + eyes[j].x + eyes[j].width * 0.5),
                          (int) (faces[i].y + eyes[j].y + eyes[j].height * 0.5));
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( frame, cen, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
            }
        }
//    }


}



}