#include <AEEngine.h>
#include "Engine\Rasterizer\Rasterizer.h"
#include "Levels\GameStates.h"
#include "Levels\Common.h"

int main()
{
	// Initialize the alpha engine
	if(AESysInit("Assignment 2 - Triangle Scan Conversion") == false)
	{
		return 0;
	}

	// initialize graphics system
	if (!Rasterizer::FrameBuffer::Allocate(AEGetSysWindowWidth(), AEGetSysWindowHeight()))
		return 0;

	// Add a game state to the game state manager
	AEGameStateMgrAdd(
		GS_SIMPLE_LINES,
		0,
		0,
		SimpleLines::Update,
		SimpleLines::Render,
		0,
		0);
	AEGameStateMgrAdd(
		GS_SIMPLE_TRIANGLES,
		0,
		0,
		SimpleTriangles::Update,
		SimpleTriangles::Render,
		0,
		0);
	AEGameStateMgrAdd(
		GS_FREE_DRAW,
		0,
		FreeDraw::Init,
		FreeDraw::Update,
		FreeDraw::Render,
		0,
		0);

	// Initialize the game state manager
	AEGameStateMgrInit(GS_FREE_DRAW);

	// Add the game states to the framework menu
	RegisterGameState("Free Draw", GS_FREE_DRAW);
	RegisterGameState("Draw Line Tests", GS_SIMPLE_LINES);
	RegisterGameState("Draw Triangles", GS_SIMPLE_TRIANGLES);

	// launch engine
	AESysGameLoop();

	// Terminate Graphics System
	Rasterizer::FrameBuffer::Delete();

	// Terminate AECore
	AESysExit();

	return 0;
}