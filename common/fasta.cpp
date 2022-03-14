#include "fasta.hpp"
#include "error.hpp"

void fasta::readSeries ( std::istream& s, const std::function<void(fasta::Entry&&)>& reader )
{
	fasta::Entry f;
	bool data_open = false;
	while ( ! s.eof() )
	{
		std::string line;
		std::getline ( s, line );
		if ( line.empty() )
			continue;

		if ( line.front() == '>' )
		{
			if ( data_open )
				reader ( std::move( f ) );
			f.label = line.substr ( 1 );
			f.data.clear ( );
			data_open = false;
		}
		else
		{
			f.data += line;
			data_open = true;
		}
	}
	if ( data_open )
		reader ( std::move( f ) );
}

void fasta::readSingleEntry ( std::istream& s, fasta::Entry& f )
{
	bool f_valid = false;
	readSeries ( s,
		[&f, &f_valid]( fasta::Entry&& _f )
		{
			validate ( ! f_valid, "single entry expected" );
			f = std::move ( _f );
			f_valid = true;
		}
	);
	validate ( f_valid, "no entries" );
}
