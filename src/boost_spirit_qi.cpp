#include "boost_spirit_qi.hpp"

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_uint.hpp>

std::uint64_t boost_spirit_qi( const char* first, const char* last )
{
  std::uint64_t result;
  boost::spirit::qi::parse
    ( first, last, boost::spirit::qi::uint_, result );
  return result;
}
