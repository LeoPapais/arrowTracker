#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void thresh_callback(Mat gray_image);

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */

int main(int argc, char* argv[])
{
    cout << "Hello World!" << endl;
    VideoCapture cap; //
    Mat templ;
    cout << "Connecting to camera." << endl;
    cap.open("http://192.168.43.1:8080/video?x.mjpeg");
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    while (1)
    {
      Mat frame;

      bool bSuccess = cap.read(frame); // read a new frame from video
      Mat gray_image;
      Mat threshold_img;
      cvtColor(frame , gray_image, CV_BGR2GRAY);


      blur( gray_image, gray_image, Size(3,3) );
      threshold(gray_image,threshold_img,128,255,0); // ajustar esse 128

      imshow("MyVideo", threshold_img);

      thresh_callback(threshold_img);
       if (!bSuccess) //if not success, break loop
      {
           cout << "Cannot read a frame from video stream" << endl;
           break;
      }

      if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
         }
    }
    return 0;
}

int thresh = 50;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(Mat gray_image)
{
  Mat threshold_output = gray_image;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
  vector< int > n_of_vertices( contours.size() );
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ ){
     approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
     minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
  }


  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );

       if((contours_poly[i].size() ) == 7 ){
        circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 ); //talvez esse center[i] é o "centro" da seta
        cout << "center: " << center[i] << endl;
       }


     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}