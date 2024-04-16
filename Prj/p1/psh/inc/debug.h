/**
 * @author Qucheng Jiang / Shihan Zhao
 * @author jiang.qu@northeastern.edu / zhao.shiha@northeastern.edu
 * @author NUID 001569593 / 002772845
 */

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
