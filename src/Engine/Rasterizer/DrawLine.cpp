#include <AEEngine.h>
#include "Rasterizer.h"


namespace Rasterizer
{
	/// -----------------------------------------------------------------------
	///	LINE ALGORITHM IMPLEMENTATIONS
	/// 
	/// 
	/// enum	EDrawLineMethod
	///	\brief	Specifies which method should be used when drawing a line. 
	EDrawLineMethod currentDrawLine = eDL_NAIVE;

	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//Get the points and the deltas
		float delta_x = p2.x - p1.x;
		float b = p1.y;
		int step = delta_x > 0 ? 1 : -1;
		int sX = Round(p1.x);
		int eX = Round(p2.x) + step;
		//Go though the points on x to get the values
		for (int x = sX; x != eX; x += step)
		{
			FrameBuffer::SetPixel(x, Round(b), c);
		}
	}

	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float delta_y = p2.y - p1.y;
		int step = delta_y > 0 ? 1 : -1;
		int sY = Round(p1.y);
		int eY = Round(p2.y) + step;
		int x = Round(p1.x);
		//Go though the points on y to get the values
		for (int y = sY; y != eY; y += step)
		{
			FrameBuffer::SetPixel(x, y, c);


		}
	}

	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float delta_x = p2.x - p1.x;
		float delta_y = p2.y - p1.y;
		int x = 0;
		int y = 0;
		int xstep = delta_x > 0 ? 1 : -1;
		int ystep = delta_y > 0 ? 1 : -1;
		//Constant values that only are round one time
		int sX = Round(p1.x);
		int eX = Round(p2.x) + xstep;
		int sY = Round(p1.y);
		int eY = Round(p2.y) + ystep;
		//To avoid problems use different so it can go from any direction
		for (x = sX, y = sY; x != eX && y != eY; x += xstep, y += ystep)
		{
			//With the equation of the naive line compute the function
			FrameBuffer::SetPixel(x, y, c);
		}
	}

	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c)
	{
		//Get each of the corners of the rectangle and with each of those get a vertex
		float left = r.x - size.x / 2;
		float right = r.x + size.x / 2;
		float top = r.y + size.y / 2;
		float bottom = r.y - size.y / 2;
		AEVec2 vertex_tr = { right ,top };
		AEVec2 vertex_tl = { left ,top };
		AEVec2 vertex_br = { right ,bottom };
		AEVec2 vertex_bl = { left ,bottom };
		//With each vertex draw four lines to get the rectangle
		DrawHorizontalLine(vertex_br, vertex_bl, c);
		DrawHorizontalLine(vertex_tr, vertex_tl, c);
		DrawVerticalLine(vertex_br, vertex_tr, c);
		DrawVerticalLine(vertex_bl, vertex_tl, c);
	}


	void DrawLine(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//Check the slope and depending of the value
		float delta_x = p2.x - p1.x;
		float delta_y = p2.y - p1.y;
		//Do a different draw method
		if ((int)delta_y == 0)
		{
			DrawHorizontalLine(p1, p2, c);

		}
		else if ((int)delta_x == 0)
		{
			DrawVerticalLine(p1, p2, c);

		}
		else if (abs(delta_y / delta_x) == 1)
		{
			DrawDiagonalLine(p1, p2, c);

		}
		else if (currentDrawLine == eDL_NAIVE)
		{
			DrawLineNaive(p1, p2, c);
		}
		else if (currentDrawLine == eDL_DDA)
		{
			DrawLineDDA(p1, p2, c);
		}
		else if (currentDrawLine == eDL_BRESENHAM)
		{
			DrawLineBresenham(p1, p2, c);
		}
	}

	void DrawLineNaive(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float delta_x = p2.x - p1.x;
		float delta_y = p2.y - p1.y;
		int y = 0;
		int x = 0;
		//The slope
		float m = delta_y / delta_x;
		//The bias
		float b = -m * p1.x + p1.y;
		//If the slope is less than 1
		if (abs(m) < 1)
		{
			//To avoid problems get the steps
			int xstep = delta_x > 0 ? 1 : -1;
			//Constant
			int sX = Round(p1.x);
			int eX = Round(p2.x) + xstep;

			for (x = sX; x != eX; x += xstep)
			{
				//Use the normal form of the explicit equation
				y = m * x + b;
				FrameBuffer::SetPixel(x, Round(y), c);
			}
		}
		else
		{
			int ystep = delta_y > 0 ? 1 : -1;
			//Constant
			int sY = Round(p1.y);
			int eY = Round(p2.y) + ystep;

			for (y = sY; y != eY; y += ystep)
			{
				//Invert it if the slope is bigger than 1 then get the x
				x = (y - b) / m;
				FrameBuffer::SetPixel(Round(x), y, c);
			}
		}
	}

	void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		//I get the bigger axis in both delta x and y
		float axis = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);
		//Get the amount of steps for each axis
		float xstep = dx / axis;
		float ystep = dy / axis;
		//Draw the line from the origin to the end
		for (int i = 0; i <= axis; i++)
		{
			//Get the values of each pixel with the i going to the next pixel in
			//Each iteration of the loop
			int x = Round(p1.x + (xstep * i));
			int y = Round(p1.y + (ystep * i));
			FrameBuffer::SetPixel(x, y, c);



		}
	}
	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawLineDDA
	/// \brief	Draws a line using the DDA algorithm presented in class.
	void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
		//Get the deltas o both points and their absoulute values
		int deltax = Round(p2.x - p1.x);
		int deltay = Round(p2.y - p1.y);
		int Ax = Round(abs(deltax));
		int Ay = Round(abs(deltay));
		//Depending of the value of the delta get a different value for the steps
		int stepx = deltax > 0 ? 1 : -1;
		int stepy = deltay > 0 ? 1 : -1;
		//Set all the variables that won´t change during the process
		int sX = Round(p1.x);
		int eX = Round(p2.x) + stepx;
		int sY = Round(p1.y);
		int eY = Round(p2.y) + stepy;

		if (Ax > Ay) // m <1
		{
			//Get the differential point with the general formula
			int dp = 2 * Ay - Ax;
			//Set the point of y
			int y = sY;
			for (int x = sX; x != eX;)
			{
				FrameBuffer::SetPixel(x, y, c);
				//Both coordinates change
				if (dp > 0)
				{
					x += stepx;
					y += stepy;
					//Change dp with the formula applying the simetry
					dp += 2 * Ay - 2 * Ax;
				}
				//Only the principal coordinate change
				else
				{
					x += stepx;
					dp += 2 * Ay;
				}
			}

		}
		else // m > 1
		{
			int dp = 2 * Ax - Ay;
			//Set x variable on the start
			int x = sX;
			//Move on y which makes the change be inverted
			for (int y = sY; y != eY;)
			{
				FrameBuffer::SetPixel(x, y, c);
				//Both coordinates change
				if (dp > 0)
				{
					x += stepx;
					y += stepy;
					dp += 2 * Ax - 2 * Ay;

				}
				//Only the principal coordinate change
				else
				{
					y += stepy;
					dp += 2 * Ax;
				}
			}


		}
	}
	void DrawLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2) 
	{
		float delta_x = p2.x - p1.x;
		float delta_y = p2.y - p1.y;
		//The slope
		float m = delta_y / delta_x;
		Color c = c1;		
		//If the slope is less than 1
		if (abs(m) < 1)
		{
			//To avoid problems get the steps
			int xstep = delta_x > 0 ? 1 : -1;
			//Constant
			int sX = Round(p1.x);
			int eX = Round(p2.x) + xstep;
			float ystep = delta_y > 0 ? abs(m) : -abs(m);
			float y = p1.y;
			Color cStep = (c2 - c1) / abs(delta_x);
			for (int x = sX; x != eX; x += xstep, y += ystep)
			{
				c += cStep;
				FrameBuffer::SetPixel(x, Round(y), c);
			}

		}
		else
		{
			int ystep = delta_y > 0 ? 1 : -1;
			//Constant
			int sY = Round(p1.y);
			int eY = Round(p2.y) + ystep;
			float xstep = delta_x > 0 ? abs(1 / m) : -abs(1 / m);
			float x = p1.x;
			Color cStep = (c2 - c1) / abs(delta_y);
			for (int y = sY; y != eY; y += ystep, x += xstep)
			{
				c += cStep;
				FrameBuffer::SetPixel(Round(x), y, c);
			}
		}
	}
	
	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	GetDrawLineMethod
	/// \brief	Return the current draw line method.
	EDrawLineMethod GetDrawLineMethod() {
		return currentDrawLine;
	}

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	GetDrawLineMethod
	/// \brief	Set the current draw line method to that given as input.
	void SetDrawLineMethod(EDrawLineMethod lineMethod) {
		currentDrawLine = lineMethod;
	}
}