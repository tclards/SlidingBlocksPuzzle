#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_MINIAUDIO
#include "olcPGEX_MiniAudio.h"
#include <fstream>
#include <cmath>

// Todo List
#pragma region TODO LIST

// TODO_BUGS
// Audio Problems - crackling constantly + audio delayed slightly from time it should be playing. Spam movement misses some play audio calls
// - crackling seems worst in the main menu. Only minor or not at all in game?

// TODO_AudioAdjustments
// movement fail speed * 2
// movement 1 speed * 2
// gamestartup spped * 1.25
// all music tracks speed * 0.75

// TODO_A - Finish Game
// Fill out hard levels
// Update Name
//		- project & exe output

// TODO_B - Prerelease Steps
// create promotional materials
//		- Trailer
//		- Showcases
// create itch.io page
// QA Test on multiple machines
#pragma endregion

// Controls
#pragma region Controls
// WASD or Arrow Keys to move Player Block or Make Selection in Menus
// ENTER to Choose Menu Option
// CTRL + ENTER and MINUS - Enable Debug Mode
// N (While Debug Enabled) - Next Level
// H (While Debug Enabled) - Reset Level Scores - Timer and Moves
// V (While Debug Enabled) - Open & Close Doors
// L (While Debug Enabled) - Skip to You Win Screen
#pragma endregion

// Globally Defined Variables & lambda functions
#pragma region Globals

// Defining Values for the Movement System
constexpr int NORTH = 0;
constexpr int EAST = 1;
constexpr int SOUTH = 2;
constexpr int WEST = 3;

// Defining Level Size &  Block Size
olc::vf2d vLevelSize = { 16,15 };
olc::vi2d vBlockSize = { 16,16 };

// Lambda Functions:
auto id = [&](olc::vi2d& pos) { return pos.y * vLevelSize.x + pos.x; }; // Translates Coordinates 2D to 1D
#pragma endregion

