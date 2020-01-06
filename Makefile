
all:
	arm-oe-linux-gnueabi-gcc  -march=armv7-a -mfloat-abi=softfp -mfpu=neon \
		-I/home/embedded/develop/dev/zms/tools/myiconv/include \
		-L/home/embedded/develop/dev/zms/tools/myiconv/lib -liconv \
		g2u.c -o g2u