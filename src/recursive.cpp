#include "enforce.hpp"
#include "recursive.hpp"

std::uint64_t recursive( const char* first, const char* last )
{
  if ( first == last )
    return 0;

  --last;
  enforce( ( *last >= '0' ) && ( *last <= '9' ) );

  return 10 * recursive( first, last ) + ( *last - '0' );
}

