#include <AEEngine.h>
#include "Rasterizer.h"
namespace Rasterizer
{

	void FillTriangleNaive(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{

	}

	void FillTriangleTopLeft(const AEVec2& v0, const AEVec2& v1, const AEVec2& v2, const Color& c)
	{
	}

	/// -----------------------------------------------------------------------
	///	DRAW TRIANGLE ALGORITHM IMPLEMENTATIONS
	/// 
	/// 
	/// enum	EDrawLineMethod
	///	\brief	Specifies which method should be used when drawing a line. 
	EDrawTriangleMethod currentDrawTriangle = eDT_BILINEAR;

	void DrawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
	}

	void DrawTriangleBiLinear(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
	}

	EDrawTriangleMethod GetDrawTriangleMethod() {
		return {};
	}

	void SetDrawTriangleMethod(EDrawTriangleMethod triangleMethod) {

	}
}