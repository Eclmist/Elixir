#ifndef __CORE_SYSTEM_ERROR_H__
#define __CORE_SYSTEM_ERROR_H__

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//   ** Use the table below to pick the appropriate log function **                                                                                                                                //
//                                                                                                                                                                                                 //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | Function       | Usage                                 | Shown as popup / Can continue? | Shown in ErrorList         | Comment                                                              | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrTrace       | For advanced debugging purpose,       | No / Yes                       | No, only shown in debugger | Args: LogCategory, Message, ...                                      | //
// |                | only visible to programmers in tty    |                                |                            | Ex: exrTrace(LogAI, "entering function X");                          | //
// |                |                                       |                                |                            | Note: need to change category's minloglevel in code, cmdline or ini  | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrDebug       | For displaying debug messages         | No / Yes                       | Yes but hidden by default  | Args: LogCategory, Message, ...                                      | //
// |                | that are useful to programmers        |                                |                            | Ex: exrDebug(LogAI, "value X = %0.2f", 0.5f);                        | //
// |                | and advanced users                    |                                |                            | Note: your classic debug message                                     | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrInfo        | For displaying important messages     | No / Yes                       | Yes                        | Args: LogCategory, Message, ...                                      | //
// |                | that normal users should not miss     |                                |                            | Ex: exrInfo(LogAI, "Process Y is finished");                         | //
// |                |                                       |                                |                            | Note: info messages are shown in Anvil                               | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrWarning     | For displaying a mild problem to      | No / Yes                       | Yes                        | Args: condition, LogCategory, Role, Message, ...                     | //
// |                | users, something that is suspicious   |                                |                            | Ex: exrWarning(0, LogAI, Strange value X:%d", val);                  | //
// |                |                                       |                                |                            | Note: some categories have an 'Advanced' version                     | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrError       | For displaying a serious problem      | No / Yes (it's not a crash)    | Yes                        | Args: condition, LogCategory, Role, Message, ...                     | //
// |                | something that must be fixed ASAP     |                                |                            | Ex: exrError(0, LogAI, "Huge problem Y:%f", val);                    | //
// |                |                                       |                                |                            | Note: some categories have an 'Advanced' version                     | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrSoftAssert  | For reporting a rare and severe       | Yes / Yes                      | Yes                        | Args: condition, [opt]Message, ...                                   | //
// |                | issue (not fatal but blocking, a      | Note: it will create a Jira    |                            | Ex: exrSoftAssert(0, "NaN detected");                                | //
// |                | message box will be displayed)        |                                |                            | Note: message is optional but useful to have it                      | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
// | exrAssert      | For unrecoverable issues              | Yes via Bloomberg / No         | No, Bloomberg deals with   | Args: condition, [opt]Message, ...                                   | //
// | exrAssertWithID| and irreparably broken states         | Note: it will create a Jira    | crashes on its own         | Ex: exrAssert(0, "fatal situation #42");                             | //
// |                | (fatal)                               |                                |                            | Note: message is optional but useful to have it                      | //
// |                | Each ObjectID will create a new jira  |                                |                            |                                                                      | //
// |----------------|---------------------------------------|--------------------------------|----------------------------|----------------------------------------------------------------------| //
//                                                                                                                                                                                                 //
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#define EXR_ENABLE_ERRORS
#define EXR_ENABLE_LOGGING

#if !defined(EXR_FINAL) && !defined(EXR_PROFILE)
#define EXR_ENABLE_ASSERTS
#endif

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