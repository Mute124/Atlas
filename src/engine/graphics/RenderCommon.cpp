/**************************************************************************************************
 * @file RenderCommon.cpp
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "RenderCommon.h"
#include "../core/Common.h"

Atlas::ColorBit::ColorBit(float normalizedValue) : value(Convert<float, unsigned char>(normalizedValue* Literals::COLOR_BIT_DENORMALIZING_FACTOR))
{
}

Atlas::ColorBit::ColorBit(unsigned char initialValue) : value(value) 
{
}

Atlas::ColorBit::operator unsigned char() const {
	return value;
}

Atlas::ColorBit::operator float() const {
	float result = 0.0f;
	result = value / static_cast<float>(Literals::COLOR_BIT_DENORMALIZING_FACTOR);

	result = Clamp(result, 0.0f, 1.0f);

	return result; 
}

Atlas::Color::Color(ColorBit const& red, ColorBit const& green, ColorBit const& blue, ColorBit const& alpha)
: red(red), green(green), blue(blue), alpha(alpha)
{
}

Atlas::Color::Color(ColorBit const& red, ColorBit const& green, ColorBit const& blue)
	: Color(red, green, blue, 1.0f)
{
}

Atlas::Color::Color(ColorBit const& color)
	: Color(color, color, color)
{
}
