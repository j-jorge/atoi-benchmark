#include <boost/lexical_cast.hpp>

#include <cstdlib>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_map>

void enforce( bool b )
{
  if ( !b )
    throw std::range_error( "ehm" );
}

template <class T>
void do_not_optimize_away(T&& datum)
{
  if (getpid() == 1)
    {
      const void* p = &datum;
      putchar(*static_cast<const char*>(p));
    }
}

std::uint64_t from_lib( const char* first, const char* last )
{
  char* end;
  return std::strtoul( first, &end, 10 );
}

std::uint64_t from_clib( const char* first, const char* last )
{
  char* end;
  return ::strtoul( first, &end, 10 );
}

std::uint64_t from_boost( const char* first, const char* last )
{
  return boost::lexical_cast< std::uint64_t >( first );
}

std::uint64_t recursive( const char* first, const char* last )
{
  if ( first == last )
    return 0;

  --last;
  enforce( ( *last >= '0' ) && ( *last <= '9' ) );

  return 10 * recursive( first, last ) + ( *last - '0' );
}

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
      enforce( ( *first >= '0' ) && ( *first <= '9' ) );
      result += pow10[ i++ ] * ( *first - '0' );
    }

  return result;
}

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
typedef std::array< std::uint64_t, 20 > time_per_length;

struct bench_result
{
  std::unordered_map< std::string, time_per_length > time;
  time_per_length baseline;
};

template< typename std::uint64_t (*F)( const char* first, const char* last ) >
void run_benchmark( std::uint64_t runs, const char* first, const char* last )
{
#ifndef NDEBUG
  const std::uint64_t ref( from_lib( first, last ) );
  const std::uint64_t tested( F( first, last ) );

  if ( ref != tested )
    {
      std::cerr << "tested function is incorrect: input '" << first
                << "', output " << tested << ", expecting " << ref << '\n';
      abort();
    }
#endif
      
  while( runs != 0 )
    {
      do_not_optimize_away( F( first, last ) );
      --runs;
    }
}

template< typename std::uint64_t (*F)( const char* first, const char* last ) >
time_per_length run_benchmark
( std::uint64_t runs, const std::vector< std::string >& words )
{
  time_per_length result;
  std::fill
    ( result.begin(), result.end(),
      std::numeric_limits< std::uint64_t >::max() );
  
  for ( const std::string& word : words )
    {
      const std::size_t length( word.size() );
      const char* first( word.c_str() );
      const char* last( first + length );

      const auto start( std::chrono::high_resolution_clock::now() );
      run_benchmark< F >( runs, first, last );

      const std::uint64_t duration
        ( ( std::chrono::high_resolution_clock::now() - start ).count() );

      if ( duration < result[ length - 1 ] )
        result[ length - 1 ] = duration;
    }

  return result;
}

bench_result
bench( std::uint64_t runs, const std::vector< std::string >& words )
{
  bench_result result;

  std::cout << "std::strtoul ";
  auto start( std::chrono::high_resolution_clock::now() );
  result.baseline = run_benchmark< &from_lib >( runs, words );
  result.time[ "std::strtoul" ] = result.baseline;
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  std::cout << "strtoul ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "strtoul" ] = run_benchmark< &from_clib >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';

  std::cout << "boost::lexical-cast ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "boost::lexical-cast" ] =
    run_benchmark< &from_boost >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  std::cout << "naive ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "naive" ] = run_benchmark< &naive >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  std::cout << "recursive ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "recursive" ] = run_benchmark< &recursive >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  std::cout << "table-pow ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "table-pow" ] = run_benchmark< &table_pow >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  std::cout << "unrolled-4 ";
  start = std::chrono::high_resolution_clock::now();
  result.time[ "unrolled-4" ] = run_benchmark< &unrolled_4 >( runs, words );
  std::cout << ( std::chrono::high_resolution_clock::now() - start ).count()
            << '\n';
  
  return result;
}

std::vector< std::string > create_integer_strings()
{
  std::vector< std::string > words;
  words.reserve( 200 );
  
  std::random_device rd;
  std::mt19937 generator( rd() );

  std::uint64_t min( 1 );
  for ( std::size_t i( 0 ); i != 20; ++i )
    {
      std::uint64_t max( 10 * min - 1 );
      if ( max < min )
        max = std::numeric_limits< std::uint64_t >::max();
      
      std::uniform_int_distribution< std::uint64_t > random( min, max );

      for ( std::size_t j( 0 ); j != 10; ++j )
        {
          words.push_back
            ( boost::lexical_cast< std::string >( random( generator ) ) );
          assert( words.back().size() == i + 1 );
        }

      min *= 10;
    }

  std::shuffle( words.begin(), words.end(), generator );

  return words;
}

void generate_plot_files( const bench_result& r, std::uint64_t runs )
{
  std::ofstream baseline( "output/baseline.data" );
  static constexpr double scale
    ( double( std::chrono::high_resolution_clock::period::num )
      / std::chrono::high_resolution_clock::period::den );

  for ( std::size_t i( 0 ); i != r.baseline.size(); ++i )
    baseline << ( i + 1 ) << '\t' << r.baseline[ i ] * scale << '\n';
  
  for ( const auto& e : r.time )
    {
      std::ofstream f( "output/atoi-" + e.first + ".data" );

      for ( std::size_t i( 0 ); i != e.second.size(); ++i )
        {
          const std::uint64_t baseline( r.baseline[ i ] );

          if ( baseline != 0 )
            f << ( i + 1 ) << '\t'
              << float( r.baseline[ i ] ) / e.second[ i ]  << '\n';
        }
    }

  std::ofstream f( "output/atoi.plot" );
  f << "set term png linewidth 2 size 2048,1536 font Arial 20\n";
  f << "set output \"atoi-benchmark.png\"\n";
  f << "set xlabel \"#digits\"\n";
  f << "set ylabel \"relative speed\"\n";
  f << "set title \"string to integer (" << runs
    << " runs, baseline=std::strtoul)\"\n";
  f << "set xrange [ 0 : 21 ]\n";
  f << "set key outside center bottom horizontal Left reverse\n";
  f << '\n';

  auto it( r.time.begin() );

  f << "plot \"atoi-" + it->first + ".data\" title \"" + it->first
    + "\" with linespoints ps 2";
  
  for ( ++it; it != r.time.end(); ++it )
    {
      f << ",\\\n    \"atoi-" + it->first + ".data\" title \"" + it->first
        + "\" with linespoints ps 2";
    }
}

int main( int argc, char** argv )
{
  if ( argc != 2 )
    {
      std::cout << "Usage: " << argv[ 0 ] << " iterations\n";
      return 1;
    }

  const std::vector< std::string > words( create_integer_strings() );
  const std::uint64_t runs( boost::lexical_cast< std::uint64_t >( argv[ 1 ] ) );

  generate_plot_files( bench( runs, words ), runs );


  return 0;
}
