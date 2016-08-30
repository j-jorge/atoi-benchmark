#pragma once

#include <unistd.h>
#include <cstdio>

template <class T>
void do_not_optimize_away(T&& datum)
{
  if (getpid() == 1)
    {
      const void* p = &datum;
      putchar(*static_cast<const char*>(p));
    }
}
