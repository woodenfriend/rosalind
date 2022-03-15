#include "../common/error.hpp"
#include "../common/fasta.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <optional>

class Profile
{
public:
	void add ( const fasta::Entry& s )
	{
		if ( ! m_length )
			m_length = s.data.size();
		else
			validate ( *m_length == s.data.size(), "DNA strings must be the same length" );

		for ( size_t i = 0; i < *m_length; ++i )
		{
			auto& counts = m_profile[s.data[i]];
			if ( counts.empty() )
				counts.assign ( *m_length, 0 );
			++counts[i];
		}
	}

	void show_consensus ( std::ostream& s )
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

	void show_profile ( std::ostream& s )
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

	profile.show_consensus ( std::cout );
	profile.show_profile ( std::cout );
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

