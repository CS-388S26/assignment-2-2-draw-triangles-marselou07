// ----------------------------------------------------------------------------
// File: FreeDraw.cpp
// Project: CS200 Assignment 1
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// INCLUDES
#include <AEEngine.h>
#include <tuple>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"

namespace FreeDraw {

	// ----------------------------------------------------------------------------
	// GLOBALS AND DEFINITIONS
	std::vector<std::pair<AEVec2, AEVec2>> gLineArray;
	std::vector<std::pair<AEVec2, AEVec2>> gCircleArray;
	std::vector<std::tuple<AEVec2, AEVec2, AEVec2>> gTriangleArray;
	unsigned int	gCurrentLinePoint = 0;
	unsigned int	gCurrentCirclePoint = 0;
	unsigned int	gCurrentTrianglePoint = 0;
	unsigned int	gCurrentShape = 0;  
	bool	gInterpolation = 0;
	
	static void CancelInProgressShape(unsigned int shape);

	static bool HasTriangleArea(const AEVec2& p0, const AEVec2& p1, const AEVec2& p2);

	static void DrawTriangleOutline(const AEVec2& p0, const AEVec2& p1, const AEVec2& p2, const Rasterizer::Color& c);

	// ----------------------------------------------------------------------------
	// GAMESTATEFUNCTIONS
	void Init()
	{
		gLineArray.clear();
		gCircleArray.clear();
		gTriangleArray.clear();
		gCurrentLinePoint = 0;
		gCurrentTrianglePoint = 0;
		gCurrentShape = 0;
		gCurrentCirclePoint = 0;
		gInterpolation = 0;
	}

