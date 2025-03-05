/*****************************************************************//**
 * @file   Language.h
 * 
 * @brief  This file has everything that is used for the
 * localization tools that Atlas provides. Everything
 * within this file provides everything that is needed
 * for the ability to use translate strings based on
 * a language.
 * 
 * @date   January 2025
 * 
 * @since v0.0.9
 *********************************************************************/
#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

// This macro is temporarily being defined so that this
// code can be developed.
#define ATLAS_ENABLE_CHECKS

#ifdef ATLAS_ENABLE_CHECKS
	#include <assert.h>
#endif

#include "../core/CompileTimeCommons.h"

namespace Atlas {
	/**
	 * @brief Specifies a country's full formal
	 * name as a string. The value should be
	 * set according to the ISO 3166 (International
	 * Organization for Standardization) standard.
	 * 
	 * @note Atlas will not check if the country
	 * name is according to the ISO 3166 standard,
	 * so be sure to check it yourself.
	 * 
	 * @since v0.0.9
	 * 
	 * @sa @link
	 * https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
	 * @endlink
	 * For the International Organization for
	 * Standardization's ISO 3166 standard for
	 * countries as a Wikipedia article. Use this
	 * as a reference when setting the value.
	 */
	using Country = std::string;

	/**
	 * @brief Specifies a country's code based on the
	 * ISO 3166 (International Organization for
	 * Standardization). Even though this is a string,
	 * it is being set as a using statement so that
	 * it is more readable when it is used in the code.
	 * As an example, The United States has the code "US".
	 * For the link to the standard, see the "see also"
	 * section in the documentation of this using
	 * statement.
	 * 
	 * @note Atlas will not check if the country
	 * code is according to the ISO 3166 standard,
	 * so be sure to check it yourself. However, this
	 * is only true if ATLAS_ENABLE_CHECKS is defined.
	 * 
	 * @since v0.0.9
	 * 
	 * @sa @link
	 * https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
	 * @endlink 
	 * For the International Organization for
	 * Standardization's ISO 3166 standard for
	 * countries as a Wikipedia article. Use this
	 * as a reference when setting the value.
	 */
	using CountryCode = std::string;

	/**
	 * @brief .
	 * 
	 * @since v0.0.9
	 */
	class Region final {
	private:

		/**
		 * @brief Stores the country and the country code
		 * as a pair.
		 * 
		 * @remarks The reason for why this is a pair is
		 * that it is a bit more efficient than a struct
		 * or having two variables because a pair provides
		 * the same functionality without having to repeat
		 * certain code.
		 * 
		 * @note As is stated in the documentation of
		 * the @ref Country and @ref CountryCode using
		 * statements, the values should be according
		 * to the ISO 3166 standard.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref Country
		 * 
		 * @sa @ref CountryCode
		 * 
		 * @sa @link
		 * https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
		 * @endlink
		 * For the International Organization for
		 * Standardization's ISO 3166 standard for
		 * countries as a Wikipedia article. Use this
		 * as a reference when setting the value.
		 */
		std::pair<Country, CountryCode> mRegionData;

#ifdef ATLAS_ENABLE_CHECKS

		/**
		 * @brief Returns true if the country code is a valid
		 * ISO 3166 country code. This function is only available
		 * if ATLAS_ENABLE_CHECKS is defined and it is not advised
		 * that you use it in distribution code.
		 * 
		 * @todo Implement the ability to check if the country code
		 * is valid.
		 * 
		 * @param countryCode The code to check if it is valid
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		static inline bool IsValidCountryCode(CountryCode const& countryCode) {
			
		}
#endif
	public:

		void setCountry(Country const& country) {

#ifdef ATLAS_ENABLE_CHECKS

#endif
			this->mRegionData.first = country;
		}

		/**
		 * @brief Returns the @b first element of the pair.
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		Country getCountry() const {
			return this->mRegionData.first;
		}

		CountryCode getCountryCode() const {
			return this->mRegionData.second;
		}
	};

	/**
	 * @brief .
	 * 
	 * @since 
	 */
	class Language final {
	public:
		/**
		 * @brief .
		 * 
		 * @since 
		 */
		std::string language; // Ex. English, French

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		std::string region; // Ex. United States, United Kingdom

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		std::string regionCode; // Ex. US, GB

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		std::string code; // Ex. en-US, en-GB. If not present, it will set it to the filename
	};

	template<typename T_STRING_TYPE, typename T_COUNTRY_TYPE, typename T_COUNTRY_CODE_TYPE>
	class ILocalizedString {
	public:

		virtual void translate(Language const& translateToLanguage) = 0;

		virtual Language& getLanguage() const = 0;

		virtual void setText(T_STRING_TYPE const& text) = 0;
		
		virtual T_STRING_TYPE getText() const = 0;

		virtual ATLAS_IMPLICIT operator T_STRING_TYPE() const = 0;
	};

	/**
	 * @brief A string that can be translated based on
	 * a language.
	 * 
	 * @since 
	 */
	class BLocalizedString : public ILocalizedString<std::string, Country, CountryCode> {
	private:
		/**
		 * @brief .
		 * 
		 * @since 
		 */
		std::string mText;
	public:

		/**
		 * @brief .
		 * 
		 * @param translateToLanguage
		 * 
		 * @since 
		 */
		virtual void translate(Language const& translateToLanguage) {

		}

		/**
		 * @brief .
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual std::string getText() const override {
			return mText;
		}

		/**
		 * @brief .
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		virtual ATLAS_IMPLICIT operator std::string() const override {
			return getText();
		}
	};
}