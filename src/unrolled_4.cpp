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
      const auto d_0( unsigned( first[ 0 ] ) - '0' );
      enforce( d_0 < 10 );
      const std::uint64_t r1( pow10[ i++ ] * d_0 );

      const auto d_1( unsigned( first[ 1 ] ) - '0' );
      enforce( d_1 < 10 );
      const std::uint64_t r2( pow10[ i++ ] * d_1 );
      
      const auto d_2( unsigned( first[ 2 ] ) - '0' );
      enforce( d_2 < 10 );
      const std::uint64_t r3( pow10[ i++ ] * d_2 );
      
      const auto d_3( unsigned( first[ 3 ] ) - '0' );
      enforce( d_3 < 10 );
      const std::uint64_t r4( pow10[ i++ ] * d_3 );

      result += r1 + r2 + r3 + r4;
    }

  for( ; first != last; ++first )
    {
      const auto d( unsigned( first[ 0 ] ) - '0' );
      enforce( d < 10 );
      result += pow10[ i++ ] * d;
    }

  return result;
}
