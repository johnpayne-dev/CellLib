#ifndef CellLib_h
#define CellLib_h
#include <stdbool.h>

#define CLColorBlack   0b000000
#define CLColorRed     0b110000
#define CLColorGreen   0b001100
#define CLColorYellow  0b111100
#define CLColorBlue    0b000011
#define CLColorMagenta 0b110011
#define CLColorCyan    0b001111
#define CLColorWhite   0b111111

typedef enum CLKey
{
	CLKeyInvalid          = 0, CLKeySpace             = 32, CLKeyApostrophe       = 39, CLKeyComma            = 44,
	CLKeyMinus            = 45, CLKeyPeriod           = 46, CLKeySlash            = 47, CLKey0                = 48,
	CLKey1                = 49, CLKey2                = 50, CLKey3                = 51, CLKey4                = 52,
	CLKey5                = 53, CLKey6                = 54, CLKey7                = 55, CLKey8                = 56,
	CLKey9                = 57, CLKeySemicolon        = 59, CLKeyEqual            = 61, CLKeyA                = 65,
	CLKeyB                = 66, CLKeyC                = 67, CLKeyD                = 68, CLKeyE                = 69,
	CLKeyF                = 70, CLKeyG                = 71, CLKeyH                = 72, CLKeyI                = 73,
	CLKeyJ                = 74, CLKeyK                = 75, CLKeyL                = 76, CLKeyM                = 77,
	CLKeyN                = 78, CLKeyO                = 79, CLKeyP                = 80, CLKeyQ                = 81,
	CLKeyR                = 82, CLKeyS                = 83, CLKeyT                = 84, CLKeyU                = 85,
	CLKeyV                = 86, CLKeyW                = 87, CLKeyX                = 88, CLKeyY                = 89,
	CLKeyZ                = 90, CLKeyLeftBracket      = 91, CLKeyBackslash        = 92, CLKeyRightBracket     = 93,
	CLKeyGraveAccent      = 96, CLKeyWorld1           = 161, CLKeyWorld2          = 162, CLKeyEscape          = 256,
	CLKeyEnter            = 257, CLKeyTab             = 258, CLKeyBackspace       = 259, CLKeyInsert          = 260,
	CLKeyDelete           = 261, CLKeyRight           = 262, CLKeyLeft            = 263, CLKeyDown            = 264,
	CLKeyUp               = 265, CLKeyPageUp          = 266, CLKeyPageDown        = 267, CLKeyHome            = 268,
	CLKeyEnd              = 269, CLKeyCapsLock        = 280, CLKeyScrollLock      = 281, CLKeyNumLock         = 282,
	CLKeyPrintScreen      = 283, CLKeyPause           = 284, CLKeyF1              = 290, CLKeyF2              = 291,
	CLKeyF3               = 292, CLKeyF4              = 293, CLKeyF5              = 294, CLKeyF6              = 295,
	CLKeyF7               = 296, CLKeyF8              = 297, CLKeyF9              = 298, CLKeyF10             = 299,
	CLKeyF11              = 300, CLKeyF12             = 301, CLKeyF13             = 302, CLKeyF14             = 303,
	CLKeyF15              = 304, CLKeyF16             = 305, CLKeyF17             = 306, CLKeyF18             = 307,
	CLKeyF19              = 308, CLKeyF20             = 309, CLKeyF21             = 310, CLKeyF22             = 311,
	CLKeyF23              = 312, CLKeyF24             = 313, CLKeyF25             = 314, CLKeyKP0             = 320,
	CLKeyKP1              = 321, CLKeyKP2             = 322, CLKeyKP3             = 323, CLKeyKP4             = 324,
	CLKeyKP5              = 325, CLKeyKP6             = 326, CLKeyKP7             = 327, CLKeyKP8             = 328,
	CLKeyKP9              = 329, CLKeyKPDecimal       = 330, CLKeyKPDivide        = 331, CLKeyKPMultiply      = 332,
	CLKeyKPSubtract       = 333, CLKeyKPAdd           = 334, CLKeyKPEnter         = 335, CLKeyKPEqual         = 336,
	CLKeyLeftShift        = 340, CLKeyLeftControl     = 341, CLKeyLeftAlt         = 342, CLKeyLeftSuper       = 343,
	CLKeyRightShift       = 344, CLKeyRightControl    = 345, CLKeyRightAlt        = 346, CLKeyRightSuper      = 347,
	CLKeyMenu             = 348, CLKeyCount,
} CLKey;

typedef enum CLMouseButton
{
	CLMouseButtonLeft = 0x0,
	CLMouseButtonRight = 0x1,
	CLMouseButtonMiddle = 0x2,
	CLMouseButtonInvalid = 0x100,
} CLMouseButton;

typedef struct CLAppInfo
{
	int CellSize;
	int Width;
	int Height;
	char * Title;
	void (*Startup)(void);
	void (*Frame)(void);
	void (*KeyPressed)(CLKey key, bool keyRepeat);
	void (*KeyReleased)(CLKey key, bool keyRepeat);
	void (*MousePressed)(CLMouseButton button, int x, int y);
	void (*MouseReleased)(CLMouseButton button, int x, int y);
	void (*Shutdown)(void);
} CLAppInfo;

CLAppInfo CLMain(int argc, char * argv[]);

void CLClear(unsigned char c);

void CLSet(int x, int y, unsigned char c);

void CLSetAll(unsigned char * buffer);

unsigned char CLGet(int x, int y);

void CLGetAll(unsigned char * buffer);

bool CLIsKeyDown(CLKey key);

bool CLIsMouseButtonDown(CLMouseButton button);

int CLMouseX(void);

int CLMouseY(void);

int CLCellSize(void);

int CLWidth(void);

int CLHeight(void);

#endif
