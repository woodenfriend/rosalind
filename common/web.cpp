#include "web.hpp"
#include <curl/curl.h>

//---------------------------------------------------------------------------//
class web::Session::Impl
{
public:
	Impl ( )
	{
		curl_global_init ( CURL_GLOBAL_ALL );
		m_handle = curl_easy_init ( );
		curl_easy_setopt ( m_handle, CURLOPT_VERBOSE, 0 );
		curl_easy_setopt ( m_handle, CURLOPT_NOPROGRESS, 1 );
		curl_easy_setopt ( m_handle, CURLOPT_FOLLOWLOCATION, 1 );
	}

	~Impl ( )
	{
		curl_easy_cleanup ( m_handle);
		curl_global_cleanup ( );
	}

	void getContents ( const char* url, std::ostream& s )
	{
		curl_easy_setopt  ( m_handle, CURLOPT_URL, url );
		curl_easy_setopt  ( m_handle, CURLOPT_WRITEFUNCTION, writeData );
		curl_easy_setopt  ( m_handle, CURLOPT_WRITEDATA, &s );
		curl_easy_perform ( m_handle );
		s.flush ( );
	}

private:
	static size_t writeData ( void* ptr, size_t size, size_t count, void* ext )
	{
		std::ostream* s = reinterpret_cast<std::ostream*> ( ext );
		s->write ( reinterpret_cast<const char*>( ptr ), size * count );
		return count;
	}

	CURL* m_handle;
};

//---------------------------------------------------------------------------//
web::Session::Session ( )
	: m_impl ( std::make_unique<Impl>() )
{
}

web::Session::~Session ( )
{
}

void web::Session::getContents ( const char* url, std::ostream& s )
{
	return m_impl->getContents ( url, s );
}
