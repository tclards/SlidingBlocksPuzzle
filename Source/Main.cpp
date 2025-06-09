#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Puzzle : public olc::PixelGameEngine
{
public:
	Puzzle()
	{
		sAppName = "Sliding Blocks Puzzle Game";
	}

public:
	bool OnUserCreate() override
	{


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		

		return true;
	}
};

int main()
{
	Puzzle demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}