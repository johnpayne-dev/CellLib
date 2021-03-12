# CellLib
A simple framework for interacting with a cellular grid.

## Functions:
***
#### `CLAppInfo CLMain(int argc, char * argv[]);`
- The entry point that overloads `main`, must be defined by the user and return a completed `CLAppInfo` struct.
***
#### `void CLClear(unsigned char color);`
- Clears the grid to a single color
***
#### `void CLSet(int x, int y, unsigned char c);`
- Sets the given cell position to the given color.
***
#### `void CLSetAll(unsigned char * buffer);`
- Copies the contents of `buffer` onto the grid.
***
#### `unsigned char CLGet(int x, int y);`
- Gets the color of a given cell position.
***
#### `void CLGetAll(unsigned char * buffer);`
- Copies of the current state of the grid into `buffer`.
***
#### `int CLCellSize(void);`
- Returns the cell size defined in the `CLAppInfo` struct.
***
#### `int CLWidth(void);`
- Returns the cell grid width defined in the `CLAppInfo` struct.
***
#### `int CLHeight(void);`
- Returns the cell grid height defined in the `CLAppInfo` struct.
***

## Example:
```C
// An implementation of Conway's Game of Life

#include <stdlib.h>
#include <CellLib.h>

static unsigned char * CellBuffer;
static int Frame = 0;

static void Initialize()
{
	for (int i = 0; i < 0.1 * CLWidth() * CLHeight(); i++)
	{
		int x = rand() % CLWidth();
		int y = rand() % CLHeight();
		CLSet(x, y, CLColorBlack);
	}
	CellBuffer = malloc(CLWidth() * CLHeight());
}

static void Update()
{
	CLGetAll(CellBuffer);
	if (Frame % 4 == 0)
	{
		for (int x = 0; x < CLWidth(); x++)
		{
			for (int y = 0; y < CLHeight(); y++)
			{
				int xl = (x - 1 + CLWidth()) % CLWidth();
				int xr = (x + 1 + CLWidth()) % CLWidth();
				int yl = (y - 1 + CLHeight()) % CLHeight();
				int yr = (y + 1 + CLHeight()) % CLHeight();
				int count = 0;
				count += CLGet(xl, yl) == CLColorBlack;
				count += CLGet(x, yl) == CLColorBlack;
				count += CLGet(xr, yl) == CLColorBlack;
				count += CLGet(xl, yr) == CLColorBlack;
				count += CLGet(x, yr) == CLColorBlack;
				count += CLGet(xr, yr) == CLColorBlack;
				count += CLGet(xl, y) == CLColorBlack;
				count += CLGet(xr, y) == CLColorBlack;
				if (CLGet(x, y) == CLColorBlack && (count < 2 || count > 3)) { CellBuffer[y * CLWidth() + x] = CLColorWhite; }
				if (CLGet(x, y) == CLColorWhite && count == 3) { CellBuffer[y * CLWidth() + x] = CLColorBlack; }
			}
		}
	}
	CLSetAll(CellBuffer);
	Frame++;
}

static void Deinitialize()
{
	free(CellBuffer);
}

CLAppInfo CLMain(int argc, char * argv[])
{
	return (CLAppInfo)
	{
		.CellSize = 10,
		.Width = 120,
		.Height = 72,
		.Title = "Conway's Game of Life",
		.Initialize = Initialize,
		.Update = Update,
		.Deinitialize = Deinitialize,
	};
}
```
### Output:
<img width="1196" alt="Screen Shot 2021-03-11 at 4 37 08 PM" src="https://user-images.githubusercontent.com/79012575/110858578-3d449500-8288-11eb-9b33-c029a7c1a37f.png">
