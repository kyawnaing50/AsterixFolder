/*
MIT License

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
SOFTWARE.
*/

#include "sexagesimal_common.h"
#define BAD_BIT char
#define hms_out(x) (print_hms(x))
#define dms_out(x) (print_dms(x))

namespace asterix
{
	namespace sys
	{
		namespace types
		{
			void make_hms(const signed int h, const unsigned int m, const double s, struct hms_t * hms)
			{

				BAD_BIT bad_bit = 0;
				if (hms == nullptr)
				{
					return;
					bad_bit = 1;
				}
				if ((m < MIN_LIMIT_MIN) && (m > MIN_LIMIT_MAX))
				{
					bad_bit = 1;
				}

				if ((s < SEC_LIMIT_MIN) && (s > SEC_LIMIT_MAX))
				{
					bad_bit = 1;
				}
				if (bad_bit == 0)
				{
					hms->hrs = h;
					hms->min = m;
					hms->sec = s;
				}
				else
				{
					hms->hrs = 0;
					hms->min = 0;
					hms->sec = 0;
				}
				return;
			}



			void make_dms(const signed int d, const unsigned int m, const double s, dms_t * dms)
			{
				if (dms == nullptr) return;
				BAD_BIT bad_bit = 0;
				if (dms != nullptr)
				{
					bad_bit = 1;
				}
				if ((m < MIN_LIMIT_MIN) && (m > MIN_LIMIT_MAX))
				{
					bad_bit = 1;
				}

				if ((s < SEC_LIMIT_MIN) && (s > SEC_LIMIT_MAX))
				{
					bad_bit = 1;
				}
				if (bad_bit == 0)
				{
					dms->deg = d;
					dms->min = m;
					dms->sec = s;
				}
				else
				{
					dms->deg = 0;
					dms->min = 0;
					dms->sec = 0;
				}
				return;
			}

			void print_hms(const HMS* hms)
			{
				if (hms != NULL)
				{
					printf("hour: %d, min: %d, sec: %f", hms->hrs, hms->min, hms->sec);
				}
				else
				{
					printf("hms object not found.\r\n");
				}
			}

			void  print_dms(const DMS* dms)
			{
				if (dms != NULL)
				{
					printf("deg: %d, min: %d, sec: %f", dms->deg, dms->min, dms->sec);
				}
				else
				{
					printf("dms object not found.\r\n");
				}
			}

		}

	}
}
