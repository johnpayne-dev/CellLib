#ifndef CellLib_h
#define CellLib_h

#define CLColorBlack   0b000000
#define CLColorRed     0b110000
#define CLColorGreen   0b001100
#define CLColorYellow  0b111100
#define CLColorBlue    0b000011
#define CLColorMagenta 0b110011
#define CLColorCyan    0b001111
#define CLColorWhite   0b111111

typedef struct CLAppInfo
{
	int CellSize;
	int Width;
	int Height;
	char * Title;
	void (*Initialize)(void);
	void (*Frame)(void);
	void (*Deinitialize)(void);
} CLAppInfo;

CLAppInfo CLMain(int argc, char * argv[]);

void CLClear(unsigned char c);

void CLSet(int x, int y, unsigned char c);

void CLSetAll(unsigned char * buffer);

unsigned char CLGet(int x, int y);

void CLGetAll(unsigned char * buffer);

int CLCellSize(void);

int CLWidth(void);

int CLHeight(void);

#endif
