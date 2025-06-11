#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// 30.26

class Puzzle : public olc::PixelGameEngine
{
public:
	Puzzle()
	{
		sAppName = "Sliding Blocks Puzzle Game";
	}

	// Definition for level size and block size
	olc::vf2d vLevelSize = { 16,15 };
	olc::vi2d vBlockSize = { 16,16 };

	// Strings containing levels
	std::string sLevel = 
		"################"
		"#..............#"
		"#...+....+.....#"
		"#........+..|..#"
		"#........+..|..#"
		"#....P......|..#"
		"#..5....---....#"
		"#..............#"
		"#..+++....#..@.#"
		"#.........#..@.#"
		"#..-.........@.#"
		"#.....+....|...#"
		"#........+.....#"
		"#..............#"
		"################";

	// Structs containing definitions & logic for block types
	struct block
	{
		block()
		{

		}

		virtual void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size)
		{

		}
	};
	struct block_solid : public block
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::BLUE);
		}
	};

	// Vector of unique pointers to the blocks making up a level
	std::vector<std::unique_ptr<block>> vLevel;

public:
	// Function for loading a level from string template
	void LoadLevel(int n)
	{
		// iterate over level
		for (int y = 0; y < vLevelSize.y; y++)
		{
			for (int x = 0; x < vLevelSize.x; x++)
			{
				// index into 1D version of array coordinates
				switch (sLevel[y * vLevelSize.x + x])
				{
				case '#':
					vLevel.emplace_back(std::make_unique<block_solid>());
					break;

				default:
					vLevel.emplace_back(nullptr);
				}
			}
		}
	}

	// Runs once at start of game
	bool OnUserCreate() override
	{
		LoadLevel(0);

		return true;
	}

	// Runs every frame
	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear screen to black at the start of every frame
		Clear(olc::BLACK);

		// lambda function for translating our 2D coordinates into 1D
		auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; };

		olc::vi2d vBlockPos = { 0,0 };
		for (vBlockPos.y = 0; vBlockPos.y < vLevelSize.y; vBlockPos.y++)
		{
			for (vBlockPos.x = 0; vBlockPos.x < vLevelSize.x; vBlockPos.x++)
			{
				// get pointer to block at a particular position
				auto& b = vLevel[id(vBlockPos)];

				// check for nullptr, then draw
				if (b)
				{
					b->DrawSelf(this, vBlockPos, vBlockSize);
				}
			}
		}

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