#include <iostream>
#include <sstream>
#include <vector>
#include <string>

void generate ( const std::vector<char>& a, size_t pos, std::string& s )
{
	auto next_pos = pos + 1;
	bool last_pos = next_pos == s.length ( );
	for ( char c : a )
	{
		s[pos] = c;
		if ( last_pos )
			std::cout << s << std::endl;
		else
			generate ( a, next_pos, s );
	}
}

void run ( )
{
	std::vector<char> a;
	{
		std::string line;
		std::getline ( std::cin, line );

		a.reserve ( ( line.length() + 1 ) / 2 );
		for ( char c : line )
			if ( c != ' ' )
				a.push_back ( c );
	}

	size_t n;
	{
		std::string line;
		std::getline ( std::cin, line );
		n = std::stoul ( line );
	}

	std::string s ( n, ' ' );
	generate ( a, 0, s );
}

int main ( )
{
	try
	{
		run ( );
	}
	catch ( std::exception& s )
	{
		std::cout << "error: " << s.what() << std::endl;
		return 1;
	}

    return 0;
}
