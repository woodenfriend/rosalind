#pragma once

#include <istream>
#include <ostream>
#include <cstdint>
#include <boost/range.hpp>

//---------------------------------------------------------------------------//
struct Codon
{
	Codon ( const char c[3] ) : str{ c[0], c[1], c[2], 0 } { }

	union
	{
		char n[3];
		char str[4];
		std::uint32_t i;
	};
};
inline bool operator<  ( const Codon& lhs, const Codon& rhs ) { return lhs.i < rhs.i; }
inline bool operator== ( const Codon& lhs, const Codon& rhs ) { return lhs.i == rhs.i; }

//---------------------------------------------------------------------------//
enum class AA : char
{
	Ala, Arg, Asn, Asp, Cys,
	Gln, Glu, Gly, His, Ile,
	Leu, Lys, Met, Phe, Pro,
	Ser, Thr, Trp, Tyr, Val,
	START,
	STOP
};

//---------------------------------------------------------------------------//
char aa2symbol ( AA aa );
AA symbol2aa ( char sym );
AA codon2aa ( const Codon& c );
boost::iterator_range<const Codon*> aa2codon ( AA aa );
