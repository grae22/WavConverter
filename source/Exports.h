#ifndef EXPORTS_H
#define EXPORTS_H

#ifdef WAVCONVERTER_EXPORTS
#define WAVCONVERTER_API __declspec( dllexport )
#else
#define WAVCONVERTER_API __declspec( dllimport )
#endif

#endif