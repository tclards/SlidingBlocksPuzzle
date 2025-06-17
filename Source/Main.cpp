#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

// 1.20.50

// TODO_BUGS
// Crash on closing program - seemingly related to audio engine wave deconstructor
// Crash on moving too fast, seemingly related to movement SFX

// TODO_A
// SFX & Music 
//		- Add Pause Jingle file path and file
// Level Completion timer logic
// number of moves functionality
// Score Tracking Screen - Gold Star, Silver Star,or Bronze Star
// ReadMe File
// Create More Levels!
// Create Graphics for each Block Type. 
//		- Tilable Background Sprite for empty space?

// TODO_B
// animations for level transition
// Main Menu?
// Teleport Tile 
//		- SFX
//		- Graphics

// Controls:
// WASD or Arrow Keys to move Player Block
// R Key to Restart Level in case of softlock

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

	// Only currently supporting up to 9 win tiles per level

	// internal use, manually set, variable to track how many levels exist
	int iNumOfLevels = 2;

	// Vector containing all levels
	std::vector <std::string> vAllLevels;

	std::string sLevel_Error = // default to this if we run out of levels
		"################"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"################";

	std::string sLevel_0 = // test level - not added to allLevels vector
		"################"
		"#P.............#"
		"#..............#"
		"##.##########.##"
		"#..............#"
		"#..............#"
		"#.|...++++...|.#"
		"#..............#"
		"#.....@@@@.....#"
		"#..............#"
		"#.....____.....#"
		"#.4..........3.#"
		"#..............#"
		"#..............#"
		"################";

	float iTime_1 = 0.0f;
	int iNumOfMoves_1 = 0;
	std::string sLevel_1 = // starting here -> make sure to add any new levels to the LoadAll function that is used in OnUserCreate, and update UI for timer and move counter
		"################"
		"#P.............#"
		"#..............#"
		"##.##########.##"
		"#..............#"
		"#..............#"
		"#.|...+.++...|.#"
		"#..............#"
		"#.....@@@@.....#"
		"#..............#"
		"#.....____.....#"
		"#.2..........3.#"
		"#..............#"
		"#..............#"
		"################";

	float iTime_2 = 0.0f;
	int iNumOfMoves_2 = 0;
	std::string sLevel_2 =
		"################"
		"#P..#..........#"
		"###2#..........#"
		"#.........######"
		"#....-.+...@@@@#"
		"#.........###.##"
		"#..............#"
		"#...........5..#"
		"#..............#"
		"################"
		"################"
		"################"
		"################"
		"################"
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

// Variables & Flags
#pragma region Vars & Flags
	// Definition for level size and block size
	olc::vf2d vLevelSize = { 16,15 };
	olc::vi2d vBlockSize = { 16,16 };

	// Vector containing level goals
	std::vector<olc::vi2d> vGoals;

	// Vector of unique pointers to the blocks making up a level
	std::vector<std::unique_ptr<block>> vLevel;

	// vector holding player location
	olc::vi2d vPlayerPos;

	// Variable tracking which level the player is on
	int iCurLevel = 1;

	// Flags for pausing game
	bool bPaused = false;
	bool bPauseJinglePlayed = false;
#pragma endregion

// Audio
#pragma region Sound
	// Audio Engine
	olc::sound::WaveEngine audioEngine;
	olc::sound::WaveEngine audioEngine_Music;
	olc::sound::Wave audioSlot_Music;
	olc::sound::Wave audioSlot_SFX_WinTile;
	olc::sound::Wave audioSlot_Movement_Succeed;
	olc::sound::Wave audioSlot_Movement_Fail;
	olc::sound::Wave audioSlot_RestartLevel;
	olc::sound::Wave audioSlot_WinJingle;
	olc::sound::Wave audioSlot_LevelTransition;
	olc::sound::Wave audioSlot_PauseJingle;
	float fAudioSpeed = 1.0f;
	float fMusicVolume = 0.5f;
	float fSFXVolume = 1.0f;

	// Background Music
	std::string sBackgroundMusic_1 = "SFX//BackgroundMusic_1.wav";	

	// SFX 
	std::string sMovement_1 = "SFX//Movement_1.wav";
	std::string sMovementFailure_1 = "SFX//Movement_Failed_1.wav";
	std::string sRestartLevel_1 = "SFX//RestartLevel_1.wav";			
	std::string sWinJingle_1 = "SFX//WinJingle_1.wav";					
	std::string sLevelTransition_1 = "SFX//LevelTransition_1.wav";	
	std::string sPauseJingle_1 = "SFX//Silent.wav";
