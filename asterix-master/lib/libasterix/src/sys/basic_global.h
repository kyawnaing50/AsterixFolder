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
//
// Define global constant & function
//
#define PI								3.14159265358979323846
#define TO_RAD(GRAD) ((double)((GRAD)*(PI))/180)
#define TO_GRAD(RAD) ((double)((RAD)*180)/(PI))
//
// Exports
//
//
// Export function
// 
// Source: Explicitly exporting shared library functions in Linux
// https://stackoverflow.com/questions/2164827/explicitly-exporting-shared-library-functions-in-linux/2164962
//
#if defined(_MSC_VER)
//  Microsoft 
#define ASX_EXPORT __declspec(dllexport)
#define ASX_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#if (__GNUC__ >= 4)
#define ASX_EXPORT __attribute__((visibility("default")))
#define ASX_IMPORT
#else
#define ASX_EXPORT extern "C"
#define ASX_IMPORT
#endif
#else
//  do nothing and hope for the best?
#define ASX_EXPORT
#define ASX_IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif
//
// cdecl definition
//
#if defined __GNUC__
#define CDECL __attribute__((__cdecl__))
#else 
#define CDECL __cdecl
#endif
