#include "enforce.hpp"
#include "unrolled_4.hpp"

std::uint64_t unrolled_4( const char* first, const char* last )
{
  static const std::uint64_t pow10[ 20 ] =
    {
      10000000000000000000ull,
      1000000000000000000ull,
      100000000000000000ull,
      10000000000000000ull,
      1000000000000000ull,
      100000000000000ull,
      10000000000000ull,
      1000000000000ull,
      100000000000ull,
      10000000000ull,
      1000000000ull,
      100000000ull,
      10000000ull,
      1000000ull,
      100000ull,
      10000ull,
      1000ull,
      100ull,
      10ull,
      1ull
    };
  std::uint64_t result( 0 );
  std::uint64_t i( 20 - ( last - first ) );

  for( ; last - first >= 4; first += 4 )
    {
      enforce( ( first[ 0 ] >= '0' ) && ( first[ 0 ] <= '9' ) );
      const std::uint64_t r1( pow10[ i++ ] * ( first[ 0 ] - '0' ) );

      enforce( ( first[ 1 ] >= '0' ) && ( first[ 1 ] <= '9' ) );
      const std::uint64_t r2( pow10[ i++ ] * ( first[ 1 ] - '0' ) );
      
      enforce( ( first[ 2 ] >= '0' ) && ( first[ 2 ] <= '9' ) );
      const std::uint64_t r3( pow10[ i++ ] * ( first[ 2 ] - '0' ) );
      
      enforce( ( first[ 3 ] >= '0' ) && ( first[ 3 ] <= '9' ) );
      const std::uint64_t r4( pow10[ i++ ] * ( first[ 3 ] - '0' ) );

      result += r1 + r2 + r3 + r4;
    }

  for( ; first != last; ++first )
    {
      enforce( ( *first >= '0' ) && ( *first <= '9' ) );
      result += pow10[ i++ ] * ( *first - '0' );
    }

  return result;
}
