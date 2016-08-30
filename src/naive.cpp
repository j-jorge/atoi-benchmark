#include "enforce.hpp"
#include "naive.hpp"

std::uint64_t naive( const char* first, const char* last )
{
  std::uint64_t result( 0 );

  for( ; first != last; ++first )
    {
      enforce( ( *first >= '0' ) && ( *first <= '9' ) );
      result = result * 10 + ( *first - '0' );
    }

  return result;
}
