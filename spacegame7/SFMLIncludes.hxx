/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
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