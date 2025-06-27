#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"
#include <fstream>
#include <cmath>

// TODO_BUGS
// Crash on closing program - seemingly related to audio engine wave deconstructor
// Level Code Succeed SFX not audible
// random crashes related to a nullptr in soundwaveengine, usually while player is moving
// level timer UI clips with level checkpoint UI

// TODO_A
// 
// change debug H to level scores not high scores
// 
// Fill out Levels!
// 
// update level skip codes

// Controls:
// WASD or Arrow Keys to move Player Block
// R Key to Restart Level in case of softlock
// ESC or P to Pause
// O - Options Menu 
// I - Score Tracking Screen 
// C - Cheat Code Skip 
// CTRL + ENTER and MINUS - Enable Debug Mode
// N (While Debug Enabled) - Next Level
// H (While Debug Enabled) - Reset High Scores
// V (While Debug Enabled) - Open & Close Doors

// Created by Tyler Clardy in June 2025
// Thanks to oneLoneCoder for the PixelGameEngine and SoundWaveEngine, as well as the inspiration for the start of the project. You rock, Javid!

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
	// D		= door
	// S		= door switch
	// B		= Teleport Blue
	// O		= Teleport Orange

	// Only currently supporting up to 9 win tiles per level
	// Only currently supporting up to 9 door switches per level
	// Only currently supporting 1 set of teleport tiles per level - ie 1 'O' and 1 'B'

	// Once ALL door switches are covered ALL doors in the level will open

	// Teleporters accept Players but not blocks, and work both ways; i.eorange to blue and ALSO blue to orange

	// internal use, manually set, variable to track how many levels exist
	int iNumOfLevels = 50;

	// Codes for Skipping Levels
	std::string sMediumLevelCode = "MEDIUM"; // Skip to Level 16
	std::string sHardLevelCode = "HARD";   // Skip to Level 36

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
		"#.....-..-.....#"
		"#..............#"
		"#.|...+.++...|.#"
		"#.............S#"
		"#.....@@@@.....#"
		"#.............S#"
		"#.....____.....#"
		"#.2..........3.#"
		"#######DD#######"
		"#B............O#"
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

	float iTime_3 = 0.0f;
	int iNumOfMoves_3 = 0;
	std::string sLevel_3 =
		"################"
		"#@...-........@#"
		"#...######.#####"
		"#............@@#"
		"#|#.############"
		"#.#....++......#"
		"#.#............#"
		"#.#............#"
		"#.#............#"
		"#.#............#"
		"#.#............#"
		"#.#............#"
		"#.#............#"
		"#@.....P.-....@#"
		"################";

	float iTime_4 = 0.0f;
	int iNumOfMoves_4 = 0;
	std::string sLevel_4 =
		"################"
		"#@............P#"
		"####...........#"
		"#.7............#"
		"#.......@@@....#"
		"#....###########"
		"#..-.....@...@@#"
		"#.9..####.######"
		"#..............#"
		"#........|.....#"
		"#...+..........#"
		"#..............#"
		"#.......+......#"
		"#..............#"
		"################";

	float iTime_5 = 0.0f;
	int iNumOfMoves_5 = 0;
	std::string sLevel_5 =
		"################"
		"#...@@.........#"
		"#..............#"
		"#...3..........#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#........P..+..#"
		"#..............#"
		"#..............#"
		"###@...........#"
		"##@#...6.......#"
		"#@.........-...#"
		"################";

	float iTime_6 = 0.0f;
	int iNumOfMoves_6 = 0;
	std::string sLevel_6 =
		"################"
		"#P......@-...@@#"
		"############.#.#"
		"#..............#"
		"#......+....5..#"
		"#..............#"
		"#########.######"
		"###.......######"
		"###.#####.######"
		"###.#####.######"
		"###.#####.######"
		"###.....9.######"
		"#########..#####"
		"###@.......#####"
		"################";

	float iTime_7 = 0.0f;
	int iNumOfMoves_7 = 0;
	std::string sLevel_7 =
		"################"
		"#@1.....##....@#"
		"#######.##.#####"
		"#.............@#"
		"#..........@.###"
		"#......9.....#@#"
		"#.....5P5......#"
		"#@#....9.......#"
		"###.########.###"
		"###.########.###"
		"###.########.###"
		"###.########.###"
		"###.....+....###"
		"###..######..###"
		"################";

	float iTime_8 = 0.0f;
	int iNumOfMoves_8 = 0;
	std::string sLevel_8 =
		"################"
		"#P...-@@@@...-.#"
		"#..............#"
		"#..............#"
		"#......3|......#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#.......4......#"
		"#..............#"
		"#......|.......#"
		"#..............#"
		"#..-..@@@@.....#"
		"################";

	float iTime_9 = 0.0f;
	int iNumOfMoves_9 = 0;
	std::string sLevel_9 = // todo
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
		"#...-.+.|.5....#"
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
		"#...-.+.|.5....#"
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
		bool bCanTeleport = false;

		block()
		{
			
		}

		virtual void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet)
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
		bool bCanTeleport = false;

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			switch (iLevelSet)
			{
			case 0:		// easy
				pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(1, 1) * size, size);
				break;
			case 1:		// medium
				pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(2, 1) * size, size);
				break;
			case 2:		// hard
				pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(3, 1) * size, size);
				break;
			default:	// menu
				pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(0, 0) * size, size);
				break;
			}
		}

		bool Push(const int direction) override
		{
			return false;
		}
	};
	struct block_player : public block // player block
	{
		bool bCanTeleport = true;

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(0, 1) * size, size);
		}

		bool Push(const int direction) override
		{
			return true;
		}
	};
	struct block_simple : public block // block that moves in any direction
	{
		bool bCanTeleport = false;

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(1, 0) * size, size);
		}

		bool Push(const int direction) override
		{
			return true;
		}
	};
	struct block_horizontal : public block // block that moves side to side
	{
		bool bCanTeleport = false;

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(2, 0) * size, size);
		}

		bool Push(const int direction) override
		{
			return direction == EAST || direction == WEST;
		}
	};
	struct block_vertical : public block // block that moves up and down
	{
		bool bCanTeleport = false;

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(3, 0) * size, size);
		}

		bool Push(const int direction) override
		{
			return direction == NORTH || direction == SOUTH;
		}
	};
	struct block_countdown : public block // block that a specific number of times in any direction
	{
		bool bCanTeleport = false;

		int iMoveCount = 0; // number of moves the countdown block can move

		block_countdown(int iNumOfMovesAllowed)
		{
			iMoveCount = iNumOfMovesAllowed;
		}

		void DrawSelf(olc::PixelGameEngine* pge, const olc::vi2d& pos, olc::vi2d& size, const olc::Renderable& skin, int iLevelSet) override
		{
			pge->DrawPartialSprite(pos * size, skin.Sprite(), olc::vi2d(4, 0) * size, size);
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

	// Vector containing door positions
	std::vector<olc::vi2d> vDoors_pos;

	// Vector containing door switches
	std::vector<olc::vi2d> vSwitches;

	// Vector of unique pointers to the blocks making up a level
	std::vector<std::unique_ptr<block>> vLevel;

	// Variables holding the teleport tile locations for any loaded level
	olc::vi2d vTele_Blue;
	olc::vi2d vTele_Orange;

	// Bool indicating if a level has teleports
	bool bLevelHasTeleports = false;

	// int determining the facing of the teleport graphics. 0 through 3 indicating the 4 ways
	int iTele_Facing = 0;

	// Timer for executing teleport rotate
	float fTele_GFX_Flip = 0.0f;
	float fTele_RotateSpeed = 0.35f;

	// Variables for Button Click SFX
	bool bPlayButtonSFX_1 = false;
	bool bPlayButtonSFX_2 = false;

	// Variables for switch tracking - door switches and win tiles
	int nSwitches = 0;
	int nGoals = 0;

	// vector holding player location
	olc::vi2d vPlayerPos;

	// Graphics
	olc::Renderable gfxTiles;
	olc::Renderable gfxWin;
	olc::Renderable gfxSplash;

	// Flag for enabling and disabling input
	bool bEnableInput = true;

	// Flag for controlling door behaivor
	bool bDoorsOpen = false;
	bool bDoors_DebugForceOpen = false;

	// Variable tracking which level the player is on
	int iCurLevel = 1;

	// Flags for pausing game
	bool bPaused = false;
	bool bPauseJinglePlayed = false;

	// Main Menu Flags
	bool bMainMenu = true;
	int iCurDisplay = -1; // - 1: main menu, 0: high score screen, 1: options screen, 2: Level Select Screen

	// Page number variable used in some menus
	int iPageNum = 1;

	// Variables for checking success on file io operations
	int iOptionsSave = 0;
	int iOptionsLoad = 0;
	int iHighScoreSave = 0;
	int iHighScoreLoad = 0;

	// Variables storing total High Score Values for display in main menu
	float fTotal_HS = 0;
	int iTotal_HS = 0;

	// Time Variable for Win Screen 
	float fTime_WinScreen = 0.0f;

	// vector storing High Scores
	std::vector<int> vHighScore_Moves = {0};
	std::vector<float> vHighScore_Time = {0};

	// Flag for Debug Testing Mode
	bool bDebugMode = false;

	// Variable storing current level set
	int iLevelSet = 0; // -1 = menu, 0 = easy, 1 = medium, 2 = hard

	// Text Input for Level Select
	std::string sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";

	// Flags and Vars for Start Up Routine
	bool bGameStarted = false;
	bool bDoStartUpJingle = true;
	olc::sound::PlayingWave pwStart; // Variable to store a Reference to the StartUp Jingle
	float fStartTime = 0.0f;
	double dDuration = 0.0f;
	float fTarget = 0.0f;
	float fTimer = 0.0f;

	// Flag for Background Music
	bool bDoBackgroundMusic = false;

	// Speed for Start Up Jingle - This determines how long the Splash Screen is displayed
	float fSplashScreenSpeed = 1.25f;
#pragma endregion

// Audio
#pragma region Sound
	// Audio Engine
	olc::sound::WaveEngine audioEngine;
	olc::sound::WaveEngine audioEngine_Music;
	olc::sound::Wave audioSlot_Music_1;
	olc::sound::Wave audioSlot_Music_2;
	olc::sound::Wave audioSlot_Music_3;
	olc::sound::Wave audioSlot_Music_Menu;
	olc::sound::Wave audioSlot_SFX_WinTile;
	olc::sound::Wave audioSlot_Movement_Succeed;
	olc::sound::Wave audioSlot_Movement_Fail;
	olc::sound::Wave audioSlot_RestartLevel;
	olc::sound::Wave audioSlot_WinJingle;
	olc::sound::Wave audioSlot_LevelTransition;
	olc::sound::Wave audioSlot_PauseJingle;
	olc::sound::Wave audioSlot_UnPauseJingle;
	olc::sound::Wave audioSlot_LevelCode_Succeed;
	olc::sound::Wave audioSlot_LevelCode_Fail;
	olc::sound::Wave audioSlot_GameStartUp;
	olc::sound::Wave audioSlot_DoorOpen;
	olc::sound::Wave audioSlot_DoorClose;
	olc::sound::Wave audioSlot_Teleport_Fail;
	olc::sound::Wave audioSlot_Teleport_Succeed;
	olc::sound::Wave audioSlot_ButtonClick_1;
	olc::sound::Wave audioSlot_ButtonClick_2;

	float fAudioSpeed = 1.0f;
	float fMusicVolume = 0.3f;
	float fSFXVolume = 0.2f;

	// Background Music
	std::string sBackgroundMusic_1 = "SFX//BackgroundMusic_1.wav";	
	std::string sBackgroundMusic_2 = "SFX//BackgroundMusic_2.wav";
	std::string sBackgroundMusic_3 = "SFX//BackgroundMusic_3.wav";
	std::string sBackgroundMusic_Menu = "SFX//MenuMusic_1.wav";

	// SFX 
	std::string sMovement_1 = "SFX//Movement_1.wav";
	std::string sMovementFailure_1 = "SFX//Movement_Failed_1.wav";
	std::string sRestartLevel_1 = "SFX//RestartLevel_1.wav";			
	std::string sWinJingle_1 = "SFX//WinJingle_1.wav";					
	std::string sLevelTransition_1 = "SFX//LevelTransition_1.wav";	
	std::string sPauseJingle_1 = "SFX//Pause_1.wav";
	std::string sUnPauseJingle_1 = "SFX//Unpause_1.wav";
	std::string sLevelCode_1 = "SFX//InputCode_Succeed_1.wav";		
	std::string sLevelCodeFail_1 = "SFX//InputCode_Fail_1.wav";	
	std::string sGameStartUp = "SFX//GameStartup_1.wav";
	std::string sDoorOpen_1 = "SFX//door_Open.wav";
	std::string sDoorClose_1 = "SFX//door_Close.wav";
	std::string sTeleport_1 = "SFX//teleport_Succeed.wav";
	std::string sTeleportFailure_1 = "SFX//teleport_Fail.wav";
	std::string sButtonClick_1 = "SFX//buttonClick.wav";
	std::string sButtonClick_2 = "SFX//buttonClick_2.wav";
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

	// Utility function to truncate a float to 2 decimal places 
	float TruncateFloat(float fIn)
	{
		float fScaled = fIn * 100.0f;

		float fTruncatedScaled = std::trunc(fScaled);

		float fTruncated = fTruncatedScaled / 100.0f;

		return fTruncated;
	}

	// Utility function to convert float to string with only 2 decimal places
	std::string FloatToString(float fIn)
	{
		std::stringstream ss;

		ss << std::fixed << std::setprecision(2) << fIn;

		return ss.str();
	}

	// Utility function for rotating teleport tiles
	void DoTeleportFlipCheck(float fElapsedTime)
	{
		fTele_GFX_Flip += fElapsedTime;

		// if timing threshold has been reached, reset timer and execute rotate operation
		if (fTele_GFX_Flip >= fTele_RotateSpeed)
		{
			fTele_GFX_Flip = 0.0f;

			iTele_Facing++;

			if (iTele_Facing == 4) // clamp values to the range of 0 - 4
			{
				iTele_Facing = 0;
			}
		}
	}

	// Function for loading options settings
	int LoadOptions() // returns -1 for failure, or 1 for success
	{
		std::ifstream inOptions("Options.txt");

		if (inOptions.is_open())
		{
			std::string sCurLine;
			bool bSFX = false;
			bool bMusic = false;

			while (std::getline(inOptions, sCurLine))
			{
				if (bSFX == true)
				{
					fSFXVolume = std::stof(sCurLine);
					bSFX = false;
				}
				else if (bMusic == true)
				{
					fMusicVolume = std::stof(sCurLine);
					bMusic = false;
				}

				if (sCurLine == "{Volume_SFX}=")
				{
					bSFX = true;
				}
				else if (sCurLine == "{Volume_Music}=")
				{
					bMusic = true;
				}
			}

			inOptions.close();
			return 1;
		}
		else
		{
			return -1;
		}

		return -1;
	}

	// Function for setting options settings
	int SaveOptions() // returns -1 for failure, or 1 for success
	{
		std::ofstream outOptions("Options.txt");
		outOptions.clear();

		if (outOptions.is_open())
		{
			outOptions << "{Volume_SFX}=\n" << fSFXVolume << "\n";
			outOptions << "{Volume_Music}=\n" << fMusicVolume;

			outOptions.close();
			return 1;
		}
		else
		{
			return -1;
		}

		return -1;
	}

	// Function for loading High Scores
	// Populates the vHighScore_Moves and vHighScore_Time vectors with data
	int LoadHighScores() // returns -1 for failure, or 1 for success
	{
		// Open Streams
		std::ifstream inHighScores_Moves("HighScores_Moves.txt");
		std::ifstream inHighScores_Time("HighScores_Time.txt");

		// Check for streams opening correctly
		if (inHighScores_Moves.is_open() && inHighScores_Time.is_open())
		{
			// Clear vectors
			vHighScore_Moves.clear();
			vHighScore_Time.clear();

			std::string sCurLine;
			int iLineCount = 0;

			// read move data into vector
			while (std::getline(inHighScores_Moves, sCurLine))
			{
				vHighScore_Moves.push_back(std::stoi(sCurLine));
			}

			iLineCount = 0;

			// read time data into vector
			while (std::getline(inHighScores_Time, sCurLine))
			{
				vHighScore_Time.push_back(std::stof(sCurLine));
			}

			// close streams
			inHighScores_Moves.close();
			inHighScores_Time.close();

			return 1;
		}
		else
		{
			return -1;
		}

		return -1;
	}

	// Checks if new Time score is better or worse than saved one, then saves to vector if better
	void internalHighScoreUtility_Moves()
	{
		if (vHighScore_Moves[0] > iNumOfMoves_1 && iNumOfMoves_1 != 0) vHighScore_Moves[0] = iNumOfMoves_1;
		if (vHighScore_Moves[1] > iNumOfMoves_2 && iNumOfMoves_2 != 0) vHighScore_Moves[1] = iNumOfMoves_2;
		if (vHighScore_Moves[2] > iNumOfMoves_3 && iNumOfMoves_3 != 0) vHighScore_Moves[2] = iNumOfMoves_3;
		if (vHighScore_Moves[3] > iNumOfMoves_4 && iNumOfMoves_4 != 0) vHighScore_Moves[3] = iNumOfMoves_4;
		if (vHighScore_Moves[4] > iNumOfMoves_5 && iNumOfMoves_5 != 0) vHighScore_Moves[4] = iNumOfMoves_5;
		if (vHighScore_Moves[5] > iNumOfMoves_6 && iNumOfMoves_6 != 0) vHighScore_Moves[5] = iNumOfMoves_6;
		if (vHighScore_Moves[6] > iNumOfMoves_7 && iNumOfMoves_7 != 0) vHighScore_Moves[6] = iNumOfMoves_7;
		if (vHighScore_Moves[7] > iNumOfMoves_8 && iNumOfMoves_8 != 0) vHighScore_Moves[7] = iNumOfMoves_8;
		if (vHighScore_Moves[8] > iNumOfMoves_9 && iNumOfMoves_9 != 0) vHighScore_Moves[8] = iNumOfMoves_9;
		if (vHighScore_Moves[9] > iNumOfMoves_10 && iNumOfMoves_10 != 0) vHighScore_Moves[9] = iNumOfMoves_10;
		if (vHighScore_Moves[10] > iNumOfMoves_11 && iNumOfMoves_11 != 0) vHighScore_Moves[10] = iNumOfMoves_11;
		if (vHighScore_Moves[11] > iNumOfMoves_12 && iNumOfMoves_12 != 0) vHighScore_Moves[11] = iNumOfMoves_12;
		if (vHighScore_Moves[12] > iNumOfMoves_13 && iNumOfMoves_13 != 0) vHighScore_Moves[12] = iNumOfMoves_13;
		if (vHighScore_Moves[13] > iNumOfMoves_14 && iNumOfMoves_14 != 0) vHighScore_Moves[13] = iNumOfMoves_14;
		if (vHighScore_Moves[14] > iNumOfMoves_15 && iNumOfMoves_15 != 0) vHighScore_Moves[14] = iNumOfMoves_15;
		if (vHighScore_Moves[15] > iNumOfMoves_16 && iNumOfMoves_16 != 0) vHighScore_Moves[15] = iNumOfMoves_16;
		if (vHighScore_Moves[16] > iNumOfMoves_17 && iNumOfMoves_17 != 0) vHighScore_Moves[16] = iNumOfMoves_17;
		if (vHighScore_Moves[17] > iNumOfMoves_18 && iNumOfMoves_18 != 0) vHighScore_Moves[17] = iNumOfMoves_18;
		if (vHighScore_Moves[18] > iNumOfMoves_19 && iNumOfMoves_19 != 0) vHighScore_Moves[18] = iNumOfMoves_19;
		if (vHighScore_Moves[19] > iNumOfMoves_20 && iNumOfMoves_20 != 0) vHighScore_Moves[19] = iNumOfMoves_20;
		if (vHighScore_Moves[20] > iNumOfMoves_21 && iNumOfMoves_21 != 0) vHighScore_Moves[20] = iNumOfMoves_21;
		if (vHighScore_Moves[21] > iNumOfMoves_22 && iNumOfMoves_22 != 0) vHighScore_Moves[21] = iNumOfMoves_22;
		if (vHighScore_Moves[22] > iNumOfMoves_23 && iNumOfMoves_23 != 0) vHighScore_Moves[22] = iNumOfMoves_23;
		if (vHighScore_Moves[23] > iNumOfMoves_24 && iNumOfMoves_24 != 0) vHighScore_Moves[23] = iNumOfMoves_24;
		if (vHighScore_Moves[24] > iNumOfMoves_25 && iNumOfMoves_25 != 0) vHighScore_Moves[24] = iNumOfMoves_25;
		if (vHighScore_Moves[25] > iNumOfMoves_26 && iNumOfMoves_26 != 0) vHighScore_Moves[25] = iNumOfMoves_26;
		if (vHighScore_Moves[26] > iNumOfMoves_27 && iNumOfMoves_27 != 0) vHighScore_Moves[26] = iNumOfMoves_27;
		if (vHighScore_Moves[27] > iNumOfMoves_28 && iNumOfMoves_28 != 0) vHighScore_Moves[27] = iNumOfMoves_28;
		if (vHighScore_Moves[28] > iNumOfMoves_29 && iNumOfMoves_29 != 0) vHighScore_Moves[28] = iNumOfMoves_29;
		if (vHighScore_Moves[29] > iNumOfMoves_30 && iNumOfMoves_30 != 0) vHighScore_Moves[29] = iNumOfMoves_30;
		if (vHighScore_Moves[30] > iNumOfMoves_31 && iNumOfMoves_31 != 0) vHighScore_Moves[30] = iNumOfMoves_31;
		if (vHighScore_Moves[31] > iNumOfMoves_32 && iNumOfMoves_32 != 0) vHighScore_Moves[31] = iNumOfMoves_32;
		if (vHighScore_Moves[32] > iNumOfMoves_33 && iNumOfMoves_33 != 0) vHighScore_Moves[32] = iNumOfMoves_33;
		if (vHighScore_Moves[33] > iNumOfMoves_34 && iNumOfMoves_34 != 0) vHighScore_Moves[33] = iNumOfMoves_34;
		if (vHighScore_Moves[34] > iNumOfMoves_35 && iNumOfMoves_35 != 0) vHighScore_Moves[34] = iNumOfMoves_35;
		if (vHighScore_Moves[35] > iNumOfMoves_36 && iNumOfMoves_36 != 0) vHighScore_Moves[35] = iNumOfMoves_36;
		if (vHighScore_Moves[36] > iNumOfMoves_37 && iNumOfMoves_37 != 0) vHighScore_Moves[36] = iNumOfMoves_37;
		if (vHighScore_Moves[37] > iNumOfMoves_38 && iNumOfMoves_38 != 0) vHighScore_Moves[37] = iNumOfMoves_38;
		if (vHighScore_Moves[38] > iNumOfMoves_39 && iNumOfMoves_39 != 0) vHighScore_Moves[38] = iNumOfMoves_39;
		if (vHighScore_Moves[39] > iNumOfMoves_40 && iNumOfMoves_40 != 0) vHighScore_Moves[39] = iNumOfMoves_40;
		if (vHighScore_Moves[40] > iNumOfMoves_41 && iNumOfMoves_41 != 0) vHighScore_Moves[40] = iNumOfMoves_41;
		if (vHighScore_Moves[41] > iNumOfMoves_42 && iNumOfMoves_42 != 0) vHighScore_Moves[41] = iNumOfMoves_42;
		if (vHighScore_Moves[42] > iNumOfMoves_43 && iNumOfMoves_43 != 0) vHighScore_Moves[42] = iNumOfMoves_43;
		if (vHighScore_Moves[43] > iNumOfMoves_44 && iNumOfMoves_44 != 0) vHighScore_Moves[43] = iNumOfMoves_44;
		if (vHighScore_Moves[44] > iNumOfMoves_45 && iNumOfMoves_45 != 0) vHighScore_Moves[44] = iNumOfMoves_45;
		if (vHighScore_Moves[45] > iNumOfMoves_46 && iNumOfMoves_46 != 0) vHighScore_Moves[45] = iNumOfMoves_46;
		if (vHighScore_Moves[46] > iNumOfMoves_47 && iNumOfMoves_47 != 0) vHighScore_Moves[46] = iNumOfMoves_47;
		if (vHighScore_Moves[47] > iNumOfMoves_48 && iNumOfMoves_48 != 0) vHighScore_Moves[47] = iNumOfMoves_48;
		if (vHighScore_Moves[48] > iNumOfMoves_49 && iNumOfMoves_49 != 0) vHighScore_Moves[48] = iNumOfMoves_49;
		if (vHighScore_Moves[49] > iNumOfMoves_50 && iNumOfMoves_50 != 0) vHighScore_Moves[49] = iNumOfMoves_50;
	}

	// Checks if new Time score is better or worse than saved one, then saves to vector if better
	void internalHighScoreUtility_Time()
	{
		if (vHighScore_Time[0] > iTime_1 && iTime_1 != 0) vHighScore_Time[0] = iTime_1;
		if (vHighScore_Time[1] > iTime_2 && iTime_2 != 0) vHighScore_Time[1] = iTime_2;
		if (vHighScore_Time[2] > iTime_3 && iTime_3 != 0) vHighScore_Time[2] = iTime_3;
		if (vHighScore_Time[3] > iTime_4 && iTime_4 != 0) vHighScore_Time[3] = iTime_4;
		if (vHighScore_Time[4] > iTime_5 && iTime_5 != 0) vHighScore_Time[4] = iTime_5;
		if (vHighScore_Time[5] > iTime_6 && iTime_6 != 0) vHighScore_Time[5] = iTime_6;
		if (vHighScore_Time[6] > iTime_7 && iTime_7 != 0) vHighScore_Time[6] = iTime_7;
		if (vHighScore_Time[7] > iTime_8 && iTime_8 != 0) vHighScore_Time[7] = iTime_8;
		if (vHighScore_Time[8] > iTime_9 && iTime_9 != 0) vHighScore_Time[8] = iTime_9;
		if (vHighScore_Time[9] > iTime_10 && iTime_10 != 0) vHighScore_Time[9] = iTime_10;
		if (vHighScore_Time[10] > iTime_11 && iTime_11 != 0) vHighScore_Time[10] = iTime_11;
		if (vHighScore_Time[11] > iTime_12 && iTime_12 != 0) vHighScore_Time[11] = iTime_12;
		if (vHighScore_Time[12] > iTime_13 && iTime_13 != 0) vHighScore_Time[12] = iTime_13;
		if (vHighScore_Time[13] > iTime_14 && iTime_14 != 0) vHighScore_Time[13] = iTime_14;
		if (vHighScore_Time[14] > iTime_15 && iTime_15 != 0) vHighScore_Time[14] = iTime_15;
		if (vHighScore_Time[15] > iTime_16 && iTime_16 != 0) vHighScore_Time[15] = iTime_16;
		if (vHighScore_Time[16] > iTime_17 && iTime_17 != 0) vHighScore_Time[16] = iTime_17;
		if (vHighScore_Time[17] > iTime_18 && iTime_18 != 0) vHighScore_Time[17] = iTime_18;
		if (vHighScore_Time[18] > iTime_19 && iTime_19 != 0) vHighScore_Time[18] = iTime_19;
		if (vHighScore_Time[19] > iTime_20 && iTime_20 != 0) vHighScore_Time[19] = iTime_20;
		if (vHighScore_Time[20] > iTime_21 && iTime_21 != 0) vHighScore_Time[20] = iTime_21;
		if (vHighScore_Time[21] > iTime_22 && iTime_22 != 0) vHighScore_Time[21] = iTime_22;
		if (vHighScore_Time[22] > iTime_23 && iTime_23 != 0) vHighScore_Time[22] = iTime_23;
		if (vHighScore_Time[23] > iTime_24 && iTime_24 != 0) vHighScore_Time[23] = iTime_24;
		if (vHighScore_Time[24] > iTime_25 && iTime_25 != 0) vHighScore_Time[24] = iTime_25;
		if (vHighScore_Time[25] > iTime_26 && iTime_26 != 0) vHighScore_Time[25] = iTime_26;
		if (vHighScore_Time[26] > iTime_27 && iTime_27 != 0) vHighScore_Time[26] = iTime_27;
		if (vHighScore_Time[27] > iTime_28 && iTime_28 != 0) vHighScore_Time[27] = iTime_28;
		if (vHighScore_Time[28] > iTime_29 && iTime_29 != 0) vHighScore_Time[28] = iTime_29;
		if (vHighScore_Time[29] > iTime_30 && iTime_30 != 0) vHighScore_Time[29] = iTime_30;
		if (vHighScore_Time[30] > iTime_31 && iTime_31 != 0) vHighScore_Time[30] = iTime_31;
		if (vHighScore_Time[31] > iTime_32 && iTime_32 != 0) vHighScore_Time[31] = iTime_32;
		if (vHighScore_Time[32] > iTime_33 && iTime_33 != 0) vHighScore_Time[32] = iTime_33;
		if (vHighScore_Time[33] > iTime_34 && iTime_34 != 0) vHighScore_Time[33] = iTime_34;
		if (vHighScore_Time[34] > iTime_35 && iTime_35 != 0) vHighScore_Time[34] = iTime_35;
		if (vHighScore_Time[35] > iTime_36 && iTime_36 != 0) vHighScore_Time[35] = iTime_36;
		if (vHighScore_Time[36] > iTime_37 && iTime_37 != 0) vHighScore_Time[36] = iTime_37;
		if (vHighScore_Time[37] > iTime_38 && iTime_38 != 0) vHighScore_Time[37] = iTime_38;
		if (vHighScore_Time[38] > iTime_39 && iTime_39 != 0) vHighScore_Time[38] = iTime_39;
		if (vHighScore_Time[39] > iTime_40 && iTime_40 != 0) vHighScore_Time[39] = iTime_40;
		if (vHighScore_Time[40] > iTime_41 && iTime_41 != 0) vHighScore_Time[40] = iTime_41;
		if (vHighScore_Time[41] > iTime_42 && iTime_42 != 0) vHighScore_Time[41] = iTime_42;
		if (vHighScore_Time[42] > iTime_43 && iTime_43 != 0) vHighScore_Time[42] = iTime_43;
		if (vHighScore_Time[43] > iTime_44 && iTime_44 != 0) vHighScore_Time[43] = iTime_44;
		if (vHighScore_Time[44] > iTime_45 && iTime_45 != 0) vHighScore_Time[44] = iTime_45;
		if (vHighScore_Time[45] > iTime_46 && iTime_46 != 0) vHighScore_Time[45] = iTime_46;
		if (vHighScore_Time[46] > iTime_47 && iTime_47 != 0) vHighScore_Time[46] = iTime_47;
		if (vHighScore_Time[47] > iTime_48 && iTime_48 != 0) vHighScore_Time[47] = iTime_48;
		if (vHighScore_Time[48] > iTime_49 && iTime_49 != 0) vHighScore_Time[48] = iTime_49;
		if (vHighScore_Time[49] > iTime_50 && iTime_50 != 0) vHighScore_Time[49] = iTime_50;
	}

	// Function for saving High Scores
	int SaveHighScores() // returns -1 for failure, or 1 for success
	{
		std::ofstream outHighScores_Moves("HighScores_Moves.txt");
		std::ofstream outHighScores_Time("HighScores_Time.txt");
		outHighScores_Moves.clear();
		outHighScores_Time.clear();

		if (outHighScores_Moves.is_open() && outHighScores_Time.is_open())
		{
			for (int i = 0; i < vHighScore_Moves.size(); i++)
			{
				outHighScores_Moves << vHighScore_Moves[i] << "\n";
			}

			for (int i = 0; i < vHighScore_Time.size(); i++)
			{
				outHighScores_Time << vHighScore_Time[i] << "\n";
			}

			outHighScores_Moves.close();
			outHighScores_Time.close();

			return 1;
		}
		else
		{
			return -1;
		}

		return -1;
	}

	// Function to load all audio into memory
	void LoadAllAudio()
	{
		// Initialize Audio Engines
		audioEngine_Music.InitialiseAudio();	
		audioEngine.InitialiseAudio();

		// Load Slots
		audioSlot_Music_1.LoadAudioWaveform(sBackgroundMusic_1);
		audioSlot_Music_2.LoadAudioWaveform(sBackgroundMusic_2);
		audioSlot_Music_3.LoadAudioWaveform(sBackgroundMusic_3);
		audioSlot_Music_Menu.LoadAudioWaveform(sBackgroundMusic_Menu);
		audioSlot_Movement_Succeed.LoadAudioWaveform(sMovement_1);
		audioSlot_Movement_Fail.LoadAudioWaveform(sMovementFailure_1);
		audioSlot_RestartLevel.LoadAudioWaveform(sRestartLevel_1);
		audioSlot_WinJingle.LoadAudioWaveform(sWinJingle_1);
		audioSlot_LevelTransition.LoadAudioWaveform(sLevelTransition_1);
		audioSlot_PauseJingle.LoadAudioWaveform(sPauseJingle_1);	
		audioSlot_UnPauseJingle.LoadAudioWaveform(sUnPauseJingle_1);
		audioSlot_LevelCode_Succeed.LoadAudioWaveform(sLevelCode_1);
		audioSlot_LevelCode_Fail.LoadAudioWaveform(sLevelCodeFail_1);
		audioSlot_GameStartUp.LoadAudioWaveform(sGameStartUp);
		audioSlot_DoorOpen.LoadAudioWaveform(sDoorOpen_1);
		audioSlot_DoorClose.LoadAudioWaveform(sDoorClose_1);
		audioSlot_Teleport_Succeed.LoadAudioWaveform(sTeleport_1);
		audioSlot_Teleport_Fail.LoadAudioWaveform(sTeleportFailure_1);
		audioSlot_ButtonClick_1.LoadAudioWaveform(sButtonClick_1);
		audioSlot_ButtonClick_2.LoadAudioWaveform(sButtonClick_2);

		// Set Volume					
		audioEngine_Music.SetOutputVolume(fMusicVolume);
		audioEngine.SetOutputVolume(fSFXVolume);
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

	// utility function to reset move and time tracking var for a single level
	void ResetLevelScore()
	{
		switch (iCurLevel)
		{
		case 1:
			iNumOfMoves_1 = 0;
			iTime_1 = 0;
			break;
		case 2:
			iNumOfMoves_2 = 0;
			iTime_2 = 0;
			break;
		case 3:
			iNumOfMoves_3 = 0;
			iTime_3 = 0;
			break;
		case 4:
			iNumOfMoves_4 = 0;
			iTime_4 = 0;
			break;
		case 5:
			iNumOfMoves_5 = 0;
			iTime_5 = 0;
			break;
		case 6:
			iNumOfMoves_6 = 0;
			iTime_6 = 0;
			break;
		case 7:
			iNumOfMoves_7 = 0;
			iTime_7 = 0;
			break;
		case 8:
			iNumOfMoves_8 = 0;
			iTime_8 = 0;
			break;
		case 9:
			iNumOfMoves_9 = 0;
			iTime_9 = 0;
			break;
		case 10:
			iNumOfMoves_10 = 0;
			iTime_10 = 0;
			break;
		case 11:
			iNumOfMoves_11 = 0;
			iTime_11 = 0;
			break;
		case 12:
			iNumOfMoves_12 = 0;
			iTime_12 = 0;
			break;
		case 13:
			iNumOfMoves_13 = 0;
			iTime_13 = 0;
			break;
		case 14:
			iNumOfMoves_14 = 0;
			iTime_14 = 0;
			break;
		case 15:
			iNumOfMoves_15 = 0;
			iTime_15 = 0;
			break;
		case 16:
			iNumOfMoves_16 = 0;
			iTime_16 = 0;
			break;
		case 17:
			iNumOfMoves_17 = 0;
			iTime_17 = 0;
			break;
		case 18:
			iNumOfMoves_18 = 0;
			iTime_18 = 0;
			break;
		case 19:
			iNumOfMoves_19 = 0;
			iTime_19 = 0;
			break;
		case 20:
			iNumOfMoves_20 = 0;
			iTime_20 = 0;
			break;
		case 21:
			iNumOfMoves_21 = 0;
			iTime_21 = 0;
			break;
		case 22:
			iNumOfMoves_22 = 0;
			iTime_22 = 0;
			break;
		case 23:
			iNumOfMoves_23 = 0;
			iTime_23 = 0;
			break;
		case 24:
			iNumOfMoves_24 = 0;
			iTime_24 = 0;
			break;
		case 25:
			iNumOfMoves_25 = 0;
			iTime_25 = 0;
			break;
		case 26:
			iNumOfMoves_26 = 0;
			iTime_26 = 0;
			break;
		case 27:
			iNumOfMoves_27 = 0;
			iTime_27 = 0;
			break;
		case 28:
			iNumOfMoves_28 = 0;
			iTime_28 = 0;
			break;
		case 29:
			iNumOfMoves_29 = 0;
			iTime_29 = 0;
			break;
		case 30:
			iNumOfMoves_30 = 0;
			iTime_30 = 0;
			break;
		case 31:
			iNumOfMoves_31 = 0;
			iTime_31 = 0;
			break;
		case 32:
			iNumOfMoves_32 = 0;
			iTime_32 = 0;
			break;
		case 33:
			iNumOfMoves_33 = 0;
			iTime_33 = 0;
			break;
		case 34:
			iNumOfMoves_34 = 0;
			iTime_34 = 0;
			break;
		case 35:
			iNumOfMoves_35 = 0;
			iTime_35 = 0;
			break;
		case 36:
			iNumOfMoves_36 = 0;
			iTime_36 = 0;
			break;
		case 37:
			iNumOfMoves_37 = 0;
			iTime_37 = 0;
			break;
		case 38:
			iNumOfMoves_38 = 0;
			iTime_38 = 0;
			break;
		case 39:
			iNumOfMoves_39 = 0;
			iTime_39 = 0;
			break;
		case 40:
			iNumOfMoves_40 = 0;
			iTime_40 = 0;
			break;
		case 41:
			iNumOfMoves_41 = 0;
			iTime_41 = 0;
			break;
		case 42:
			iNumOfMoves_42 = 0;
			iTime_42 = 0;
			break;
		case 43:
			iNumOfMoves_43 = 0;
			iTime_43 = 0;
			break;
		case 44:
			iNumOfMoves_44 = 0;
			iTime_44 = 0;
			break;
		case 45:
			iNumOfMoves_45 = 0;
			iTime_45 = 0;
			break;
		case 46:
			iNumOfMoves_46 = 0;
			iTime_46 = 0;
			break;
		case 47:
			iNumOfMoves_47 = 0;
			iTime_47 = 0;
			break;
		case 48:
			iNumOfMoves_48 = 0;
			iTime_48 = 0;
			break;
		case 49:
			iNumOfMoves_49 = 0;
			iTime_49 = 0;
			break;
		case 50:
			iNumOfMoves_50 = 0;
			iTime_50 = 0;
			break;
		default:
			break;
		}

		internalHighScoreUtility_Moves();
		internalHighScoreUtility_Time();
	}

	// Function for loading a level from string template
	void LoadLevel(int n, bool bWasRestart)
	{
		// Clear existing level data
		vLevel.clear();
		vGoals.clear();
		vDoors_pos.clear();
		vSwitches.clear();
		bLevelHasTeleports = false;
		bDoorsOpen = false;

		// reset audio
		audioEngine.StopAll();
		audioEngine_Music.StopAll();

		// check for no more levels in memory
		if (vAllLevels[iCurLevel] == "End" || bMainMenu)
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
		if (!bMainMenu) // Dont Play Load SFX when loading Main Menu
		{
			if (iCurLevel == -1)
			{
				audioEngine.PlayWaveform(&audioSlot_WinJingle, false, fAudioSpeed);			// Play SFX
			}
			else if (bWasRestart == true)
			{
				audioEngine.PlayWaveform(&audioSlot_RestartLevel, false, fAudioSpeed);		// Play SFX
			}
			else
			{
				audioEngine.PlayWaveform(&audioSlot_LevelTransition, false, fAudioSpeed);	// Play SFX
			}
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
				case 'D':
					vDoors_pos.push_back({ x,y });
					vLevel.emplace_back(nullptr);
					break;
				case 'S':
					vSwitches.push_back({ x,y });
					vLevel.emplace_back(nullptr);
					break;
				case 'B':
					vTele_Blue = { x,y };
					bLevelHasTeleports = true;
					vLevel.emplace_back(nullptr);
					break;
				case 'O':
					vTele_Orange = { x,y };
					bLevelHasTeleports = true;
					vLevel.emplace_back(nullptr);
					break;
				default:
					vLevel.emplace_back(nullptr);
				}
			}
		}

		// restart music
		if (iCurLevel >= 1 && iCurLevel <= 15 && !bMainMenu)
		{
			audioEngine_Music.PlayWaveform(&audioSlot_Music_1, true, fAudioSpeed * 0.75f);
		}
		else if (iCurLevel >= 16 && iCurLevel <= 35 && !bMainMenu)
		{
			audioEngine_Music.PlayWaveform(&audioSlot_Music_2, true, fAudioSpeed * 0.75f);
		}
		else if (iCurLevel >= 36 && iCurLevel <= 50 && !bMainMenu)
		{
			audioEngine_Music.PlayWaveform(&audioSlot_Music_3, true, fAudioSpeed * 0.75f);
		}
		else if (bMainMenu)
		{
			audioEngine_Music.PlayWaveform(&audioSlot_Music_Menu, true, fAudioSpeed * 0.75f);
		}
	}

	// Overridden Text Entry function for use in Level Select Menu
	void OnTextEntryComplete(const std::string& sText) override 
	{ 
		sInputCode = sText;
	}

	// Called Every Frame while the Main Menu system is open
	void MainMenu()
	{
		iLevelSet = -1;

		switch (iCurDisplay)
		{
		case 0: // High Score Screen
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)		// Close Menu
			{
				iPageNum = 1;
				iCurDisplay = -1;
			}
			if (GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::A).bPressed)		// Move to next page
			{
				if (iPageNum > 1)
				{
					iPageNum--;
				}
			}
			if (GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::D).bPressed)	    // Move to previous page
			{
				if (iPageNum < 8)
				{
					iPageNum++;
				}
			}
			if (GetKey(olc::Key::H).bPressed)											// Reset Scores
			{
				for (int i = 0; i < vHighScore_Moves.size(); i++)
				{
					vHighScore_Moves[i] = 1000;
				}
				for (int i = 0; i < vHighScore_Time.size(); i++)
				{
					vHighScore_Time[i] = 10000;
				}

				fTotal_HS = 500000.0f;
				iTotal_HS = 50000;

				int iSave = SaveHighScores();
				int iLoad = LoadHighScores();
			}

			// Draw Blank Menu Level
			DrawLevel(iLevelSet);

			// Draw UI
			DrawString((this->ScreenWidth() / 2) - 42, 24, "HIGH SCORES", olc::WHITE);
			
			DrawString(20, 180, "Press ESC to Close", olc::WHITE);
			DrawString(20, 190, "Press H to Reset Scores", olc::WHITE);

			DrawString(20, 202, "Press Left or Right to", olc::WHITE);
			DrawString(20, 212, "Select Page", olc::WHITE);
			DrawString(230, 215, std::to_string(iPageNum), olc::WHITE);

			// Display Scores 
			switch (iPageNum)
			{
			case 1:
				DrawString(20, 33, "Level 1:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[0]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[0])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 2:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[1]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[1])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 3:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[2]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[2])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 4:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[3]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[3])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 5:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[4]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[4])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 6:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[5]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[5])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 7:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[6]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[6])) + "s", olc::WHITE);
				break;
			case 2:
				DrawString(20, 33, "Level 8:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[7]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[7])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 9:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[8]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[8])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 10:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[9]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[9])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 11:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[10]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[10])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 12:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[11]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[11])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 13:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[12]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[12])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 14:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[13]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[13])) + "s", olc::WHITE);
				break;
			case 3:
				DrawString(20, 33, "Level 15:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[14]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[14])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 16:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[15]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[15])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 17:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[16]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[16])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 18:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[17]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[17])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 19:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[18]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[18])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 20:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[19]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[19])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 21:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[20]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[20])) + "s", olc::WHITE);
				break;
			case 4:
				DrawString(20, 33, "Level 22:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[21]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[21])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 23:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[22]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[22])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 24:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[23]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[23])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 25:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[24]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[24])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 26:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[25]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[25])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 27:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[26]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[26])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 28:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[27]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[27])) + "s", olc::WHITE);
				break;
			case 5:
				DrawString(20, 33, "Level 29:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[28]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[28])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 30:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[29]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[29])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 31:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[30]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[30])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 32:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[31]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[31])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 33:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[32]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[32])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 34:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[33]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[33])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 35:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[34]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[34])) + "s", olc::WHITE);
				break;
			case 6:
				DrawString(20, 33, "Level 36:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[35]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[35])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 37:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[36]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[36])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 38:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[37]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[37])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 39:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[38]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[38])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 40:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[39]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[39])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 41:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[40]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[40])) + "s", olc::WHITE);
				DrawString(20, 153, "Level 42:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[41]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[41])) + "s", olc::WHITE);
				break;
			case 7:
				DrawString(20, 33, "Level 43:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[42]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[42])) + "s", olc::WHITE);
				DrawString(20, 53, "Level 44:", olc::WHITE);
				DrawString(20, 63, "Moves: " + std::to_string(vHighScore_Moves[43]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[43])) + "s", olc::WHITE);
				DrawString(20, 73, "Level 45:", olc::WHITE);
				DrawString(20, 83, "Moves: " + std::to_string(vHighScore_Moves[44]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[44])) + "s", olc::WHITE);
				DrawString(20, 93, "Level 46:", olc::WHITE);
				DrawString(20, 103, "Moves: " + std::to_string(vHighScore_Moves[45]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[45])) + "s", olc::WHITE);
				DrawString(20, 113, "Level 47:", olc::WHITE);
				DrawString(20, 123, "Moves: " + std::to_string(vHighScore_Moves[46]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[46])) + "s", olc::WHITE);
				DrawString(20, 133, "Level 48:", olc::WHITE);
				DrawString(20, 143, "Moves: " + std::to_string(vHighScore_Moves[47]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[47])) + "s",olc::WHITE);
				DrawString(20, 153, "Level 49:", olc::WHITE);
				DrawString(20, 163, "Moves: " + std::to_string(vHighScore_Moves[48]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[48])) + "s", olc::WHITE);
				break;
			case 8:
				DrawString(20, 33, "Level 50:", olc::WHITE);
				DrawString(20, 43, "Moves: " + std::to_string(vHighScore_Moves[49]) + " Time: " + FloatToString(TruncateFloat(vHighScore_Time[49])) + "s", olc::WHITE);

				iTotal_HS = 0;
				fTotal_HS = 0.0f;
				for (int i = 0; i < vHighScore_Moves.size(); i++)
				{
					iTotal_HS += vHighScore_Moves[i];
				}
				for (int i = 0; i < vHighScore_Time.size(); i++)
				{
					fTotal_HS += vHighScore_Time[i];
				}

				DrawString(20, 143, "Total:", olc::WHITE);
				DrawString(20, 153, "Moves: " + std::to_string(iTotal_HS) + "\nTime: " + FloatToString(TruncateFloat(fTotal_HS)) + "s", olc::WHITE);
				break;
			default:
				break;
			}
			break;
		case 1: // Options Screen
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)	// Close Menu
			{
				// Save Changes
				iOptionsSave = SaveOptions();

				iCurDisplay = -1;
			}
			if (GetKey(olc::Key::D).bPressed)		// Increase SFX Volume
			{
				fSFXVolume += 0.1f;
				audioEngine.PlayWaveform(&audioSlot_Movement_Succeed, false, fAudioSpeed);
			}
			if (GetKey(olc::Key::A).bPressed)		// Decrease SFX Volume
			{
				fSFXVolume -= 0.1f;
				audioEngine.PlayWaveform(&audioSlot_Movement_Succeed, false, fAudioSpeed);
			}
			if (GetKey(olc::Key::RIGHT).bPressed)	// Increase Music Volume
			{
				fMusicVolume += 0.1f;
			}
			if (GetKey(olc::Key::LEFT).bPressed)	// Decrease Music Volume
			{
				fMusicVolume -= 0.1f;
			}

			// Clamp Volume Values
			if (fMusicVolume > 1.0f)
			{
				fMusicVolume = 1.0f;
			}
			if (fMusicVolume < 0.0f)
			{
				fMusicVolume = 0.0f;
			}
			if (fSFXVolume > 1.0f)
			{
				fSFXVolume = 1.0f;
			}
			if (fSFXVolume < 0.0f)
			{
				fSFXVolume = 0.0f;
			}

			// Apply Changes
			audioEngine.SetOutputVolume(fSFXVolume);
			audioEngine_Music.SetOutputVolume(fMusicVolume);

			// Draw Blank Menu Level
			DrawLevel(iLevelSet);

			// Draw UI
			DrawString((this->ScreenWidth() / 2) - 42, (240 / 2) - 96, "OPTIONS", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) - 85, "Press Arrow Keys to", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) - 75, "Adjust Music Volume", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) - 60, "Press A or D to", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) - 50, "Adjust SFX Volume", olc::WHITE);

			DrawString((256 / 2) - 108, (240 / 2) + 72, "Music: " + std::to_string(fMusicVolume), olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 82, "SFX: " +std::to_string(fSFXVolume), olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 92, "Press ESC to Close", olc::WHITE);
			break;
		case 2: // Level Select Code Screen
			// User Input
			if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)		// Close Menu
			{
				sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
				iCurDisplay = -1;
			}
			if (GetKey(olc::Key::ENTER).bPressed && IsTextEntryEnabled() == false)		// Enable Text Entry
			{
				TextEntryEnable(true);
			}

			// Use Input Input, if player has inputted one
			if (sInputCode != "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd")		// Some kind of user input has been detected in the string
			{
				if (sInputCode == sMediumLevelCode)
				{
					// Play Success SFX
					audioEngine.PlayWaveform(&audioSlot_LevelCode_Succeed, false, fAudioSpeed);

					// Set Level
					iCurLevel = 16;

					// Disable Text Entry
					TextEntryEnable(false);

					// Exit Menus, reset sInput string, Load Level						
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
					iCurDisplay = -1;
					bMainMenu = false;
					LoadLevel(iCurLevel, false);

				}
				else if (sInputCode == sHardLevelCode)
				{
					// Play Success SFX
					audioEngine.PlayWaveform(&audioSlot_LevelCode_Succeed, false, fAudioSpeed);

					// Set Level
					iCurLevel = 36;

					// Disable Text Entry
					TextEntryEnable(false);

					// Exit Menus, reset sInput string, Load Level
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
					iCurDisplay = -1;
					bMainMenu = false;
					LoadLevel(iCurLevel, false);
				}
				else
				{
					// Play Fail SFX
					audioEngine.PlayWaveform(&audioSlot_LevelCode_Fail, false, fAudioSpeed);

					// Disable Text Entry
					TextEntryEnable(false);

					// Reset sInput string
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
				}
			}

			// Draw Blank Menu Level
			DrawLevel(iLevelSet);

			// Draw UI
			DrawString((this->ScreenWidth() / 2) - 50, (240 / 2) - 96, "LEVEL SELECT", olc::WHITE);
			DrawString(20, 199, "Press Enter to Input Code", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 92, "Press ESC to Close", olc::WHITE);
			
			// Text Entry UI
			if (IsTextEntryEnabled() == true)
			{
				DrawString(65, 35, "Text Entry Mode", olc::YELLOW);
			}
			break;
		default: // Main Menu
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)  // Close Game
			{
				internalHighScoreUtility_Time();
				internalHighScoreUtility_Moves();
				SaveHighScores();
				this->~Puzzle();
			}
			if (GetKey(olc::Key::H).bPressed)								    	// Open High Score Menu
			{
				iCurDisplay = 0;
			}
			if (GetKey(olc::Key::O).bPressed)								    	// Open Options Menu
			{
				iCurDisplay = 1;
			}
			if (GetKey(olc::Key::L).bPressed)										 // Open Level Select Menu
			{
				iCurDisplay = 2;
			}
			if (GetKey(olc::Key::ENTER).bPressed)								    // Start Game
			{
				// Start Game
				bMainMenu = false;													// Set Flag to close Main Menu System
				iCurLevel = 1;
				LoadLevel(iCurLevel, false);										// Load First Level
			}

			// Draw Blank Menu Level
			DrawLevel(iLevelSet);

			// Draw UI
			DrawString((this->ScreenWidth() / 2) - 42, (240 / 2) - 96, "MAIN MENU", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 40, "Press Enter to Start Game", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 52, "Press L for Level Select", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 65, "Press H to see High Scores", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 78, "Press O for Options", olc::WHITE);
			DrawString((256 / 2) - 108, (240 / 2) + 92, "Press ESC to Quit", olc::WHITE);
			break;
		}
	}

	// Draws the current level to the screen
	void DrawLevel(int iLevelSet)
	{
		// lambda function for translating our 2D coordinates into 1D
		auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; };

		// Clear screen to black before drawing each frame
		Clear(olc::BLACK);
		
		// door drawing
		for (int i = 0; i < vDoors_pos.size(); i++)
		{
			switch (iLevelSet)
			{
			case 0:		// easy
				if (bDoorsOpen)
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(0, 2) * vBlockSize, vBlockSize);
				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 1) * vBlockSize, vBlockSize);
				}
				break;
			case 1:		// medium
				if (bDoorsOpen)
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(2, 2) * vBlockSize, vBlockSize);
				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);
				}
				break;
			case 2:		// hard
				if (bDoorsOpen)
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(3, 2) * vBlockSize, vBlockSize);
				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 2) * vBlockSize, vBlockSize);
				}
				break;
			default:
				break;
			}
		}

		// win condition drawing
		for (auto& g : vGoals)
		{
			FillCircle(g * vBlockSize + vBlockSize / 2, vBlockSize.x / 2 - 2, olc::YELLOW);
		}

		// door switch drawing
		for (auto& d : vSwitches)
		{
			FillCircle(d * vBlockSize + vBlockSize / 2, vBlockSize.x / 2 - 2, olc::RED);
		}

		// teleport drawing
		if (bLevelHasTeleports)
		{
			switch (iTele_Facing)
			{
			case 0:
				DrawPartialSprite(vTele_Blue * vBlockSize, gfxTiles.Sprite(), olc::vi2d(3, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite(vTele_Orange * vBlockSize, gfxTiles.Sprite(), olc::vi2d(3, 4) * vBlockSize, vBlockSize);
				break;
			case 1:
				DrawPartialSprite(vTele_Blue * vBlockSize, gfxTiles.Sprite(), olc::vi2d(2, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite(vTele_Orange * vBlockSize, gfxTiles.Sprite(), olc::vi2d(2, 4) * vBlockSize, vBlockSize);
				break;
			case 2:
				DrawPartialSprite(vTele_Blue * vBlockSize, gfxTiles.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite(vTele_Orange * vBlockSize, gfxTiles.Sprite(), olc::vi2d(1, 4) * vBlockSize, vBlockSize);
				break;
			case 3:
				DrawPartialSprite(vTele_Blue * vBlockSize, gfxTiles.Sprite(), olc::vi2d(0, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite(vTele_Orange * vBlockSize, gfxTiles.Sprite(), olc::vi2d(0, 4) * vBlockSize, vBlockSize);
				break;
			}
		}

		// block drawing
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
					b->DrawSelf(this, vBlockPos, vBlockSize, gfxTiles, iLevelSet);
				}
			}
		}
	}

	// Utility function for tracking time per frame within each level
	void RecordLevelTimePerFrame(float fElapsedTime)
	{
		switch (iCurLevel)
		{
		case 1:
			iTime_1 += fElapsedTime;
			break;
		case 2:
			iTime_2 += fElapsedTime;
			break;
		case 3:
			iTime_3 += fElapsedTime;
			break;
		case 4:
			iTime_4 += fElapsedTime;
			break;
		case 5:
			iTime_5 += fElapsedTime;
			break;
		case 6:
			iTime_6 += fElapsedTime;
			break;
		case 7:
			iTime_7 += fElapsedTime;
			break;
		case 8:
			iTime_8 += fElapsedTime;
			break;
		case 9:
			iTime_9 += fElapsedTime;
			break;
		case 10:
			iTime_10 += fElapsedTime;
			break;
		case 11:
			iTime_11 += fElapsedTime;
			break;
		case 12:
			iTime_12 += fElapsedTime;
			break;
		case 13:
			iTime_13 += fElapsedTime;
			break;
		case 14:
			iTime_14 += fElapsedTime;
			break;
		case 15:
			iTime_15 += fElapsedTime;
			break;
		case 16:
			iTime_16 += fElapsedTime;
			break;
		case 17:
			iTime_17 += fElapsedTime;
			break;
		case 18:
			iTime_18 += fElapsedTime;
			break;
		case 19:
			iTime_19 += fElapsedTime;
			break;
		case 20:
			iTime_20 += fElapsedTime;
			break;
		case 21:
			iTime_21 += fElapsedTime;
			break;
		case 22:
			iTime_22 += fElapsedTime;
			break;
		case 23:
			iTime_23 += fElapsedTime;
			break;
		case 24:
			iTime_24 += fElapsedTime;
			break;
		case 25:
			iTime_25 += fElapsedTime;
			break;
		case 26:
			iTime_26 += fElapsedTime;
			break;
		case 27:
			iTime_27 += fElapsedTime;
			break;
		case 28:
			iTime_28 += fElapsedTime;
			break;
		case 29:
			iTime_29 += fElapsedTime;
			break;
		case 30:
			iTime_30 += fElapsedTime;
			break;
		case 31:
			iTime_31 += fElapsedTime;
			break;
		case 32:
			iTime_32 += fElapsedTime;
			break;
		case 33:
			iTime_33 += fElapsedTime;
			break;
		case 34:
			iTime_34 += fElapsedTime;
			break;
		case 35:
			iTime_35 += fElapsedTime;
			break;
		case 36:
			iTime_36 += fElapsedTime;
			break;
		case 37:
			iTime_37 += fElapsedTime;
			break;
		case 38:
			iTime_38 += fElapsedTime;
			break;
		case 39:
			iTime_39 += fElapsedTime;
			break;
		case 40:
			iTime_40 += fElapsedTime;
			break;
		case 41:
			iTime_41 += fElapsedTime;
			break;
		case 42:
			iTime_42 += fElapsedTime;
			break;
		case 43:
			iTime_43 += fElapsedTime;
			break;
		case 44:
			iTime_44 += fElapsedTime;
			break;
		case 45:
			iTime_45 += fElapsedTime;
			break;
		case 46:
			iTime_46 += fElapsedTime;
			break;
		case 47:
			iTime_47 += fElapsedTime;
			break;
		case 48:
			iTime_48 += fElapsedTime;
			break;
		case 49:
			iTime_49 += fElapsedTime;
			break;
		case 50:
			iTime_50 += fElapsedTime;
			break;
		default:
			break;
		}
	}

	// Runs once at start of game
	bool OnUserCreate() override
	{
		// Audio Loading
		LoadAllAudio();

		// Graphics Loading
		gfxTiles.Load("Graphics//TileSheet.png");
		gfxSplash.Load("Graphics//SplashScreen.png");
		gfxWin.Load("Graphics//WinScreen.png"); 

		// Level Loading
		LoadAllLevels();

		// Load Blank Menu Level
		LoadLevel(iCurLevel, false);
		bMainMenu = true;
			
		return true;
	}

	// Runs every frame
	bool OnUserUpdate(float fElapsedTime) override
	{

// Startup Routine for beginning of Game
#pragma region StartUp Routine
		// Check Background Music Flag
		if (!bDoBackgroundMusic)
		{
			audioEngine_Music.SetOutputVolume(0.0f);
		}
		else
		{
			audioEngine_Music.SetOutputVolume(fMusicVolume);
		}

		// Do StartUp Routine at beginning of game
		if (!bGameStarted)
		{
			// Play the StartUP SFX one time
			if (bDoStartUpJingle)
			{
				// Play Game Start Up SFX
				pwStart = audioEngine.PlayWaveform(&audioSlot_GameStartUp, false, fSplashScreenSpeed);
				fStartTime = fElapsedTime;
				dDuration = pwStart->dDuration;
				fTarget = fElapsedTime + dDuration;

				// Set Flag
				bDoStartUpJingle = false;
			}

			// Draw Splash Screen
			DrawSprite(olc::vi2d(0, 0), gfxSplash.Sprite());

			// Wait for StartUp SFX to finish, then set flags to stop Start Up Routine
			fTimer += fElapsedTime;
			if (fTimer >= fTarget)
			{
				// Set Flags
				bGameStarted = true;
				bDoBackgroundMusic = true;
			}

			// Load Options
			iOptionsLoad = LoadOptions();

			// Load High Scores
			iHighScoreLoad = LoadHighScores();

			return true;
		}
#pragma endregion

		// Game Logic
		if (bGameStarted)
		{
			// Main Menu Stuff
			if (bMainMenu)
			{
				MainMenu();

				return true;
			}

			if (!bPaused)
			{
				// user input
				bool bPushing = false;
				int iDirPush = NORTH;
				if (iCurLevel != -1) // disable most input on end screen
				{
					if (bEnableInput && GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed)
					{
						iDirPush = 0;
						bPushing = true;
					}
					if (bEnableInput && GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed)
					{
						iDirPush = SOUTH;
						bPushing = true;
					}
					if (bEnableInput && GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed)
					{
						iDirPush = WEST;
						bPushing = true;
					}
					if (bEnableInput && GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed)
					{
						iDirPush = EAST;
						bPushing = true;
					}
					if (bEnableInput && GetKey(olc::Key::R).bPressed)
					{
						ResetLevelScore();
						LoadLevel(iCurLevel, true);
					}
					if (bEnableInput && GetKey(olc::Key::ESCAPE).bPressed && !bPaused || GetKey(olc::Key::P).bPressed) // Gameplay Pause
					{
						bPaused = true;	// Pause Game
					}
					if (bEnableInput && GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::ENTER).bPressed && GetKey(olc::Key::MINUS).bPressed) // DebugMode
					{
						if (bDebugMode)
						{
							bDebugMode = false;
						}
						else
						{
							bDebugMode = true;
						}
					}
					if (bEnableInput && bDebugMode && GetKey(olc::Key::V).bPressed) // Debug Mode Door Open/Close
					{
						if (bDoors_DebugForceOpen)
						{
							bDoors_DebugForceOpen = false;
						}
						else
						{
							bDoors_DebugForceOpen = true;
						}
					}
				}
				else
				{
					// Win Screen ESC
					if (bEnableInput && GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed) 
					{
						fTime_WinScreen = 0.0f;
						audioEngine_Music.SetOutputVolume(fMusicVolume);	// Unmute Background Music
						bMainMenu = true;									// Set Flag

						// Reset Move Counters
						iNumOfMoves_1 = 0;
						iNumOfMoves_2 = 0;
						iNumOfMoves_3 = 0;
						iNumOfMoves_4 = 0;
						iNumOfMoves_5 = 0;
						iNumOfMoves_6 = 0;
						iNumOfMoves_7 = 0;
						iNumOfMoves_8 = 0;
						iNumOfMoves_9 = 0;
						iNumOfMoves_10 = 0;
						iNumOfMoves_11 = 0;
						iNumOfMoves_12 = 0;
						iNumOfMoves_13 = 0;
						iNumOfMoves_14 = 0;
						iNumOfMoves_15 = 0;
						iNumOfMoves_16 = 0;
						iNumOfMoves_17 = 0;
						iNumOfMoves_18 = 0;
						iNumOfMoves_19 = 0;
						iNumOfMoves_20 = 0;
						iNumOfMoves_21 = 0;
						iNumOfMoves_22 = 0;
						iNumOfMoves_23 = 0;
						iNumOfMoves_24 = 0;
						iNumOfMoves_25 = 0;
						iNumOfMoves_26 = 0;
						iNumOfMoves_27 = 0;
						iNumOfMoves_28 = 0;
						iNumOfMoves_29 = 0;
						iNumOfMoves_30 = 0;
						iNumOfMoves_31 = 0;
						iNumOfMoves_32 = 0;
						iNumOfMoves_33 = 0;
						iNumOfMoves_34 = 0;
						iNumOfMoves_35 = 0;
						iNumOfMoves_36 = 0;
						iNumOfMoves_37 = 0;
						iNumOfMoves_38 = 0;
						iNumOfMoves_39 = 0;
						iNumOfMoves_40 = 0;
						iNumOfMoves_41 = 0;
						iNumOfMoves_42 = 0;
						iNumOfMoves_43 = 0;
						iNumOfMoves_44 = 0;
						iNumOfMoves_45 = 0;
						iNumOfMoves_46 = 0;
						iNumOfMoves_47 = 0;
						iNumOfMoves_48 = 0;
						iNumOfMoves_49 = 0;
						iNumOfMoves_50 = 0;

						// Reset Timers
						iTime_1 = 0.0f;
						iTime_2 = 0.0f;
						iTime_3 = 0.0f;
						iTime_4 = 0.0f;
						iTime_5 = 0.0f;
						iTime_6 = 0.0f;
						iTime_7 = 0.0f;
						iTime_8 = 0.0f;
						iTime_9 = 0.0f;
						iTime_10 = 0.0f;
						iTime_11 = 0.0f;
						iTime_12 = 0.0f;
						iTime_13 = 0.0f;
						iTime_14 = 0.0f;
						iTime_15 = 0.0f;
						iTime_16 = 0.0f;
						iTime_17 = 0.0f;
						iTime_18 = 0.0f;
						iTime_19 = 0.0f;
						iTime_20 = 0.0f;
						iTime_21 = 0.0f;
						iTime_22 = 0.0f;
						iTime_23 = 0.0f;
						iTime_24 = 0.0f;
						iTime_25 = 0.0f;
						iTime_26 = 0.0f;
						iTime_27 = 0.0f;
						iTime_28 = 0.0f;
						iTime_29 = 0.0f;
						iTime_30 = 0.0f;
						iTime_31 = 0.0f;
						iTime_32 = 0.0f;
						iTime_33 = 0.0f;
						iTime_34 = 0.0f;
						iTime_35 = 0.0f;
						iTime_36 = 0.0f;
						iTime_37 = 0.0f;
						iTime_38 = 0.0f;
						iTime_39 = 0.0f;
						iTime_40 = 0.0f;
						iTime_41 = 0.0f;
						iTime_42 = 0.0f;
						iTime_43 = 0.0f;
						iTime_44 = 0.0f;
						iTime_45 = 0.0f;
						iTime_46 = 0.0f;
						iTime_47 = 0.0f;
						iTime_48 = 0.0f;
						iTime_49 = 0.0f;
						iTime_50 = 0.0f;

						return true;
					}
				}

				// lambda function for translating our 2D coordinates into 1D
				auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; };

// movement logic
#pragma region Movement
				bool bPlayerMoved = false;
				bool bTeleported = false;
				int iCursorTracker = 0;
				int iTeleportSucceededOrFailed = -1; // -1, no tele attempt, 0 = tele failed, 1 = tele succeeded
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
								case NORTH: vBlockPos.y--; iCursorTracker++; break;
								case SOUTH: vBlockPos.y++; iCursorTracker++; break;
								case EAST: vBlockPos.x++; iCursorTracker++; break;
								case WEST: vBlockPos.x--; iCursorTracker++; break;
								}
							}
							else // block cant be pushed that way -- end testing
							{
								bPlayerMoved = true;
								iMovementSuceededOrFailed = 0;
								bTest = false;
							}
						}
						else // target space was nullptr
						{
							// check target space against all door locations
							bool bTargetIsDoor = false;
							for (auto& d : vDoors_pos)
							{
								if (vBlockPos == d)
								{
									bTargetIsDoor = true;
								}
							}

							if (bTargetIsDoor)	// Target Space is door
							{
								if (!bDoorsOpen) // doors are closed - player cannot move there. End testing
								{
									bPlayerMoved = true;
									iMovementSuceededOrFailed = 0;
									bTest = false;
								}
								else			// doors are open - player can move there. End testing
								{
									bPlayerMoved = true;
									iMovementSuceededOrFailed = 1;
									bAllowPush = true;
									bTest = false;
								}
							}
							else				// Target space is not a door - check for teleport 
							{
								bool bTargetIsTeleporter = false;
								if (vBlockPos == vTele_Blue || vBlockPos == vTele_Orange)
								{
									bTargetIsTeleporter = true;
								}

								if (bTargetIsTeleporter) 
								{
									// check if player - blocks cannot teleport, but player can
									bool bTeleAllowed = false;
									if (iCursorTracker == 1)
									{
										bTeleAllowed = true;
									}

									if (!bTeleAllowed) // Teleport is not allowed - Player cannot move there. End testing
									{
										bPlayerMoved = true;
										iTeleportSucceededOrFailed = 0;
										iMovementSuceededOrFailed = 0;
										bTest = false;
									}
									else // teleport is allowed - player can move there. End testing
									{
										bTeleported = true;
										bPlayerMoved = true;
										iTeleportSucceededOrFailed = 1;
										iMovementSuceededOrFailed = 1;
										bAllowPush = true;
										bTest = false;
									}
								}
								else // target space was nullptr and did not contain a door or teleporter - player can move there. End testing
								{
									bPlayerMoved = true;
									iMovementSuceededOrFailed = 1;
									bAllowPush = true;
									bTest = false;
								}
							}
						}
					}

					if (bAllowPush) // if push is allowed - execute push logic
					{
						if (bTeleported) // teleport movement
						{
							if (vBlockPos == vTele_Blue)
							{
								std::swap(vLevel[id(vTele_Orange)], vLevel[id(vPlayerPos)]);
								vPlayerPos = vTele_Orange;
							}
							else if (vBlockPos == vTele_Orange)
							{
								std::swap(vLevel[id(vTele_Blue)], vLevel[id(vPlayerPos)]);
								vPlayerPos = vTele_Blue;
							}
						}
						else // normal movement
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
				}
