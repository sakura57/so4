#pragma once

#define _WINSOCKAPI_

#if defined(_MSC_VER)
#	if defined(_DEBUG)
#		pragma comment(lib, "sfml-system-d.lib")
#		pragma comment(lib, "sfml-window-d.lib")
#		pragma comment(lib, "sfml-graphics-d.lib")
#		pragma comment(lib, "sfml-audio-d.lib")
#	else
#		pragma comment(lib, "sfml-system.lib")
#		pragma comment(lib, "sfml-window.lib")
#		pragma comment(lib, "sfml-graphics.lib")
#		pragma comment(lib, "sfml-audio.lib")
#	endif
#endif

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>