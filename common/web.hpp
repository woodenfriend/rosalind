#pragma once

#include <ostream>
#include <memory>

namespace web {

class Session
{
public:
	Session ( );
	~Session ( );

	void getContents ( const char* url, std::ostream& s );

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

} // namespace web
