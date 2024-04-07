#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#undef DEBUG
#define DEBUG(X) {X;}
#else
#define DEBUG(X) {}
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define PRINT(X) {std::cout << X ;}
#define ERRPRINT(X) {std::cerr << X ;}

#define __LOCATION__ "[" << __FILE__ << ":" << std::to_string(__LINE__) << "]"

#endif // DEBUG_H