// Game Class
class Game : public olc::PixelGameEngine
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
	std::string sMediumLevelCode = "M1b3z"; // Skip to Level 16
	std::string sHardLevelCode = "Hx2p1";   // Skip to Level 36

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
	// Easy Levels - 15
	float iTime_1 = 0.0f;
	int iNumOfMoves_1 = 0;
	std::string sLevel_1 = 
		"################"
		"#P.............#"
		"#..............#"
		"##.##########.##"
		"#..............#"
		"#.....-..-.....#"
		"#.|...+.++...|.#"
		"#.............S#"
		"#.....@@@@.....#"
		"#.............S#"
		"#.....____.....#"
		"#.2..........3.#"
		"######D##D######"
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

	float iTime_4 = 0.0f;
	int iNumOfMoves_4 = 0;
	std::string sLevel_4 =
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

	float iTime_5 = 0.0f;
	int iNumOfMoves_5 = 0;
	std::string sLevel_5 =
		"################"
		"#P.........D..@#"
		"#.........######"
		"#..........9...#"
		"#..............#"
		"#..............#"
		"#....+.........#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#...........##D#"
		"#..........S#B.#"
		"#.....6.########"
		"#@@.....#O.-..@#"
		"################";

	float iTime_6 = 0.0f;
	int iNumOfMoves_6 = 0;
	std::string sLevel_6 =
		"################"
		"#S..-.........O#"
		"#.......+....9.#"
		"##D##########D##"
		"#..............#"
		"#.......@......#"
		"#......@P@.....#"
		"#.......@......#"
		"#..............#"
		"#......#D#.....#"
		"#......#|#.....#"
		"#......#.D.....#"
		"##############.#"
		"#B.............#"
		"################";

	float iTime_7 = 0.0f;
	int iNumOfMoves_7 = 0;
	std::string sLevel_7 =
		"################"
		"#@.......-....@#"
		"#..............#"
		"#........9.#...#"
		"#.........#@#..#"
		"#..............#"
		"#.............|#"
		"#......P.......#"
		"#..............#"
		"#..............#"
		"#..#@#.........#"
		"#...#.9........#"
		"#.......+......#"
		"#@............@#"
		"################";

	float iTime_8 = 0.0f;
	int iNumOfMoves_8 = 0;
	std::string sLevel_8 =
		"################"
		"#P.......-...@@#"
		"#..............#"
		"#..............#"
		"#..............#"
		"#@...........9B#"
		"#############..#"
		"#..#############"
		"#O9.........#..#"
		"#9..........#..#"
		"#........+..D..#"
		"#...........#|.#"
		"#...S.......#..#"
		"#@@.........#@@#"
		"################";

	float iTime_9 = 0.0f;
	int iNumOfMoves_9 = 0;
	std::string sLevel_9 =
		"################"
		"#P.............#"
		"#...........##.#"
		"#...+......#@..#"
		"#.........#@...#"
		"#..+.....#@....#"
		"#....|.@#......#"
		"#.....@#....+..#"
		"#....@#........#"
		"#4...#....+....#"
		"#...#..........#"
		"#..#@...+......#"
		"#@#.....6......#"
		"##............@#"
		"################";

	float iTime_10 = 0.0f;
	int iNumOfMoves_10 = 0;
	std::string sLevel_10 =
		"################"
		"#P-...........@#"
		"#############.##"
		"#@........@##..#"
		"#..........###|#"
		"#..........#O#.#"
		"#|.........#.#.#"
		"#..........#|#.#"
		"#..........#.#.#"
		"#.........+D.#.#"
		"#....8.....#S#.#"
		"#@........@###.#"
		"############...#"
		"#B...........#@#"
		"################";

	float iTime_11 = 0.0f;
	int iNumOfMoves_11 = 0;
	std::string sLevel_11 =
		"################"
		"#S.-P.B##O..-.S#"
		"###D########D###"
		"#..9...##...9..#"
		"#......##......#"
		"#......##......#"
		"#....+.##.+....#"
		"#......##......#"
		"#.#@#..##..#@#.#"
		"#|.#.#.##.#.#.|#"
		"#......##......#"
		"#......##......#"
		"#......##......#"
		"#@....@##@....@#"
		"################";

	float iTime_12 = 0.0f;
	int iNumOfMoves_12 = 0;
	std::string sLevel_12 =
		"################"
		"#P.....SS......#"
		"#.+..##||##5.+.#"
		"#....--@@--....#"
		"#....#@@@@#....#"
		"###D##.@@.##D###"
		"#B....|..|....O#"
		"#####......#####"
		"################"
		"################"
		"################"
		"################"
		"################"
		"################"
		"################";

	float iTime_13 = 0.0f;
	int iNumOfMoves_13 = 0;
	std::string sLevel_13 =
		"################"
		"#P1...........@#"
		"#1.1..9........#"
		"#.@.........4..#"
		"#........@.....#"
		"#...........1..#"
		"#....5.........#"
		"#.7.......@....#"
		"#...........8..#"
		"#..@...........#"
		"#..............#"
		"#..3....5......#"
		"#...........@..#"
		"#....@...6.....#"
		"################";

	float iTime_14 = 0.0f;
	int iNumOfMoves_14 = 0;
	std::string sLevel_14 =
		"################"
		"#PB#.......#@@@#"
		"####......9D@@@#"
		"#..........##DD#"
		"#..............#"
		"#............|.#"
		"#..............#"
		"#.....+........#"
		"#..............#"
		"#..........+...#"
		"#.6............#"
		"#..............#"
		"#...+........###"
		"#S............O#"
		"################";

	float iTime_15 = 0.0f;
	int iNumOfMoves_15 = 0;
	std::string sLevel_15 =
		"################"
		"#P............B#"
		"################"
		"#######......###"
		"#######.####.###"
		"#######.###@.###"
		"#######D########"
		"#@.............#"
		"#......@O...-|S#"
		"#.........-..SS#"
		"###D############"
		"#....9..6.....@#"
		"#..7..+........#"
		"#...........@..#"
		"################";

	// Medium Levels - 20
	float iTime_16 = 0.0f;
	int iNumOfMoves_16 = 0;
	std::string sLevel_16 =
		"################"
		"##@@############"
		"#.............P#"
		"#..D#########+##"
		"#..............#"
		"##D##########D.#"
		"#..............#"
		"#.D##########D##"
		"#..............#"
		"##D##########D.#"
		"#..............#"
		"#.+#############"
		"#.|..........-S#"
		"#.@@############"
		"################";

	float iTime_17 = 0.0f;
	int iNumOfMoves_17 = 0;
	std::string sLevel_17 =
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

	float iTime_18 = 0.0f;
	int iNumOfMoves_18 = 0;
	std::string sLevel_18 =
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

	float iTime_19 = 0.0f;
	int iNumOfMoves_19 = 0;
	std::string sLevel_19 =
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

	float iTime_20 = 0.0f;
	int iNumOfMoves_20 = 0;
	std::string sLevel_20 = 
		"################"
		"##@#########@###"
		"#.............##"
		"#.-----------.##"
		"#.............##"
		"#.|.....6.....##"
		"#.............##"
		"#......P......##"
		"#.|..........+##"
		"#.........9....#"
		"#....3......4..#"
		"#.-----------..#"
		"#..............#"
		"##@#########@@@#"
		"################";

	float iTime_21 = 0.0f;
	int iNumOfMoves_21 = 0;
	std::string sLevel_21 =
		"################"
		"#@O...........P#"
		"#B##############"
		"#|#@@.....3..@@#"
		"#|#@....-.....@#"
		"#|#....5..6....#"
		"#.#|..5........#"
		"#.#........9...#"
		"#.###..........#"
		"#..+D..........#"
		"#.###..........#"
		"#S###...+......#"
		"#S#.11111.1111.#"
		"#S#@.........@@#"
		"################";

	float iTime_22 = 0.0f;
	int iNumOfMoves_22 = 0;
	std::string sLevel_22 =
		"################"
		"#@...........#@#"
		"#.........-...@#"
		"#|.....9.......#"
		"#....+......5..#"
		"#@............@#"
		"############D###"
		"#@......@#S#.#S#"
		"#........#.#.#.#"
		"#|..+...|#.#.#.#"
		"#........#|#.#|#"
		"#@...-..@#..O..#"
		"########.#######"
		"#B............P#"
		"################";

	float iTime_23 = 0.0f;
	int iNumOfMoves_23 = 0;
	std::string sLevel_23 =
		"################"
		"#P1....1....@..#"
		"#1..1........1.#"
		"#....-...@.3...#"
		"#.11.........4.#"
		"#...@...|.1....#"
		"#.3............#"
		"#..1.+..1...@..#"
		"#..@.........1.#"
		"#.......@......#"
		"#.1..........@.#"
		"#..1.@...1....1#"
		"#.8...1.....1..#"
		"#...1....@....1#"
		"################";

	float iTime_24 = 0.0f;
	int iNumOfMoves_24 = 0;
	std::string sLevel_24 =
		"################"
		"#PD@############"
		"#.##############"
		"#.-...........S#"
		"####.########.##"
		"####.########.##"
		"####.S#######.##"
		"####.|#S....-.##"
		"#S.-..#######.##"
		"####.########.##"
		"##...#S.2.#S.4.#"
		"##2#.####.#...2#"
		"##.#......#....#"
		"##S##.-..S##..S#"
		"################";

	float iTime_25 = 0.0f;
	int iNumOfMoves_25 = 0;
	std::string sLevel_25 =
		"################"
		"#@#@...P....@#@#"
		"#D#####..#####D#"
		"#.....9..9.....#"
		"#..............#"
		"#..............#"
		"#D#..........#D#"
		"#|##....O...##|#"
		"#D.##########.D#"
		"#S....5B9.....S#"
		"#......9.......#"
		"#.............|#"
		"#..............#"
		"#S1...........S#"
		"################";

	float iTime_26 = 0.0f;
	int iNumOfMoves_26 = 0;
	std::string sLevel_26 =
		"################"
		"#P.....##@.....#"
		"#......#####D###"
		"#..............#"
		"#......##......#"
		"#......##.....B#"
		"################"
		"#.....O#########"
		"#++++++#########"
		"#......#########"
		"#......#########"
		"#......##......#"
		"#.........@@@@@#"
		"#S.....##......#"
		"################";

	float iTime_27 = 0.0f;
	int iNumOfMoves_27 = 0;
	std::string sLevel_27 =
		"################"
		"#P.#.1...1...1S#"
		"##.#1.1.191....#"
		"#.@#....1..1...#"
		"#.##..1....51..#"
		"#.#@....1..1...#"
		"#.####.#########"
		"#............1@#"
		"#############D##"
		"#...222B#O...8.#"
		"#...141.#......#"
		"#.......#......#"
		"#...@...#......#"
		"#.......#@.....#"
		"################";

	float iTime_28 = 0.0f;
	int iNumOfMoves_28 = 0;
	std::string sLevel_28 =
		"################"
		"#..............#"
		"#...#@..|..-...#"
		"#...#@..|..-...#"
		"#.P.#@..|..-..O#"
		"########|#######"
		"#.......|......#"
		"#......6D.7....#"
		"#.......|....9.#"
		"#.7.....|......#"
		"#.......|......#"
		"#......S1S.....#"
		"#......S1S.....#"
		"#B.....S.S.....#"
		"################";

	float iTime_29 = 0.0f;
	int iNumOfMoves_29 = 0;
	std::string sLevel_29 =
		"################"
		"#....##.#......#"
		"#...O#@....-+..#"
		"#....##.#...+..#"
		"#@...##.#...+..#"
		"#######.####D###"
		"#P.............#"
		"#######.####9###"
		"#...@.#.#......#"
		"#.....#.#.++...#"
		"#.B..5D.#......#"
		"#...|.#.#......#"
		"#.....#.#@....@#"
		"#...@.#.#S@..@S#"
		"################";

	float iTime_30 = 0.0f;
	int iNumOfMoves_30 = 0;
	std::string sLevel_30 =
		"################"
		"#....@...@.....#"
		"#..............#"
		"#.........+....#"
		"#..............#"
		"#......6.......#"
		"#..|...O...|...#"
		"#..-..@#@..-...#"
		"#-#S#D#P#D#S#--#"
		"#..#.|...|.#...#"
		"#......B.......#"
		"#..2........2..#"
		"###S#......#S###"
		"#..@#......#@..#"
		"################";

	float iTime_31 = 0.0f;
	int iNumOfMoves_31 = 0;
	std::string sLevel_31 =
		"################"
		"#@......P-....@#"
		"#.#3#########4##"
		"#....#@##@#....#"
		"##2..#.##.#...##"
		"#@...3.##.3...@#"
		"##...#D##D#...##"
		"#......##......#"
		"#.S....DD.+....#"
		"##1#...##...####"
		"#...#..##B.#.@.#"
		"#....#@#@##....#"
		"#....O##D##..|.#"
		"#..............#"
		"################";

	float iTime_32 = 0.0f;
	int iNumOfMoves_32 = 0;
	std::string sLevel_32 =
		"################"
		"#..............#"
		"#...+........3.#"
		"#........+...1.#"
		"#......6.......#"
		"#@@##.8##7.##@@#"
		"###@@#D##D#@@###"
		"#......P.......#"
		"#..1....+...-D@#"
		"###D###.....|#.#"
		"#.....#........#"
		"#...6.##6#######"
		"#.....#........#"
		"#.....#S....S-S#"
		"################";

	float iTime_33 = 0.0f;
	int iNumOfMoves_33 = 0;
	std::string sLevel_33 =
		"################"
		"#..B#......#O..#"
		"#|..#..|...#.9.#"
		"#...#......#...#"
		"#.P.#......#...#"
		"#...#......#...#"
		"#...#......#...#"
		"#.3.#......#...#"
		"#.###.#@@#.###.#"
		"#S....#++#..-.S#"
		"#S....#++#....S#"
		"##....#++#....##"
		"#####D#DD#D#####"
		"#@@@........@@@#"
		"################";

	float iTime_34 = 0.0f;
	int iNumOfMoves_34 = 0;
	std::string sLevel_34 =
		"################"
		"#@.2.PO##B..2.S#"
		"###D############"
		"#..2...##..@...#"
		"#.@3...##......#"
		"#......##......#"
		"#...@..##......#"
		"#......##......#"
		"#......##.....@#"
		"##4########.####"
		"#.3........1...#"
		"#.3.@......2.@##"
		"#@.........|...#"
		"#@.........1...#"
		"################";

	float iTime_35 = 0.0f;
	int iNumOfMoves_35 = 0;
	std::string sLevel_35 =
		"################"
		"#S-........P..@#"
		"#@#3.....#######"
		"#S....6-...#.@.#"
		"#@#........#.1.#"
		"#S..+......#.@.#"
		"#@#........-S..#"
		"#D#........#+@.#"
		"#+#............#"
		"#.#............#"
		"#.#............#"
		"#..............#"
		"#|.............#"
		"#..............#"
		"################";

	// Hard Levels - 15
	float iTime_36 = 0.0f;
	int iNumOfMoves_36 = 0;
	std::string sLevel_36 =
		"################"
		"################"
		"################"
		"###1...1....|@##"
		"###....222.1..##"
		"###1..1.7.1...##"
		"###..2.131.2..##"
		"###.12.2P2.21.##"
		"###..2.131.2..##"
		"###.1.1.7.1...##"
		"###....222..1.##"
		"###@|.1.1.....##"
		"#######..#######"
		"##@....-.....@##"
		"################";

	float iTime_37 = 0.0f;
	int iNumOfMoves_37 = 0;
	std::string sLevel_37 =
		"################"
		"#P......+.....S#"
		"########|#######"
		"#@##..##D#######"
		"#.....##.###..##"
		"#.###.##.###.9##"
		"#.##.4.#.###..##"
		"#.#@..@#.####.##"
		"#.######.#.....#"
		"#............+.#"
		"#..3.#.#@#@#####"
		"#@####.###@#@1@#"
		"#|.....##.+..#.#"
		"#...##....-....#"
		"################";

	float iTime_38 = 0.0f;
	int iNumOfMoves_38 = 0;
	std::string sLevel_38 =
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

	float iTime_39 = 0.0f;
	int iNumOfMoves_39 = 0;
	std::string sLevel_39 =
		"################"
		"###P#.###O#.#.##"
		"###....#.......#"
		"##.#.#.#####@-@#"
		"#..........#####"
		"#.#.|.#.#.##...#"
		"#...3.....#..S.#"
		"##.#S#.#.##..#@#"
		"#.+###.+......##"
		"#...#.###.#.7..#"
		"#..|....#......#"
		"#D#D#D####.#+#.#"
		"#.-....@####...#"
		"#B#@#@########@#"
		"################";

	float iTime_40 = 0.0f;
	int iNumOfMoves_40 = 0;
	std::string sLevel_40 =
		"################"
		"#1.1.1.1@1.1.1P#"
		"#.1.1.1.1.1.1.1#"
		"#1.1@1.1.1.1@1.#"
		"#.1.1.1.1.1.1.1#"
		"#1.1.1.1.1.1.1.#"
		"#.1.1@1.1.1.1.1#"
		"#1.1.1.1.1@1.1.#"
		"#@1.1.1.1.1.1.1#"
		"#1.1.1.1.1.1.1.#"
		"#.1.1.1@1.1.1.1#"
		"#1.1.1.1.1.1.1.#"
		"#.1@1.1.1.1.1.1#"
		"#1.1.1.1.1.1.1@#"
		"################";

	float iTime_41 = 0.0f;
	int iNumOfMoves_41 = 0;
	std::string sLevel_41 =
		"################"
		"#P...........#@#"
		"#...........S#@#"
		"#............#@#"
		"#............#@#"
		"#...........-D@#"
		"#............#@#"
		"#............#@#"
		"#............#7#"
		"#............#.#"
		"#...........9#|#"
		"#...........19.#"
		"#....+..._.+.9@#"
		"#@.............#"
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

	// Vector containing level goals
	std::vector<olc::vi2d> vGoals;

	// Vector containing door positions
	std::vector<olc::vi2d> vDoors_pos;

	// Flag for shutting down game
	bool bQuitGame = false;

	// Flag for controller debug frame numbers for main menu scene
	bool bDrawMainMenuSceneFrameNumber = false;

	// Vector containing door switches
	std::vector<olc::vi2d> vSwitches;

	// Variables for UI Draw Color
	olc::Pixel pColor_GameUI = olc::WHITE;
	olc::Pixel pColor_PauseUI = olc::WHITE;
	olc::Pixel pColor_WinUI = olc::WHITE;
	olc::Pixel pColor_RectUI_Fill = olc::BLACK;
	olc::Pixel pColor_RectUI_Border = olc::WHITE;

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
	float fTele_RotateSpeed = 0.1f;

	// Flag for initial options loading during startup routine
	bool bInitialLoad = false;

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
	olc::Renderable gfxCursors;
	olc::Renderable gfxMenuScene;
	olc::Renderable gfxBackground_MainMenu;
	olc::Renderable gfxBackground_LevelSelect;
	olc::Renderable gfxBackground_HighScores;
	olc::Renderable gfxBackground_Options;
	olc::Renderable gfxBackground_Credits;
	olc::Renderable gfxBackground_Pause;
	olc::Renderable gfxBackground_Easy;
	olc::Renderable gfxBackground_Medium;
	olc::Renderable gfxBackground_Hard;
	olc::Renderable gfxBackground_YouWin;

	// Flag for enabling and disabling input
	bool bEnableInput = true;

	// Flag for controlling door behaivor
	bool bDoorsOpen = false;
	bool bDoors_DebugForceOpen = false;

	// Variable tracking which level the player is on
	int iCurLevel = 1;

	// Variables used for main menu cursor/selection system
	int iMenuSelectCursor_main = 0; // 0 - Start Game, 1 - Level Select, 2 - High Scores, 3 - Options, 4 - Credits, 5, Quit Game
	int iMenuSelectCursor_options = 0; // 0 - Music Volume, 1 - SFX Volume
	float fCursorBlinkTimer = 0.0f;
	float fCursorBlinkSpeed = 0.5f; // Timing for cursor blink
	bool bCursorBlink = false;
	bool bDoCursorBlink = true; // debug variable for disabling cursor blink

	// Variables for Main Menu Game Title Visual Effect
	int iTitleSwitch = 0; // 0 to 5
	float fTitleSwitchTimer = 0.0f;
	float fTitleSwitchSpeed = 0.2f;

	// Variables for Main Menu Scene
	bool bRestartScene = false;
	int iMenuSceneState = -3; // -1 to 32
	float fMenuSceneTimer = 0.0f; // timer variables for speed of menu scene playback
	float fMenuSceneSpeed = 0.4f;
	float fMenuSceneRestartEffectTimer = 0.0f; // timer variables for how long to do restart scene effect
	float fMenuSceneRestartEffectTimerSpeed = 2.1f;
	float fSceneEffectTimer = 0.0f; // timer variables for actual blink effect
	float fSceneEffectSpeed = 0.35f;
	float fSceneFreezeTimer = 0.0f; // timer variables for freeze at end of restart effect
	float fSceneFreezeSpeed = 1.5f;
	bool bSceneEffect = false;

	// Flags for pausing game
	bool bPaused = false;
	bool bPauseJinglePlayed = false;

	// Main Menu Flags
	bool bMainMenu = true;
	int iCurDisplay = -1; // - 1: main menu, 0: high score screen, 1: options screen, 2: Level Select Screen, 3: Credits Screen

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
	olc::MiniAudio audioEngine;
	olc::MiniAudio audioEngine_Music;

	// Saved Reference to currently looping background music
	int* audio_CurrentBackgroundTrack;

	// Audio IDs
	int audio_backgroundMusic_1;
	int audio_backgroundMusic_2;
	int audio_backgroundMusic_3;
	int audio_backgroundMusic_Menu;
	int audio_Movement_1;
	int audio_Movement_Fail;
	int audio_RestartLevel;
	int audio_WinJingle;
	int audio_LevelTransition;
	int audio_PauseJingle;
	int audio_UnPauseJingle;
	int audio_LevelCode;
	int audio_LevelCode_Fail;
	int audio_GameStartUp;
	int audio_Door_Open;
	int audio_Door_Close;
	int audio_Teleport;
	int audio_Teleport_Fail;
	int audio_ButtonClick_1;
	int audio_ButtonClick_2;

	// Volume Variables
	float fMusicVolume = 0.3f;
	float fSFXVolume = 0.2f;

	// Audio File Paths
	std::string sBackgroundMusic_1 = "SFX//BackgroundMusic_1.wav";	
	std::string sBackgroundMusic_2 = "SFX//BackgroundMusic_2.wav";
	std::string sBackgroundMusic_3 = "SFX//BackgroundMusic_3.wav";
	std::string sBackgroundMusic_Menu = "SFX//MenuMusic_1.wav";
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
	Game()
	{
		sAppName = "I N C R E M E N T O";
	}

	// Deconstructor
	~Game()
	{

	}

	// Core Functions
	#pragma region Core Functions

	// Runs once at start of game
	bool OnUserCreate() override
	{
		// Audio Loading
		LoadAllAudio();

		// Graphics Loading
		gfxTiles.Load("Graphics//TileSheet.png");
		gfxSplash.Load("Graphics//SplashScreen.png");
		gfxWin.Load("Graphics//WinScreen.png");
		gfxCursors.Load("Graphics//Cursors.png");
		gfxMenuScene.Load("Graphics//MainMenuScene.png");
		gfxBackground_MainMenu.Load("Graphics//background//background_Menu_MainMenu.png");
		gfxBackground_LevelSelect.Load("Graphics//background//background_Menu_Levelselect.png");
		gfxBackground_HighScores.Load("Graphics//background//background_Menu_HighScores.png");
		gfxBackground_Options.Load("Graphics//background//background_Menu_Options.png");
		gfxBackground_Credits.Load("Graphics//background//background_Menu_Credits.png");
		gfxBackground_Pause.Load("Graphics//background//background_Pause.png");
		gfxBackground_Easy.Load("Graphics//background//background_Easy.png");
		gfxBackground_Medium.Load("Graphics//background//background_Medium.png");
		gfxBackground_Hard.Load("Graphics//background//background_Hard.png");
		gfxBackground_YouWin.Load("Graphics//background//background_Menu_YouWin.png");

		// Level Loading
		LoadAllLevels();

		// Load Blank Menu Level
		LoadLevel(iCurLevel, false);
		bMainMenu = true;

		return true;
	}

	// Gameplay per Frame
	bool DoGameplayLogic(float fElapsedTime)
	{
		// Teleport GFX Check
		if (bLevelHasTeleports) { DoTeleportFlipCheck(fElapsedTime); }

		// User Input
		#pragma region User Input

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
			if (bEnableInput && GetKey(olc::Key::ESCAPE).bPressed && !bPaused) // Gameplay Pause
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
			if (bEnableInput && bDebugMode && GetKey(olc::Key::L).bPressed) // Skip to end game You Win Screen, also disable debug mode
			{
				bDebugMode = false; 
				iCurLevel = 51;
				LoadLevel(iCurLevel, false);
			}
		}
		else
		{
			// Win Screen ESC
			if (bEnableInput && GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::P).bPressed)
			{
				fTime_WinScreen = 0.0f;
				ToggleMusic();										// Unpause Background Music
				bMainMenu = true;									// Set Flag

				// Reset All Level Score Data
				DoLevelScoreReset();

				return true;
			}
		}