#pragma endregion

				// Movement & Teleport SFX
				if (bPlayerMoved == true)
				{
					if (iTeleportSucceededOrFailed == -1) // no teleport, just regular movement
					{
						// normal move SFX
						if (iMovementSuceededOrFailed == 0) // Movement Failed
						{
							audioEngine.PlayWaveform(&audioSlot_Movement_Fail, false, fAudioSpeed * 2.0f);
						}
						else if (iMovementSuceededOrFailed == 1) // Movement Succeeded
						{
							audioEngine.PlayWaveform(&audioSlot_Movement_Succeed, false, fAudioSpeed * 2.0f);
						}
					}
					else if (iTeleportSucceededOrFailed == 0) // teleport failed
					{
						audioEngine.PlayWaveform(&audioSlot_Teleport_Fail, false, fAudioSpeed);
					}
					else if (iTeleportSucceededOrFailed == 1) // teleport succeeded
					{
						audioEngine.PlayWaveform(&audioSlot_Teleport_Succeed, false, fAudioSpeed);
					}
				}

				// Move Counting
				if (bPlayerMoved == true && iMovementSuceededOrFailed == 1)
				{
					switch (iCurLevel)
					{
					case 1:
						iNumOfMoves_1++;
						break;
					case 2:
						iNumOfMoves_2++;
						break;
					case 3:
						iNumOfMoves_3++;
						break;
					case 4:
						iNumOfMoves_4++;
						break;
					case 5:
						iNumOfMoves_5++;
						break;
					case 6:
						iNumOfMoves_6++;
						break;
					case 7:
						iNumOfMoves_7++;
						break;
					case 8:
						iNumOfMoves_8++;
						break;
					case 9:
						iNumOfMoves_8++;
						break;
					case 10:
						iNumOfMoves_10++;
						break;
					case 11:
						iNumOfMoves_11++;
						break;
					case 12:
						iNumOfMoves_12++;
						break;
					case 13:
						iNumOfMoves_13++;
						break;
					case 14:
						iNumOfMoves_14++;
						break;
					case 15:
						iNumOfMoves_15++;
						break;
					case 16:
						iNumOfMoves_16++;
						break;
					case 17:
						iNumOfMoves_17++;
						break;
					case 18:
						iNumOfMoves_18++;
						break;
					case 19:
						iNumOfMoves_19++;
						break;
					case 20:
						iNumOfMoves_20++;
						break;
					case 21:
						iNumOfMoves_21++;
						break;
					case 22:
						iNumOfMoves_22++;
						break;
					case 23:
						iNumOfMoves_23++;
						break;
					case 24:
						iNumOfMoves_24++;
						break;
					case 25:
						iNumOfMoves_25++;
						break;
					case 26:
						iNumOfMoves_26++;
						break;
					case 27:
						iNumOfMoves_27++;
						break;
					case 28:
						iNumOfMoves_28++;
						break;
					case 29:
						iNumOfMoves_29++;
						break;
					case 30:
						iNumOfMoves_30++;
						break;
					case 31:
						iNumOfMoves_31++;
						break;
					case 32:
						iNumOfMoves_32++;
						break;
					case 33:
						iNumOfMoves_33++;
						break;
					case 34:
						iNumOfMoves_34++;
						break;
					case 35:
						iNumOfMoves_35++;
						break;
					case 36:
						iNumOfMoves_36++;
						break;
					case 37:
						iNumOfMoves_37++;
						break;
					case 38:
						iNumOfMoves_38++;
						break;
					case 39:
						iNumOfMoves_39++;
						break;
					case 40:
						iNumOfMoves_40++;
						break;
					case 41:
						iNumOfMoves_41++;
						break;
					case 42:
						iNumOfMoves_42++;
						break;
					case 43:
						iNumOfMoves_43++;
						break;
					case 44:
						iNumOfMoves_44++;
						break;
					case 45:
						iNumOfMoves_45++;
						break;
					case 46:
						iNumOfMoves_46++;
						break;
					case 47:
						iNumOfMoves_47++;
						break;
					case 48:
						iNumOfMoves_48++;
						break;
					case 49:
						iNumOfMoves_49++;
						break;
					case 50:
						iNumOfMoves_50++;
						break;
					default:
						break;
					}
				}

				// win condition checking 
				int nGoals_Previous = nGoals; // take current switch count before updating number for this frame
				nGoals = 0;
				for (auto& g : vGoals)
				{
					if (vLevel[id(g)] != nullptr)
					{
						nGoals++; // Increment Goals
					}
				} // Button SFX:
				if (nGoals > nGoals_Previous) // a new switch has been covered this frame - play sfx
				{
					audioEngine.PlayWaveform(&audioSlot_ButtonClick_1, false, fAudioSpeed);
				}
				else if (nGoals < nGoals_Previous) // a previously covered switch has been uncovered this frame - play sfx
				{
					audioEngine.PlayWaveform(&audioSlot_ButtonClick_2, false, fAudioSpeed);
				}
				else if (nGoals != 0 && nGoals == nGoals_Previous && bPlayerMoved)
				{
					bool bDoEdgeCaseButtonSFX = false;
					for (auto& g : vGoals)
					{
						if (vPlayerPos == g) // Player moved from one switch onto another - play both SFX
						{
							bDoEdgeCaseButtonSFX = true;
						}
					}
					if (bDoEdgeCaseButtonSFX)
					{
						audioEngine.PlayWaveform(&audioSlot_ButtonClick_2, false, fAudioSpeed);
						audioEngine.PlayWaveform(&audioSlot_ButtonClick_1, false, fAudioSpeed);
					}
				}

				// door switch checking
				int nSwitches_Previous = nSwitches; // take current switch count before updating number for this frame
				nSwitches = 0;
				for (auto& s : vSwitches)
				{
					if (vLevel[id(s)] != nullptr)
					{
						nSwitches++; // Increment Goals
					}
				} // Buton SFX:
				if (nSwitches > nSwitches_Previous) // a new switch has been covered this frame - play sfx
				{
					audioEngine.PlayWaveform(&audioSlot_ButtonClick_1, false, fAudioSpeed);
				}
				else if (nSwitches < nSwitches_Previous) // a previously covered switch has been uncovered this frame - play sfx
				{
					audioEngine.PlayWaveform(&audioSlot_ButtonClick_2, false, fAudioSpeed);
				}
				else if (nSwitches != 0 && nSwitches == nSwitches_Previous && bPlayerMoved)
				{
					bool bDoEdgeCaseButtonSFX = false;
					for (auto& s : vSwitches)
					{
						if (vPlayerPos == s) // Player moved from one switch onto another - play both SFX
						{
							bDoEdgeCaseButtonSFX = true;
						}
					}
					if (bDoEdgeCaseButtonSFX)
					{
						audioEngine.PlayWaveform(&audioSlot_ButtonClick_2, false, fAudioSpeed);
						audioEngine.PlayWaveform(&audioSlot_ButtonClick_1, false, fAudioSpeed);
					}
				}

				// Draw Function
				DrawLevel(iLevelSet);

				// teleport checking
				if (bLevelHasTeleports)
				{
					DoTeleportFlipCheck(fElapsedTime);
				}

				// UI and Level Set Tracking
				if (iCurLevel != -1) // UI for active gameplay
				{
					// Goal Tracking UI
					DrawString(4, 4, "Goals: " + std::to_string(nGoals) + " / " + std::to_string(vGoals.size()), olc::WHITE);

					// Level Tracking UI
					if (iCurLevel < 16)
					{
						iLevelSet = 0;
						DrawString(128 + 20, 4, "Level: ", olc::WHITE);
						DrawString(128 + 70, 4, std::to_string(iCurLevel) + " / " + std::to_string(iNumOfLevels), olc::WHITE);

					}
					else if (iCurLevel < 36)
					{
						iLevelSet = 1;
						DrawString(128 + 20, 4, "Level: ", olc::WHITE);
						DrawString(128 + 70, 4, std::to_string(iCurLevel) + " / " + std::to_string(iNumOfLevels), olc::WHITE);
					}
					else
					{
						iLevelSet = 2;
						DrawString(128 + 20, 4, "Level: ", olc::WHITE);
						DrawString(128 + 70, 4, std::to_string(iCurLevel) + " / " + std::to_string(iNumOfLevels), olc::WHITE);
					}

					if (bDebugMode) // Debug Mode Indicator
					{
						DrawString(0 + 17, 0 + 17, "DEBUG", olc::DARK_GREY);
					}

					// Move Number UI
					switch (iCurLevel)
					{
					case 1:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_1), olc::WHITE);
						break;
					case 2:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_2), olc::WHITE);
						break;
					case 3:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_3), olc::WHITE);
						break;
					case 4:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_4), olc::WHITE);
						break;
					case 5:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_5), olc::WHITE);
						break;
					case 6:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_6), olc::WHITE);
						break;
					case 7:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_7), olc::WHITE);
						break;
					case 8:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_8), olc::WHITE);
						break;
					case 9:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_9), olc::WHITE);
						break;
					case 10:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_10), olc::WHITE);
						break;
					case 11:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_11), olc::WHITE);
						break;
					case 12:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_12), olc::WHITE);
						break;
					case 13:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_13), olc::WHITE);
						break;
					case 14:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_14), olc::WHITE);
						break;
					case 15:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_15), olc::WHITE);
						break;
					case 16:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_16), olc::WHITE);
						break;
					case 17:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_17), olc::WHITE);
						break;
					case 18:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_18), olc::WHITE);
						break;
					case 19:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_19), olc::WHITE);
						break;
					case 20:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_20), olc::WHITE);
						break;
					case 21:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_21), olc::WHITE);
						break;
					case 22:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_22), olc::WHITE);
						break;
					case 23:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_23), olc::WHITE);
						break;
					case 24:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_24), olc::WHITE);
						break;
					case 25:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_25), olc::WHITE);
						break;
					case 26:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_26), olc::WHITE);
						break;
					case 27:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_27), olc::WHITE);
						break;
					case 28:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_28), olc::WHITE);
						break;
					case 29:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_29), olc::WHITE);
						break;
					case 30:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_30), olc::WHITE);
						break;
					case 31:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_31), olc::WHITE);
						break;
					case 32:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_32), olc::WHITE);
						break;
					case 33:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_33), olc::WHITE);
						break;
					case 34:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_34), olc::WHITE);
						break;
					case 35:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_35), olc::WHITE);
						break;
					case 36:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_36), olc::WHITE);
						break;
					case 37:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_37), olc::WHITE);
						break;
					case 38:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_38), olc::WHITE);
						break;
					case 39:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_39), olc::WHITE);
						break;
					case 40:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_40), olc::WHITE);
						break;
					case 41:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_41), olc::WHITE);
						break;
					case 42:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_42), olc::WHITE);
						break;
					case 43:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_43), olc::WHITE);
						break;
					case 44:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_44), olc::WHITE);
						break;
					case 45:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_45), olc::WHITE);
						break;
					case 46:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_46), olc::WHITE);
						break;
					case 47:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_47), olc::WHITE);
						break;
					case 48:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_48), olc::WHITE);
						break;
					case 49:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_49), olc::WHITE);
						break;
					case 50:
						DrawString(4, (240 / 2) + 108, "Moves: " + std::to_string(iNumOfMoves_50), olc::WHITE);
						break;
					default:
						break;
					}

					// Timer Number UI
					switch (iCurLevel)
					{
					case 1:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_1)), olc::WHITE);
						break;
					case 2:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_2)), olc::WHITE);
						break;
					case 3:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_3)), olc::WHITE);
						break;
					case 4:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_4)), olc::WHITE);
						break;
					case 5:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_5)), olc::WHITE);
						break;
					case 6:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_6)), olc::WHITE);
						break;
					case 7:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_7)), olc::WHITE);
						break;
					case 8:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_8)), olc::WHITE);
						break;
					case 9:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_9)), olc::WHITE);
						break;
					case 10:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_10)), olc::WHITE);
						break;
					case 11:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_11)), olc::WHITE);
						break;
					case 12:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_12)), olc::WHITE);
						break;
					case 13:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_13)), olc::WHITE);
						break;
					case 14:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_14)), olc::WHITE);
						break;
					case 15:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_15)), olc::WHITE);
						break;
					case 16:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_16)), olc::WHITE);
						break;
					case 17:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_17)), olc::WHITE);
						break;
					case 18:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_18)), olc::WHITE);
						break;
					case 19:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_19)), olc::WHITE);
						break;
					case 20:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_20)), olc::WHITE);
						break;
					case 21:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_21)), olc::WHITE);
						break;
					case 22:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_22)), olc::WHITE);
						break;
					case 23:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_23)), olc::WHITE);
						break;
					case 24:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_24)), olc::WHITE);
						break;
					case 25:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_25)), olc::WHITE);
						break;
					case 26:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_26)), olc::WHITE);
						break;
					case 27:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_27)), olc::WHITE);
						break;
					case 28:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_28)), olc::WHITE);
						break;
					case 29:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_29)), olc::WHITE);
						break;
					case 30:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_30)), olc::WHITE);
						break;
					case 31:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_31)), olc::WHITE);
						break;
					case 32:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_32)), olc::WHITE);
						break;
					case 33:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_33)), olc::WHITE);
						break;
					case 34:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_34)), olc::WHITE);
						break;
					case 35:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_35)), olc::WHITE);
						break;
					case 36:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_36)), olc::WHITE);
						break;
					case 37:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_37)), olc::WHITE);
						break;
					case 38:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_38)), olc::WHITE);
						break;
					case 39:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_39)), olc::WHITE);
						break;
					case 40:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_40)), olc::WHITE);
						break;
					case 41:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_41)), olc::WHITE);
						break;
					case 42:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_42)), olc::WHITE);
						break;
					case 43:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_43)), olc::WHITE);
						break;
					case 44:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_44)), olc::WHITE);
						break;
					case 45:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_45)), olc::WHITE);
						break;
					case 46:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_46)), olc::WHITE);
						break;
					case 47:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_47)), olc::WHITE);
						break;
					case 48:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_48)), olc::WHITE);
						break;
					case 49:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_49)), olc::WHITE);
						break;
					case 50:
						DrawString(148, (240 / 2) + 108, "Time: " + FloatToString(TruncateFloat(iTime_50)), olc::WHITE);
						break;
					default:
						break;
					}
				}
				else // UI for win conditions
				{
					// Win Screen Stuff
					fTime_WinScreen += fElapsedTime;
					bEnableInput = false;
					if (fTime_WinScreen < 4.0f) // after 4 seconds remove splash, draw UI, and re-enable input
					{
						DrawSprite(olc::vi2d(0, 0), gfxWin.Sprite());
					}
					else
					{
						bEnableInput = true;
						iLevelSet = -1;
						audioEngine_Music.SetOutputVolume(0.0f);
						DrawString((256 / 2) - 108, (240 / 2) - 96, "YOU WIN!", olc::WHITE);
						DrawString((256 / 2) - 108, (240 / 2) + 92, "Thank you for playing!", olc::WHITE);
						DrawString((256 / 2) - 108, (240 / 2) + 78, "Press ESC for Main Menu", olc::WHITE);
					}
				}

				// Special UI for Displaying Checkpoint Codes that can be inputted on Main Menu Level Select screen
				if (iCurLevel == 16)
				{
					DrawString(148, 228, "Code: " + sMediumLevelCode, olc::WHITE);
				}
				else if (iCurLevel == 36)
				{
					DrawString(148, 228, "Code: " + sHardLevelCode, olc::WHITE);
				}

				// door switch tracking
				if (nSwitches >= vSwitches.size() && iCurLevel != -1)
				{
					if (bDoorsOpen == false) // play SFX on toggle
					{
						audioEngine.PlayWaveform(&audioSlot_DoorOpen, false, fAudioSpeed);
					}

					bDoorsOpen = true;
				}
				else
				{
					if (bDoorsOpen == true) // play SFX on toggle
					{
						audioEngine.PlayWaveform(&audioSlot_DoorClose, false, fAudioSpeed);
					}

					bDoorsOpen = false;
				}
				if (bDoors_DebugForceOpen)
				{
					bDoorsOpen = true;
				}

				// Win Tracking
				if (nGoals >= vGoals.size() && iCurLevel != -1)
				{
					RecordLevelTimePerFrame(fElapsedTime);

					// Update High Score vectors
					internalHighScoreUtility_Moves();
					internalHighScoreUtility_Time();

					iCurLevel++;
					LoadLevel(iCurLevel, false);
				}
				else if (bEnableInput && bDebugMode && GetKey(olc::Key::N).bPressed) // Debug Mode Skip
				{
					iCurLevel++;
					LoadLevel(iCurLevel, false);
				}
				else if (bEnableInput && bDebugMode && GetKey(olc::Key::H).bPressed) // debug mode score reset
				{
					for (int i = 0; i < vHighScore_Moves.size(); i++)
					{
						vHighScore_Moves[i] = 1000;
					}
					for (int i = 0; i < vHighScore_Time.size(); i++)
					{
						vHighScore_Time[i] = 10000;
					}

					fTotal_HS = 500000.0f;
					iTotal_HS = 50000;

					int iSave = SaveHighScores();
					int iLoad = LoadHighScores();
				}

				RecordLevelTimePerFrame(fElapsedTime);

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

				// Check for user input to unpause or Quit
				if (GetKey(olc::Key::ENTER).bPressed)										// Unpause
				{
					audioEngine.PlayWaveform(&audioSlot_UnPauseJingle, false, fAudioSpeed);	// Play UnPause Jingle SFX
					audioEngine_Music.SetOutputVolume(fMusicVolume);						// Unmute Music
					bPaused = false;														// Reset Flags
					bPauseJinglePlayed = false;
				}
				if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)		// Quit to Main Menu
				{
					audioEngine_Music.SetOutputVolume(fMusicVolume);						// Unmute Music
					bPaused = false;														// Reset Pause Flags
					bPauseJinglePlayed = false;

					ResetLevelScore();														// Reset Score for current level
					SaveHighScores();														// Save HighScores

					bMainMenu = true;														// Set Main Menu Flag
					LoadLevel(52, false);

					return true;
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
				DrawString(20, 45, "Stats for Current Level", olc::WHITE);
				DrawString(20, 70, sTimerUI, olc::WHITE);
				DrawString(20, 57, sMovementUI, olc::WHITE);
				DrawString(20, 212, "Press Enter to Resume", olc::WHITE);
				DrawString(20, 198, "Press ESC for Main Menu", olc::WHITE);

				return true;
			}
		}
	}
};

int main()
{
	Puzzle demo;
	// 1024 x 960
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}