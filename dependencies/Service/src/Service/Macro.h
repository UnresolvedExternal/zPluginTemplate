#ifndef SERVICE_MACRO
#define SERVICE_MACRO

#if ENGINE == Engine_G1

#define GOTHIC_NAMESPACE Gothic_I_Classic
#define ZENDEF(g1, g1a, g2, g2a) g1

#elif ENGINE == Engine_G1A

#define GOTHIC_NAMESPACE Gothic_I_Addon
#define ZENDEF(g1, g1a, g2, g2a) g1a

#elif ENGINE == Engine_G2

#define GOTHIC_NAMESPACE Gothic_II_Classic
#define ZENDEF(g1, g1a, g2, g2a) g2

#elif ENGINE == Engine_G2A

#define GOTHIC_NAMESPACE Gothic_II_Addon
#define ZENDEF(g1, g1a, g2, g2a) g2a

#endif

#define ZENFOR(g1, g1a, g2, g2a) (STD_ENGINE_CHECKING ? (ZENDEF(g1, g1a, g2, g2a)) : decltype(ZENDEF(g1, g1a, g2, g2a)){})
#define ZTEST(e) STD_ENGINE_CHECKING ? (e) : decltype(e){}

#else

#undef SERVICE_MACRO
#undef ENGINE
#undef GOTHIC_NAMESPACE
#undef ZENDEF
#undef ZENFOR
#undef ZTEST

#endif
