/******************************************************************************************
 *	Chili Direct3D Engine
 ** Copyright 2018 PlanetChili <http://www.planetchili.net>
 **
 *																						  *
 *	This file is part of Chili Direct3D Engine.
 **
 *																						  *
 *	Chili Direct3D Engine is free software: you can redistribute it and/or
 *modify		  * it under the terms of the GNU General Public License
 *as published by				  * the Free Software
 *Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 *																						  *
 *	The Chili Direct3D Engine is distributed in the hope that it will be
 *useful,		  * but WITHOUT ANY WARRANTY; without even the implied
 *warranty of						  * MERCHANTABILITY or
 *FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 *																						  *
 *	You should have received a copy of the GNU General Public License
 ** along with The Chili Direct3D Engine.  If not, see
 *<http://www.gnu.org/licenses/>.    *
 ******************************************************************************************/
#include "ChiliException.h"
#include <sstream>

ChiliException::ChiliException(int line, const char* file)
  : line(line)
  , file(file)
{
}

const char*
ChiliException::what() const
{
  std::ostringstream oss;
  oss << GetType() << std::endl << GetOriginString();
  whatBuffer = oss.str();
  return whatBuffer.c_str();
}

const char*
ChiliException::GetType() const
{
  return "Chili Exception";
}

int
ChiliException::GetLine() const
{
  return line;
}

const std::string&
ChiliException::GetFile() const
{
  return file;
}

std::string
ChiliException::GetOriginString() const
{
  std::ostringstream oss;
  oss << "[File] " << file << std::endl << "[Line] " << line;
  return oss.str();
}