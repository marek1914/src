#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <stdio.h>

int main( int argc, char** argv )
{
    CvCapture* capture = cvCreateCameraCapture(0);
    CvMat* warp = cvCreateMat(3,3,CV_32FC1);
	IplImage *srcimg, *dstimg;
	CvPoint2D32f src[4], dst[4];

	srcimg = cvQueryFrame(capture);
	dstimg = cvCloneImage(srcimg);

	src[0].x = 184;
	src[0].y = 345;
	src[1].x = 514;
	src[1].y = 338;
	src[2].x = 89;
	src[2].y = 476;
	src[3].x = 549;
	src[3].y = 474;


	dst[0].x = 89;
	dst[0].y = 116;
	dst[1].x = 549;
	dst[1].y = 116;
	dst[2].x = 89;
	dst[2].y = 476;
	dst[3].x = 549;
	dst[3].y = 474;

	cvGetPerspectiveTransform(src, dst, warp);
	

    cvNamedWindow("1", 1);
	cvNamedWindow("2", 1);

    while(1) {
        srcimg = cvQueryFrame(capture);
		cvShowImage("1", srcimg);
		cvWarpPerspective(srcimg, dstimg, warp,
							CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
 		cvShowImage("2", dstimg);

        if(cvWaitKey(30)>=0)
            break;
    }
    cvReleaseCapture(&capture);
    return 0;
}
