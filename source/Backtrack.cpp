#include "../include/ECS/Backtrack.hpp"

#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace ECS
{
    #ifdef ARCHETYPE_DEBUG
    #ifdef _MSC_VER
    // Thanks ChatGPT :'>
    void ARCHETYPE_API printStackTrace()
    {
        DWORD dwSize = 63;
        // Initialize the symbol handler
        SymInitialize(GetCurrentProcess(), NULL, TRUE);

        // Get the current stack frame
        CONTEXT context;
        RtlCaptureContext(&context);

        // Initialize the stack frame
        STACKFRAME64 stackFrame;
        memset(&stackFrame, 0, sizeof(STACKFRAME64));
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Mode = AddrModeFlat;
        stackFrame.AddrPC.Offset = context.Rip;
        stackFrame.AddrFrame.Offset = context.Rbp;
        stackFrame.AddrStack.Offset = context.Rsp;

        // Print the call stack
        std::vector<void*> addresses;
        addresses.reserve(dwSize);
        for (DWORD i = 0; i < dwSize; ++i) {
            // Advance to the next stack frame
            if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
                break;
            }

            // Add the function address to the list
            addresses.push_back((void*)stackFrame.AddrPC.Offset);
        }

        // Enumerate the symbols for each address
        for (void* address : addresses) {
            DWORD64 baseAddress = SymGetModuleBase64(GetCurrentProcess(), (DWORD64)address);
            if (baseAddress == 0) {
                std::cout << address << ": <unknown>" << std::endl;
                continue;
            }

            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
            PSYMBOL_INFO pSymInfo = (PSYMBOL_INFO)buffer;
            pSymInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymInfo->MaxNameLen = MAX_SYM_NAME;
            if (SymFromAddr(GetCurrentProcess(), (DWORD64)address, NULL, pSymInfo)) {
                std::cout << address << ": " << pSymInfo->Name << std::endl;
            }
            else {
                std::cout << address << ": <unknown>" << std::endl;
            }
        }

        // Cleanup the symbol handler
        SymCleanup(GetCurrentProcess());
    }
    #endif // _MSC_VER
    #ifndef _MSC_VER
    void ARCHETYPE_API printStackTrace()
    {
        std::cout << "This feature is only available on MSVC\n";
    }
    #endif // _MSC_VER
    #endif // ARCHETYPE_DEBUG
}
