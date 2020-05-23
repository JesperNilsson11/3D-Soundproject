// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#ifndef UNICODE
#define UNICODE
#endif 

// add headers that you want to pre-compile here
#include <Windows.h>
#include <xaudio2.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <cmath>
#include <unordered_map>
#include <string>
#include <chrono>
#include <vector>
#include <memory>

namespace dx = DirectX;

const std::string assets("..\\Assets\\");

#endif //PCH_H
