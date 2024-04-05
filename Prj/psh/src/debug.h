#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#undef DEBUG
#define DEBUG(X) {X;}
#else
#define DEBUG(X) {}
#endif

#define PRINT(X) {std::cout << X ;}
#define ERRPRINT(X) {std::cerr << X ;}
#define __LOCATION__ "[" __FILE__ ":" << __LINE__ << "]"

#endif // DEBUG_H
