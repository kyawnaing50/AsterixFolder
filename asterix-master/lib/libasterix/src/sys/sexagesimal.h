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

#include "sexagesimal_common.h"
#include "sexagesimal_common.c"
#include "basic.h"
//#include "sexagesimal_frmt.hpp"

#ifndef NDEBUG
#define NDEBUG
#include<cassert>
#endif // !NDEBUG
#include <string>
#include <cstdlib>
#include <cmath>



#define ASX_GLOBAL_LIMIT_HRS_MIN		-9999
#define ASX_GLOBAL_LIMIT_HRS_MAX		9999
#define ASX_GLOBAL_LIMIT_DEG_MIN		-9999
#define ASX_GLOBAL_LIMIT_DEG_MAX		9999
#define ASX_GLOBAL_LIMIT_DAD_MIN		-9999
#define ASX_GLOBAL_LIMIT_RAD_MAX		9999
#define ASX_SEXAGESIMAL_NO_RESULT		0.0


#pragma warning (push)
#pragma warning(disable : 4244)


namespace asterix
{
	namespace sys
	{
		namespace types
		{
			/*
			Перечисление, определяющее тип координаты (высота-широта)
			*/
			typedef enum class coordinate_type
			{
				UNKNOWN,			// unknown
				ALTITUDE,			// altitude
				LONGITUDE			// longitude
			}asx_coordinate_type, alt_lon_type;

			/*
			Структура данных угловых величин
			*/
			typedef struct sexagesimal_arcdata_tag
			{
				struct angle_and_time_tag
				{
					signed						arcdeg;				// угол в градусах (знаковое целое)
					signed						archrs;				// угол во временных единицах (знаковое целое)
				}deg_hrs;
				unsigned						arcmin;				// минуты дуги (беззнаковое целое)
				double							arcsec;				// секунды дуги (вещественное)
				double							arcrad;				// угол в радианах (вещественное)
				double							arcgrad;			// угол в градусах (вещественное)
				
			}asx_sexigesimal_arcdata_t;

