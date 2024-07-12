#include <windows.h>
#include <stdio.h>
#include <winsvc.h> // For SERVICE_DESCRIPTION and ChangeServiceConfig2

#define SERVICE_NAME L"PreventHibernateService"
#define SERVICE_DISPLAY_NAME L"Prevent Hibernate Service"

// Define SERVICE_DESCRIPTION as a variable
WCHAR szServiceDescription[] = L"This service prevents the system from entering hibernate mode and reboots instead.";

int wmain(int argc, wchar_t* argv[]) {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (hSCManager == NULL) {
        wprintf(L"OpenSCManager failed. Error: %ld\n", GetLastError());
        return 1;
    }

    SC_HANDLE hService = CreateServiceW(
        hSCManager,
        SERVICE_NAME,
        SERVICE_DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        L"C:\\Users\\Сultist\\AppData\\Local\\SystemHib\\HookHibernate.exe", // Replace with your executable path
        NULL, NULL, NULL, NULL, NULL);

    if (hService == NULL) {
        wprintf(L"CreateService failed. Error: %ld\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return 1;
    }

    // Set service description
    SERVICE_DESCRIPTION sd;
    sd.lpDescription = szServiceDescription; // Assign the pointer to the buffer

    if (!ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd)) {
        wprintf(L"ChangeServiceConfig2 failed to set description. Error: %ld\n", GetLastError());
    }
    else {
        wprintf(L"Service installed successfully.\n");
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return 0;
}