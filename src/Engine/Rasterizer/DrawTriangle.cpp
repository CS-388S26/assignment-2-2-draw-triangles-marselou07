#include <AEEngine.h>
#include "Rasterizer.h"
namespace Rasterizer
{

	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{
		//Order the cases to know which is the order of corners
		const AEVec2* top = &v0;
		const AEVec2* mid = &v1;
		const AEVec2* bottom = &v2;
		bool mIdIsLeft;
		if (top->y < mid->y)
		{
			std::swap(top, mid);

		}
		if (mid->y < bottom->y)
		{
			std::swap(mid, bottom);
		}
		if (top->y < mid->y)
		{
			std::swap(top, mid);
		}
		//Get the direction
		AEVec2 TB = *bottom - *top;
		AEVec2 TM = *mid - *top;
		float dot = TB.x * TM.y - TB.y * TM.x;
		if (dot < 0)
		{
			mIdIsLeft = true;
		}
		else
		{
			mIdIsLeft = false;
		}
		//Know start the set up to draw 
		//Get the inverse slope of each line
		float slopetm = (mid->x - top->x) / (mid->y - top->y);
		float slopetb = (bottom->x - top->x) / (bottom->y - top->y);
		float slopemb = (bottom->x - mid->x) / (bottom->y - mid->y);
		//Get the starting points the x must be floats for the inv slope to work
		int sY = Round(top->y);
		float xright = Round(top->x);
		float xleft = Round(top->x);
		//Top middle region
		int y;
		for (y = sY; y >= Round(mid->y); --y)
		{
			for (unsigned int x = Round(xleft); x <= Round(xright); x++)
			{
				FrameBuffer::SetPixel(x, y, c);
			}
			xleft -= mIdIsLeft ? slopetm : slopetb;
			xright -= mIdIsLeft ? slopetb : slopetm;
		}
		//Adjust the xright and xleft to avoid floating errors
		if (mIdIsLeft)
		{
			xleft = mid->x;
		}
		else
		{
			xright = mid->x;
		}
		//Bottom region
		for (; y >= Round(bottom->y); --y)
		{
			for (unsigned int x = Round(xleft); x <= Round(xright); x++)
			{
				FrameBuffer::SetPixel(x, y, c);
			}
			xleft -= mIdIsLeft ? slopemb : slopetb;
			xright -= mIdIsLeft ? slopetb : slopemb;
		}



	}

	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{
		//Order the cases to know which is the order of corners
		const AEVec2* top = &v0;
		const AEVec2* mid = &v1;
		const AEVec2* bottom = &v2;
		bool mIdIsLeft;
		if (top->y < mid->y)
		{
			std::swap(top, mid);

		}
		if (mid->y < bottom->y)
		{
			std::swap(mid, bottom);
		}
		if (top->y < mid->y)
		{
			std::swap(top, mid);
		}
		//Get the direction
		AEVec2 TB = *bottom - *top;
		AEVec2 TM = *mid - *top;
		float dot = TB.x * TM.y - TB.y * TM.x;
		if (dot < 0)
		{
			mIdIsLeft = true;
		}
		else
		{
			mIdIsLeft = false;
		}
		//Know start the set up to draw 
		//Get the inverse slope of each line
		float slopetm = (mid->x - top->x) / (mid->y - top->y);
		float slopetb = (bottom->x - top->x) / (bottom->y - top->y);
		float slopemb = (bottom->x - mid->x) / (bottom->y - mid->y);
		//Get the starting points the x must be floats for the inv slope to work
		int sY = Ceiling(top->y);
		float xright = Round(top->x);
		float xleft = Round(top->x);
		//Top middle region
		int y;
		for (y = sY; y >= Ceiling(mid->y) + 1; --y)
		{
			for (unsigned int x = Floor(xleft); x <= Floor(xright) - 1; x++)
			{
				FrameBuffer::SetPixel(x, y, c);
			}
			xleft -= mIdIsLeft ? slopetm : slopetb;
			xright -= mIdIsLeft ? slopetb : slopetm;
		}
		//Adjust the xright and xleft to avoid floating errors
		if (mIdIsLeft)
		{
			xleft = mid->x;
		}
		else
		{
			xright = mid->x;
		}
		//Bottom region
		for (; y >= Ceiling(bottom->y) + 1; --y)
		{
			for (unsigned int x = Floor(xleft); x <= Floor(xright) - 1; x++)
			{
				FrameBuffer::SetPixel(x, y, c);
			}
			xleft -= mIdIsLeft ? slopemb : slopetb;
			xright -= mIdIsLeft ? slopetb : slopemb;
		}
	}

