/**
 @filename DrawTriangle.h
 
	http://joshbeam.com/articles/simple_line_drawing
*/
#pragma once


namespace Rasterizer
{
	class Color
	{
	public:
		Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
		unsigned int ToUInt32() const;
		Color operator + (const Color &c) const;
		Color operator - (const Color &c) const;
		Color operator * (float f) const;
		float R, G, B, A;
	};


	void DrawTriangle(HDC hdc, 
		const Color &color1, float x1, float y1,
		const Color &color2, float x2, float y2,
		const Color &color3, float x3, float y3);

	void DrawLine(HDC hdc, const Color &color1, float x1, float y1,
		const Color &color2, float x2, float y2);

}
