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

	void DrawHorizontalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

	}

	void DrawVerticalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {
	}

	void DrawDiagonalLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {
	}

	void DrawRect(const AEVec2& r, const AEVec2& size, const Color& c) {
	}


	void DrawLine(const AEVec2& p1, const AEVec2& p2, const Color& c) {

	}

	void DrawLineNaive(const AEVec2& p1, const AEVec2& p2, const Color& c) {
	}

	void DrawLineDDA(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{
	}

	void DrawLineBresenham(const AEVec2& p1, const AEVec2& p2, const Color& c)
	{

	}

	void DrawLine(const AEVec2& p1, const Color& c1, const AEVec2& p2, const Color& c2) 
	{

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