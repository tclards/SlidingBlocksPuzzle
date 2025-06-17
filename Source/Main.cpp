#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

// 1.20.50

// TODO_BUGS
// Crash on closing program - seemingly related to audio engine wave deconstructor

// TODO_A
// SFX & Music 
//		- Add Pause Jingle file path and file
// Level Completion timer logic
// number of moves functionality
// add difficulty level UI
// Score Tracking Screen - Gold Star, Silver Star,or Bronze Star
// ReadMe File
// Create More Levels!
// Create Graphics for each Block Type. 
//		- Tilable Background Sprite for empty space?
// create code system to skip to checkpoint levels

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
	int iNumOfLevels = 50;

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

	// starting here -> make sure to add any new levels to the LoadAll function that is used in OnUserCreate, and update UI for timer and move counter on pause menu and main UI
	// Easy Levels - 10
	float iTime_1 = 0.0f;
	int iNumOfMoves_1 = 0;
	std::string sLevel_1 = 
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

	float iTime_3 = 0.0f;	// todo
	int iNumOfMoves_3 = 0;
	std::string sLevel_3 =
		"################"
		"#P+@...........#"
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

	float iTime_4 = 0.0f;
	int iNumOfMoves_4 = 0;
	std::string sLevel_4 =
		"################"
		"#P+@...........#"
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

	float iTime_5 = 0.0f;
	int iNumOfMoves_5 = 0;
	std::string sLevel_5 =
		"################"
		"#P+@...........#"
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

	float iTime_6 = 0.0f;
	int iNumOfMoves_6 = 0;
	std::string sLevel_6 =
		"################"
		"#P+@...........#"
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

	float iTime_7 = 0.0f;
	int iNumOfMoves_7 = 0;
	std::string sLevel_7 =
		"################"
		"#P+@...........#"
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

	float iTime_8 = 0.0f;
	int iNumOfMoves_8 = 0;
	std::string sLevel_8 =
		"################"
		"#P+@...........#"
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

	float iTime_9 = 0.0f;
	int iNumOfMoves_9 = 0;
	std::string sLevel_9 =
		"################"
		"#P+@...........#"
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

	float iTime_10 = 0.0f;
	int iNumOfMoves_10 = 0;
	std::string sLevel_10 =
		"################"
		"#P+@...........#"
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

	float iTime_11 = 0.0f;
	int iNumOfMoves_11 = 0;
	std::string sLevel_11 =
		"################"
		"#P+@...........#"
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

	float iTime_12 = 0.0f;
	int iNumOfMoves_12 = 0;
	std::string sLevel_12 =
		"################"
		"#P+@...........#"
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

	float iTime_13 = 0.0f;
	int iNumOfMoves_13 = 0;
	std::string sLevel_13 =
		"################"
		"#P+@...........#"
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

	float iTime_14 = 0.0f;
	int iNumOfMoves_14 = 0;
	std::string sLevel_14 =
		"################"
		"#P+@...........#"
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

	float iTime_15 = 0.0f;
	int iNumOfMoves_15 = 0;
	std::string sLevel_15 =
		"################"
		"#P+@...........#"
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

	// Medium Levels - 20
	float iTime_16 = 0.0f;
	int iNumOfMoves_16 = 0;
	std::string sLevel_16 =
		"################"
		"#P+@...........#"
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

	float iTime_17 = 0.0f;
	int iNumOfMoves_17 = 0;
	std::string sLevel_17 =
		"################"
		"#P+@...........#"
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

	float iTime_18 = 0.0f;
	int iNumOfMoves_18 = 0;
	std::string sLevel_18 =
		"################"
		"#P+@...........#"
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

	float iTime_19 = 0.0f;
	int iNumOfMoves_19 = 0;
	std::string sLevel_19 =
		"################"
		"#P+@...........#"
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

	float iTime_20 = 0.0f;
	int iNumOfMoves_20 = 0;
	std::string sLevel_20 =
		"################"
		"#P+@...........#"
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

	float iTime_21 = 0.0f;
	int iNumOfMoves_21 = 0;
	std::string sLevel_21 =
		"################"
		"#P+@...........#"
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

	float iTime_22 = 0.0f;
	int iNumOfMoves_22 = 0;
	std::string sLevel_22 =
		"################"
		"#P+@...........#"
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

	float iTime_23 = 0.0f;
	int iNumOfMoves_23 = 0;
	std::string sLevel_23 =
		"################"
		"#P+@...........#"
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

	float iTime_24 = 0.0f;
	int iNumOfMoves_24 = 0;
	std::string sLevel_24 =
		"################"
		"#P+@...........#"
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

	float iTime_25 = 0.0f;
	int iNumOfMoves_25 = 0;
	std::string sLevel_25 =
		"################"
		"#P+@...........#"
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

	float iTime_26 = 0.0f;
	int iNumOfMoves_26 = 0;
	std::string sLevel_26 =
		"################"
		"#P+@...........#"
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

	float iTime_27 = 0.0f;
	int iNumOfMoves_27 = 0;
	std::string sLevel_27 =
		"################"
		"#P+@...........#"
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

	float iTime_28 = 0.0f;
	int iNumOfMoves_28 = 0;
	std::string sLevel_28 =
		"################"
		"#P+@...........#"
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

	float iTime_29 = 0.0f;
	int iNumOfMoves_29 = 0;
	std::string sLevel_29 =
		"################"
		"#P+@...........#"
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

	float iTime_30 = 0.0f;
	int iNumOfMoves_30 = 0;
	std::string sLevel_30 =
		"################"
		"#P+@...........#"
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

	float iTime_31 = 0.0f;
	int iNumOfMoves_31 = 0;
	std::string sLevel_31 =
		"################"
		"#P+@...........#"
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

	float iTime_32 = 0.0f;
	int iNumOfMoves_32 = 0;
	std::string sLevel_32 =
		"################"
		"#P+@...........#"
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

	float iTime_33 = 0.0f;
	int iNumOfMoves_33 = 0;
	std::string sLevel_33 =
		"################"
		"#P+@...........#"
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

	float iTime_34 = 0.0f;
	int iNumOfMoves_34 = 0;
	std::string sLevel_34 =
		"################"
		"#P+@...........#"
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

	float iTime_35 = 0.0f;
	int iNumOfMoves_35 = 0;
	std::string sLevel_35 =
		"################"
		"#P+@...........#"
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

	// Hard Levels - 15
	float iTime_36 = 0.0f;
	int iNumOfMoves_36 = 0;
	std::string sLevel_36 =
		"################"
		"#P+@...........#"
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

	float iTime_37 = 0.0f;
	int iNumOfMoves_37 = 0;
	std::string sLevel_37 =
		"################"
		"#P+@...........#"
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

	float iTime_38 = 0.0f;
	int iNumOfMoves_38 = 0;
	std::string sLevel_38 =
		"################"
		"#P+@...........#"
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

	float iTime_39 = 0.0f;
	int iNumOfMoves_39 = 0;
	std::string sLevel_39 =
		"################"
		"#P+@...........#"
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

	float iTime_40 = 0.0f;
	int iNumOfMoves_40 = 0;
	std::string sLevel_40 =
		"################"
		"#P+@...........#"
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

	float iTime_41 = 0.0f;
	int iNumOfMoves_41 = 0;
	std::string sLevel_41 =
		"################"
		"#P+@...........#"
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

	float iTime_42 = 0.0f;
	int iNumOfMoves_42 = 0;
	std::string sLevel_42 =
		"################"
		"#P+@...........#"
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

	float iTime_43 = 0.0f;
	int iNumOfMoves_43 = 0;
	std::string sLevel_43 =
		"################"
		"#P+@...........#"
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

	float iTime_44 = 0.0f;
	int iNumOfMoves_44 = 0;
	std::string sLevel_44 =
		"################"
		"#P+@...........#"
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

	float iTime_45 = 0.0f;
	int iNumOfMoves_45 = 0;
	std::string sLevel_45 =
		"################"
		"#P+@...........#"
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

	float iTime_46 = 0.0f;
	int iNumOfMoves_46 = 0;
	std::string sLevel_46 =
		"################"
		"#P+@...........#"
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

	float iTime_47 = 0.0f;
	int iNumOfMoves_47 = 0;
	std::string sLevel_47 =
		"################"
		"#P+@...........#"
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

	float iTime_48 = 0.0f;
	int iNumOfMoves_48 = 0;
	std::string sLevel_48 =
		"################"
		"#P+@...........#"
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

	float iTime_49 = 0.0f;
	int iNumOfMoves_49 = 0;
	std::string sLevel_49 =
		"################"
		"#P+@...........#"
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

	float iTime_50 = 0.0f;
	int iNumOfMoves_50 = 0;
	std::string sLevel_50 =
		"################"
		"#P+@...........#"
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
		audioEngine.StopAll();
		audioEngine_Music.StopAll();
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
		vAllLevels.push_back(sLevel_3);
		vAllLevels.push_back(sLevel_4);
		vAllLevels.push_back(sLevel_5);
		vAllLevels.push_back(sLevel_6);
		vAllLevels.push_back(sLevel_7);
		vAllLevels.push_back(sLevel_8);
		vAllLevels.push_back(sLevel_9);
		vAllLevels.push_back(sLevel_10);
		vAllLevels.push_back(sLevel_11);
		vAllLevels.push_back(sLevel_12);
		vAllLevels.push_back(sLevel_13);
		vAllLevels.push_back(sLevel_14);
		vAllLevels.push_back(sLevel_15);
		vAllLevels.push_back(sLevel_16);
		vAllLevels.push_back(sLevel_17);
		vAllLevels.push_back(sLevel_18);
		vAllLevels.push_back(sLevel_19);
		vAllLevels.push_back(sLevel_20);
		vAllLevels.push_back(sLevel_21);
		vAllLevels.push_back(sLevel_22);
		vAllLevels.push_back(sLevel_23);
		vAllLevels.push_back(sLevel_24);
		vAllLevels.push_back(sLevel_25);
		vAllLevels.push_back(sLevel_26);
		vAllLevels.push_back(sLevel_27);
		vAllLevels.push_back(sLevel_28);
		vAllLevels.push_back(sLevel_29);
		vAllLevels.push_back(sLevel_30);
		vAllLevels.push_back(sLevel_31);
		vAllLevels.push_back(sLevel_32);
		vAllLevels.push_back(sLevel_33);
		vAllLevels.push_back(sLevel_34);
		vAllLevels.push_back(sLevel_35);
		vAllLevels.push_back(sLevel_36);
		vAllLevels.push_back(sLevel_37);
		vAllLevels.push_back(sLevel_38);
		vAllLevels.push_back(sLevel_39);
		vAllLevels.push_back(sLevel_40);
		vAllLevels.push_back(sLevel_41);
		vAllLevels.push_back(sLevel_42);
		vAllLevels.push_back(sLevel_43);
		vAllLevels.push_back(sLevel_44);
		vAllLevels.push_back(sLevel_45);
		vAllLevels.push_back(sLevel_46);
		vAllLevels.push_back(sLevel_47);
		vAllLevels.push_back(sLevel_48);
		vAllLevels.push_back(sLevel_49);
		vAllLevels.push_back(sLevel_50);

		// End cap for vector, used in End Of Game logic
		vAllLevels.push_back("End");
	}

	// Function for loading a level from string template
	void LoadLevel(int n, bool bWasRestart)
	{
		// Clear existing level data
		vLevel.clear();
		vGoals.clear();

		// reset audio
		audioEngine.StopAll();
		audioEngine_Music.StopAll();

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

		// restart music
		audioEngine_Music.PlayWaveform(&audioSlot_Music, true, fAudioSpeed * 0.75f);
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
			case 3:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_3);
				sTimerUI = "Level Timer: " + std::to_string(iTime_3);
				break;
			case 4:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_4);
				sTimerUI = "Level Timer: " + std::to_string(iTime_4);
				break;
			case 5:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_5);
				sTimerUI = "Level Timer: " + std::to_string(iTime_5);
				break;
			case 6:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_6);
				sTimerUI = "Level Timer: " + std::to_string(iTime_6);
				break;
			case 7:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_7);
				sTimerUI = "Level Timer: " + std::to_string(iTime_7);
				break;
			case 8:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_8);
				sTimerUI = "Level Timer: " + std::to_string(iTime_8);
				break;
			case 9:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_9);
				sTimerUI = "Level Timer: " + std::to_string(iTime_9);
				break;
			case 10:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_10);
				sTimerUI = "Level Timer: " + std::to_string(iTime_10);
				break;
			case 11:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_11);
				sTimerUI = "Level Timer: " + std::to_string(iTime_11);
				break;
			case 12:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_12);
				sTimerUI = "Level Timer: " + std::to_string(iTime_12);
				break;
			case 13:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_13);
				sTimerUI = "Level Timer: " + std::to_string(iTime_13);
				break;
			case 14:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_14);
				sTimerUI = "Level Timer: " + std::to_string(iTime_14);
				break;
			case 15:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_15);
				sTimerUI = "Level Timer: " + std::to_string(iTime_15);
				break;
			case 16:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_16);
				sTimerUI = "Level Timer: " + std::to_string(iTime_16);
				break;
			case 17:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_17);
				sTimerUI = "Level Timer: " + std::to_string(iTime_17);
				break;
			case 18:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_18);
				sTimerUI = "Level Timer: " + std::to_string(iTime_18);
				break;
			case 19:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_19);
				sTimerUI = "Level Timer: " + std::to_string(iTime_19);
				break;
			case 20:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_20);
				sTimerUI = "Level Timer: " + std::to_string(iTime_20);
				break;
			case 21:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_21);
				sTimerUI = "Level Timer: " + std::to_string(iTime_21);
				break;
			case 22:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_22);
				sTimerUI = "Level Timer: " + std::to_string(iTime_22);
				break;
			case 23:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_23);
				sTimerUI = "Level Timer: " + std::to_string(iTime_23);
				break;
			case 24:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_24);
				sTimerUI = "Level Timer: " + std::to_string(iTime_24);
				break;
			case 25:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_25);
				sTimerUI = "Level Timer: " + std::to_string(iTime_25);
				break;
			case 26:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_26);
				sTimerUI = "Level Timer: " + std::to_string(iTime_26);
				break;
			case 27:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_27);
				sTimerUI = "Level Timer: " + std::to_string(iTime_27);
				break;
			case 28:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_28);
				sTimerUI = "Level Timer: " + std::to_string(iTime_28);
				break;
			case 29:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_29);
				sTimerUI = "Level Timer: " + std::to_string(iTime_29);
				break;
			case 30:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_30);
				sTimerUI = "Level Timer: " + std::to_string(iTime_30);
				break;
			case 31:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_31);
				sTimerUI = "Level Timer: " + std::to_string(iTime_31);
				break;
			case 32:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_32);
				sTimerUI = "Level Timer: " + std::to_string(iTime_32);
				break;
			case 33:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_33);
				sTimerUI = "Level Timer: " + std::to_string(iTime_33);
				break;
			case 34:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_34);
				sTimerUI = "Level Timer: " + std::to_string(iTime_34);
				break;
			case 35:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_35);
				sTimerUI = "Level Timer: " + std::to_string(iTime_35);
				break;
			case 36:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_36);
				sTimerUI = "Level Timer: " + std::to_string(iTime_36);
				break;
			case 37:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_37);
				sTimerUI = "Level Timer: " + std::to_string(iTime_37);
				break;
			case 38:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_38);
				sTimerUI = "Level Timer: " + std::to_string(iTime_38);
				break;
			case 39:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_39);
				sTimerUI = "Level Timer: " + std::to_string(iTime_39);
				break;
			case 40:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_40);
				sTimerUI = "Level Timer: " + std::to_string(iTime_40);
				break;
			case 41:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_41);
				sTimerUI = "Level Timer: " + std::to_string(iTime_41);
				break;
			case 42:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_42);
				sTimerUI = "Level Timer: " + std::to_string(iTime_42);
				break;
			case 43:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_43);
				sTimerUI = "Level Timer: " + std::to_string(iTime_43);
				break;
			case 44:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_44);
				sTimerUI = "Level Timer: " + std::to_string(iTime_44);
				break;
			case 45:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_45);
				sTimerUI = "Level Timer: " + std::to_string(iTime_45);
				break;
			case 46:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_46);
				sTimerUI = "Level Timer: " + std::to_string(iTime_46);
				break;
			case 47:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_47);
				sTimerUI = "Level Timer: " + std::to_string(iTime_47);
				break;
			case 48:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_48);
				sTimerUI = "Level Timer: " + std::to_string(iTime_48);
				break;
			case 49:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_49);
				sTimerUI = "Level Timer: " + std::to_string(iTime_49);
				break;
			case 50:
				sMovementUI = "Moves: " + std::to_string(iNumOfMoves_50);
				sTimerUI = "Level Timer: " + std::to_string(iTime_50);
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