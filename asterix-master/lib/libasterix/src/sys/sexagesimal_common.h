/*MIT License

Copyright(c) 2018 pu1s

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef SEXAGESIMAL_COMMON_H
#define SEXAGESIMAL_COMMON_H


#include "stdio.h"
namespace asterix
{
	namespace sys
	{
		
		namespace types
		{
			typedef struct hms_t
			{
					signed int		hrs;
					unsigned int	min;
					double			sec;
			}HMS;

			typedef struct dms_t
			{
					signed int		deg;
					unsigned int	min;
					double			sec;
			}DMS;

			void make_hms(const signed int h, const unsigned int m, const double s, hms_t * hms);

			void make_dms(const signed int d, const unsigned int m, const double s, dms_t * dms);

			void print_hms(const HMS* hms);

			void print_dms(const DMS* dms);
		}
	}
}



#endif //SEXAGESIMAL_H