#ifndef CS200_DRAW_CIRCLE_H_
#define CS200_DRAW_CIRCLE_H_

namespace Rasterizer
{
	/// @TODO
	// ------------------------------------------------------------------------
	/// enum	EDrawLineMethod
	///	\brief	Specifies which method should be used when drawing a line. 
	enum EDrawCircleMethod { eDC_NAIVE, eDC_PARAMETRIC, eDC_MIDPOINT, eDC_BRESENHAM };

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		GetDrawLineMethod
	/// \brief	Sets the line rendering method.
	EDrawCircleMethod GetDrawCircleMethod();

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	SetDrawLineMethod	
	/// \brief	Sets the line rendering method.
	void SetDrawCircleMethod(EDrawCircleMethod dlm);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawLine
	/// \brief	Wrapper function, draws a circle using the current method. 
	void DrawCircle(const AEVec2& center, float radius, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawCircleNaive
	/// \brief	Draws a circle using the naive algorithm presented in class, based on the algebraic equation
	void DrawCircleNaive(const AEVec2& center, float radius, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn		DrawCircleParametric
	/// \brief	Draws a circle using the parametric algorithm presented in class.
	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawCircleMidpoint
	/// \brief	Draws a circle using the midpoint algorithm presented in class. 
	void DrawCircleMidpoint(const AEVec2& center, float radius, const Color& c);

	/// @TODO
	// ------------------------------------------------------------------------
	/// \fn	DrawCircleMidpoint
	/// \brief	Draws a circle using the midpoint algorithm presented in class. 
	void DrawCircleBresenham(const AEVec2& center, float radius, const Color& c);
}
#endif