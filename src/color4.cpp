// Copyright (C) 2008 Roland Schilter <rolansch@ethz.ch>
// This file is part of SuperHud Editor.
//
// SuperHud Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SuperHud Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SuperHud Editor.  If not, see <http://www.gnu.org/licenses/>.
#include "color4.h"
#include "misc.h"

wxString Color4::to_string() const
{
  switch( type )
  {
  case COLOR_RGBA:
    return she::pretty_print_float( r ) + wxT(" ") +
      she::pretty_print_float( g ) + wxT(" ") +
      she::pretty_print_float( b ) + wxT(" ") +
      she::pretty_print_float( a );
  
  case COLOR_T:
    return wxT("T");

  case COLOR_E:
    return wxT("E");
  }
  return wxT("<error>");
}

wxTextOutputStream& operator<<( wxTextOutputStream& stream, const Color4& c )
{ // pretty print Color4.
  
  stream << c.to_string();
  return stream;
}

#include <wx/sstream.h>
#include <wx/txtstrm.h>

bool Color4::from_string( const wxString& str )
{
  float _r, _g, _b, _a;

  wxStringInputStream sis(str);
  wxTextInputStream tis(sis);

  // FIXME use wxTextInputStream
  /*
  _r = tis.ReadDouble();
  _g = tis.ReadDouble();
  _b = tis.ReadDouble();
  _a = tis.ReadDouble();
  */
  if( 4 != sscanf( str.mb_str(), "%f %f %f %f", &_r, &_g, &_b, &_a ) )
  {
    char special;
    if( 1 != sscanf( str.mb_str(), "%c", &special ) )
      return false;
    else
    {
      if( toupper(special) == 'T' )
        type = COLOR_T;
      else if( toupper(special) == 'E' )
        type = COLOR_E;
      else
        return false;

      return true;
    }
  }
  r = _r;
  g = _g;
  b = _b;
  a = _a;
  type = COLOR_RGBA;
  wxcolisvalid = false;
  return true;
}
