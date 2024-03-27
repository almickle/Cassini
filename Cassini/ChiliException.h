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
#pragma once
#include <exception>
#include <string>

class ChiliException : public std::exception
{
public:
  ChiliException(int line, const char* file);
  const char* what() const override;
  virtual const char* GetType() const;
  int GetLine() const;
  const std::string& GetFile() const;
  std::string GetOriginString() const;

private:
  int line;
  std::string file;

protected:
  mutable std::string whatBuffer;
};