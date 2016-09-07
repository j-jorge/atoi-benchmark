#include "enforce.hpp"
#include "table_pow.hpp"

std::uint64_t table_pow( const char* first, const char* last )
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
  
  for( std::uint64_t i( 20 - ( last - first ) ); first != last; ++first )
    {
      const auto d( unsigned( *first ) - '0' );
      enforce( d < 10 );
      result += pow10[ i++ ] * d;
    }

  return result;
}
