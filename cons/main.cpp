#include "../common/error.hpp"
#include "../common/fasta.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <optional>

class Profile
{
public:
	void add ( const fasta::Entry& f )
	{
		if ( ! m_length )
			m_length = f.data.size();
		else
			validate ( *m_length == f.data.size(), "DNA strings must be the same length" );

		for ( size_t i = 0; i < *m_length; ++i )
		{
			auto& counts = m_profile[f.data[i]];
			if ( counts.empty() )
				counts.assign ( *m_length, 0 );
			++counts[i];
		}
	}

	void showConsensus ( std::ostream& s )
	{
		for ( size_t i = 0; i < *m_length; ++i )
		{
			char key;
			int max_value = -1;
			for ( const auto& j : m_profile )
			{
				int value = j.second[i];
				if ( value > max_value )
				{
					max_value = value;
					key = j.first;
				}
			}
			s << key;
		}
		s << std::endl;
	}

	void showProfile ( std::ostream& s )
	{
		for ( const auto& i : m_profile )
		{
			s << i.first << ":";
			for ( auto j : i.second )
				s << " " << j;
			s << std::endl;
		}
	}

private:
	std::optional<size_t> m_length;
	std::map<char, std::vector<int> > m_profile;
};

void run ( )
{
	Profile profile;
	fasta::readSeries ( std::cin, [&profile]( fasta::Entry&& f ){ profile.add( f ); } );

	profile.showConsensus ( std::cout );
	profile.showProfile ( std::cout );
}

int main ( )
{
	try
	{
		run ( );
	}
	catch ( const std::exception& s )
	{
		std::cout << "error: " << s.what() << std::endl;
		return 1;
	}
	return 0;
}

