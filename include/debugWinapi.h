#include <Windows.h>

#include <sstream>
/*
 * TRACE(msg) - a convenience macro for sending debug output
 * directly to the Visual Studio "Output" window (or any debugger
 * attached to the process) using the WinAPI function OutputDebugStringA.
 *
 * How it works:
 * 1. Creates a temporary std::ostringstream (string stream) - works like
 * std::cout but stores the output in memory as a string.
 * 2. Writes whatever is passed via 'msg' into that stream
 *    (you can chain multiple << operations just like with std::cout).
 * 3. Converts the collected data to a C-style string (c_str()).
 * 4. Passes it to OutputDebugStringA, which sends the string
 *    to the debugger's output pane.
 *
 * Why use it:
 *  - Great for printing variables, states, and messages while debugging
 *    without requiring a console window.
 *  - Only visible when a debugger is attached - it won't appear for end users.
 *
 * Example:
 *    TRACE("x = " << x << ", y = " << y << "\n");
 */
#define TRACE(msg)                        \
  {                                       \
    std::ostringstream os;                \
    os << msg;                            \
    OutputDebugStringA(os.str().c_str()); \
  }