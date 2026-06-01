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


	void DrawCircle(const AEVec2& center, float radius, const Color& c)
	{

	}

	void DrawCircleNaive(const AEVec2& center, float radius, const Color& c)
	{
	}

	void DrawCircleParametric(const AEVec2& center, float radius, const Color& c)
	{
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