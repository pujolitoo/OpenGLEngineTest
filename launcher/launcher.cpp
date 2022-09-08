#include "launcher.h"

#include <iostream>
#include <string>

 
typedef int (__cdecl *MYPROC)();

static bool console = false;
static bool utf8 = true;


bool ParseCommandLine(int* argc, LPWSTR* argv)
{
    if(*argc<=1){
        return false;
    }

    int offset = 0;
    while(offset<=(*argc))
    {

        if(lstrcmpW(argv[offset], L"--console")){
            console = true;
        }else if(lstrcmpW(argv[offset], L"--disable-utf-8")){
            LOG("UTF-8 disabled.");
            utf8 = false;
        }

        offset++;
    }

    return true;
}

void PrintArgs(int* argc, LPWSTR* argv)
{
    if(*argc<=1){
        return;
    }

    int offset = 0;
    while(offset<(*argc))
    {
        std::wcout << std::wstring(argv[offset]) << std::endl;

        offset++;
    }
}


bool IsRunning()
{
    const char* procName = "launcher.exe";
    char moduleName[_MAX_PATH] = {0};
    GetModuleFileNameA(GetModuleHandle(NULL), moduleName, sizeof(moduleName));
    std::string entryProc;
    PROCESSENTRY32 pEntry;
    HANDLE hSnapshot;
    bool isRunning = false;
    int pCount = 0;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pEntry.dwSize = sizeof(PROCESSENTRY32);

    if(hSnapshot!=NULL)
    {
        Process32First(hSnapshot, &pEntry);
        entryProc = std::string((char*)pEntry.szExeFile);
        if(!entryProc.compare(procName))
        {
            pCount++;
            if(pCount>=2) isRunning = true;
        }        
        while(Process32Next(hSnapshot, &pEntry)){
            entryProc = std::string((char*)pEntry.szExeFile);
            if(!entryProc.compare(procName))
            {
                pCount++;
                if(pCount>=2){ isRunning = true; break; };
            }        
        }
    }else
    {
        LOG("Error snapshot.");
        CloseHandle(hSnapshot);
        exit(EXIT_FAILURE);
    }

    CloseHandle(hSnapshot);
    return isRunning;
}

int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
) 
{ 

    //Global vars

    HINSTANCE hinstLib; 
    MYPROC ProcAdd; 
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
    int argc;
    LPWSTR* argv;
    int rt;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if(argv!=NULL)
    {
        std::cout << argc << std::endl;
        PrintArgs(&argc, argv);
        ParseCommandLine(&argc, argv);
    }

    if(utf8)
    {
        SetConsoleOutputCP(CP_UTF8);
    }


    if(IsRunning())
    {
        MessageBox(NULL, TEXT("Another instance of this process is already open."), TEXT("Error"), MB_ICONERROR);
        return EXIT_FAILURE;
    }

    LOG("Lélé");
    
 
    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary(TEXT("engine.dll")); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
        ProcAdd = (MYPROC) GetProcAddress(hinstLib, "mainfunc"); 
 
        // If the function address is valid, call the function.
 
        if (NULL != ProcAdd) 
        {
            fRunTimeLinkSuccess = TRUE;
            rt = (ProcAdd) (); 
        }
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    } 

    // If unable to call the DLL function, use an alternative.
    if (! fRunTimeLinkSuccess) 
        return -1;


    return rt;

}