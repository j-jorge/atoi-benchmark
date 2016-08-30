#include "c_strtoul.hpp"

#include <cstdlib>

std::uint64_t c_strtoul( const char* first, const char* last )
{
  char* end;
  return ::strtoul( first, &end, 10 );
}
