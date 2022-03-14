#pragma once

#include <stdexcept>

inline void validate ( bool v, const char* what = "" )
{
	if ( ! v )
		throw std::invalid_argument ( what );
}

inline void check ( bool v, const char* what = "" )
{
	if ( ! v )
		throw std::runtime_error ( what );
}

inline void fail ( const char* what = "" )
{
    throw std::runtime_error ( what );
}
