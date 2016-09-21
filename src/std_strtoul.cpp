#include "std_strtoul.hpp"

#include <cstdlib>

std::uint64_t std_strtoul( const char* first, const char* last )
{
  char* end;
  return std::strtoull( first, &end, 10 );
}
