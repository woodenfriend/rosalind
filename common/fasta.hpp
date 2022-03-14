#pragma once

#include <string>
#include <istream>
#include <functional>

/*
FASTA is text format used for naming genetic strings in databases.
Every string in a FASTA file begins with a single-line that contains the symbol '>' along with some labeling information about the string.
The word following the '>' symbol is the identifier of the sequence, and the rest of the line is its description (both are optional).
There should be no space between the ">" and the first letter of the identifier.
All subsequent lines contain the string itself; it is recommended that all lines of text are shorter than 80 symbols.
The string ends when another line starting with '>' appears, indicating the start of another sequence (or if the end of the file is reached).
*/

namespace fasta {

struct Entry
{
	std::string label;
	std::string data;
};

void readSeries ( std::istream& s, const std::function<void(Entry&&)>& reader );
void readSingleEntry ( std::istream& s, Entry& f );

} // namespace fasta
