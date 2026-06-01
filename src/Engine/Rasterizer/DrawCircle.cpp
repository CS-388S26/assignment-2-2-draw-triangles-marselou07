#include <AEEngine.h>
#include "Rasterizer.h"


namespace Rasterizer
{
	/// -----------------------------------------------------------------------
	///	Circle ALGORITHM IMPLEMENTATIONS
	/// 
	/// 
	/// enum	EDrawCircleMethod
	///	\brief	Specifies which method should be used when drawing a circle. 
	EDrawCircleMethod currentDrawCircle = eDC_NAIVE;


	void SetPixelEightWay(unsigned int cx, unsigned int cy, unsigned int x, unsigned int y, const Color& c)
	{
		FrameBuffer::SetPixel(cx + x, cy + y, c);
		FrameBuffer::SetPixel(cx - x, cy - y, c);
		FrameBuffer::SetPixel(cx + x, cy - y, c);
		FrameBuffer::SetPixel(cx - x, cy + y, c);


		FrameBuffer::SetPixel(cx + y, cy + x, c);
		FrameBuffer::SetPixel(cx - y, cy - x, c);
		FrameBuffer::SetPixel(cx + y, cy - x, c);
		FrameBuffer::SetPixel(cx - y, cy + x, c);
	}

	void DrawCircle(const AEVec2& center, float radius, const Color& c)
	{
		if (currentDrawCircle == eDC_NAIVE)
		{
			DrawCircleNaive(center, radius, c);
		}
		else if (currentDrawCircle == eDC_PARAMETRIC)
		{

			DrawCircleParametric(center, radius, c);

		}

	}

	void DrawCircleNaive(const AEVec2& center, float radius, const Color& c)
	{

		//The end point
		unsigned int ex = Round((sqrt(2) / 2) * radius);
		//From 0 to the end set the y with the formula with the center point at 0
		for (unsigned int x = 0; x <= ex; x++)
		{
			int y = Round(sqrtf((radius * radius) - (x * x)));


			//Set eight pixels when draw
			SetPixelEightWay(Round(center.x), Round(center.y), x, y, c);
		}


	}

	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c)
	{
		//Start from the angle 0 to be less than 2PI
		float angle = 0.f;
		//To find the angle we need to use the formula of S = angle * radius 
		float s = 1.f;
		float step = s / radius;
		int x;
		int y;
		//Set the circle with the parametric formula and move it with the center
		for (; angle < PI / 4; angle += step)
		{
			//The x and y with the parametric equation
			x = Round(radius * cos(angle));
			y = Round(radius * sin(angle));
			//FrameBuffer::SetPixel(x,y,c);
			SetPixelEightWay(Round(center.x), Round(center.y), x, y, c);


		}
		//Faster go to 45 degress using the octan to optimize


	}


	void DrawCircleMidpoint(const AEVec2& center, float radius, const Color& c)
	{
	}

	void DrawCircleBresenham(const AEVec2& center, float radius, const Color& c)
	{
	}

	EDrawCircleMethod GetDrawCircleMethod()
	{
		return currentDrawCircle;
	}

	void SetDrawCircleMethod(EDrawCircleMethod dlm) {
		currentDrawCircle = dlm;	
	}
}