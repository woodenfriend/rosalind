#include "../common/error.hpp"
#include "../common/fasta.hpp"
#include <iostream>
#include <set>
#include <algorithm>
#include <string_view>

class Collector
{
public:
	void add ( fasta::Entry&& f )
	{
		if ( m_s_count == 0 )
			m_s1 = std::move ( f.data );
		else if ( m_s_count == 1 )
		{
			m_s2 = std::move ( f.data );
			findCandidates ( );
		}
		else
			testCandidates ( f.data );
		++m_s_count;
	}

	std::string_view findLCS ( ) const
	{
		std::string_view lcs;
		size_t max_len = lcs.size();
		for ( const auto& cd : m_cds )
			if ( cd.size() > max_len )
			{
				max_len = cd.size ( );
				lcs = cd;
			}
		return lcs;
	}

private:
	void findCandidates ( )
	{
		if ( m_s1.size() > m_s2.size() )
			std::swap ( m_s1, m_s2 );
		for ( size_t i1 = 0; i1 < m_s1.size(); ++i1 )
			for ( size_t i2 = 0; i2 < m_s2.size(); ++i2)
			{
				i2 = m_s2.find ( m_s1[i1], i2 );
				if ( i2 == std::string::npos )
					break;

				size_t len = 1;
				for ( size_t max_len = std::min( m_s1.size()-i1, m_s2.size()-i2 ); len < max_len; ++len )
					if ( m_s1[i1+len] != m_s2[i2+len] )
						break;

				m_cds.emplace ( m_s1.data() + i1, len );
			}
	}

	void testCandidates ( const std::string& s )
	{
		for ( auto i = m_cds.begin(); i != m_cds.end(); )
			if ( s.find( *i ) == std::string::npos )
				i = m_cds.erase ( i );
			else
				++i;
	}

	int m_s_count = 0;
	std::string m_s1, m_s2;
	std::set<std::string_view> m_cds;
};

void run ( )
{
	Collector c;
	fasta::readSeries ( std::cin, [&c]( fasta::Entry&& f ){ c.add( std::move( f ) ); } );
	std::cout << c.findLCS() << std::endl;
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