#pragma endregion

		// Movement Logic
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
						if (bLevelHasTeleports)
						{
							if (vBlockPos == vTele_Blue || vBlockPos == vTele_Orange)
							{
								bTargetIsTeleporter = true;
							}
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
					audioEngine.Play(audio_Movement_Fail);
				}
				else if (iMovementSuceededOrFailed == 1) // Movement Succeeded
				{
					audioEngine.Play(audio_Movement_1);
				}
			}
			else if (iTeleportSucceededOrFailed == 0) // teleport failed
			{
				audioEngine.Play(audio_Teleport_Fail);
			}
			else if (iTeleportSucceededOrFailed == 1) // teleport succeeded
			{
				audioEngine.Play(audio_Teleport);
			}
		}

		// Move Counting
		DoLevelMoveCounting(bPlayerMoved, iMovementSuceededOrFailed);

		// Draw Function
		DrawLevel(iLevelSet, false);

		// UI and Level Set Tracking
		DrawUI(fElapsedTime);

		// Per Frame Button Checking - Win Tiles & Door Switches
		DoButtonCheck(bPlayerMoved, fElapsedTime);

		// Add to Level Timer per frame
		RecordLevelTimePerFrame(fElapsedTime);

		return true;
	}

	// Check Win Conditions per Frame
	void DoWinLogic(float fElapsedTime)
	{
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
			ResetLevelScore();
		}
	}

	// Button Checking & Button Logic per frame
	void DoButtonCheck(bool bPlayerMoved, float fElapsedTime)
	{
		// Win Tile Button Check
		if (vGoals.size() != 0)
		{
			int nGoals_Previous = nGoals; // take current win tile count before updating number for this frame
			nGoals = 0;
			for (auto& g : vGoals)
			{
				if (vLevel[id(g)] != nullptr)
				{
					nGoals++; // Increment Goals
				}
			} // Button SFX:
			if (nGoals > nGoals_Previous) // a new win tile has been covered this frame - play sfx
			{
				audioEngine.Play(audio_ButtonClick_1);
			}
			else if (nGoals < nGoals_Previous) // a previously covered win tile has been uncovered this frame - play sfx
			{
				audioEngine.Play(audio_ButtonClick_2);
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
					audioEngine.Play(audio_ButtonClick_2);
					audioEngine.Play(audio_ButtonClick_1);
				}
			}
		}

		// Door Switch Checking
		if (vSwitches.size() != 0)
		{
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
				audioEngine.Play(audio_ButtonClick_1);
			}
			else if (nSwitches < nSwitches_Previous) // a previously covered switch has been uncovered this frame - play sfx
			{
				audioEngine.Play(audio_ButtonClick_2);
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
					audioEngine.Play(audio_ButtonClick_2);
					audioEngine.Play(audio_ButtonClick_1);
				}
			}
		}

		// Door Switch Button Checking
		if (vSwitches.size() != 0)
		{
			if (bDoors_DebugForceOpen)
			{
				if (bDoorsOpen == false) // play SFX on toggle
				{
					audioEngine.Play(audio_Door_Open);
				}

				bDoorsOpen = true;
			}
			else if (nSwitches >= vSwitches.size() && iCurLevel != -1)
			{
				if (bDoorsOpen == false) // play SFX on toggle
				{
					audioEngine.Play(audio_Door_Open);
				}

				bDoorsOpen = true;
			}
			else
			{
				if (bDoorsOpen == true) // play SFX on toggle
				{
					audioEngine.Play(audio_Door_Close);
				}

				bDoorsOpen = false;
			}
		}

		// Win Tile Button Logic
		DoWinLogic(fElapsedTime);
	}

	// Runs every frame
	bool OnUserUpdate(float fElapsedTime) override
	{
		// Startup Routine
		#pragma region StartUp Routine

		// Do StartUp Routine at beginning of game
		if (!bGameStarted)
		{
			// Make sure Background music is stopped
			audioEngine_Music.Stop(audio_backgroundMusic_Menu);

			// Load options into memory once on first frame of game - before startup
			if (!bInitialLoad)
			{
				// Load Options
				iOptionsLoad = LoadOptions();

				// Check & Set Volume
				SetVolume();

				// Load High Scores
				iHighScoreLoad = LoadHighScores();

				bInitialLoad = true;
			}

			// Play the StartUP SFX one time
			if (bDoStartUpJingle)
			{
				// Play Game Start Up SFX
				audioEngine.Play(audio_GameStartUp);

				// Set Flag
				bDoStartUpJingle = false;
			}

			// Draw Splash Screen
			DrawSprite(olc::vi2d(0, 0), gfxSplash.Sprite());

			// Wait for StartUp SFX to finish, then set flags to stop Start Up Routine
			if (audioEngine.GetSound(audio_GameStartUp)->atEnd)
			{
				// Start Background Music
				audioEngine_Music.Play(audio_backgroundMusic_Menu);

				// Set Flags
				bGameStarted = true;
			}

			return true;
		}
#pragma endregion

		// Game Logic Per Frame
		if (bGameStarted)
		{
			// Check for Quit Game flag
			if (bQuitGame) { StopAllAudio(); return false; }

			// Main Menu Logic
			if (bMainMenu) { MainMenu(fElapsedTime); return true; }

			// Gameplay Logic
			if (!bPaused) { return DoGameplayLogic(fElapsedTime); }

			// Pause Logic
			if (bPaused) { return DoPauseLogic(fElapsedTime); }
		}
	}

	// Pause Function
	bool DoPauseLogic(float fElapsedTime)
	{
		// Play SFX
		if (bPauseJinglePlayed == false)
		{
			audioEngine.Play(audio_PauseJingle);
			bPauseJinglePlayed = true;
		}

		// Pause Background Music
		ToggleMusic();

		// Check for user input to unpause or Quit
		if (GetKey(olc::Key::ENTER).bPressed)										// Unpause
		{
			audioEngine.Play(audio_UnPauseJingle);									// Play UnPause Jingle SFX
			ToggleMusic();															// Unmute Music
			bPaused = false;														// Reset Flags
			bPauseJinglePlayed = false;
		}
		if (GetKey(olc::Key::ESCAPE).bPressed)										// Quit to Main Menu
		{
			ToggleMusic();															// Unmute Music
			bPaused = false;														// Reset Pause Flags
			bPauseJinglePlayed = false;

			ResetLevelScore();														// Reset Score for current level
			SaveHighScores();														// Save HighScores

			bMainMenu = true;														// Set Main Menu Flag
			LoadLevel(52, false);

			return true;
		}

		// Draw Pause UI
		DrawPauseMenu();

		return true;
	}

	// Called per Frame while Main Menu flag is on
	void MainMenu(float fElapsedTime)
	{
		if (!audioEngine_Music.IsPlaying(audio_backgroundMusic_Menu))
		{
			audioEngine_Music.Play(audio_backgroundMusic_Menu);
		}

		iLevelSet = -1;

		if (bDebugMode)
		{
			bDebugMode = false;
		}

		switch (iCurDisplay)
		{
		case 0: // High Score Screen
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed)										// Close Menu
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

			// Draw Background & Blank Menu Level
			DrawSprite(olc::vi2d(0, 0), gfxBackground_HighScores.Sprite());
			DrawLevel(iLevelSet, false);

			// Draw High Score UI
			DrawUI_HighScores();

			break;
		case 1: // Options Screen
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed)	// Close Menu
			{
				// Save Changes
				iOptionsSave = SaveOptions();

				iMenuSelectCursor_options = 0;
				bCursorBlink = false;
				fCursorBlinkTimer = 0.0f;
				iCurDisplay = -1;
			}

			// User Input:
			if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
			{
				bCursorBlink = false;
				fCursorBlinkTimer = 0.0f;

				iMenuSelectCursor_options--;
				if (iMenuSelectCursor_options < 0)
				{
					iMenuSelectCursor_options = 1;
				}
			}
			if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
			{
				bCursorBlink = false;
				fCursorBlinkTimer = 0.0f;

				iMenuSelectCursor_options++;
				if (iMenuSelectCursor_options > 1)
				{
					iMenuSelectCursor_options = 0;
				}
			}

			if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed)
			{
				if (iMenuSelectCursor_options == 0) // Increase music volume
				{
					fMusicVolume += 0.1f;
				}
				else if (iMenuSelectCursor_options == 1) // Increase SFX Volume
				{
					fSFXVolume += 0.1f;
					audioEngine.Play(audio_Movement_1);
				}
			}
			if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed)
			{
				if (iMenuSelectCursor_options == 0) // Decrease music volume
				{
					fMusicVolume -= 0.1f;
				}
				else if (iMenuSelectCursor_options == 1) // Decrease SFX Volume
				{
					fSFXVolume -= 0.1f;
					audioEngine.Play(audio_Movement_1);
				}
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
			SetVolume();

			// Draw Background & Blank Menu Level
			DrawSprite(olc::vi2d(0, 0), gfxBackground_Options.Sprite());
			DrawLevel(iLevelSet, false);

			// Draw Options UI
			DrawUI_Options(fElapsedTime);
			
			break;
		case 2: // Level Select Code Screen
			// User Input
			if (GetKey(olc::Key::ESCAPE).bPressed)
			{
				if (IsTextEntryEnabled() == true) // Cancel Text Entry on ESC if Text Entry is Active
				{
					// Play Fail SFX
					audioEngine.Play(audio_LevelCode_Fail);

					// Disable Text Entry
					TextEntryEnable(false);

					// Reset sInput string
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
				}
				else // Close Level Select Menu on ESC by default
				{
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
					iCurDisplay = -1;
				}
			}

			// Enable Text Entry on Enter
			if (GetKey(olc::Key::ENTER).bPressed && IsTextEntryEnabled() == false)
			{
				TextEntryEnable(true);
			}

			// Use Checkpoint Code, if player has inputted one
			if (sInputCode != "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd")		// Some kind of user input has been detected in the string
			{
				if (sInputCode == sMediumLevelCode)
				{
					// Play Success SFX
					audioEngine.Play(audio_LevelCode);

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
					audioEngine.Play(audio_LevelCode);

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
					audioEngine.Play(audio_LevelCode_Fail);

					// Disable Text Entry
					TextEntryEnable(false);

					// Reset sInput string
					sInputCode = "1jhksvdfjhkghkdjsgfakjhs2376834876387236hsvbfdhjd";
				}
			}

			// Draw Background & Blank Menu Level
			DrawSprite(olc::vi2d(0, 0), gfxBackground_LevelSelect.Sprite());
			DrawLevel(iLevelSet, false);

			// Draw Level Select UI
			DrawUI_LevelSelect();

			break;
		case 3: // Credits Screen
			// User Input:
			if (GetKey(olc::Key::ESCAPE).bPressed)	// Close Menu
			{
				// Save Changes
				iOptionsSave = SaveOptions();

				iCurDisplay = -1;
			}

			// Draw Background & Blank Menu Level
			DrawSprite(olc::vi2d(0, 0), gfxBackground_Credits.Sprite());
			DrawLevel(iLevelSet, false);

			// Draw UI
			DrawUI_Credits();

			break;
		default: // Main Menu
			// User Input:
			if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
			{
				bCursorBlink = false;
				fCursorBlinkTimer = 0.0f;

				iMenuSelectCursor_main--;
				if (iMenuSelectCursor_main < 0)
				{
					iMenuSelectCursor_main = 5;
				}
			}
			if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
			{
				bCursorBlink = false;
				fCursorBlinkTimer = 0.0f;

				iMenuSelectCursor_main++;
				if (iMenuSelectCursor_main > 5)
				{
					iMenuSelectCursor_main = 0;
				}
			}
			if (GetKey(olc::Key::ENTER).bPressed)
			{
				switch (iMenuSelectCursor_main)
				{
				case 0: // Start Game
					ResetMainMenu();
					bMainMenu = false;
					iCurLevel = 1;
					LoadLevel(iCurLevel, false);
					break;
				case 1: // Level Select
					ResetMainMenu();
					iMenuSelectCursor_main = 1;
					iCurDisplay = 2;
					break;
				case 2: // High Scores
					ResetMainMenu();
					iMenuSelectCursor_main = 2;
					iCurDisplay = 0;
					break;
				case 3: // Options
					ResetMainMenu();
					iMenuSelectCursor_main = 3;
					iCurDisplay = 1;
					break;
				case 4: // Credits
					ResetMainMenu();
					iMenuSelectCursor_main = 4;
					iCurDisplay = 3;
					break;
				case 5: // Quit Game
					ResetMainMenu();
					iMenuSelectCursor_main = 5;
					internalHighScoreUtility_Time();
					internalHighScoreUtility_Moves();
					SaveHighScores();
					bQuitGame = true;
				default:
					break;
				}
			}

			// Draw Background & Blank Menu Level
			DrawSprite(olc::vi2d(0, 0), gfxBackground_MainMenu.Sprite());
			DrawLevel(iLevelSet, false);

			// Draw UI
			DoGameTitle(fElapsedTime);
			DrawString((this->ScreenWidth() / 2) - 36, 51, "MAIN MENU", olc::WHITE);
			DrawString((this->ScreenWidth() / 2) - 40, 68, "Start Game", olc::CYAN);
			DrawString((this->ScreenWidth() / 2) - 48, 80, "Level Select", olc::MAGENTA);
			DrawString((this->ScreenWidth() / 2) - 44, 92, "High Scores", olc::YELLOW);
			DrawString((this->ScreenWidth() / 2) - 28, 104, "Options", olc::GREEN);
			DrawString((this->ScreenWidth() / 2) - 28, 116, "Credits", olc::BLUE);
			DrawString((this->ScreenWidth() / 2) - 15, 128, "Quit", olc::RED);

			// Draw cursor 
			DoCursorBlink(fElapsedTime); // update cursor blink variables

			if (!bCursorBlink) // only draw cursor on frames where cursor blink is toggled false
			{
				olc::vi2d vCursorPos_R = { 0,0 };
				olc::vi2d vCursorPos_L = { 0,0 };

				SetPixelMode(olc::Pixel::MASK);
				switch (iMenuSelectCursor_main)
				{
				case 0: // Start Game
					vCursorPos_R = { (this->ScreenWidth() / 2) + 40, 63 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 57, 63 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 0) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize); // L
					break;
				case 1: // Level Select
					vCursorPos_R = { (this->ScreenWidth() / 2) + 48, 75 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 65, 75 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 1) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 1) * vBlockSize, vBlockSize); // L
					break;
				case 2: // High Scores
					vCursorPos_R = { (this->ScreenWidth() / 2) + 45, 87 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 61, 87 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 4) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 4) * vBlockSize, vBlockSize); // L
					break;
				case 3: // Options
					vCursorPos_R = { (this->ScreenWidth() / 2) + 28, 99 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 45, 99 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 3) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize); // L
					break;
				case 4: // Credits
					vCursorPos_R = { (this->ScreenWidth() / 2) + 28, 111 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 45, 111 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 2) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize); // L
					break;
				case 5: // Quit Game
					vCursorPos_R = { (this->ScreenWidth() / 2) + 18, 123 };
					vCursorPos_L = { (this->ScreenWidth() / 2) - 34, 123 };
					DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(2, 0) * vBlockSize, vBlockSize); // R
					DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(3, 0) * vBlockSize, vBlockSize); // L
					break;
				default:
					break;
				}
				SetPixelMode(olc::Pixel::NORMAL);
			}

			// Draw game scene loop
			DoMainMenuGameScene(fElapsedTime);

			break;
		}
	}
