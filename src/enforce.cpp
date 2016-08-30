#include <stdexcept>

void enforce( bool b )
{
  if ( !b )
    throw std::range_error( "ehm" );
}
