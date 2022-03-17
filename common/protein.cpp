#include "error.hpp"
#include "protein.hpp"
#include <map>
#include <initializer_list>
#include <boost/container/static_vector.hpp>

//---------------------------------------------------------------------------//
namespace
{
	class IBuilder
	{
	public:
		using Codons = std::initializer_list<const char*>;
		virtual void add ( AA aa, char sym, Codons cs ) = 0;
	};

	void buildDB ( IBuilder& builder, bool start = true, bool stop = true )
	{
		builder.add ( AA::Ala, 'A', { "GCT", "GCC", "GCA", "GCG" } );
		builder.add ( AA::Arg, 'R', { "CGT", "CGC", "CGA", "CGG", "AGA", "AGG" } );
		builder.add ( AA::Asn, 'N', { "AAT", "AAC" } );
		builder.add ( AA::Asp, 'D', { "GAT", "GAC" } );
		builder.add ( AA::Cys, 'C', { "TGT", "TGC" } );
		builder.add ( AA::Gln, 'Q', { "CAA", "CAG" } );
		builder.add ( AA::Glu, 'E', { "GAA", "GAG" } );
		builder.add ( AA::Gly, 'G', { "GGT", "GGC", "GGA", "GGG" } );
		builder.add ( AA::His, 'H', { "CAT", "CAC" } );
		builder.add ( AA::Ile, 'I', { "ATT", "ATC", "ATA" } );
		builder.add ( AA::Leu, 'L', { "TTA", "TTG", "CTT", "CTC", "CTA", "CTG" } );
		builder.add ( AA::Lys, 'K', { "AAA", "AAG" } );
		builder.add ( AA::Met, 'M', { "ATG" } );
		builder.add ( AA::Phe, 'F', { "TTT", "TTC" } );
		builder.add ( AA::Pro, 'P', { "CCT", "CCC", "CCA", "CCG" } );
		builder.add ( AA::Ser, 'S', { "TCT", "TCC", "TCA", "TCG", "AGT", "AGC" } );
		builder.add ( AA::Thr, 'T', { "ACT", "ACC", "ACA", "ACG" } );
		builder.add ( AA::Trp, 'W', { "TGG" } );
		builder.add ( AA::Tyr, 'Y', { "TAT", "TAC" } );
		builder.add ( AA::Val, 'V', { "GTT", "GTC", "GTA", "GTG" } );

		if ( start )
			builder.add ( AA::START, '[', { "ATG" } );
		if ( stop )
			builder.add ( AA::STOP, ']', { "TAA", "TGA", "TAG" } );
	}
}

//---------------------------------------------------------------------------//
char aa2symbol ( AA aa )
{
	static const auto m =
		[]( )
		{
			struct Builder
				: public IBuilder
			{
				virtual void add ( AA aa, char sym, Codons ) override { m.emplace( aa, sym ); }
				std::map<AA, char> m;
			} builder;
			buildDB ( builder );
			return std::move ( builder.m );
		} ( );

	auto f = m.find ( aa );
	check ( f != m.end(), "bad aminoacid" );
	return f->second;
}

AA symbol2aa ( char sym )
{
	static const auto m =
		[]( )
		{
			struct Builder
				: public IBuilder
			{
				virtual void add ( AA aa, char sym, Codons ) override { m.emplace( sym, aa ); }
				std::map<char, AA> m;
			} builder;
			buildDB ( builder );
			return std::move ( builder.m );
		} ( );
	auto f = m.find ( sym );
	check ( f != m.end(), "bad symbol" );
	return f->second;
}

AA codon2aa ( const Codon& c )
{
	static const auto m =
		[]( )
		{
			struct Builder
				: public IBuilder
			{
				virtual void add ( AA aa, char, Codons cs ) override
				{
					for ( const auto& c : cs )
						check ( m.emplace( c, aa ).second, "invalid dataset" );
				}
				std::map<Codon, AA> m;
			} builder;
			buildDB ( builder, false, false );
			return std::move ( builder.m );
		} ( );
	auto f = m.find ( c );
	check ( f != m.end(), "c2aa bad argument" );
	return f->second;
}

boost::iterator_range<const Codon*> aa2codon ( AA aa )
{
	static const auto m =
		[]( )
		{
			struct Builder
				: public IBuilder
			{
				virtual void add ( AA aa, char, Codons cs ) override
				{
					auto& aacs = m[aa];
					check ( aacs.empty(), "invalid dataset" );
					for ( const auto& c : cs )
						aacs.emplace_back ( c );
				}
				std::map<AA, boost::container::static_vector<Codon, 6>> m;
			} builder;
			buildDB ( builder );
			return std::move ( builder.m );
		} ( );
	auto f = m.find ( aa );
	check ( f != m.end(), "aa2c bad argument" );
	return { f->second.data(), f->second.data() + f->second.size() };
}
