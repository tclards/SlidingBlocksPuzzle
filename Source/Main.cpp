#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// 1.10.25

// TODO
// SFX
// music?
// WIN SCREEN
// Restart Level Button - in case of softlock
// Level Completion timing
// Main Menu and Score/Time screen
// README
// Credits
// create many many many levels
// Cool sprites to make game prettier
// teleport tile

// defining global values for the movement system
constexpr int NORTH = 0;
constexpr int EAST = 1;
constexpr int SOUTH = 2;
constexpr int WEST = 3;

class Puzzle : public olc::PixelGameEngine
{
public:

// Strings containing levels
#pragma region Levels

	// +		= any direction movable block
	// -		= horizontal movement block
	// P		= player block
	// #		= solid immovable wall block
	// integer 	= countdown block, any direction but limited number of movements as defined by integer (1 - 9)
	// @		= level completion tiles

	std::string sLevel =
		"################"
		"#..............#"
		"#...+.9..+.....#"
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
		"#.....1........#"
		"################";
#pragma endregion

// Structs containing definitions & logic for block types
#pragma region Block Types
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

		virtual void Move()
		{

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
			return direction == EAST || direction == WEST;
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
			return direction == NORTH || direction == SOUTH;
		}
	};
	struct block_countdown : public block // block that a specific number of times in any direction
	{
		int iMoveCount = 0; // number of moves the countdown block can move

		block_countdown(int iNumOfMovesAllowed)
		{
			iMoveCount = iNumOfMovesAllowed;
		}

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size) override
		{
			pge->FillRect(pos * size, size, olc::CYAN);
			pge->DrawString(pos * size + olc::vi2d(4, 4), std::to_string(iMoveCount), olc::BLACK);
		}

		bool Push(const int direction) override
		{
			return iMoveCount > 0;
		}

		void Move() override
		{
			iMoveCount--;
		}
	};
#pragma endregion

	Puzzle()
	{
		sAppName = "Sliding Blocks Puzzle Game";
	}

	// Definition for level size and block size
	olc::vf2d vLevelSize = { 16,15 };
	olc::vi2d vBlockSize = { 16,16 };

	// Vector containing level goals
	std::vector<olc::vi2d> vGoals;

	// Vector of unique pointers to the blocks making up a level
	std::vector<std::unique_ptr<block>> vLevel;

	// vector holding player location
	olc::vi2d vPlayerPos;

public:
	// Function for loading a level from string template
	void LoadLevel(int n)
	{
		// Clear existing level data
		vLevel.clear();
		vGoals.clear();

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
					vPlayerPos = { x,y };
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
				case '1':
					vLevel.emplace_back(std::make_unique<block_countdown>(1));
					break;
				case '2':
					vLevel.emplace_back(std::make_unique<block_countdown>(2));
					break;
				case '3':
					vLevel.emplace_back(std::make_unique<block_countdown>(3));
					break;
				case '4':
					vLevel.emplace_back(std::make_unique<block_countdown>(4));
					break;
				case '5':
					vLevel.emplace_back(std::make_unique<block_countdown>(5));
					break;
				case '6':
					vLevel.emplace_back(std::make_unique<block_countdown>(6));
					break;
				case '7':
					vLevel.emplace_back(std::make_unique<block_countdown>(7));
					break;
				case '8':
					vLevel.emplace_back(std::make_unique<block_countdown>(8));
					break;
				case '9':
					vLevel.emplace_back(std::make_unique<block_countdown>(9));
					break;
				case '@':
					vGoals.push_back({ x,y });
					vLevel.emplace_back(nullptr);
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

		// lambda function for translating our 2D coordinates into 1D
		auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; };

		// movement logic
		if (bPushing) // check if a push attempt is happening this frame
		{
			olc::vi2d vBlockPos = vPlayerPos; // 'cursor' to track attempted movement

			bool bAllowPush = false;
			bool bTest = true;

			// test for ability to move in a specific direction
			while (bTest)
			{
				if (vLevel[id(vBlockPos)]!= nullptr) // check target block space for nullptr
				{
					if (vLevel[id(vBlockPos)]->Push((iDirPush + 2) % 4 )) // call blocks push function to determine if it can be pushed in the direction specified
					{
						// if block is allowed to be pushed in that direction - move cursor that direction so we can check for collision in neighboring space
						switch (iDirPush) // select neighbor
						{
						case NORTH: vBlockPos.y--; break;
						case SOUTH: vBlockPos.y++; break;
						case EAST: vBlockPos.x++; break;
						case WEST: vBlockPos.x--; break;
						}
					}
					else // block cant be pushed that way -- end testing
					{
						bTest = false;
					}
				}
				else // target space was nullptr - player can move there. End testing
				{
					bAllowPush = true;
					bTest = false;
				}
			}

			if (bAllowPush) // if push is allowed - execute push logic
			{
				while (vBlockPos != vPlayerPos) // walk backwards until reaching player position from valid move location that cursor found, swapping block positions as needed
				{
					olc::vi2d vSourcePos = vBlockPos;
					switch (iDirPush)
					{
					case NORTH: vSourcePos.y++; break;
					case SOUTH: vSourcePos.y--; break;
					case EAST: vSourcePos.x--; break;
					case WEST: vSourcePos.x++; break;
					}

					if (vLevel[id(vSourcePos)] != nullptr) // check for nullptr
					{
						vLevel[id(vSourcePos)]->Move(); // call any custom move logic before actually excecuting move
					}

					std::swap(vLevel[id(vSourcePos)], vLevel[id(vBlockPos)]); // swap blocks
					vBlockPos = vSourcePos; // increment 'cursor' backwards
				}

				// update player location after movement logic loop completes
				switch (iDirPush) 
				{
				case NORTH: vPlayerPos.y--; break;
				case SOUTH: vPlayerPos.y++; break;
				case EAST: vPlayerPos.x++; break;
				case WEST: vPlayerPos.x--; break;
				}
			}
		}

		// win condition checking 
		int nGoals = 0;
		for (auto& g : vGoals)
		{
			if (vLevel[id(g)] != nullptr)
			{
				nGoals++;
			}
		}

		// Clear screen to black before drawing each frame
		Clear(olc::BLACK);

		// draw logic
		olc::vi2d vBlockPos = { 0,0 };

		for (auto& g : vGoals) // win condition drawing
		{
			FillCircle(g * vBlockSize + vBlockSize / 2, vBlockSize.x / 2 - 2, olc::YELLOW);
		}
		for (vBlockPos.y = 0; vBlockPos.y < vLevelSize.y; vBlockPos.y++) // block drawing
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

		// Goal Tracking UI
		DrawString(4, 4, "Goals: " + std::to_string(nGoals) + "/ " + std::to_string(vGoals.size()), olc::WHITE);

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