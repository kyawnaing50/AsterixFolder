/*
MIT License

Copyright (c) 2018 Alexander Gorin pu1s@outlook.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "basic_global.h"


//
// Define Main Types Open Astronomical Library
//
#define ASX_WSTRP	const wchar_t*
#define ASX_BOOL	bool
#define ASX_UINT	unsigned int
//


#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define ASX_FRMT_CUSTOM_DEFAULT_FLAG_DELIM	'%'
#define ASX_FRMT_CUSTOM_EQUAL				'='
#define ASX_FRMT_CUSTOM_SHOW_POS			'p'
#define ASX_FRMT_CUSTOM_POINT				'P'


namespace asterix
{
	namespace sys
	{
		namespace types
		{			
			template<typename _t, size_t N, template<class T =_t, class _Alloc = std::allocator<_t>> class vec = std::vector>
			struct basic_point_universal
			{
			private:
				vec<_t> _list;
			public:
				basic_point_universal()
				{
					_list = vec<_t>(N);
				}
				vec<_t>& get_val()
				{
					return *_list;
				}
			};
			
			struct asx_basic_point_util
			{
				virtual std::string CDECL to_str() noexcept;
				
			};

			template<typename T>
			struct asx_point2 : asx_basic_point_util
			{
			private:
				T _x;
				T _y;
			public:
				std::string CDECL to_str() noexcept override;
				T operator[](const signed index) noexcept;
			};
		}
	}
}
#include "basic.tmpl"