#ifndef __CORE_SYSTEM_ERROR_H__
#define __CORE_SYSTEM_ERROR_H__

#ifdef EXR_ENABLE_LOGGING
#define exrDebugLine(msg)                           std::cout << "[Debug] " << msg << '\n';
#define exrDebug(msg)                               std::cout << "[Debug] " << msg;
#define exrInfoLine(msg)                            std::cout << "[Info] " << msg << '\n';
#define exrInfo(msg)                                std::cout << "[Info] " << msg;
#else
#define exrDebug(msg)                               ((void)0)
#define exrInfo(msg)                                ((void)0)
#endif

#ifdef EXR_ENABLE_ASSERTS
#include <cassert>
#define exrSoftAssert(condition, msg)               std::cerr << "Soft assert failed: " #condition << " " << msg << std::endl;
#define exrAssert(condition, msg)                   assert((msg, condition))
#define exrStaticAssertMsg(condition, msg)          static_assert(condition, "static_assert: " msg)
#else
#define exrSoftAssert(condition, msg)               ((void)0)
#define exrAssert(condition, msg)                   ((void)0)
#define exrStaticAssertMsg(condition, msg)          ((void)0)
#endif


#endif // !__CORE_SYSTEM_ERROR_H__