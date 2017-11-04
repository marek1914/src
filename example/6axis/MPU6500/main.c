#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "inv_mpu.h"
#include "dmpmap.h"
#include "dmpKey.h"
#include "inv_mpu_dmp_motion_driver.h"


int i2c_write(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* data) 
{
	int8_t count = 0;
	uint8_t buf[128];
	int fd; 
	
	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0) {
		printf("open fail\n");
		return -1; 
	}   
	
	if (ioctl(fd, I2C_SLAVE, dev) < 0) {
		printf("Failed to select device \n");
		return -1; 					    
	}   
	buf[0] = reg;
	memcpy(buf+1,data,len);
								    
	count = write(fd, buf, len+1);
	printf("count: %d  dev: 0x%x reg:0x%x len:%d\n", count, dev, reg, len);
    close(fd);

	return 0;
}

int8_t i2c_read(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data) {
    int8_t count = 0;

    int fd = open("/dev/i2c-1", O_RDWR);

    ioctl(fd, I2C_SLAVE, dev);
    write(fd, &reg, 1);
	read(fd, data, len);
    close(fd);
    return count;
}

#define DEFAULT_MPU_HZ  (100)

int main(void)
{
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4];
	unsigned long sensor_timestamp;

	unsigned char accel_fsr;
    unsigned short gyro_rate, gyro_fsr;
    unsigned long timestamp;
    struct int_param_s int_param;
	uint8_t devStatus; 
	//struct int_param_s int_param;
	
	//int_param.cb = ;
	int ret;

	ret = mpu_init(NULL);
	printf("ret= %d\n", ret);	

   /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
    ret += mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    /* Push both gyro and accel data into the FIFO. */
    ret += mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    ret += mpu_set_sample_rate(DEFAULT_MPU_HZ);
    /* Read back configuration in case it was set improperly. */
    ret += mpu_get_sample_rate(&gyro_rate);
    ret += mpu_get_gyro_fsr(&gyro_fsr);
    ret += mpu_get_accel_fsr(&accel_fsr);

	printf("ret: %d\n", ret);
	printf("%d %d %d\n", gyro_rate, gyro_fsr, accel_fsr);

	mpu_get_power_state(&devStatus);
	printf("devStatus: %d \n", devStatus);


	dmp_load_motion_driver_firmware();

    dmp_enable_feature(
        DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | 
		DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL);
    dmp_set_fifo_rate(DEFAULT_MPU_HZ);

	mpu_set_dmp_state(1);


	while (1) {
		dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,
                &more);
		printf("%d %d %d, %d %d %d\n",gyro[0], gyro[1], gyro[2],
						accel[0], accel[1], accel[2]);
		sleep(1);
	}

	return 0;
}
