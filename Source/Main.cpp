#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// 30.26

// defining values for the movement system
constexpr int NORTH = 0;
constexpr int EAST = 1;
constexpr int SOUTH = 2;
constexpr int WEST = 3;

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
	struct block // basic block definition
	{
		block()
		{

		}

		virtual void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size)
		{

		}

		virtual bool Push(const int direction)
		{
			return true;
		}
	};
	struct block_solid : public block // solid immovable block -- ie walls
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::BLUE);
		}

		bool Push(const int direction) override
		{
			return false;
		}
	};
	struct block_player : public block // player block
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::WHITE);
		}

		bool Push(const int direction) override
		{
			return true;
		}
	};
	struct block_simple : public block // block that moves in any direction
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::RED);
		}

		bool Push(const int direction) override
		{
			return true;
		}
	};
	struct block_horizontal : public block // block that moves side to side
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::GREEN);
		}

		bool Push(const int direction) override
		{
			return true;
		}
	};
	struct block_vertical : public block // block that moves up and down
	{
		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::YELLOW);
		}

		bool Push(const int direction) override
		{
			return true;
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
				case 'P':
					vLevel.emplace_back(std::make_unique<block_player>());
					break;
				case '+':
					vLevel.emplace_back(std::make_unique<block_simple>());
					break;
				case '-':
					vLevel.emplace_back(std::make_unique<block_horizontal>());
					break;
				case '|':
					vLevel.emplace_back(std::make_unique<block_vertical>());
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
		// user input
		bool bPushing = false;
		int iDirPush = NORTH;
		if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed)
		{
			iDirPush = 0;
			bPushing = true;
		}
		if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed)
		{
			iDirPush = SOUTH;
			bPushing = true;
		}
		if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed)
		{
			iDirPush = WEST;
			bPushing = true;
		}
		if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed)
		{
			iDirPush = EAST;
			bPushing = true;
		}

		// movement logic
		if (bPushing) // check if a push attempt is happening this frame
		{

		}

		// Clear screen to black before drawing each frame
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