/**************************************************************************************************
 * @file Color.h
 * 
 * @brief Contains everything related to colors.
 * 
 * @date September 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <array>

#include "../core/Core.h"
#include "../core/Common.h"
#include "../core/Math.h"

namespace Atlas {
	
	/**
	 * @brief A single color value. Each color channel is a 8-bit value.
	 * 
	 * @since v0.0.1
	 * 
	 * @sa @ref Color
	 */
	using ColorBit = uint8_t;

	class Color {
	private:
		
		/**
		 * @brief The color's red ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit mRed;

		/**
		 * @brief The color's green ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit mGreen;
		
		/**
		 * @brief The color's blue ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit mBlue;
		
		/**
		 * @brief The color's alpha (opacity) ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit mAlpha;

		/**
		 * @brief Converts a float value to a ColorBit value.
		 * 
		 * @param value The float value to convert.
		 * 
		 * @return ColorBit The converted value.
		 * 
		 * @since v0.0.1
		 */
		static inline ColorBit FloatToColorBit(float value) {
			return Normalize<ColorBit>(value, 0, 255);
		}

	public:

		/**
		 * @brief Construct a new Color object with the specified ColorBit channels.
		 * 
		 * @param red The red ColorBit channel.
		 * @param green The green ColorBit channel.
		 * @param blue The blue ColorBit channel.
		 * @param alpha The alpha (opacity) ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		explicit Color(ColorBit red, ColorBit green, ColorBit blue, ColorBit alpha) : mRed(red), mGreen(green), mBlue(blue), mAlpha(alpha) {}
		
		/**
		 * @brief Construct a new Color object with the specified float values that will be converted to ColorBit using @ref FloatToColorBit.
		 * 
		 * @param red The red float value.
		 * @param green The green float value.
		 * @param blue The blue float value.
		 * @param alpha The alpha (opacity) float value.
		 * 
		 * @since v0.0.1
		 */
		explicit Color(float red, float green, float blue, float alpha) 
			: mRed(FloatToColorBit(red)), mGreen(FloatToColorBit(green)), mBlue(FloatToColorBit(blue)), mAlpha(FloatToColorBit(alpha)) {}

		/**
		 * @brief Construct a new Color object with the specified rgba uint32_t value. The rgba value is expected to be in the format 0xAARRGGBB.
		 * 
		 * @param rgba The rgba value in the format 0xAARRGGBB.
		 * 
		 * @since v0.0.1
		 */
		explicit Color(uint32_t rgba) {
			// make sure the rgba value is in the correct format
			ATLAS_ASSERT((rgba & 0xFF000000) == 0xFF000000, "rgba value is not in the correct format");

			mRed = (rgba >> 24) & 0xFF;
			mGreen = (rgba >> 16) & 0xFF;
			mBlue = (rgba >> 8) & 0xFF;
			mAlpha = rgba & 0xFF;
		}
		
		/**
		 * @brief Default copy constructor.
		 * 
		 * @param other The other Color object to copy.
		 * 
		 * @since v0.0.1
		 */
		Color(const Color& other) = default;

		/**
		 * @brief Default move constructor.
		 * 
		 * @param other The other Color object to move.
		 * 
		 * @since v0.0.1
		 */
		Color(Color&& other) = default;

		/**
		 * @brief Default constructor.
		 * 
		 * @since v0.0.1
		 */
		Color() = default;

		/**
		 * @brief Default destructor.
		 * 
		 * @since v0.0.1
		 */
		~Color() = default;

		/**
		 * @brief Sets the red ColorBit channel and clamps it to a value between 0 and 255.
		 * 
		 * @param red The value to set the red ColorBit channel to.
		 * 
		 * @since v0.0.1
		 */
		void setRed(ColorBit red) {
			mRed = std::clamp(red, ColorBit(0), ColorBit(255));
		}

		/**
		 * @brief Sets the green ColorBit channel and clamps it to a value between 0 and 255.
		 * 
		 * @param green The value to set the green ColorBit channel to.
		 * 
		 * @since v0.0.1
		 */
		void setGreen(ColorBit green) {
			mGreen = std::clamp(green, ColorBit(0), ColorBit(255));
		}

		/**
		 * @brief Sets the blue ColorBit channel and clamps it to a value between 0 and 255.
		 * 
		 * @param blue The value to set the blue ColorBit channel to.
		 * 
		 * @since v0.0.1
		 */
		void setBlue(ColorBit blue) {
			mBlue = std::clamp(blue, ColorBit(0), ColorBit(255));
		}

		/**
		 * @brief Sets the alpha (opacity) ColorBit channel and clamps it to a value between 0 and 255.
		 * 
		 * @param alpha The value to set the alpha (opacity) ColorBit channel to.
		 * 
		 * @since v0.0.1
		 */
		void setAlpha(ColorBit alpha) {
			mAlpha = std::clamp(alpha, ColorBit(0), ColorBit(255));
		}

		/**
		 * @brief Returns the red ColorBit channel.
		 * 
		 * @return ColorBit The red ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit getRed() const { 
			return mRed;
		}

		/**
		 * @brief Returns the green ColorBit channel.
		 * 
		 * @return ColorBit The green ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit getGreen() const {
			return mGreen;
		}

		/**
		 * @brief Returns the blue ColorBit channel.
		 * 
		 * @return ColorBit The blue ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit getBlue() const {
			return mBlue; 
		}

		/**
		 * @brief Returns the alpha (opacity) ColorBit channel.
		 * 
		 * @return ColorBit The alpha (opacity) ColorBit channel.
		 * 
		 * @since v0.0.1
		 */
		ColorBit getAlpha() const {
			return mAlpha; 
		}

		// Conversion operators

		/**
		 * @brief Converts the Color object to an array of ColorBit values
		 * 
		 * @return std::array<ColorBit, 4> An array of ColorBit values
		 * 
		 * @since v
		 */
		std::array<ColorBit, 4> toArray() const { 
			return std::array<ColorBit, 4>{ mRed, mGreen, mBlue, mAlpha }; 
		}

		/**
		 * @brief Adds the specified Color object to this Color object.
		 * 
		 * @param other The other Color object to add.
		 * 
		 * @return Color& A reference to this Color object.
		 * 
		 * @since v0.0.1
		 */
		Color& operator+=(const Color& other) {
			mRed += other.mRed;
			mGreen += other.mGreen;
			mBlue += other.mBlue;
			mAlpha += other.mAlpha;

			return *this;
		}
	};
}