	/// -----------------------------------------------------------------------
	///	DRAW TRIANGLE ALGORITHM IMPLEMENTATIONS
	/// 
	/// 
	/// enum	EDrawLineMethod
	///	\brief	Specifies which method should be used when drawing a line. 
	EDrawTriangleMethod currentDrawTriangle = eDT_BILINEAR;
	// TODO
	/// -----------------------------------------------------------------------
	/// \fn		DrawTriangle
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2. It delegates
	///			the call to one of the methods implemented internally. See
	///			enum EDrawTriangleMethod above. 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		if (currentDrawTriangle == eDT_BILINEAR)
		{
			DrawTriangleBiLinear(v0, v1, v2);
		}










		



	}
	/// ------------------------------------------------------------------------
	/// \fn		DrawTriangleBiLinear
	/// \brief	Rasterizes a CCW triangle defined by v0, v1, v2, using the top
	///			left method and the bi-linear interpolation method for color 
	/// \param	v0	First triangle vertex (position/color).
	///	\param	v1	Second triangle vertex (position/color).
	///	\param	v2	Third triangle vertex (position/color).
	void DrawTriangleBiLinear(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		Vertex top = v0;
		Vertex mid = v1;
		Vertex bottom = v2;
		bool mIdIsLeft = false;
		if (top.mPosition.y < mid.mPosition.y)
		{
			std::swap(top, mid);
		}
		if (mid.mPosition.y < bottom.mPosition.y)
		{
			std::swap(mid, bottom);
		}
		if (top.mPosition.y < mid.mPosition.y)
		{
			std::swap(top, mid);
		}
		AEVec2 TB = bottom.mPosition - top.mPosition;
		AEVec2 TM = mid.mPosition - top.mPosition;
		float dot = TB.x * TM.y - TB.y * TM.x;
		if(dot <0)
		{
			mIdIsLeft = true;

		}
		else
		{
			mIdIsLeft = false;
		}
		//All the colors
		Color cstepTM = (mid.mColor - top.mColor) / (mid.mPosition.y - top.mPosition.y);
		Color cstepTB = (bottom.mColor - top.mColor) / (bottom.mPosition.y - top.mPosition.y);
		Color cstepMB = (bottom.mColor - mid.mColor) / (bottom.mPosition.y - mid.mPosition.y);
		Color CL = top.mColor;
		Color CR = top.mColor;
		Color cStepLS;
		Color cFinal = top.mColor;
		//Get the slopes and constant vector
		float slopetm = (mid.mPosition.x - top.mPosition.x) / (mid.mPosition.y - top.mPosition.y);
		float slopetb = (bottom.mPosition.x - top.mPosition.x) / (bottom.mPosition.y - top.mPosition.y);
		float slopemb = (bottom.mPosition.x - mid.mPosition.x) / (bottom.mPosition.y - mid.mPosition.y);
		int sY = Ceiling(top.mPosition.y);
		float xright = Round(top.mPosition.x);
		float xleft = Round(top.mPosition.x);
		int y;
		//The  middle region
		for (y = sY; y >= Ceiling(mid.mPosition.y) + 1; --y)
		{
			cFinal = CL;
			cStepLS = (CR -CL)/fabs(xright-xleft);
			for ( int x = Floor(xleft); x <= Floor(xright) - 1; x++)
			{
				cFinal += cStepLS;
				FrameBuffer::SetPixel(x, y, cFinal);
				
			}
			//Decrease the left and right x
			xleft -= mIdIsLeft ? slopetm : slopetb;
			xright -= mIdIsLeft ? slopetb : slopetm;
			//Change the color
			CL += mIdIsLeft ? cstepTM : cstepTB;
			CR += mIdIsLeft ? cstepTB : cstepTM;
		}
		if (mIdIsLeft)
		{
			xleft = mid.mPosition.x;

		}
		else
		{
			xright = mid.mPosition.x;
			
		}
		for (; y >= Ceiling(bottom.mPosition.y); --y)
		{
			cFinal = CL;
			cStepLS = (CR - CL) / fabs(xright - xleft);
			for ( int x = Floor(xleft); x <= Floor(xright) - 1; x++)
			{
				cFinal += cStepLS;
				FrameBuffer::SetPixel(x, y, cFinal);
				
			}
			//Decrease the left and right x
			xleft -= mIdIsLeft ? slopemb : slopetb;
			xright -= mIdIsLeft ? slopetb : slopemb;
			//Change the color
			CL += mIdIsLeft ? cstepMB : cstepTB;
			CR += mIdIsLeft ? cstepTB : cstepMB;

		}
		//cL += mIdIsLeft ? cstepTM : cstepTB
		// cr += mIdIsLeft ? cstepTB : cstepTM
		//cFinal = c;
		//cStepLS = (CR - CL) / abs(XR - XL);
		//Or t or s also valid name
		//interpolate
		//cFinal += cStepLS
		//eL QUE DIBUJA
	}

	EDrawTriangleMethod GetDrawTriangleMethod() {
		return currentDrawTriangle;
	}

	void SetDrawTriangleMethod(EDrawTriangleMethod triangleMethod) {
		currentDrawTriangle = triangleMethod;
	}
}