			/*
			Шестидесятеричное представление угловых величин
			*/
			typedef  class ASX_EXPORT  sexagesimal_tag
			{
				typedef
				enum class arcdata_type_t
				{
					GRAD = 0,
					RAD = 1
				}asx_arcdata_type_t;

				asx_sexigesimal_arcdata_t		_m_arcdata;			// 
				asx_coordinate_type				_m_coord_type;		//  указывает на склонение-высоту (alt), широту-восхождение (lon)
				bool							_m_is_calculated;	// флаг, полноты вычисления структуры
				
			public:
				/*
				Шестидесятеричное представление угловых величин
				*/
				sexagesimal_tag() noexcept
				{
					//
					// начальная инициализация переменных
					//
					this->_m_arcdata.arcgrad		= double();
					this->_m_arcdata.arcrad			= double();
					this->_m_arcdata.deg_hrs.arcdeg = signed();
					this->_m_arcdata.deg_hrs.archrs = signed();
					this->_m_arcdata.arcmin			= unsigned();
					this->_m_arcdata.arcsec			= double();
					this->_m_is_calculated			= false;
					this->_m_coord_type				= alt_lon_type::UNKNOWN;
				}
			private:
				/*
				Пересчет из натурального числа в угловые величины
				*/
				inline void CDECL _asx_swap_arcdata(
					const double& _in_val,													// значение в градусах, радианах
					const asx_arcdata_type_t _in_arcdata_type,								// флаг: градусы | радианы
					const asx_coordinate_type _in_coordinate_type = alt_lon_type::UNKNOWN	// флаг: склонение  | прямое восхождение
				) noexcept
				{
					_m_is_calculated = false;
					//
					// объявляем переменные
					//
					double arcgrad, arcrad;
					double	sgn;									//знак
					double	mod;									//модуль
					double	intpart;								//целая часть 
					double	frcpart;								//дробная часть

					//
					// выстовляем флаги типа координаты
					switch (_in_coordinate_type)
					{
					case asx_coordinate_type::ALTITUDE:
						_m_coord_type = asx_coordinate_type::ALTITUDE;
						break;
					case asx_coordinate_type::LONGITUDE:
						_m_coord_type = asx_coordinate_type::LONGITUDE;
						break;
					default:
						_m_coord_type = asx_coordinate_type::UNKNOWN;
						break;
					}
					//
					switch (_in_arcdata_type)
					{
					case asx_arcdata_type_t::GRAD:
						arcgrad = _in_val;
						arcrad = TO_RAD(_in_val);
						break;
					default:
						arcgrad = TO_GRAD(_in_val);
						arcrad = _in_val;
						break;
					}
					//
					_m_arcdata.arcgrad = arcgrad;
					_m_arcdata.arcrad = arcrad;
										
					//
					// вычисляем знак
					//
					sgn = arcgrad > 0 ? 1.0 : -1.0;
					//
					// вычисляем модуль
					//
					mod = abs(arcgrad);
					//
					// разделяем число на целую и дробную части
					frcpart = modf(mod, &intpart);
					//
					// вычисляем значение угла и времени
					//
					_m_arcdata.deg_hrs.arcdeg = (int)(intpart * sgn);
					_m_arcdata.deg_hrs.archrs = (int)(intpart / 15) * sgn;
					//
					// вычисляем значение минут
					//
					_m_arcdata.arcmin = (int)(frcpart * 60);
					//
					// обновляем значение дробной части
					//
					//frcpart = (frcpart * 60) - data->arcmin;
					frcpart = (frcpart * 60) - (int)(frcpart * 60); /*fixed 27/05/2018*/
					//
					// вычисляем значение секунд
					//
					_m_arcdata.arcsec = frcpart * 60;
					//
					// устанавливаем флаг вычисления в истину
					//
					_m_is_calculated = true;
				}
				
			public:
				/*
				Возвращает значение угловой величины в градусах дуги (вещественное) 
				*/
				double CDECL get_grad_value(void) const noexcept
				{
						return _m_arcdata.arcgrad;
				}

				/*
				Возвращает значение угловой величины в радианах дуги (вещественное)
				*/
				double CDECL get_radian_value(void) const noexcept
				{
					return _m_arcdata.arcrad;
				}
				double CDECL get_value(const asx_arcdata_type_t arcdata_type) const noexcept
				{
					switch (arcdata_type)
					{
					case asx_arcdata_type_t::GRAD:
						return _m_arcdata.arcgrad;
						break;
					default:
						return _m_arcdata.arcrad;
						break;
					}
				}

				/*
				Возвращает значение угловой величины (строка)
				*/
				std::string CDECL to_string() noexcept
				{
					return "";
					// TODO: n impl
				}
				
				//friend std::ostream& operator<<(std::ostream& os, sexagesimal_tag& sg) noexcept
				//{
					// TODO: n impl
				//	return os;
				//}

				/*
				Присваивает значение выраженное в градусах дуги (вещественное)
				*/
				void CDECL set_grad(
					const double& arcgrad,												// градусы дуги (вещественное)
					const asx_coordinate_type coord_type = asx_coordinate_type::UNKNOWN	// тип координаты
				) noexcept
				{
					_asx_swap_arcdata(arcgrad, asx_arcdata_type_t::GRAD, coord_type);
				}

				/*
				Присваивает значение выраженное в радианах дуги (вещественное)
				*/
				void CDECL set_radian(
					const double& arcrad,												// радианы (вещественное)
					const asx_coordinate_type coord_type = asx_coordinate_type::UNKNOWN	// тип координаты
				) noexcept
				{
					_asx_swap_arcdata(arcrad, asx_arcdata_type_t::RAD, coord_type);
				}

				/*
				Возвращает ссылку на тип координаты
				*/
				const asx_coordinate_type& CDECL get_coordinate_type(void) noexcept
				{
					return _m_coord_type;
				}
			}sexagesimal_t;
		}
	}
}
#pragma warning( pop ) 





