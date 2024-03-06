#pragma once

#ifdef UF0_DBG
namespace {
  static void DBG_SETUP(unsigned long baud){
    UF0_DBG.begin(baud);
    while (!UF0_DBG);
  }

  template <typename T>
  static void DBG_PLAIN(T last){
    UF0_DBG.println(last);
  }

  template <typename T, typename... Args>
  static void DBG_PLAIN(T head, Args... tail){
    UF0_DBG.print(head);
    UF0_DBG.print(' ');
    DBG_PLAIN(tail...);
  }

  template <typename... Args>
  static void DBG(Args... args){
    DBG_PLAIN(args...);
  }
}

#else
#define DBG_SETUP(...)
#define DBG_PLAIN(...)
#define DBG(...)
#endif