	void Update()
	{
		KeyboardInput();
		bool isGuiActive = ShowFrameworkMenu();

		// current the mouse position to frame buffer space (origin at topleft)
		AEVec2 mouseVP = AEGetMouseData().position + AEVec2((float)AEGetSysWindowWidth() / 2.0f, (float)AEGetSysWindowHeight() / 2.0f);

		// clear all primitives
		if (AEInputKeyTriggered('C'))
		{
			gLineArray.clear();
			gCircleArray.clear();
			gTriangleArray.clear();
			gCurrentLinePoint = 0;
			gCurrentTrianglePoint = 0;
			gCurrentShape = 0;
			gCurrentCirclePoint = 0;
		}
		
		if (AEInputKeyTriggered('1') && gCurrentShape != 0)
		{
			CancelInProgressShape(gCurrentShape);
			gCurrentShape = 0;
		}
		if (AEInputKeyTriggered('2') && gCurrentShape != 1)
		{
			CancelInProgressShape(gCurrentShape);
			gCurrentShape = 1;
		}
		if (AEInputKeyTriggered('3') && gCurrentShape != 2)
		{
			CancelInProgressShape(gCurrentShape);
			gCurrentShape = 2;
		}

		if (AEInputKeyTriggered('4'))
			gInterpolation = !gInterpolation;

		if (!isGuiActive) {
			// new line
			if (AEInputMouseTriggered(AE_MOUSE_LEFT))
			{
				if (gCurrentShape == 0) {
					if (gCurrentLinePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) {
						gLineArray.push_back(std::pair<AEVec2, AEVec2>(mouseVP, mouseVP));
						gCurrentLinePoint++;
					}
					else gCurrentLinePoint = 0;
				}

				if (gCurrentShape == 1) {
					if (gCurrentCirclePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) {
						gCircleArray.push_back(std::pair<AEVec2, AEVec2>(mouseVP, mouseVP));
						gCurrentCirclePoint++;
					}
					else gCurrentCirclePoint = 0;
				}

				if (gCurrentShape == 2) {
					if (gCurrentTrianglePoint == 0) {
						gTriangleArray.push_back(std::make_tuple(mouseVP, mouseVP, mouseVP));
						gCurrentTrianglePoint = 1;
					}
					else if (AEInputKeyPressed(AE_KEY_CTRL)) {
						gTriangleArray.push_back(std::make_tuple(mouseVP, mouseVP, mouseVP));
						gCurrentTrianglePoint = 1;
					}
					else if (gCurrentTrianglePoint == 1) {
						std::get<1>(gTriangleArray.back()) = mouseVP;
						gCurrentTrianglePoint = 2;
					}
					else if (gCurrentTrianglePoint == 2) {
						std::get<2>(gTriangleArray.back()) = mouseVP;
						gCurrentTrianglePoint = 0;
					}
				}
			}

			// follow mouse
			if (gCurrentLinePoint)
			{
				gLineArray.back().second = mouseVP;
			}

			if (gCurrentCirclePoint)
			{
				gCircleArray.back().second = mouseVP;
			}

			if (gCurrentTrianglePoint && !gTriangleArray.empty())
			{
				if (gCurrentTrianglePoint == 1)
					std::get<1>(gTriangleArray.back()) = mouseVP;
				else if (gCurrentTrianglePoint == 2)
					std::get<2>(gTriangleArray.back()) = mouseVP;
			}
		}
	}
	void Render()
	{
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));		

		// render all the lines
		for (unsigned int i = 0; i < gLineArray.size(); ++i)
		{
			// get line endpoints
			AEVec2& p0 = gLineArray[i].first;
			AEVec2& p1 = gLineArray[i].second;

			// Draw the line
			if (gInterpolation == 0)
				Rasterizer::DrawLine(p0, p1, Rasterizer::Color());
			else
				Rasterizer::DrawLine(p0, Rasterizer::Color(1, 0, 0, 1), p1, Rasterizer::Color(0, 0, 1, 1));
		}

		for (unsigned int i = 0; i < gCircleArray.size(); ++i)
		{
			// get circle endpoints
			AEVec2& p0 = gCircleArray[i].first;
			AEVec2& p1 = gCircleArray[i].second;

			float r = p0.Distance(p1) / 2.0f;
			AEVec2 dir = (p1 - p0).Normalize();
			
			// Draw the circle
			Rasterizer::DrawCircle(p0 + dir*r, r, Rasterizer::Color());
		}

		for (unsigned int i = 0; i < gTriangleArray.size(); ++i)
		{
			AEVec2& p0 = std::get<0>(gTriangleArray[i]);
			AEVec2& p1 = std::get<1>(gTriangleArray[i]);
			AEVec2& p2 = std::get<2>(gTriangleArray[i]);

			const bool inProgress = (i == gTriangleArray.size() - 1) && gCurrentTrianglePoint != 0;

			if (inProgress)
			{
				// Do not fill while placing: p0 == p2 until the third click.
				Rasterizer::DrawLine(p0, p1, Rasterizer::Color());
				if (gCurrentTrianglePoint == 2)
				{
					Rasterizer::DrawLine(p1, p2, Rasterizer::Color());
					Rasterizer::DrawLine(p2, p0, Rasterizer::Color());
				}
			}
			else if (HasTriangleArea(p0, p1, p2))
			{
				if(gInterpolation == 0)
					Rasterizer::FillTriangleNaive(p0, p1, p2, Rasterizer::Color());
				else 
				{
					Rasterizer::Vertex v0{ p0, Rasterizer::Color(1, 0, 0, 1) };
					Rasterizer::Vertex v1{ p1, Rasterizer::Color(0, 1, 0, 1) };
					Rasterizer::Vertex v2{ p2, Rasterizer::Color(0, 0, 1, 1) };

					Rasterizer::DrawTriangle(v0, v1, v2);
				}
				}
			else
			{
				DrawTriangleOutline(p0, p1, p2, Rasterizer::Color());
			}
		}

		Rasterizer::FrameBuffer::Present();

		AEGfxPrint(10, 30, AE_COLORS_BLACK, "1. Lines, 2. Circles, 3. Triangles, 4. Interpolation, C. Clear");
		
		const char* shapeName = "Triangles";

		switch (gCurrentShape)
		{
		case 0: shapeName = "Lines"; break;
		case 1: shapeName = "Circles"; break;
		case 2: shapeName = "Triangles"; break;
		default: break;
		}
		AEGfxPrint(10, 50, AE_COLORS_BLACK, shapeName);
	}

	static void CancelInProgressShape(unsigned int shape)
	{
		switch (shape)
		{
		case 0:
			if (gCurrentLinePoint && !gLineArray.empty())
				gLineArray.pop_back();
			gCurrentLinePoint = 0;
			break;
		case 1:
			if (gCurrentCirclePoint && !gCircleArray.empty())
				gCircleArray.pop_back();
			gCurrentCirclePoint = 0;
			break;
		case 2:
			if (gCurrentTrianglePoint && !gTriangleArray.empty())
				gTriangleArray.pop_back();
			gCurrentTrianglePoint = 0;
			break;
		}
	}

	static bool HasTriangleArea(const AEVec2& p0, const AEVec2& p1, const AEVec2& p2)
	{
		float area = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
		if (area < 0.0f)
			area = -area;
		return area >= 1.0f;
	}

	static void DrawTriangleOutline(const AEVec2& p0, const AEVec2& p1, const AEVec2& p2, const Rasterizer::Color& c)
	{
		Rasterizer::DrawLine(p0, p1, c);
		Rasterizer::DrawLine(p1, p2, c);
		Rasterizer::DrawLine(p2, p0, c);
	}
} // namespace FreeDraw