
#define EXPLOSION100_FRAME_COUNT 9
#define EXPLOSION100_WIDTH 32
#define EXPLOSION100_HEIGHT 16

// Converted using https://www.cemetech.net/sc/  (BMP/PNG to C converter)

//UINT32 explosions100Palette[5] = {0x0000, 0xffff, 0xffe0, 0x8000, 0xf800};
UINT32 explosion100Palette[5] = {0x00000000, 0xffffffff, 0xff00ffff, 0xff000080, 0xff0000ff};

UINT8 explosion100Data[9][256] = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x22,0x22,0x11,0x11,0x10,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x12,0x22,0x22,0x21,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x12,0x22,0x22,0x21,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x22,0x22,0x11,0x11,0x10,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,
		0x00,0x00,0x01,0x11,0x11,0x11,0x22,0x22,0x22,0x22,0x11,0x11,0x11,0x10,0x00,0x00,
		0x00,0x00,0x11,0x11,0x12,0x22,0x22,0x22,0x22,0x22,0x22,0x21,0x11,0x11,0x00,0x00,
		0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,
		0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,
		0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,
		0x00,0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x00,0x00,
		0x00,0x00,0x11,0x11,0x12,0x22,0x22,0x22,0x22,0x22,0x22,0x21,0x11,0x11,0x00,0x00,
		0x00,0x00,0x01,0x11,0x11,0x11,0x22,0x22,0x22,0x22,0x11,0x11,0x11,0x10,0x00,0x00,
		0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x13,0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x11,0x31,0x11,0x11,0x11,0x11,0x11,0x00,0x00,
		0x00,0x01,0x11,0x11,0x11,0x22,0x22,0x23,0x22,0x22,0x22,0x11,0x11,0x11,0x10,0x00,
		0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x22,0x32,0x22,0x22,0x22,0x22,0x11,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x22,0x23,0x22,0x22,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x24,0x44,0x34,0x42,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x44,0x44,0x43,0x44,0x44,0x44,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x44,0x44,0x44,0x34,0x44,0x44,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x24,0x43,0x44,0x42,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x22,0x22,0x32,0x22,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x11,0x22,0x22,0x22,0x22,0x23,0x22,0x22,0x22,0x22,0x22,0x11,0x11,0x00,
		0x00,0x01,0x11,0x11,0x11,0x22,0x22,0x22,0x32,0x22,0x22,0x11,0x11,0x11,0x10,0x00,
		0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x13,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x00,
		0x00,0x00,0x00,0x11,0x11,0x11,0x11,0x11,0x31,0x11,0x11,0x11,0x11,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x01,0x11,0x11,0x11,0x11,0x33,0x33,0x31,0x11,0x11,0x11,0x11,0x11,0x10,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x22,0x23,0x33,0x33,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x11,0x22,0x22,0x22,0x23,0x33,0x33,0x32,0x22,0x22,0x22,0x22,0x22,0x11,0x00,
		0x01,0x11,0x22,0x22,0x44,0x44,0x44,0x43,0x33,0x33,0x44,0x44,0x22,0x22,0x11,0x10,
		0x01,0x12,0x22,0x24,0x44,0x14,0x31,0x11,0x13,0x11,0x11,0x44,0x42,0x22,0x21,0x10,
		0x01,0x12,0x22,0x44,0x41,0x14,0x41,0x43,0x13,0x13,0x31,0x44,0x44,0x22,0x21,0x10,
		0x01,0x12,0x22,0x44,0x44,0x14,0x31,0x33,0x14,0x14,0x41,0x44,0x44,0x22,0x21,0x10,
		0x01,0x12,0x22,0x44,0x44,0x14,0x41,0x33,0x13,0x13,0x41,0x44,0x44,0x22,0x21,0x10,
		0x01,0x12,0x22,0x44,0x44,0x13,0x31,0x33,0x14,0x14,0x41,0x44,0x44,0x22,0x21,0x10,
		0x01,0x12,0x22,0x24,0x44,0x14,0x41,0x11,0x13,0x11,0x11,0x44,0x42,0x22,0x21,0x10,
		0x01,0x11,0x22,0x22,0x44,0x44,0x33,0x33,0x34,0x44,0x44,0x44,0x22,0x22,0x11,0x10,
		0x00,0x11,0x22,0x22,0x22,0x22,0x22,0x23,0x33,0x33,0x32,0x22,0x22,0x22,0x11,0x00,
		0x00,0x11,0x12,0x22,0x22,0x22,0x33,0x33,0x32,0x22,0x22,0x22,0x22,0x21,0x11,0x00,
		0x00,0x01,0x11,0x11,0x11,0x11,0x11,0x13,0x33,0x33,0x11,0x11,0x11,0x11,0x10,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x11,0x11,0x11,0x23,0x33,0x33,0x00,0x33,0x33,0x32,0x21,0x11,0x11,0x11,0x00,
		0x01,0x12,0x22,0x22,0x22,0x22,0x33,0x30,0x03,0x33,0x33,0x32,0x22,0x22,0x21,0x10,
		0x01,0x22,0x22,0x44,0x43,0x33,0x30,0x00,0x33,0x33,0x33,0x44,0x44,0x22,0x22,0x10,
		0x11,0x22,0x44,0x44,0x44,0x44,0x43,0x30,0x00,0x33,0x33,0x33,0x44,0x44,0x22,0x11,
		0x12,0x22,0x44,0x44,0x33,0x13,0x31,0x11,0x13,0x11,0x11,0x44,0x44,0x44,0x22,0x21,
		0x12,0x24,0x44,0x44,0x41,0x14,0x31,0x00,0x13,0x13,0x31,0x33,0x34,0x44,0x42,0x21,
		0x12,0x24,0x44,0x43,0x33,0x13,0x31,0x30,0x13,0x13,0x31,0x44,0x44,0x44,0x42,0x21,
		0x12,0x24,0x44,0x44,0x44,0x13,0x31,0x00,0x13,0x13,0x31,0x33,0x44,0x44,0x42,0x21,
		0x12,0x24,0x44,0x44,0x33,0x13,0x31,0x30,0x13,0x13,0x31,0x44,0x44,0x44,0x42,0x21,
		0x12,0x22,0x44,0x44,0x44,0x13,0x31,0x11,0x13,0x11,0x11,0x33,0x44,0x44,0x22,0x21,
		0x11,0x22,0x44,0x44,0x43,0x33,0x33,0x30,0x03,0x33,0x44,0x44,0x44,0x44,0x22,0x11,
		0x01,0x22,0x22,0x44,0x44,0x43,0x30,0x00,0x33,0x33,0x33,0x34,0x44,0x22,0x22,0x10,
		0x01,0x12,0x22,0x22,0x33,0x33,0x33,0x30,0x00,0x33,0x33,0x22,0x22,0x22,0x21,0x10,
		0x00,0x11,0x11,0x11,0x22,0x22,0x33,0x00,0x33,0x33,0x32,0x22,0x11,0x11,0x11,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x22,0x22,0x22,0x33,0x30,0x00,0x00,0x00,0x00,0x33,0x33,0x22,0x22,0x22,0x00,
		0x02,0x22,0x24,0x44,0x43,0x33,0x00,0x00,0x00,0x00,0x03,0x33,0x34,0x42,0x22,0x20,
		0x02,0x24,0x44,0x43,0x33,0x00,0x00,0x00,0x00,0x03,0x33,0x33,0x44,0x44,0x42,0x20,
		0x22,0x44,0x44,0x44,0x43,0x30,0x00,0x00,0x00,0x00,0x03,0x33,0x33,0x34,0x44,0x22,
		0x24,0x44,0x44,0x33,0x33,0x13,0x01,0x11,0x10,0x11,0x11,0x33,0x34,0x44,0x44,0x42,
		0x24,0x44,0x44,0x44,0x31,0x10,0x01,0x00,0x10,0x10,0x31,0x33,0x33,0x33,0x44,0x42,
		0x24,0x44,0x43,0x33,0x33,0x13,0x01,0x00,0x10,0x10,0x31,0x33,0x34,0x44,0x44,0x42,
		0x24,0x44,0x44,0x44,0x43,0x10,0x01,0x00,0x10,0x10,0x01,0x33,0x33,0x34,0x44,0x42,
		0x24,0x44,0x44,0x33,0x33,0x10,0x01,0x00,0x10,0x13,0x31,0x33,0x34,0x44,0x44,0x42,
		0x24,0x44,0x44,0x44,0x43,0x10,0x01,0x11,0x10,0x11,0x11,0x33,0x33,0x33,0x44,0x42,
		0x22,0x44,0x43,0x33,0x33,0x33,0x00,0x00,0x00,0x00,0x03,0x33,0x44,0x44,0x44,0x22,
		0x02,0x24,0x44,0x44,0x33,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x34,0x42,0x20,
		0x02,0x22,0x24,0x33,0x33,0x33,0x00,0x00,0x00,0x00,0x00,0x33,0x34,0x42,0x22,0x20,
		0x00,0x22,0x22,0x22,0x33,0x30,0x00,0x00,0x00,0x00,0x33,0x33,0x22,0x22,0x22,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x24,0x43,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x34,0x24,0x00,
		0x02,0x42,0x44,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x42,0x40,
		0x04,0x44,0x43,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x34,0x44,0x40,
		0x44,0x44,0x44,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x34,0x24,
		0x44,0x43,0x33,0x33,0x30,0x10,0x01,0x11,0x10,0x11,0x11,0x03,0x33,0x33,0x44,0x44,
		0x44,0x44,0x43,0x30,0x01,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x33,0x33,0x34,0x44,
		0x44,0x43,0x33,0x33,0x30,0x10,0x01,0x00,0x10,0x10,0x01,0x03,0x33,0x34,0x44,0x44,
		0x44,0x44,0x43,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x03,0x33,0x44,0x44,
		0x44,0x44,0x33,0x33,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x33,0x33,0x33,0x34,0x42,
		0x44,0x44,0x44,0x30,0x00,0x10,0x01,0x11,0x10,0x11,0x11,0x00,0x33,0x33,0x33,0x44,
		0x44,0x43,0x33,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x33,0x44,0x42,
		0x04,0x44,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x34,0x40,
		0x02,0x43,0x23,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x34,0x44,0x20,
		0x00,0x24,0x43,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x44,0x44,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x43,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x34,0x00,
		0x04,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x40,
		0x43,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x44,
		0x44,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x34,
		0x43,0x30,0x30,0x30,0x00,0x10,0x01,0x11,0x10,0x11,0x11,0x00,0x00,0x30,0x34,0x44,
		0x34,0x33,0x00,0x00,0x01,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x03,0x33,0x43,
		0x43,0x33,0x30,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x33,0x34,
		0x44,0x30,0x00,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x03,0x44,
		0x43,0x33,0x30,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x03,0x33,0x34,
		0x43,0x43,0x00,0x00,0x00,0x10,0x01,0x11,0x10,0x11,0x11,0x00,0x00,0x00,0x33,0x43,
		0x33,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x34,0x34,
		0x43,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x34,
		0x04,0x33,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x40,
		0x00,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x44,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
		0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x30,
		0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,
		0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
		0x03,0x00,0x00,0x00,0x00,0x10,0x01,0x11,0x10,0x11,0x11,0x00,0x00,0x00,0x30,0x03,
		0x30,0x00,0x00,0x00,0x01,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x00,0x30,
		0x33,0x00,0x00,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x00,0x03,
		0x03,0x00,0x00,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x03,0x00,
		0x30,0x00,0x00,0x00,0x00,0x10,0x01,0x00,0x10,0x10,0x01,0x00,0x00,0x00,0x00,0x33,
		0x30,0x00,0x00,0x00,0x00,0x10,0x01,0x11,0x10,0x11,0x11,0x00,0x00,0x00,0x00,0x03,
		0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,
		0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
		0x03,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,
		0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};