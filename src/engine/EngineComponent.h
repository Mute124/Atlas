#pragma once
#include <iostream>
#define TS_BLANK_STRUCT_MAKE(x) \
	using x = struct {} x##_blank
/*

#define TS_STRUCT(x, ...) using x = struct { __VA_ARGS__ } x;
#define TS_STRUCT_V(x, y, ...) using x = struct : y {  __VA_ARGS__ } x;*/
namespace Techstorm {


	class IEngineComponent abstract {
	public:
			
		virtual int preInit() = 0;
		virtual int init() = 0;
		virtual int postInit() = 0;

		virtual int start() = 0;
		virtual int stop() = 0;

		virtual int cleanup() = 0;
	};


}