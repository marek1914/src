#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// enough
unsigned char buf[5 * 1024];
unsigned char slice_crc[128]; //max 128*4k=512k

#define POLY (0x1070U << 3)

unsigned char _crc8(unsigned short data) {
  int i;

  for (i = 0; i < 8; i++) {
    if (data & 0x8000) data = data ^ POLY;
    data = data << 1;
  }
  return (unsigned char)(data >> 8);
}

unsigned int crc8(unsigned int crc, const unsigned char *vptr, int len) {
  int i;

  for (i = 0; i < len; i++) crc = _crc8((crc ^ vptr[i]) << 8);
  return crc;
}

int download(char* file)
{
  char path[256];
  strcpy(path, "./download.sh ");
  strcat(path, file);
  system(path);

}

void parse_date(char * filename,  off_t *packLen, int *contentLen)
{
  int fd;
  char *p;
  char *token;

  fd = open(filename, O_RDONLY);

  if (fd < 0) {
    printf("open fail\n");
    return 0;
  }

  *packLen = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  read(fd, buf, *packLen);

  p = strtok(buf, "\r\n");

  if (strcmp(p, "HTTP/1.1 200 OK")) {
    printf("fail\n");
    return;
  } else {
    printf("head ok\n");
  }

  do {
    p = strtok(NULL, "\r\n");
    token = strstr(p, "Content-Length: ");
  } while (!token && p);

  if (token) {
    *contentLen = atoi(token + 16);
  }

  printf("content-len = %d\n", *contentLen);


}

int main(void) {
  off_t len;
  int contentLen;
  unsigned char crc, crc1;

  download("index.txt");

  char *tmp;

  parse_date("./split/index.txt", &len, &contentLen);

  tmp = buf + (len - contentLen);
  //no '\n' ok?
  crc = strtol(tmp+=4, NULL, 16);
  printf("crc=%x\n", crc);

  //7byte :  crc:xx\n
  crc1 = crc8(0, tmp+=3, contentLen - 7);

  // printf("DATA=\n%s", tmp+7);
  // printf("len=%d\n", contentLen-7);
  printf("crc1=%x\n", crc1);

  
  unsigned char count;
  
  count = strtol(tmp+=6, NULL, 16);

  printf("count = %d \n", count); 
//检查一下文件头特征，如果不是stm32 bin程序，发送post，报告问题，
//升级成功上报状态？ 需要数据库了

  tmp += 3;

  char* s_crc;
  char i;
  s_crc = strtok(tmp, "\n ");
  slice_crc[0] = strtol(s_crc, NULL, 16);

  for (i=1; i<count; i++) {
    s_crc = strtok(NULL, "\n ");   
    slice_crc[i] = strtol(s_crc, NULL, 16);
  }
 
  for (i=0; i<count; i++) {
    printf("%02x\n", slice_crc[i]);
  }

  for (i=0; i<count; i++) {
    char path[128]="./split/";
    char tmp[3];
    sprintf(tmp, "%02x", i);
    download(tmp);
    strcat(path, tmp);
    parse_date(path, &len, &contentLen);

    printf("%d %d\n", len, contentLen);

    crc = crc8(0, buf + len - contentLen, contentLen);
    
    if (crc == slice_crc[i])
      printf("crc ok++++++++++++\n");
    else 
      printf("crc fail----------\n");
  }


}
