CC= $(TOOLCHAIN)g++
LD= $(TOOLCHAIN)ld
STRIP = $(TOOLCHAIN)strip

CFLAGS := `pkg-config --libs --cflags opencv` -g

SRCS += stitching_detailed.cpp

OBJS= $(SRCS:.c=.o)

TARGET := stitching_detailed

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

install:
	cp $(TARGET) ../../../out/bin/

clean:
	rm -rf $(OBJS) $(TARGET)

