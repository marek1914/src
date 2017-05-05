#include <errno.h>
#include <fcntl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define logInfo(s, fmt, args...) printf(fmt, ##args)

#define CAM_FPS 15
#define REQBUF_COUNT 128
#define CAMERA_LIVE_SEGMENTER_PORT 33336
#define CAMERA_LIVE_WIDTH 1280
#define CAMERA_LIVE_HEIGHT 720

typedef struct {
	void *start;
	unsigned int length;
} buffer;

typedef struct {
	int fd;
	int width;
	int height;
	int display_depth;
	int image_size;
	int framerate;
	int n_buffers;
	buffer *buffers;
} camera;
camera *cam = NULL;

int cameraInit = 0;

FILE *h264_fp = NULL;
FILE *yuv_fp = NULL;

int xioctl(int fd, int request, void *arg)
{
	int r = 0;
	do {
		r = ioctl(fd, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}

int camera_live_init()
{
	int ret = -1;

	cam = (camera *)malloc(sizeof(camera));

	cam->buffers = NULL;
	cam->width = CAMERA_LIVE_WIDTH;
	cam->height = CAMERA_LIVE_HEIGHT;
	cam->framerate = CAM_FPS;
	cam->n_buffers = REQBUF_COUNT;
	ret = v4l2_init(cam);
	if (ret != 0) {
		free(cam);
		cam = NULL;
		return -1;
	}

	return 0;
}

int camera_live_stop()
{
	int ret = -1;

	if (cam == NULL) return -1;

	ret = stop_capturing(cam);
	if (ret != 0) return -1;

	return 0;
}

int v4l2_init(camera *cam)
{
	int ret = -1;

	if (cam == NULL) return -1;

	cam->fd = open("/dev/video0", O_RDWR | O_NONBLOCK);
	if (cam->fd < 0) {
		cam->fd = open("/dev/video1", O_RDWR | O_NONBLOCK);
	}

	ret = init_camera_device(cam);
	ret = init_camera_mmap(cam);

	return 0;
}

int start_capturing(camera *cam)
{
	int i;
	enum v4l2_buf_type type;

	for (i = 0; i < cam->n_buffers; ++i) {
		struct v4l2_buffer buf;

		memset(&buf, 0, sizeof buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
			return -1;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(cam->fd, VIDIOC_STREAMON, &type)) {
		return -1;
	}

	return 0;
}

int stop_capturing(camera *cam)
{
	enum v4l2_buf_type type;
	if (cam == NULL) {
		return -1;
	}

	if (cam->fd == -1) {
		return 0;
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(cam->fd, VIDIOC_STREAMOFF, &type)) {
		return -1;
	}

	return 0;
}

int init_camera_device(camera *cam)
{
	int ret;

	// format
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while (ioctl(cam->fd, VIDIOC_ENUM_FMT, &fmtdesc) == 0) {
		printf("###buf-type:%s\n", fmtdesc.description);
		fmtdesc.index++;
	}

	struct v4l2_format fmt;

	memset(&fmt, 0, sizeof fmt);

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(cam->fd, VIDIOC_G_FMT, &fmt) < 0) 
		return -1;

	printf("Width = %d\n", fmt.fmt.pix.width);
	printf("Height = %d\n", fmt.fmt.pix.height);
	printf("Image size = %d\n", fmt.fmt.pix.sizeimage);
	printf("pixelformat = %d\n", fmt.fmt.pix.pixelformat);
	printf("field = %d\n", fmt.fmt.pix.field);

	if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) {
		printf("PixelFormat: V4L2_PIX_FMT_MJPEG\n");
	} else if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV) {
		printf("PixelFormat: V4L2_PIX_FMT_YUYV\n");
	} else if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV422P) {
		printf("PixelFormat: V4L2_PIX_FMT_YUV422P\n");
	} else {
		printf("PixelFormat: Unknown\n");
	}

	fmt.fmt.pix.width = CAMERA_LIVE_WIDTH;
	fmt.fmt.pix.height = CAMERA_LIVE_HEIGHT;
	if (ioctl(cam->fd, VIDIOC_S_FMT, &fmt) < 0) 
		return -1;

	// set fps
	struct v4l2_streamparm setfps = {0};

	setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(cam->fd, VIDIOC_G_PARM, &setfps) < 0) {
		printf("read parm error\n");
		return -1;
	}
	printf("VIDIOC_G_PARM  Frame rate:   %u/%u\n",
		setfps.parm.capture.timeperframe.denominator,
		setfps.parm.capture.timeperframe.numerator);

	//set fps
	if (setfps.parm.capture.capability & V4L2_CAP_TIMEPERFRAME) {

		setfps.parm.capture.timeperframe.numerator = 1;
		setfps.parm.capture.timeperframe.denominator = CAM_FPS;
		if (ioctl(cam->fd, VIDIOC_S_PARM, &setfps) < 0)
			return -1;
	} 
	return 0;
}

