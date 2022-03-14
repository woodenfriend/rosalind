#include "../common/fasta.hpp"
#include "../common/web.hpp"
#include <sstream>
#include <iostream>
#include <vector>
#include <regex>

class Session
{
public:
	Session ( )
		: m_regex ( "N[^P][ST][^P]" )
	{
	}

	void processProtein ( const char* name )
	{
		fasta::Entry f;
		readProteinFasta ( name, f );

		if ( auto positions = findMotif( f.data ); ! positions.empty() )
		{
			std::cout << name << std::endl;
			bool first = true;
			for ( int pos : positions )
			{
				if ( ! first )
					std::cout << ' ';
				std::cout << ( pos + 1 );
				first = false;
			}
			std::cout << std::endl;
		}
	}

private:
	void readProteinFasta ( const char* name, fasta::Entry& f )
	{
		std::ostringstream os;
		os << "http://www.uniprot.org/uniprot/" << name << ".fasta";

		std::stringstream ios;
		m_session.getContents ( os.str().c_str(), ios );

		fasta::readSingleEntry ( ios, f );
	}

	std::vector<int> findMotif ( const std::string& s )
	{
		std::vector<int> positions;

		std::smatch m;
		auto it = s.begin(), it_end = s.end();
		size_t shift = 0;
		while ( std::regex_search( it, it_end, m, m_regex ) )
		{
			positions.push_back ( m.position() + shift );
			shift += m.position() + 1;
			it += m.position() + 1;
		}

		return positions;
	}

	web::Session m_session;
	std::regex m_regex;
};

void run ( )
{
	Session s;
	while ( ! std::cin.eof() )
	{
		std::string name;
		std::getline ( std::cin, name );
		if ( name.empty() )
			continue;

		s.processProtein ( name.c_str() );
	}
}

int main ( )
{
	try
	{
		run ( );
	}
	catch ( std::exception& ex )
	{
		std::cout << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
