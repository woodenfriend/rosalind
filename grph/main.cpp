#include "../common/error.hpp"
#include "../common/fasta.hpp"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

template<int K>
class Graph
{
public:
	void add ( fasta::Entry&& f )
	{
		m_nodes.emplace_back ( std::move( f ) );
	}

	void showNodes ( std::ostream& s )
	{
		for ( auto& i : m_nodes )
			s << std::string( i.begin, i.begin + K ) << "..." << std::string( i.end, i.end + K ) << " (" << i.label << ")" << std::endl;
	}

	void buildEdges ( )
	{
		for ( auto i1 = m_nodes.begin(), i_end = m_nodes.end(); i1 != i_end; ++i1 )
			for ( auto i2 = i1; i2 != i_end; ++i2 )
				if ( i1 != i2 )
				{
					if ( i1->lmatch( *i2 ) )
						m_edges.emplace_back ( *i1, *i2 );
					if ( i1->rmatch( *i2 ) )
						m_edges.emplace_back ( *i2, *i1 );
				}
}

	void showEdges ( std::ostream& s )
	{
		for ( const auto& i : m_edges )
			s << i.from->label << " " << i.to->label << std::endl;
	}

private:
	struct Node
	{
		Node ( const fasta::Entry& f )
			: label ( f.label )
		{
			check ( f.data.size() >= 2 * K );
			f.data.copy ( begin, 3 );
			f.data.copy ( end, 3, f.data.size() - K );
		}

		bool lmatch ( const Node& n ) const
		{
			return std::equal ( end, end + K, n.begin );
		}

		bool rmatch ( const Node& n ) const
		{
			return std::equal ( begin, begin + K, n.end );
		}

		std::string label;
		char begin[K];
		char end[K];
	};

	struct Edge
	{
		Edge ( Node& _from, Node& _to ) : from( &_from ), to( &_to ) { }
		Node* from;
		Node* to;
	};

	std::list<Node> m_nodes;
	std::vector<Edge> m_edges;
};

void run ( )
{
	Graph<3> graph;
	fasta::readSeries ( std::cin, [&graph]( fasta::Entry&& f ){ graph.add( std::move( f ) ); } );

//	graph.showNodes ( std::cout );
	graph.buildEdges ( );
	graph.showEdges ( std::cout );
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

