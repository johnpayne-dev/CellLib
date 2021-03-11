#ifndef CellLib_h
#define CellLib_h

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

extern struct CL
{
	CLAppInfo AppInfo;
} CL;

CLAppInfo CLMain(int argc, char * argv[]);

#endif