int init_camera_mmap(camera *cam)
{
	int ret = -1;
	int i;
	struct v4l2_requestbuffers reqbuf;

	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.count = cam->n_buffers;
	reqbuf.memory = V4L2_MEMORY_MMAP;

	if (ioctl(cam->fd, VIDIOC_REQBUFS, &reqbuf) < 0)
		return -1;

	cam->n_buffers = reqbuf.count; //?

	cam->buffers = (buffer *)calloc(reqbuf.count, sizeof(*(cam->buffers)));
	if (!cam->buffers) {
		return -1;
	}

	for (i = 0; i < reqbuf.count; i++) {
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(cam->fd, VIDIOC_QUERYBUF, &buf) < 0) {
			return -1;
		}

		cam->buffers[i].length = buf.length;
		printf("buf.length = %d offset = %d\n", buf.length, buf.m.offset);

		cam->buffers[i].start =
		    mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, cam->fd,
		         buf.m.offset);
		if (cam->buffers[i].start < 0) {
			return -1;
		}
	}

	return 0;
}

int camera_live_start1()
{
	logInfo(LOG_FILE, "[%s,%d], camera_live_start enter\n", __FUNCTION__,
	        __LINE__);

	int ret = -1;
	if (cam == NULL) {
		logInfo(LOG_FILE, "[%s,%d], cam is invalid\n", __FUNCTION__, __LINE__);
		return -1;
	}

	ret = start_capturing(cam);
	if (ret != 0) {
		logInfo(LOG_FILE, "[%s,%d], start_capturing failed\n", __FUNCTION__,
		        __LINE__);
		return -1;
	}

	logInfo(LOG_FILE, "[%s,%d], camera_live_start success\n", __FUNCTION__,
	        __LINE__);
	return 0;
}

int main(void)
{
	int count = 0;
	int ret;
	fd_set fds;
	struct timeval tv;
	struct v4l2_buffer buf;

	ret = camera_live_init();
	if (ret != 0) {
		printf("camera init failed\n");
		return 1;
	}

	ret = camera_live_start1();
	if (ret != 0) {
		printf("camera_live_start fail.\n");
		return 1;
	}

	while (1) {
		FD_ZERO(&fds);
		FD_SET(cam->fd, &fds);
		tv.tv_sec = 2; /* Timeout. */
		tv.tv_usec = 0;
		ret = select(cam->fd + 1, &fds, NULL, NULL, &tv);
		if (0 == ret) {
			printf("select timeout\n");
			break;
		}

		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(cam->fd, VIDIOC_DQBUF, &buf);
		if (ret < 0) {
			continue;
		}

		ioctl(cam->fd, VIDIOC_QBUF, &buf);
		//logInfo(LOG_FILE, "[%s,%d], encode frame count = %d\n", __FUNCTION__,
		  //      __LINE__, count);

		count++;
	}
}

