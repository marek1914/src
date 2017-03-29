#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <stdio.h>

int main( int argc, char** argv )
{
    CvCapture* capture = cvCreateCameraCapture(0);

	int error = cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1280);
	int error1 = cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 720);

	CvMemStorage* storage = cvCreateMemStorage(0);

	IplImage *srcimg, *dstimg;

	double testval = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);

	CvMat *intrinsic  = (CvMat*)cvLoad("Intrinsics.xml", storage, 0, 0);
    CvMat *distortion = (CvMat*)cvLoad("Distortion.xml", storage, 0, 0);


	srcimg = cvQueryFrame(capture);
	dstimg = cvCloneImage(srcimg);

    IplImage* mapx = cvCreateImage( cvGetSize(srcimg), IPL_DEPTH_32F, 1 );
    IplImage* mapy = cvCreateImage( cvGetSize(srcimg), IPL_DEPTH_32F, 1 );
	
	cvInitUndistortMap(intrinsic, distortion, mapx, mapy );

    cvNamedWindow("1", 1);
	cvNamedWindow("2", 1);

    while(1) {
        srcimg = cvQueryFrame(capture);
		cvShowImage("1", srcimg);
		cvRemap( srcimg, dstimg, mapx, mapy,
					CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
 		cvShowImage("2", dstimg);

        if(cvWaitKey(30)>=0)
            break;
    }
    cvReleaseCapture(&capture);
    return 0;
}