#pragma endregion

	// Constructor
	Puzzle()
	{
		sAppName = "Sliding Blocks Puzzle Game";
	}

	// Deconstructor
	~Puzzle()
	{
		audioEngine.~WaveEngine();
		audioEngine_Music.~WaveEngine();
	}

	// Function to load all audio into memory
	void LoadAllAudio()
	{
		// Initialize Audio Engines
		audioEngine_Music.InitialiseAudio();	
		audioEngine.InitialiseAudio();

		// Load Slots
		audioSlot_Music.LoadAudioWaveform(sBackgroundMusic_1);		
		audioSlot_Movement_Succeed.LoadAudioWaveform(sMovement_1);
		audioSlot_Movement_Fail.LoadAudioWaveform(sMovementFailure_1);
		audioSlot_RestartLevel.LoadAudioWaveform(sRestartLevel_1);
		audioSlot_WinJingle.LoadAudioWaveform(sWinJingle_1);
		audioSlot_LevelTransition.LoadAudioWaveform(sLevelTransition_1);
		audioSlot_PauseJingle.LoadAudioWaveform(sPauseJingle_1);									

		// Set Volume					
		audioEngine_Music.SetOutputVolume(fMusicVolume);
		audioEngine.SetOutputVolume(fSFXVolume);

		// Start background music
		audioEngine_Music.PlayWaveform(&audioSlot_Music, true, fAudioSpeed * 0.75f);
	}

	// Function to load all levels into memory
	void LoadAllLevels()
	{
		// Push back each game level
		vAllLevels.push_back(sLevel_0);
		vAllLevels.push_back(sLevel_1);
		vAllLevels.push_back(sLevel_2);

		// End cap for vector, used in End Of Game logic
		vAllLevels.push_back("End");
	}

	// Function for loading a level from string template
	void LoadLevel(int n, bool bWasRestart)
	{
		// Clear existing level data
		vLevel.clear();
		vGoals.clear();

		// check for no more levels in memory
		if (vAllLevels[iCurLevel] == "End")
		{
			iCurLevel = -1;
		}

		// Begin load level
		std::string sLevelToLoad;
		if (iCurLevel == -1)
		{
			sLevelToLoad = sLevel_Error;
		}
		else
		{
			sLevelToLoad = vAllLevels[iCurLevel];
		}

		// SFX
		if (iCurLevel == -1)
		{
			audioSlot_WinJingle.LoadAudioWaveform(sWinJingle_1);						// Load WinJingle SFX
			audioEngine.PlayWaveform(&audioSlot_WinJingle, false, fAudioSpeed);			// Play SFX
		}
		else if (bWasRestart == true)
		{
			audioSlot_RestartLevel.LoadAudioWaveform(sRestartLevel_1);					// Load Restart SFX
			audioEngine.PlayWaveform(&audioSlot_RestartLevel, false, fAudioSpeed);		// Play SFX
		}
		else
		{
			audioSlot_LevelTransition.LoadAudioWaveform(sLevelTransition_1);			// Load Level Load SFX
			audioEngine.PlayWaveform(&audioSlot_LevelTransition, false, fAudioSpeed);	// Play SFX
		}

		// iterate over level
		for (int y = 0; y < vLevelSize.y; y++)
		{
			for (int x = 0; x < vLevelSize.x; x++)
			{
				// index into 1D version of array coordinates
				switch (sLevelToLoad[y * vLevelSize.x + x])
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
		// Audio Loadind
		LoadAllAudio();

		// Level Loading
		LoadAllLevels();																						// Load ALL levels into memory
		LoadLevel(iCurLevel, false);																			// Load current level for playing
			
		return true;
	}

	// Runs every frame
	bool OnUserUpdate(float fElapsedTime) override
	{
		if (!bPaused)
		{
			// user input
			bool bPushing = false;
			int iDirPush = NORTH;
			if (iCurLevel != -1) // disable most input on end screen
			{
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
				if (GetKey(olc::Key::R).bPressed)
				{
					LoadLevel(iCurLevel, true);
				}
				if (GetKey(olc::Key::ESCAPE).bPressed && !bPaused || GetKey(olc::Key::P).bPressed) // Gameplay Pause
				{
					bPaused = true;										// Pause Game
				}
			}
			else
			{
				if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed) // Win Screen ESC
				{
					iCurLevel = 1;										// Set Level Back to 1
					audioEngine_Music.SetOutputVolume(fMusicVolume);	// Unmute Background Music
					LoadLevel(iCurLevel, false);						// Reload Level to 1

					return true;
				}
			}

			// lambda function for translating our 2D coordinates into 1D
			auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; };

			// movement logic
			bool bPlayerMoved = false;
			int iMovementSuceededOrFailed = -1; // -1 = no move, 0 = move failed, 1 = move succeeded
			if (bPushing) // check if a push attempt is happening this frame
			{
				olc::vi2d vBlockPos = vPlayerPos; // 'cursor' to track attempted movement

				bool bAllowPush = false;
				bool bTest = true;

				// test for ability to move in a specific direction
				while (bTest)
				{
					if (vLevel[id(vBlockPos)] != nullptr) // check target block space for nullptr
					{
						if (vLevel[id(vBlockPos)]->Push((iDirPush + 2) % 4)) // call blocks push function to determine if it can be pushed in the direction specified
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
							bPlayerMoved = true;
							iMovementSuceededOrFailed = 0;
							bTest = false;
						}
					}
					else // target space was nullptr - player can move there. End testing
					{
						bPlayerMoved = true;
						iMovementSuceededOrFailed = 1;
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

			// Movement SFX
			if (bPlayerMoved == true)
			{
				if (iMovementSuceededOrFailed == 0) // Movement Failed
				{
					audioEngine.PlayWaveform(&audioSlot_Movement_Fail, false, fAudioSpeed * 2.0f);
				}
				else if (iMovementSuceededOrFailed == 1) // Movement Succeeded
				{
					audioEngine.PlayWaveform(&audioSlot_Movement_Succeed, false, fAudioSpeed * 2.0f);
				}
			}

			// win condition checking 
			int nGoals = 0;
			for (auto& g : vGoals)
			{
				if (vLevel[id(g)] != nullptr)
				{
					nGoals++; // Increment Goals
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

			// UI
			if (iCurLevel != -1) // UI for active gameplay
			{
				// Goal Tracking UI
				DrawString(4, 4, "Goals: " + std::to_string(nGoals) + " / " + std::to_string(vGoals.size()), olc::WHITE);

				// Level Tracking UI
				DrawString(128, 4, "Level: " + std::to_string(iCurLevel) + " / " + std::to_string(iNumOfLevels), olc::WHITE);
			}
			else				// UI for win conditions
			{
				// Win Screen Stuff
				audioEngine_Music.SetOutputVolume(0.0f);
				DrawString((256 / 2) - 108, (240 / 2) - 96, "YOU WIN!", olc::WHITE);
				DrawString((256 / 2) - 108, (240 / 2) + 92, "Thank you for playing!", olc::WHITE);
				DrawString((256 / 2) - 108, (240 / 2) + 78, "Press ESC to restart!", olc::WHITE);
			}

			// Win Tracking
			if (nGoals >= vGoals.size() && iCurLevel != -1)
			{
				iCurLevel++;
				LoadLevel(iCurLevel, false);
			}

			return true;
		}
		else if (bPaused)
		{
			// Play SFX
			if (bPauseJinglePlayed == false)
			{
				audioEngine.PlayWaveform(&audioSlot_PauseJingle, false, fAudioSpeed);
				bPauseJinglePlayed = true;
			}

			// Pause Background Music
			audioEngine_Music.SetOutputVolume(0.0f);

			// Check for user input to unpase
			if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)
			{
				audioEngine.PlayWaveform(&audioSlot_PauseJingle, false, fAudioSpeed);	// Play Pause Jingle SFX
				audioEngine_Music.SetOutputVolume(fMusicVolume);						// Unmute Music
				bPaused = false;														// Reset Flags
				bPauseJinglePlayed = false;
			}

			// Clear screen to black before drawing each frame
			Clear(olc::BLACK);

			// Draw Pause Window
			FillRect(16, 16, this->ScreenWidth() - 32, this->ScreenHeight() - 32, olc::DARK_BLUE);
			DrawRect(16, 16, this->ScreenWidth() - 32, this->ScreenHeight() - 32, olc::WHITE);

			// Get Timer and Movement Data
			std::string sMovementUI;
			std::string sTimerUI;
			switch (iCurLevel)
			{
			case 1:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_1);
				sTimerUI = "Level Timer: " + std::to_string(iTime_1);
				break;
			case 2:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_2);
				sTimerUI = "Level Timer: " + std::to_string(iTime_2);
				break;
			default:
				break;
			}

			// Draw UI Elements
			DrawString((this->ScreenWidth() / 2) - 42, (240 / 2) - 96, "GAME PAUSED", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 92, sTimerUI, olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 78, sMovementUI, olc::WHITE);

			return true;
		}
	}
};

int main()
{
	Puzzle demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}