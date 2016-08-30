#include "boost_lexical_cast.hpp"

#include <boost/lexical_cast.hpp>

std::uint64_t boost_lexical_cast( const char* first, const char* last )
{
  return boost::lexical_cast< std::uint64_t >( first );
}