#pragma endregion

	// Draw Functions
	#pragma region Draw Functions

	// Draws Level
	void DrawLevel(int iLevelSet, bool bClearScreen)
	{
		// Handle background
		if (bClearScreen)
		{
			Clear(olc::BLACK);
		}
		else
		{
			switch (iLevelSet)
			{
			case 0:
				// Draw Background 
				DrawSprite(olc::vi2d(0, 0), gfxBackground_Easy.Sprite());
				break;
			case 1:
				// Draw Background 
				DrawSprite(olc::vi2d(0, 0), gfxBackground_Medium.Sprite());
				break;
			case 2:
				// Draw Background
				DrawSprite(olc::vi2d(0, 0), gfxBackground_Hard.Sprite());
				break;
			default:
				break;
			}
		}

		// Check & Set LevelSet Variable
		SetLevelSet();

		// door drawing
		for (int i = 0; i < vDoors_pos.size(); i++)
		{
			switch (iLevelSet)
			{
			case 0:		// easy
				if (bDoorsOpen)
				{
					SetPixelMode(olc::Pixel::MASK);
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(0, 2) * vBlockSize, vBlockSize);
					SetPixelMode(olc::Pixel::NORMAL);
				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 1) * vBlockSize, vBlockSize);
				}
				break;
			case 1:		// medium
				if (bDoorsOpen)
				{
					SetPixelMode(olc::Pixel::MASK);
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(2, 2) * vBlockSize, vBlockSize);
					SetPixelMode(olc::Pixel::NORMAL);

				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);
				}
				break;
			case 2:		// hard
				if (bDoorsOpen)
				{
					SetPixelMode(olc::Pixel::MASK);
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 2) * vBlockSize, vBlockSize);
					SetPixelMode(olc::Pixel::NORMAL);
				}
				else
				{
					DrawPartialSprite(vDoors_pos[i] * vBlockSize, gfxTiles.Sprite(), olc::vi2d(3, 2) * vBlockSize, vBlockSize);
				}
				break;
			default:
				break;
			}
		}

		// win condition drawing
		SetPixelMode(olc::Pixel::MASK);
		for (auto& g : vGoals)
		{
			DrawPartialSprite(g * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 4) * vBlockSize, vBlockSize);
		}
		// door switch drawing
		for (auto& d : vSwitches)
		{
			DrawPartialSprite(d * vBlockSize, gfxTiles.Sprite(), olc::vi2d(4, 3) * vBlockSize, vBlockSize);
		}
		SetPixelMode(olc::Pixel::NORMAL);

		// teleport drawing
		if (bLevelHasTeleports)
		{
			SetPixelMode(olc::Pixel::MASK);
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
			SetPixelMode(olc::Pixel::NORMAL);
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

	// Draws UI for gameplay and win screen
	void DrawUI(float fElapsedTime)
	{
		if (iCurLevel != -1) // Active Gameplay
		{
			// Goal Tracking UI
			FillRect(3, 2, 92, 10, pColor_RectUI_Fill);
			DrawRect(3, 2, 92, 10, pColor_RectUI_Border);
			DrawString(5, 4, "Goals: ", pColor_GameUI);
			DrawString(55, 4, std::to_string(nGoals) + " / " + std::to_string(vGoals.size()), pColor_GameUI);

			// Level Tracking UI
			FillRect(166, 2, 67, 10, pColor_RectUI_Fill);
			DrawRect(166, 2, 67, 10, pColor_RectUI_Border);
			DrawString(167, 4, "Level: ", pColor_GameUI);
			if (iCurLevel < 10)
			{
				DrawString(217, 4, "0" + std::to_string(iCurLevel), pColor_GameUI);
			}
			else
			{
				DrawString(217, 4, std::to_string(iCurLevel), pColor_GameUI);
			}

			// Debug Mode Indicator
			if (bDebugMode) 
			{
				DrawString(17, 17, "DEBUG", olc::YELLOW);
			}

			// Moves & Time Score UI
			std::string sMoves = "";
			std::string sTime = "";
			CreateStrings_UI(sMoves, sTime);
			FillRect(3, 226, 92, 10, pColor_RectUI_Fill); // Draw Moves
			DrawRect(3, 226, 92, 10, pColor_RectUI_Border);
			DrawString(5, 228, "Moves: " + sMoves, pColor_GameUI);
			FillRect(144, 226, 100, 10, pColor_RectUI_Fill); // Draw Time
			DrawRect(144, 226, 100, 10, pColor_RectUI_Border);
			DrawString(146, 228, "Time: " + sTime, pColor_GameUI);


			// Special UI for Displaying Checkpoint Codes that can be inputted on Main Menu Level Select screen
			if (iCurLevel == 16)
			{
				FillRect(146, 17, 92, 10, pColor_RectUI_Fill); 
				DrawRect(146, 17, 92, 10, pColor_RectUI_Border);
				DrawString(148, 19, "Code: " + sMediumLevelCode, pColor_GameUI);
			}
			else if (iCurLevel == 36)
			{
				FillRect(146, 17, 92, 10, pColor_RectUI_Fill);
				DrawRect(146, 17, 92, 10, pColor_RectUI_Border);
				DrawString(148, 19, "Code: " + sHardLevelCode, pColor_GameUI);
			}
		}
		else // Win Screen
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
				ToggleMusic();

				// Draw Background & Blank Menu Level
				DrawSprite(olc::vi2d(0, 0), gfxBackground_YouWin.Sprite());
				DrawLevel(iLevelSet, false);

				DrawString((this->ScreenWidth() / 2) - 28, 24, "YOU WIN!", olc::MAGENTA);

				DrawString(20, 190, "ESC", olc::RED);
				DrawString(45, 190, "for", olc::WHITE);
				DrawString(73, 190, "Main Menu", olc::RED);

				DrawString(20, 202, "3bytes Studio 2025", olc::CYAN);
				DrawString(19, 214, "Thank you for playing!", olc::MAGENTA);
			}
		}
	}

	// Draws High Score UI
	void DrawUI_HighScores()
	{
		olc::Pixel pLevelColor = olc::MAGENTA;
		olc::Pixel pMovesAndTime_Easy = olc::GREEN;
		olc::Pixel pMovesAndTime_Medium = olc::YELLOW;
		olc::Pixel pMovesAndTime_Hard = olc::RED;
		olc::Pixel pNumbers = olc::CYAN;

		DrawString((this->ScreenWidth() / 2) - 42, 20, "HIGH SCORES", olc::YELLOW);

		DrawString(20, 194, "Arrow Keys", olc::CYAN);
		DrawString(106, 194, "to", olc::WHITE);
		DrawString(130, 194, "Select Page", olc::CYAN);

		DrawString(20, 204, "H", olc::YELLOW);
		DrawString(32, 204, "to", olc::WHITE);
		DrawString(54, 204, "Reset Scores", olc::YELLOW);

		DrawString(20, 214, "ESC", olc::RED);
		DrawString(50, 214, "to", olc::WHITE);
		DrawString(75, 214, "Close", olc::RED);

		DrawString(230, 215, std::to_string(iPageNum), pNumbers);

		switch (iPageNum)
		{
		case 1:
			DrawString(20, 33, "Level 1:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 43, std::to_string(vHighScore_Moves[0]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 43, FloatToString(vHighScore_Time[0], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 2:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 63, std::to_string(vHighScore_Moves[1]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 63, FloatToString(vHighScore_Time[1], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 3:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 83, std::to_string(vHighScore_Moves[2]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 83, FloatToString(vHighScore_Time[2], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 4:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 103, std::to_string(vHighScore_Moves[3]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 103, FloatToString(vHighScore_Time[3], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 5:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 123, std::to_string(vHighScore_Moves[4]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 123, FloatToString(vHighScore_Time[4], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 6:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 143, std::to_string(vHighScore_Moves[5]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 143, FloatToString(vHighScore_Time[5], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 7:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 163, std::to_string(vHighScore_Moves[6]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 163, FloatToString(vHighScore_Time[6], 2) + "s", olc::CYAN);
			break;
		case 2:
			DrawString(20, 33, "Level 8:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 43, std::to_string(vHighScore_Moves[7]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 43, FloatToString(vHighScore_Time[7], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 9:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 63, std::to_string(vHighScore_Moves[8]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 63, FloatToString(vHighScore_Time[8], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 10:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 83, std::to_string(vHighScore_Moves[9]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 83, FloatToString(vHighScore_Time[9], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 11:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 103, std::to_string(vHighScore_Moves[10]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 103, FloatToString(vHighScore_Time[10], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 12:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 123, std::to_string(vHighScore_Moves[11]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 123, FloatToString(vHighScore_Time[11], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 13:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 143, std::to_string(vHighScore_Moves[12]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 143, FloatToString(vHighScore_Time[12], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 14:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 163, std::to_string(vHighScore_Moves[13]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 163, FloatToString(vHighScore_Time[13], 2) + "s", olc::CYAN);
			break;
		case 3:
			DrawString(20, 33, "Level 15:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Easy);
			DrawString(70, 43, std::to_string(vHighScore_Moves[14]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Easy);
			DrawString(155, 43, FloatToString(vHighScore_Time[14], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 16:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 63, std::to_string(vHighScore_Moves[15]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 63, FloatToString(vHighScore_Time[15], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 17:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 83, std::to_string(vHighScore_Moves[16]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 83, FloatToString(vHighScore_Time[16], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 18:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 103, std::to_string(vHighScore_Moves[17]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 103, FloatToString(vHighScore_Time[17], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 19:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 123, std::to_string(vHighScore_Moves[18]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 123, FloatToString(vHighScore_Time[18], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 20:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 143, std::to_string(vHighScore_Moves[19]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 143, FloatToString(vHighScore_Time[19], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 21:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 163, std::to_string(vHighScore_Moves[20]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 163, FloatToString(vHighScore_Time[20], 2) + "s", olc::CYAN);
			break;
		case 4:
			DrawString(20, 33, "Level 22:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 43, std::to_string(vHighScore_Moves[21]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 43, FloatToString(vHighScore_Time[21], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 23:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 63, std::to_string(vHighScore_Moves[22]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 63, FloatToString(vHighScore_Time[22], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 24:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 83, std::to_string(vHighScore_Moves[23]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 83, FloatToString(vHighScore_Time[23], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 25:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 103, std::to_string(vHighScore_Moves[24]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 103, FloatToString(vHighScore_Time[24], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 26:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 123, std::to_string(vHighScore_Moves[25]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 123, FloatToString(vHighScore_Time[25], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 27:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 143, std::to_string(vHighScore_Moves[26]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 143, FloatToString(vHighScore_Time[26], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 28:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 163, std::to_string(vHighScore_Moves[27]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 163, FloatToString(vHighScore_Time[27], 2) + "s", olc::CYAN);
			break;
		case 5:
			DrawString(20, 33, "Level 29:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 43, std::to_string(vHighScore_Moves[28]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 43, FloatToString(vHighScore_Time[28], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 30:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 63, std::to_string(vHighScore_Moves[29]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 63, FloatToString(vHighScore_Time[29], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 31:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 83, std::to_string(vHighScore_Moves[30]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 83, FloatToString(vHighScore_Time[30], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 32:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 103, std::to_string(vHighScore_Moves[31]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 103, FloatToString(vHighScore_Time[31], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 33:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 123, std::to_string(vHighScore_Moves[32]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 123, FloatToString(vHighScore_Time[32], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 34:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 143, std::to_string(vHighScore_Moves[33]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 143, FloatToString(vHighScore_Time[33], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 35:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Medium);
			DrawString(70, 163, std::to_string(vHighScore_Moves[34]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Medium);
			DrawString(155, 163, FloatToString(vHighScore_Time[34], 2) + "s", olc::CYAN);
			break;
		case 6:
			DrawString(20, 33, "Level 36:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 43, std::to_string(vHighScore_Moves[35]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 43, FloatToString(vHighScore_Time[35], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 37:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 63, std::to_string(vHighScore_Moves[36]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 63, FloatToString(vHighScore_Time[36], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 38:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 83, std::to_string(vHighScore_Moves[37]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 83, FloatToString(vHighScore_Time[37], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 39:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 103, std::to_string(vHighScore_Moves[38]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 103, FloatToString(vHighScore_Time[38], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 40:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 123, std::to_string(vHighScore_Moves[39]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 123, FloatToString(vHighScore_Time[39], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 41:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 143, std::to_string(vHighScore_Moves[40]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 143, FloatToString(vHighScore_Time[40], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 42:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 163, std::to_string(vHighScore_Moves[41]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 163, FloatToString(vHighScore_Time[41], 2) + "s", olc::CYAN);			
			break;
		case 7:
			DrawString(20, 33, "Level 43:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 43, std::to_string(vHighScore_Moves[42]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 43, FloatToString(vHighScore_Time[42], 2) + "s", olc::CYAN);

			DrawString(20, 53, "Level 44:", pLevelColor);
			DrawString(20, 63, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 63, std::to_string(vHighScore_Moves[43]), olc::CYAN);
			DrawString(110, 63, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 63, FloatToString(vHighScore_Time[43], 2) + "s", olc::CYAN);

			DrawString(20, 73, "Level 45:", pLevelColor);
			DrawString(20, 83, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 83, std::to_string(vHighScore_Moves[44]), olc::CYAN);
			DrawString(110, 83, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 83, FloatToString(vHighScore_Time[44], 2) + "s", olc::CYAN);

			DrawString(20, 93, "Level 46:", pLevelColor);
			DrawString(20, 103, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 103, std::to_string(vHighScore_Moves[45]), olc::CYAN);
			DrawString(110, 103, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 103, FloatToString(vHighScore_Time[45], 2) + "s", olc::CYAN);

			DrawString(20, 113, "Level 47:", pLevelColor);
			DrawString(20, 123, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 123, std::to_string(vHighScore_Moves[46]), olc::CYAN);
			DrawString(110, 123, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 123, FloatToString(vHighScore_Time[46], 2) + "s", olc::CYAN);

			DrawString(20, 133, "Level 48:", pLevelColor);
			DrawString(20, 143, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 143, std::to_string(vHighScore_Moves[47]), olc::CYAN);
			DrawString(110, 143, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 143, FloatToString(vHighScore_Time[47], 2) + "s", olc::CYAN);

			DrawString(20, 153, "Level 49:", pLevelColor);
			DrawString(20, 163, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 163, std::to_string(vHighScore_Moves[48]), olc::CYAN);
			DrawString(110, 163, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 163, FloatToString(vHighScore_Time[48], 2) + "s", olc::CYAN);
			break;
		case 8:
			DrawString(20, 33, "Level 50:", pLevelColor);
			DrawString(20, 43, "Moves: ", pMovesAndTime_Hard);
			DrawString(70, 43, std::to_string(vHighScore_Moves[49]), olc::CYAN);
			DrawString(110, 43, "Time: ", pMovesAndTime_Hard);
			DrawString(155, 43, FloatToString(vHighScore_Time[49], 2) + "s", olc::CYAN);

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

			DrawString(20, 63, "Total:", olc::MAGENTA);
			DrawString(20, 73, "Moves: ", olc::WHITE);
			DrawString(70, 73, std::to_string(iTotal_HS), olc::CYAN);
			DrawString(20, 83, "Time: ", olc::WHITE);
			DrawString(65, 83, FloatToString(fTotal_HS, 2) + "s", olc::CYAN);
			break;
		default:
			break;
		}
	}

	// Draws Options UI
	void DrawUI_Options(float fElapsedTime)
	{
		DrawString(((this->ScreenWidth() / 2) - 23) + 45, 26 + 10, "OPTIONS", olc::WHITE);
		
		FillRect(((this->ScreenWidth() / 2) - 50) + 45, 35 + 10, 109, 32, olc::BLACK);
		DrawRect(((this->ScreenWidth() / 2) - 50) + 45, 35 + 10, 109, 32, olc::WHITE);
		DrawString(((this->ScreenWidth() / 2) - 18) + 45, 38 + 10, "Audio", olc::CYAN);
		DrawString(((this->ScreenWidth() / 2) - 33) + 45, 48 + 10, "Music: ", olc::GREEN);
		DrawString(((this->ScreenWidth() / 2) + 18) + 45, 48 + 10, FloatToString(fMusicVolume, 1), olc::WHITE);
		DrawString(((this->ScreenWidth() / 2) - 33) + 45, 58 + 10, "SFX: ", olc::GREEN);
		DrawString(((this->ScreenWidth() / 2) + 18) + 45, 58 + 10, FloatToString(fSFXVolume, 1), olc::WHITE);

		FillRect(18, 212, 97, 10, olc::BLACK);
		DrawRect(18, 212, 97, 10, olc::WHITE);
		DrawString(20, 214, "ESC", olc::RED);
		DrawString(50, 214, "to", olc::WHITE);
		DrawString(75, 214, "Close", olc::RED);

		// Draw cursor 
		DoCursorBlink(fElapsedTime); // update cursor blink variables

		if (!bCursorBlink) // only draw cursor on frames where cursor blink is toggled false
		{
			olc::vi2d vCursorPos_R = { 0,0 }; 
			olc::vi2d vCursorPos_L = { 0,0 }; 

			SetPixelMode(olc::Pixel::MASK);
			switch (iMenuSelectCursor_options)
			{
			case 0: // Music volume
				vCursorPos_R = { ((this->ScreenWidth() / 2) + 42) + 45, 43 + 10 };
				vCursorPos_L = { ((this->ScreenWidth() / 2) - 49) + 45, 43 + 10 };
				DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 3) * vBlockSize, vBlockSize); // R 
				DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize); // L 
				break;																			 
			case 1: // SFX Volume																 
				vCursorPos_R = { ((this->ScreenWidth() / 2) + 42) + 45, 53 + 10 };							 
				vCursorPos_L = { ((this->ScreenWidth() / 2) - 49) + 45, 53 + 10 };
				DrawPartialSprite(vCursorPos_R, gfxCursors.Sprite(), olc::vi2d(0, 3) * vBlockSize, vBlockSize); // R 
				DrawPartialSprite(vCursorPos_L, gfxCursors.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize); // L 
				break;
			default:
				break;
			}
			SetPixelMode(olc::Pixel::NORMAL);
		}
	}

	// Draws Level Select UI
	void DrawUI_LevelSelect()
	{
		DrawString(35, (240 / 2) - 96, "LEVEL SELECT", olc::MAGENTA);

		FillRect(100, 201, 139, 21, olc::BLACK);
		DrawRect(100, 201, 139, 21, olc::WHITE);
		DrawString(144, 204, "ESC", olc::RED);
		DrawString(174, 204, "to", olc::WHITE);
		DrawString(198, 204, "Close", olc::RED);
		DrawString(103, 213, "Enter", olc::MAGENTA); 
		DrawString(145, 213, "to", olc::WHITE); 
		DrawString(162, 213, "Input", olc::MAGENTA);
		DrawString(206, 213, "Code", olc::MAGENTA);

		// Text Entry UI
		if (IsTextEntryEnabled() == true)
		{
			DrawString(42, 35, "Input Mode", olc::WHITE);
		}
	}

	// Draws Credits UI
	void DrawUI_Credits()
	{
		DrawString(35, 120 - 96, "CREDITS", olc::BLUE);

			DrawString(20, 35, "Programming:", olc::YELLOW);
			DrawString(20, 45, "Tyler Clardy", olc::WHITE);

			DrawString(20, 60, "Art Design:", olc::GREEN);
			DrawString(20, 70, "Tyler Clardy", olc::WHITE);

			DrawString(20, 85, "Sound Design:", olc::RED);
			DrawString(20, 95, "Tyler Clardy", olc::WHITE);

			DrawString(20, 110, "Level Design:", olc::MAGENTA);
			DrawString(20, 120, "Tyler Clardy", olc::WHITE);
			DrawString(20, 130, "James Norman", olc::WHITE);
			DrawString(20, 140, "Aaron McBroom", olc::WHITE);
			
			DrawString(144, 204, "ESC", olc::RED);
			DrawString(174, 204, "to", olc::WHITE); 
			DrawString(199, 204, "Close", olc::RED);

			DrawString(95, 214, "3bytes Studio 2025", olc::CYAN);
	}

	// Draws Pause Menu
	void DrawPauseMenu()
	{
		// Clear screen to black at beginning of each frame
		Clear(olc::BLACK);

		// Draw Pause Menu background
		DrawSprite(olc::vi2d(0, 0), gfxBackground_Pause.Sprite());

		// Draw Menu Border
		// Corners
		DrawPartialSprite({ 0, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0) * vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		// Top
		DrawPartialSprite({ 16, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 32, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 48, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 64, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 80, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 96, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 112, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 128, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 144, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 160, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 176, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 192, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 208, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 224, 0 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);

		// Left
		DrawPartialSprite({ 0, 16 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 32 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 48 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 64 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 80 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 96 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 112 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 128 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 144 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 160 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 176 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 192 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 208 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 0, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);

		// Right
		DrawPartialSprite({ 240, 16 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 32 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 48 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 64 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 80 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 96 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 112 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 128 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 144 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 160 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 176 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 192 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 208 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 240, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		// Bottom
		DrawPartialSprite({ 16, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 32, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 48, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 64, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 80, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 96, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 112, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 128, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 144, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 160, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 176, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 192, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 208, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);
		DrawPartialSprite({ 224, 224 }, gfxTiles.Sprite(), olc::vi2d(0, 0)* vBlockSize, vBlockSize);

		// Draw Pause Window outline
		DrawRect(16, 16, this->ScreenWidth() - 33, this->ScreenHeight() - 33, olc::WHITE);

		// Get Timer and Movement Data
		std::string sMovementUI;
		std::string sTimerUI;
		switch (iCurLevel)
		{
		case 1:
			sMovementUI = std::to_string(iNumOfMoves_1);
			sTimerUI = FloatToString(iTime_1, 2);
			break;
		case 2:
			sMovementUI = std::to_string(iNumOfMoves_2);
			sTimerUI = FloatToString(iTime_2, 2);
			break;
		case 3:
			sMovementUI = std::to_string(iNumOfMoves_3);
			sTimerUI = FloatToString(iTime_3, 2);
			break;
		case 4:
			sMovementUI = std::to_string(iNumOfMoves_4);
			sTimerUI = FloatToString(iTime_4, 2);
			break;
		case 5:
			sMovementUI = std::to_string(iNumOfMoves_5);
			sTimerUI = FloatToString(iTime_5, 2);
			break;
		case 6:
			sMovementUI = std::to_string(iNumOfMoves_6);
			sTimerUI = FloatToString(iTime_6, 2);
			break;
		case 7:
			sMovementUI = std::to_string(iNumOfMoves_7);
			sTimerUI = FloatToString(iTime_7, 2);
			break;
		case 8:
			sMovementUI = std::to_string(iNumOfMoves_8);
			sTimerUI = FloatToString(iTime_8, 2);
			break;
		case 9:
			sMovementUI = std::to_string(iNumOfMoves_9);
			sTimerUI = FloatToString(iTime_9, 2);
			break;
		case 10:
			sMovementUI = std::to_string(iNumOfMoves_10);
			sTimerUI = FloatToString(iTime_10, 2);
			break;
		case 11:
			sMovementUI = std::to_string(iNumOfMoves_11);
			sTimerUI = FloatToString(iTime_11, 2);
			break;
		case 12:
			sMovementUI = std::to_string(iNumOfMoves_12);
			sTimerUI = FloatToString(iTime_12, 2);
			break;
		case 13:
			sMovementUI = std::to_string(iNumOfMoves_13);
			sTimerUI = FloatToString(iTime_13, 2);
			break;
		case 14:
			sMovementUI = std::to_string(iNumOfMoves_14);
			sTimerUI = FloatToString(iTime_14, 2);
			break;
		case 15:
			sMovementUI = std::to_string(iNumOfMoves_15);
			sTimerUI = FloatToString(iTime_15, 2);
			break;
		case 16:
			sMovementUI = std::to_string(iNumOfMoves_16);
			sTimerUI = FloatToString(iTime_16, 2);
			break;
		case 17:
			sMovementUI = std::to_string(iNumOfMoves_17);
			sTimerUI = FloatToString(iTime_17, 2);
			break;
		case 18:
			sMovementUI = std::to_string(iNumOfMoves_18);
			sTimerUI = FloatToString(iTime_18, 2);
			break;
		case 19:
			sMovementUI = std::to_string(iNumOfMoves_19);
			sTimerUI = FloatToString(iTime_19, 2);
			break;
		case 20:
			sMovementUI = std::to_string(iNumOfMoves_20);
			sTimerUI = FloatToString(iTime_20, 2);
			break;
		case 21:
			sMovementUI = std::to_string(iNumOfMoves_21);
			sTimerUI = FloatToString(iTime_21, 2);
			break;
		case 22:
			sMovementUI = std::to_string(iNumOfMoves_22);
			sTimerUI = FloatToString(iTime_22, 2);
			break;
		case 23:
			sMovementUI = std::to_string(iNumOfMoves_23);
			sTimerUI = FloatToString(iTime_23, 2);
			break;
		case 24:
			sMovementUI = std::to_string(iNumOfMoves_24);
			sTimerUI = FloatToString(iTime_24, 2);
			break;
		case 25:
			sMovementUI = std::to_string(iNumOfMoves_25);
			sTimerUI = FloatToString(iTime_25, 2);
			break;
		case 26:
			sMovementUI = std::to_string(iNumOfMoves_26);
			sTimerUI = FloatToString(iTime_26, 2);
			break;
		case 27:
			sMovementUI = std::to_string(iNumOfMoves_27);
			sTimerUI = FloatToString(iTime_27, 2);
			break;
		case 28:
			sMovementUI = std::to_string(iNumOfMoves_28);
			sTimerUI = FloatToString(iTime_28, 2);
			break;
		case 29:
			sMovementUI = std::to_string(iNumOfMoves_29);
			sTimerUI = FloatToString(iTime_29, 2);
			break;
		case 30:
			sMovementUI = std::to_string(iNumOfMoves_30);
			sTimerUI = FloatToString(iTime_30, 2);
			break;
		case 31:
			sMovementUI = std::to_string(iNumOfMoves_31);
			sTimerUI = FloatToString(iTime_31, 2);
			break;
		case 32:
			sMovementUI = std::to_string(iNumOfMoves_32);
			sTimerUI = FloatToString(iTime_32, 2);
			break;
		case 33:
			sMovementUI = std::to_string(iNumOfMoves_33);
			sTimerUI = FloatToString(iTime_33, 2);
			break;
		case 34:
			sMovementUI = std::to_string(iNumOfMoves_34);
			sTimerUI = FloatToString(iTime_34, 2);
			break;
		case 35:
			sMovementUI = std::to_string(iNumOfMoves_35);
			sTimerUI = FloatToString(iTime_35, 2);
			break;
		case 36:
			sMovementUI = std::to_string(iNumOfMoves_36);
			sTimerUI = FloatToString(iTime_36, 2);
			break;
		case 37:
			sMovementUI = std::to_string(iNumOfMoves_37);
			sTimerUI = FloatToString(iTime_37, 2);
			break;
		case 38:
			sMovementUI = std::to_string(iNumOfMoves_38);
			sTimerUI = FloatToString(iTime_38, 2);
			break;
		case 39:
			sMovementUI = std::to_string(iNumOfMoves_39);
			sTimerUI = FloatToString(iTime_39, 2);
			break;
		case 40:
			sMovementUI = std::to_string(iNumOfMoves_40);
			sTimerUI = FloatToString(iTime_40, 2);
			break;
		case 41:
			sMovementUI = std::to_string(iNumOfMoves_41);
			sTimerUI = FloatToString(iTime_41, 2);
			break;
		case 42:
			sMovementUI = std::to_string(iNumOfMoves_42);
			sTimerUI = FloatToString(iTime_42, 2);
			break;
		case 43:
			sMovementUI = std::to_string(iNumOfMoves_43);
			sTimerUI = FloatToString(iTime_43, 2);
			break;
		case 44:
			sMovementUI = std::to_string(iNumOfMoves_44);
			sTimerUI = FloatToString(iTime_44, 2);
			break;
		case 45:
			sMovementUI = std::to_string(iNumOfMoves_45);
			sTimerUI = FloatToString(iTime_45, 2);
			break;
		case 46:
			sMovementUI = std::to_string(iNumOfMoves_46);
			sTimerUI = FloatToString(iTime_46, 2);
			break;
		case 47:
			sMovementUI = std::to_string(iNumOfMoves_47);
			sTimerUI = FloatToString(iTime_47, 2);
			break;
		case 48:
			sMovementUI = std::to_string(iNumOfMoves_48);
			sTimerUI = FloatToString(iTime_48, 2);
			break;
		case 49:
			sMovementUI = std::to_string(iNumOfMoves_49);
			sTimerUI = FloatToString(iTime_49, 2);
			break;
		case 50:
			sMovementUI = std::to_string(iNumOfMoves_50);
			sTimerUI = FloatToString(iTime_50, 2);
			break;
		default:
			break;
		}

		// Set Font Color for Level UI
		if (iCurLevel < 16)
		{
			pColor_PauseUI = olc::GREEN;
		}
		else if (iCurLevel > 15 && iCurLevel < 36)
		{
			pColor_PauseUI = olc::YELLOW;
		}
		else
		{
			pColor_PauseUI = olc::RED;
		}

		// Draw UI Elements
		DrawString((this->ScreenWidth() / 2) - 40, (240 / 2) - 96, "GAME PAUSED", olc::WHITE);

		DrawString(20, 45, "Level " + std::to_string(iCurLevel), pColor_PauseUI);

		DrawString(20, 70, "Moves:", olc::MAGENTA);
		DrawString(70, 70, sMovementUI, olc::WHITE);

		DrawString(20, 57, "Timer:", olc::MAGENTA);
		DrawString(70, 57, sTimerUI + "s", olc::WHITE);

		DrawString(20, 204, "Enter", olc::CYAN);
		DrawString(64, 204, "to", olc::WHITE);
		DrawString(85, 204, "Resume", olc::CYAN);

		DrawString(20, 214, "ESC", olc::RED);
		DrawString(46, 214, "for", olc::WHITE);
		DrawString(74, 214, "Main Menu", olc::RED);

	}

	// Draws the game scene loop in the main menu
	void DoMainMenuGameScene(float fElapsedTime)
	{
		// Timer for scene game state
		fMenuSceneTimer += fElapsedTime;
		if (fMenuSceneTimer >= fMenuSceneSpeed && !bRestartScene)
		{
			fMenuSceneTimer = 0.0f;

			iMenuSceneState++;
			if (iMenuSceneState >= 32)
			{
				bRestartScene = true;
			}
		}

		// Do Restart Sequence
		if (bRestartScene)
		{
			// wait for timer to end, then 
			fMenuSceneRestartEffectTimer += fElapsedTime;
			if (fMenuSceneRestartEffectTimer >= fMenuSceneRestartEffectTimerSpeed)
			{
				// do freeze effect
				bSceneEffect = false;

				// wait for freeze timer before doing reset
				fSceneFreezeTimer += fElapsedTime;
				if (fSceneFreezeTimer >= fSceneFreezeSpeed)
				{
					// reset restart flag
					bRestartScene = false;

					// reset scene effect flag
					bSceneEffect = false;

					// reset timers
					fSceneFreezeTimer = 0.0f;
					fMenuSceneRestartEffectTimer = 0.0f;
					fSceneEffectTimer = 0.0f;

					// do restart
					iMenuSceneState = -3;
				}
			}
			else
			{
				// do effect
				fSceneEffectTimer += fElapsedTime;
				if (fSceneEffectTimer >= fSceneEffectSpeed)
				{
					// reset timer
					fSceneEffectTimer = 0.0f;

					// toggle flag
					if (bSceneEffect == false)
					{
						bSceneEffect = true;
					}
					else if (bSceneEffect == true)
					{
						bSceneEffect = false;
					}
				}
			}
		}

		// debug
		if (bDrawMainMenuSceneFrameNumber)
		{
			DrawString(17, 17, std::to_string(iMenuSceneState), olc::DARK_GREY);
		}
		/*if (GetKey(olc::Key::RIGHT).bPressed)
		{
			iMenuSceneState++;
			if (iMenuSceneState > 32)
			{
				iMenuSceneState = 32;
			}
		}
		if (GetKey(olc::Key::LEFT).bPressed)
		{
			iMenuSceneState--;
			if (iMenuSceneState < -1)
			{
				iMenuSceneState = -1;
			}
		}*/

		olc::vi2d vPos_Vertical = { 176, 176 };
		olc::vi2d vPos_Player = { 48, 160 };
		olc::vi2d vPos_Horizontal = { 112, 192 };
		olc::vi2d vPos_Omni = { 208, 176 };
		olc::vi2d vPos_Countdown = { 80, 160 };
		int iCountdownNum = 2;

		// update positions
		if (iMenuSceneState != -1)
		{
			if (iMenuSceneState >= 20) // vertical block 
			{
				vPos_Vertical.y = 160;
			}
			if (iMenuSceneState == 25) // horizontal block
			{
				vPos_Horizontal.x = 96;
			}
			else if (iMenuSceneState == 26)
			{
				vPos_Horizontal.x = 80;
			}
			else if (iMenuSceneState > 26)
			{
				vPos_Horizontal.x = 64;
			}
			if (iMenuSceneState >= 4) // omni block
			{
				vPos_Omni.y = 160;
			}
			if (iMenuSceneState >= 11) // Countdown block
			{
				vPos_Countdown.x = 96;
				iCountdownNum = 1;
			}
			switch (iMenuSceneState) // Player
			{
			case 0:
				vPos_Player = { 32, 160 };
				break;
			case 1:
				vPos_Player = { 32, 176 };
				break;
			case 2:
				vPos_Player = { 32, 192 }; // on top of blue portal
				break;
			case 3:
				vPos_Player = { 208, 192 }; // on top of orange portal
				break;
			case 4:
				vPos_Player = { 208, 176 };
				break;
			case 5:
				vPos_Player = { 208, 192 }; // on top of orange portal
				break;
			case 6:
				vPos_Player = { 32, 192 }; // on top of blue portal
				break;
			case 7:
				vPos_Player = { 32, 176 };
				break;
			case 8:
				vPos_Player = { 32, 160 };
				break;
			case 9:
				vPos_Player = { 48, 160 };
				break;
			case 10:
				vPos_Player = { 64, 160 }; // on top of door
				break;
			case 11:
				vPos_Player = { 80, 160 }; // pushing countdown
				break;
			case 12:
				vPos_Player = { 80, 176 };
				break;
			case 13:
				vPos_Player = { 96, 176 };
				break;
			case 14:
				vPos_Player = { 112, 176 };
				break;
			case 15:
				vPos_Player = { 128, 176 };
				break;
			case 16:
				vPos_Player = { 144, 176 };
				break;
			case 17:
				vPos_Player = { 144, 192 };
				break;
			case 18:
				vPos_Player = { 160, 192 };
				break;
			case 19:
				vPos_Player = { 176, 192 };
				break;
			case 20:
				vPos_Player = { 176, 176 }; // pushing vertical block
				break;
			case 21:
				vPos_Player = { 176, 192 }; //
				break;
			case 22:
				vPos_Player = { 160, 192 };
				break;
			case 23:
				vPos_Player = { 144, 192 };
				break;
			case 24:
				vPos_Player = { 128, 192 };
				break;
			case 25:
				vPos_Player = { 112, 192 };
				break;
			case 26:
				vPos_Player = { 96, 192 };
				break;
			case 27:
				vPos_Player = { 80, 192 };
				break;
			case 28:
				vPos_Player = { 80, 176 };
				break;
			case 29:
				vPos_Player = { 80, 160 };
				break;
			case 30:
				vPos_Player = { 64, 160 };
				break;
			case 31:
				vPos_Player = { 48, 160 };
				break;
			case 32:
				vPos_Player = { 32, 160 };
				break;
			default:
				break;
			}
		}

		// DRAW SCENE:
		if (!bSceneEffect)
		{
			// WALLS:
		// Left Wall
			DrawPartialSprite({ 16, 192 }, gfxMenuScene.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 16, 176 }, gfxMenuScene.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 16, 160 }, gfxMenuScene.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);

			// Right Wall
			DrawPartialSprite({ 224, 192 }, gfxMenuScene.Sprite(), olc::vi2d(1, 1) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 224, 176 }, gfxMenuScene.Sprite(), olc::vi2d(1, 1) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 224, 160 }, gfxMenuScene.Sprite(), olc::vi2d(1, 1) * vBlockSize, vBlockSize);

			// Top
			DrawPartialSprite({ 32, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 48, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 64, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 80, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 96, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 112, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 128, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 144, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 160, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 176, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 192, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 208, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);

			// Bottom
			DrawPartialSprite({ 32, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 48, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 64, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 80, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 96, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 112, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 128, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 144, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 160, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 176, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 192, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 208, 208 }, gfxMenuScene.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);

			// Corners
			DrawPartialSprite({ 224, 208 }, gfxMenuScene.Sprite(), olc::vi2d(3, 1) * vBlockSize, vBlockSize); // bottom right
			DrawPartialSprite({ 16, 144 }, gfxMenuScene.Sprite(), olc::vi2d(1, 4) * vBlockSize, vBlockSize); // top left
			DrawPartialSprite({ 16, 208 }, gfxMenuScene.Sprite(), olc::vi2d(4, 1) * vBlockSize, vBlockSize); // bottom left
			DrawPartialSprite({ 224, 144 }, gfxMenuScene.Sprite(), olc::vi2d(3, 3) * vBlockSize, vBlockSize); // top right

			// Interior
			DrawPartialSprite({ 192, 160 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize); // interior wall 1 (next to orange portal)
			DrawPartialSprite({ 192, 176 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 192, 192 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize);

			DrawPartialSprite({ 48, 176 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize); // Interior wall 2 (next to blue portal)
			DrawPartialSprite({ 48, 192 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize);
			DrawPartialSprite({ 64, 176 }, gfxTiles.Sprite(), olc::vi2d(2, 1) * vBlockSize, vBlockSize);

			// SCENE Static:
			// Portals
			DoTeleportFlipCheck(fElapsedTime);
			SetPixelMode(olc::Pixel::MASK);
			switch (iTele_Facing)
			{
			case 0:
				DrawPartialSprite({ 32, 192 }, gfxTiles.Sprite(), olc::vi2d(3, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite({ 208, 192 }, gfxTiles.Sprite(), olc::vi2d(3, 4) * vBlockSize, vBlockSize);
				break;
			case 1:
				DrawPartialSprite({ 32, 192 }, gfxTiles.Sprite(), olc::vi2d(2, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite({ 208, 192 }, gfxTiles.Sprite(), olc::vi2d(2, 4) * vBlockSize, vBlockSize);
				break;
			case 2:
				DrawPartialSprite({ 32, 192 }, gfxTiles.Sprite(), olc::vi2d(1, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite({ 208, 192 }, gfxTiles.Sprite(), olc::vi2d(1, 4) * vBlockSize, vBlockSize);
				break;
			case 3:
				DrawPartialSprite({ 32, 192 }, gfxTiles.Sprite(), olc::vi2d(0, 3) * vBlockSize, vBlockSize);
				DrawPartialSprite({ 208, 192 }, gfxTiles.Sprite(), olc::vi2d(0, 4) * vBlockSize, vBlockSize);
				break;
			}
			// Door Switch
			olc::vi2d vDoorSwitch_MenuScene = { 208, 160 };
			DrawPartialSprite(vDoorSwitch_MenuScene, gfxTiles.Sprite(), olc::vi2d(4, 3) * vBlockSize, vBlockSize);
			// Win Tiles
			olc::vi2d vWinTile_MenuScene_1 = { 64, 192 };
			olc::vi2d vWinTile_MenuScene_2 = { 176, 160 };
			olc::vi2d vWinTile_MenuScene_3 = { 32, 160 };
			DrawPartialSprite(vWinTile_MenuScene_1, gfxTiles.Sprite(), olc::vi2d(4, 4)* vBlockSize, vBlockSize);
			DrawPartialSprite(vWinTile_MenuScene_2, gfxTiles.Sprite(), olc::vi2d(4, 4)* vBlockSize, vBlockSize);
			DrawPartialSprite(vWinTile_MenuScene_3, gfxTiles.Sprite(), olc::vi2d(4, 4)* vBlockSize, vBlockSize);
			SetPixelMode(olc::Pixel::NORMAL);

			// SCENE NonStatic
			// Door
			if (iMenuSceneState >= 4) // open
			{
				SetPixelMode(olc::Pixel::MASK);
				DrawPartialSprite({ 64, 160 }, gfxTiles.Sprite(), olc::vi2d(2, 2) * vBlockSize, vBlockSize);
				SetPixelMode(olc::Pixel::NORMAL);
			}
			else // closed
			{
				DrawPartialSprite({ 64, 160 }, gfxTiles.Sprite(), olc::vi2d(1, 2) * vBlockSize, vBlockSize);
			}
			// "PLAYER"
			DrawPartialSprite(vPos_Player, gfxTiles.Sprite(), olc::vi2d(0, 1) * vBlockSize, vBlockSize);
			// Blocks
			DrawPartialSprite(vPos_Vertical, gfxTiles.Sprite(), olc::vi2d(3, 0) * vBlockSize, vBlockSize); // vertical block
			DrawPartialSprite(vPos_Horizontal, gfxTiles.Sprite(), olc::vi2d(2, 0) * vBlockSize, vBlockSize); // Horizontal Block
			DrawPartialSprite(vPos_Omni, gfxTiles.Sprite(), olc::vi2d(1, 0) * vBlockSize, vBlockSize); // Omnidirectional Block
			DrawPartialSprite(vPos_Countdown, gfxTiles.Sprite(), olc::vi2d(4, 0) * vBlockSize, vBlockSize); // countdown Block tile
			DrawString(vPos_Countdown + olc::vi2d(4, 4), std::to_string(iCountdownNum), olc::BLACK); // countdown block integer
		}
	}

	// Draws Game Title during Main Menu - includes visual effect logic
	void DoGameTitle(float fElapsedTime)
	{
		olc::Pixel p_0;
		olc::Pixel p_1;
		olc::Pixel p_2;
		olc::Pixel p_3;
		olc::Pixel p_4;
		olc::Pixel p_5;

		// Check timer
		fTitleSwitchTimer += fElapsedTime;
		if (fTitleSwitchTimer >= fTitleSwitchSpeed)
		{
			fTitleSwitchTimer = 0.0f;

			iTitleSwitch++;

			if (iTitleSwitch > 5)
			{
				iTitleSwitch = 0;
			}
		}

		// Color Picking
		switch (iTitleSwitch)
		{
		case 0:
			p_0 = olc::CYAN;
			p_1 = olc::YELLOW;
			p_2 = olc::MAGENTA;
			p_3 = olc::BLUE;
			p_4 = olc::GREEN;
			p_5 = olc::RED;
			break;
		case 1:
			p_0 = olc::RED;
			p_1 = olc::CYAN;
			p_2 = olc::YELLOW;
			p_3 = olc::MAGENTA;
			p_4 = olc::BLUE;
			p_5 = olc::GREEN;
			break;
		case 2:
			p_0 = olc::GREEN;
			p_1 = olc::RED;
			p_2 = olc::CYAN;
			p_3 = olc::YELLOW;
			p_4 = olc::MAGENTA;
			p_5 = olc::BLUE;
			break;
		case 3:
			p_0 = olc::BLUE;
			p_1 = olc::GREEN;
			p_2 = olc::RED;
			p_3 = olc::CYAN;
			p_4 = olc::YELLOW;
			p_5 = olc::MAGENTA;
			break;
		case 4:
			p_0 = olc::MAGENTA;
			p_1 = olc::BLUE;
			p_2 = olc::GREEN;
			p_3 = olc::RED;
			p_4 = olc::CYAN;
			p_5 = olc::YELLOW;
			break;
		case 5:
			p_0 = olc::YELLOW;
			p_1 = olc::MAGENTA;
			p_2 = olc::BLUE;
			p_3 = olc::GREEN;
			p_4 = olc::RED;
			p_5 = olc::CYAN;
			break;
		default:
			break;
		}

		// Draw Title
		DrawString(((this->ScreenWidth() / 2) - 77) + 5, 34, "I", p_0);
		DrawString(((this->ScreenWidth() / 2) - 63) + 5, 34, "N", p_1);
		DrawString(((this->ScreenWidth() / 2) - 48) + 5, 34, "C", p_2);
		DrawString(((this->ScreenWidth() / 2) - 33) + 5, 34, "R", p_3);
		DrawString(((this->ScreenWidth() / 2) - 18) + 5, 34, "E", p_4);
		DrawString(((this->ScreenWidth() / 2) - 3) + 5, 34, "M", p_5);
		DrawString(((this->ScreenWidth() / 2) + 12) + 5, 34, "E", p_0);
		DrawString(((this->ScreenWidth() / 2) + 27) + 5, 34, "N", p_1);
		DrawString(((this->ScreenWidth() / 2) + 42) + 5, 34, "T", p_2);
		DrawString(((this->ScreenWidth() / 2) + 57) + 5, 34, "0", p_3);
	}

	// Rotates Teleport Tile GFX
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

	// Blink Effect for Menu Cursor
	void DoCursorBlink(float fElapsedTime)
	{
		// Check flag
		if (bDoCursorBlink)
		{
			fCursorBlinkTimer += fElapsedTime;

			if (fCursorBlinkTimer >= fCursorBlinkSpeed) // Enough time has passed - toggle flag for cursor blink
			{
				fCursorBlinkTimer = 0.0f; // reset timer

				// toggle flag
				if (bCursorBlink == true)
				{
					bCursorBlink = false;
				}
				else
				{
					bCursorBlink = true;
				}
			}
		}
	}
#pragma endregion

	// Saving, Loading, and FileIO Functions
	#pragma region Saving, Loading, and FileIO Functions

	// FileIO - Load Saved Options
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

	// FileIO - Save Options Menu
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

	// FileIO - Load High Scores
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

	// FileIO - Save High Scores
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

	// Load All Audio into Memory
	void LoadAllAudio()
	{
		// Music Loading
		audio_backgroundMusic_1 = audioEngine_Music.LoadSound(sBackgroundMusic_1);
		audio_backgroundMusic_2 = audioEngine_Music.LoadSound(sBackgroundMusic_2);
		audio_backgroundMusic_3 = audioEngine_Music.LoadSound(sBackgroundMusic_3);
		audio_backgroundMusic_Menu = audioEngine_Music.LoadSound(sBackgroundMusic_Menu);

		// SFX Loading
		audio_Movement_1 = audioEngine.LoadSound(sMovement_1);
		audio_Movement_Fail = audioEngine.LoadSound(sMovementFailure_1);
		audio_RestartLevel = audioEngine.LoadSound(sRestartLevel_1);
		audio_WinJingle = audioEngine.LoadSound(sWinJingle_1);
		audio_LevelTransition = audioEngine.LoadSound(sLevelTransition_1);
		audio_PauseJingle = audioEngine.LoadSound(sPauseJingle_1);
		audio_UnPauseJingle = audioEngine.LoadSound(sUnPauseJingle_1);
		audio_LevelCode = audioEngine.LoadSound(sLevelCode_1);
		audio_LevelCode_Fail = audioEngine.LoadSound(sLevelCodeFail_1);
		audio_GameStartUp = audioEngine.LoadSound(sGameStartUp);
		audio_Door_Open = audioEngine.LoadSound(sDoorOpen_1);
		audio_Door_Close = audioEngine.LoadSound(sDoorClose_1);
		audio_Teleport = audioEngine.LoadSound(sTeleport_1);
		audio_Teleport_Fail = audioEngine.LoadSound(sTeleportFailure_1);
		audio_ButtonClick_1 = audioEngine.LoadSound(sButtonClick_1);
		audio_ButtonClick_2 = audioEngine.LoadSound(sButtonClick_2);

		// Set Volume					
		SetVolume();
	}

	// Load All Levels into Memory
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

	// Load an Individual Level from Memory
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
		StopAllAudio();

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
				audioEngine.Play(audio_WinJingle);
			}
			else if (bWasRestart == true)
			{
				audioEngine.Play(audio_RestartLevel);
			}
			else
			{
				audioEngine.Play(audio_LevelTransition);
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
			audioEngine_Music.Play(audio_backgroundMusic_1, true);
		}
		else if (iCurLevel >= 16 && iCurLevel <= 35 && !bMainMenu)
		{
			audioEngine_Music.Play(audio_backgroundMusic_2, true);
		}
		else if (iCurLevel >= 36 && iCurLevel <= 50 && !bMainMenu)
		{
			audioEngine_Music.Play(audio_backgroundMusic_3, true);
		}
		else if (bMainMenu)
		{
			audioEngine_Music.Play(audio_backgroundMusic_Menu, true);
		}
	}

	// Saves Moves High score for Current Level if it is better than previous High Score
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

	// Saves Time High score for Current Level if it is better than previous High Score
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
#pragma endregion

	// Reset Functions
	#pragma region Reset Functions

	// Resets All Main Menu Variables
	void ResetMainMenu()
	{
		iMenuSelectCursor_main = 0;
		bCursorBlink = false;
		iTitleSwitch = 0;
		iMenuSceneState = -3;
		fMenuSceneTimer = 0.0f;
		fMenuSceneRestartEffectTimer = 0.0f;
		fSceneEffectTimer = 0.0f;
		fSceneFreezeTimer = 0.0f;
		bRestartScene = false;
		bSceneEffect = false;
	}

	// Resets Time Score and Moves Score for a Single Level
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

	// Resets Time Score and Moves Score for a ALL Levels
	void DoLevelScoreReset()
	{
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
	}
#pragma endregion

	// Utility Functions
	#pragma region Utility Functions

	// saves a reference to the ID of the currently playing background music track to the audio_CurrentBackgroundTrack variable. Returns 1 for success -1 for failure
	int CurrentlyPlaying()
	{
		if (audioEngine_Music.IsPlaying(audio_backgroundMusic_1))
		{
			audio_CurrentBackgroundTrack = &audio_backgroundMusic_1;
		}
		else if (audioEngine_Music.IsPlaying(audio_backgroundMusic_2))
		{
			audio_CurrentBackgroundTrack = &audio_backgroundMusic_2;
		}
		else if (audioEngine_Music.IsPlaying(audio_backgroundMusic_3))
		{
			audio_CurrentBackgroundTrack = &audio_backgroundMusic_3;
		}
		else if(audioEngine_Music.IsPlaying(audio_backgroundMusic_Menu))
		{
			audio_CurrentBackgroundTrack = &audio_backgroundMusic_Menu;
		}
		else
		{
			audio_CurrentBackgroundTrack = nullptr;
		}

		if (audio_CurrentBackgroundTrack != nullptr)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	// Pause or Unpause Background Music
	void ToggleMusic()
	{
		int iGetBackgroundTrack = CurrentlyPlaying();

		// Succeeded in getting current background track
		if (iGetBackgroundTrack == 1)
		{
			if (audio_CurrentBackgroundTrack == &audio_backgroundMusic_1)
			{
				audioEngine_Music.Toggle(audio_backgroundMusic_1);
			}
			else if (audio_CurrentBackgroundTrack == &audio_backgroundMusic_2)
			{
				audioEngine_Music.Toggle(audio_backgroundMusic_2);
			}
			else if (audio_CurrentBackgroundTrack == &audio_backgroundMusic_3)
			{
				audioEngine_Music.Toggle(audio_backgroundMusic_3);
			}
			else if (audio_CurrentBackgroundTrack == &audio_backgroundMusic_Menu)
			{
				audioEngine_Music.Toggle(audio_backgroundMusic_Menu);
			}
		}
	}

	// Sets Volume for all sounds & music - based on global fMusicVolume and fSFXVolume variables
	void SetVolume()
	{
		// Music
		audioEngine_Music.SetVolume(audio_backgroundMusic_1, fMusicVolume);
		audioEngine_Music.SetVolume(audio_backgroundMusic_2, fMusicVolume);
		audioEngine_Music.SetVolume(audio_backgroundMusic_3, fMusicVolume);
		audioEngine_Music.SetVolume(audio_backgroundMusic_Menu, fMusicVolume);

		// SFX
		audioEngine.SetVolume(audio_Movement_1, fSFXVolume);
		audioEngine.SetVolume(audio_Movement_Fail, fSFXVolume);
		audioEngine.SetVolume(audio_RestartLevel, fSFXVolume);
		audioEngine.SetVolume(audio_WinJingle, fSFXVolume);
		audioEngine.SetVolume(audio_LevelTransition, fSFXVolume);
		audioEngine.SetVolume(audio_PauseJingle, fSFXVolume);
		audioEngine.SetVolume(audio_UnPauseJingle, fSFXVolume);
		audioEngine.SetVolume(audio_LevelCode, fSFXVolume);
		audioEngine.SetVolume(audio_LevelCode_Fail, fSFXVolume);
		audioEngine.SetVolume(audio_GameStartUp, fSFXVolume);
		audioEngine.SetVolume(audio_Door_Open, fSFXVolume);
		audioEngine.SetVolume(audio_Door_Close, fSFXVolume);
		audioEngine.SetVolume(audio_Teleport, fSFXVolume);
		audioEngine.SetVolume(audio_Teleport_Fail, fSFXVolume);
		audioEngine.SetVolume(audio_ButtonClick_1, fSFXVolume);
		audioEngine.SetVolume(audio_ButtonClick_2, fSFXVolume);
	}

	// Stops all currently playing audio
	void StopAllAudio()
	{
		// Music
		audioEngine_Music.Stop(audio_backgroundMusic_1);
		audioEngine_Music.Stop(audio_backgroundMusic_2);
		audioEngine_Music.Stop(audio_backgroundMusic_3);
		audioEngine_Music.Stop(audio_backgroundMusic_Menu);

		// SFX
		audioEngine.Stop(audio_Movement_1);
		audioEngine.Stop(audio_Movement_Fail);
		audioEngine.Stop(audio_RestartLevel);
		audioEngine.Stop(audio_WinJingle);
		audioEngine.Stop(audio_LevelTransition);
		audioEngine.Stop(audio_PauseJingle);
		audioEngine.Stop(audio_UnPauseJingle);
		audioEngine.Stop(audio_LevelCode);
		audioEngine.Stop(audio_LevelCode_Fail);
		audioEngine.Stop(audio_GameStartUp);
		audioEngine.Stop(audio_Door_Open);
		audioEngine.Stop(audio_Door_Close);
		audioEngine.Stop(audio_Teleport);
		audioEngine.Stop(audio_Teleport_Fail);
		audioEngine.Stop(audio_ButtonClick_1);
		audioEngine.Stop(audio_ButtonClick_2);
	}

	// Sets LevelSet variable based on current level
	void SetLevelSet()
	{
		// Level Tracking UI
		if (iCurLevel < 16)
		{
			iLevelSet = 0;
		}
		else if (iCurLevel > 15 && iCurLevel < 36)
		{
			iLevelSet = 1;
		}
		else
		{
			iLevelSet = 2;
		}
	}

	// Creates Strings used in UI Drawing
	void CreateStrings_UI(std::string& sMoves, std::string& sTime)
	{
		// Moves
		switch (iCurLevel)
		{
		case 1:
			sMoves = std::to_string(iNumOfMoves_1);
			break;
		case 2:
			sMoves = std::to_string(iNumOfMoves_2);
			break;
		case 3:
			sMoves = std::to_string(iNumOfMoves_3);
			break;
		case 4:
			sMoves = std::to_string(iNumOfMoves_4);
			break;
		case 5:
			sMoves = std::to_string(iNumOfMoves_5);
			break;
		case 6:
			sMoves = std::to_string(iNumOfMoves_6);
			break;
		case 7:
			sMoves = std::to_string(iNumOfMoves_7);
			break;
		case 8:
			sMoves = std::to_string(iNumOfMoves_8);
			break;
		case 9:
			sMoves = std::to_string(iNumOfMoves_9);
			break;
		case 10:
			sMoves = std::to_string(iNumOfMoves_10);
			break;
		case 11:
			sMoves = std::to_string(iNumOfMoves_11);
			break;
		case 12:
			sMoves = std::to_string(iNumOfMoves_12);
			break;
		case 13:
			sMoves = std::to_string(iNumOfMoves_13);
			break;
		case 14:
			sMoves = std::to_string(iNumOfMoves_14);
			break;
		case 15:
			sMoves = std::to_string(iNumOfMoves_15);
			break;
		case 16:
			sMoves = std::to_string(iNumOfMoves_16);
			break;
		case 17:
			sMoves = std::to_string(iNumOfMoves_17);
			break;
		case 18:
			sMoves = std::to_string(iNumOfMoves_18);
			break;
		case 19:
			sMoves = std::to_string(iNumOfMoves_19);
			break;
		case 20:
			sMoves = std::to_string(iNumOfMoves_20);
			break;
		case 21:
			sMoves = std::to_string(iNumOfMoves_21);
			break;
		case 22:
			sMoves = std::to_string(iNumOfMoves_22);
			break;
		case 23:
			sMoves = std::to_string(iNumOfMoves_23);
			break;
		case 24:
			sMoves = std::to_string(iNumOfMoves_24);
			break;
		case 25:
			sMoves = std::to_string(iNumOfMoves_25);
			break;
		case 26:
			sMoves = std::to_string(iNumOfMoves_26);
			break;
		case 27:
			sMoves = std::to_string(iNumOfMoves_27);
			break;
		case 28:
			sMoves = std::to_string(iNumOfMoves_28);
			break;
		case 29:
			sMoves = std::to_string(iNumOfMoves_29);
			break;
		case 30:
			sMoves = std::to_string(iNumOfMoves_30);
			break;
		case 31:
			sMoves = std::to_string(iNumOfMoves_31);
			break;
		case 32:
			sMoves = std::to_string(iNumOfMoves_32);
			break;
		case 33:
			sMoves = std::to_string(iNumOfMoves_33);
			break;
		case 34:
			sMoves = std::to_string(iNumOfMoves_34);
			break;
		case 35:
			sMoves = std::to_string(iNumOfMoves_35);
			break;
		case 36:
			sMoves = std::to_string(iNumOfMoves_36);
			break;
		case 37:
			sMoves = std::to_string(iNumOfMoves_37);
			break;
		case 38:
			sMoves = std::to_string(iNumOfMoves_38);
			break;
		case 39:
			sMoves = std::to_string(iNumOfMoves_39);
			break;
		case 40:
			sMoves = std::to_string(iNumOfMoves_40);
			break;
		case 41:
			sMoves = std::to_string(iNumOfMoves_41);
			break;
		case 42:
			sMoves = std::to_string(iNumOfMoves_42);
			break;
		case 43:
			sMoves = std::to_string(iNumOfMoves_43);
			break;
		case 44:
			sMoves = std::to_string(iNumOfMoves_44);
			break;
		case 45:
			sMoves = std::to_string(iNumOfMoves_45);
			break;
		case 46:
			sMoves = std::to_string(iNumOfMoves_46);
			break;
		case 47:
			sMoves = std::to_string(iNumOfMoves_47);
			break;
		case 48:
			sMoves = std::to_string(iNumOfMoves_48);
			break;
		case 49:
			sMoves = std::to_string(iNumOfMoves_49);
			break;
		case 50:
			sMoves = std::to_string(iNumOfMoves_50);
			break;
		default:
			break;
		}

		// Time
		switch (iCurLevel)
		{
		case 1:
			sTime = FloatToString(iTime_1, 2);
			break;
		case 2:
			sTime = FloatToString(iTime_2, 2);
			break;
		case 3:
			sTime = FloatToString(iTime_3, 2);
			break;
		case 4:
			sTime = FloatToString(iTime_4, 2);
			break;
		case 5:
			sTime = FloatToString(iTime_5, 2);
			break;
		case 6:
			sTime = FloatToString(iTime_6, 2);
			break;
		case 7:
			sTime = FloatToString(iTime_7, 2);
			break;
		case 8:
			sTime = FloatToString(iTime_8, 2);
			break;
		case 9:
			sTime = FloatToString(iTime_9, 2);
			break;
		case 10:
			sTime = FloatToString(iTime_10, 2);
			break;
		case 11:
			sTime = FloatToString(iTime_11, 2);
			break;
		case 12:
			sTime = FloatToString(iTime_12, 2);
			break;
		case 13:
			sTime = FloatToString(iTime_13, 2);
			break;
		case 14:
			sTime = FloatToString(iTime_14, 2);
			break;
		case 15:
			sTime = FloatToString(iTime_15, 2);
			break;
		case 16:
			sTime = FloatToString(iTime_16, 2);
			break;
		case 17:
			sTime = FloatToString(iTime_17, 2);
			break;
		case 18:
			sTime = FloatToString(iTime_18, 2);
			break;
		case 19:
			sTime = FloatToString(iTime_19, 2);
			break;
		case 20:
			sTime = FloatToString(iTime_20, 2);
			break;
		case 21:
			sTime = FloatToString(iTime_21, 2);
			break;
		case 22:
			sTime = FloatToString(iTime_22, 2);
			break;
		case 23:
			sTime = FloatToString(iTime_23, 2);
			break;
		case 24:
			sTime = FloatToString(iTime_24, 2);
			break;
		case 25:
			sTime = FloatToString(iTime_25, 2);
			break;
		case 26:
			sTime = FloatToString(iTime_26, 2);
			break;
		case 27:
			sTime = FloatToString(iTime_27, 2);
			break;
		case 28:
			sTime = FloatToString(iTime_28, 2);
			break;
		case 29:
			sTime = FloatToString(iTime_29, 2);
			break;
		case 30:
			sTime = FloatToString(iTime_30, 2);
			break;
		case 31:
			sTime = FloatToString(iTime_31, 2);
			break;
		case 32:
			sTime = FloatToString(iTime_32, 2);
			break;
		case 33:
			sTime = FloatToString(iTime_33, 2);
			break;
		case 34:
			sTime = FloatToString(iTime_34, 2);
			break;
		case 35:
			sTime = FloatToString(iTime_35, 2);
			break;
		case 36:
			sTime = FloatToString(iTime_36, 2);
			break;
		case 37:
			sTime = FloatToString(iTime_37, 2);
			break;
		case 38:
			sTime = FloatToString(iTime_38, 2);
			break;
		case 39:
			sTime = FloatToString(iTime_39, 2);
			break;
		case 40:
			sTime = FloatToString(iTime_40, 2);
			break;
		case 41:
			sTime = FloatToString(iTime_41, 2);
			break;
		case 42:
			sTime = FloatToString(iTime_42, 2);
			break;
		case 43:
			sTime = FloatToString(iTime_43, 2);
			break;
		case 44:
			sTime = FloatToString(iTime_44, 2);
			break;
		case 45:
			sTime = FloatToString(iTime_45, 2);
			break;
		case 46:
			sTime = FloatToString(iTime_46, 2);
			break;
		case 47:
			sTime = FloatToString(iTime_47, 2);
			break;
		case 48:
			sTime = FloatToString(iTime_48, 2);
			break;
		case 49:
			sTime = FloatToString(iTime_49, 2);
			break;
		case 50:
			sTime = FloatToString(iTime_50, 2);
			break;
		default:
			break;
		}
	}

	// Converts float to string, truncated to a set number of decimal places
	std::string FloatToString(float fIn, int iPrecision)
	{
		std::stringstream ss;

		ss << std::fixed << std::setprecision(iPrecision) << fIn;

		return ss.str();
	}

	// Tracks & Records Time per Frame for Level Time Score
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

	// Tracks & Records Moves per Frame for Level Moves Score
	void DoLevelMoveCounting(bool bPlayerMoved, int iMovementSuceededOrFailed)
	{
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
				iNumOfMoves_9++;
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
	}

	// Overridden Text Entry function for use in Level Select Menu
	void OnTextEntryComplete(const std::string& sText) override
	{
		sInputCode = sText;
	}
#pragma endregion
};

// Main
int main()
{
	Game Incremento;
	if (Incremento.Construct(256, 240, 4, 4))
		Incremento.Start();
	return 0;
}

// Created by Tyler Clardy in Summer 2025
// Thanks to oneLoneCoder and his community for the PixelGameEngine, as well as the inspiration for the start of the project. You rock, Javid!