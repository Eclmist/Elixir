#pragma once

#include <string>

exrBEGIN_NAMESPACE

template <class T> class Vector3;
template <class T> class Point;
class Matrix4x4;

typedef std::string             exrString;
typedef Vector3<float>          exrVector3;
typedef Point<float>            exrPoint;
typedef bool                    exrBool;
typedef float                   exrFloat;
typedef float                   exrF32;
typedef double                  exrF64;
typedef unsigned char           exrByte;
typedef unsigned char           exrChar;
typedef unsigned short          exrU16;
typedef short                   exrS16;
typedef unsigned int            exrU32;
typedef int                     exrS32;
typedef unsigned long           exrU64;
typedef long                    exrS64;

exrEND_NAMESPACE
