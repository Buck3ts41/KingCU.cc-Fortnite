bool rape = false;


#include "Utils/utils.hpp"

void extinit();
void init();



bool __stdcall DllMain(HMODULE module, ULONG reason, LPVOID reserved)
{
    if (reason == 1)
    {
         LI_FN(CreateThread).forwarded_safe_cached()(NULL, NULL, (LPTHREAD_START_ROUTINE)init, NULL, NULL, NULL);

        return TRUE;
    }
    return TRUE;
}

