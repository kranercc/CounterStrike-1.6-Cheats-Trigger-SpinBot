#include <windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h> 
#include <tchar.h>


DWORD pid;



DWORD baseAddress;
char moduleName2[] = "client.dll";
char moduleName[] = "hw.dll";

int valueHealth;

//scan modules find server and copy addrs
DWORD dwGetModuleBaseAddr(TCHAR *lpszModuleName, DWORD64 procid) {
	//temporary holder
	DWORD baseAddrNow = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procid);
	MODULEENTRY32 moduleEntry32 = { 0 };
	moduleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapShot, &moduleEntry32)) {

		do {
			if (_tcscmp(moduleEntry32.szModule, lpszModuleName) == 0) {

				baseAddrNow = (DWORD)moduleEntry32.modBaseAddr;
				break;

			}

		} while (Module32Next(hSnapShot, &moduleEntry32));


	}
	CloseHandle(hSnapShot);
	return baseAddrNow;

}


using namespace std;
int main() {

	HWND hwnd = FindWindowA(0, ("Counter-Strike"));

	// get the pid of the window and place it in pid
	GetWindowThreadProcessId(hwnd, &pid);

	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//get client base addr
	DWORD clientBase = dwGetModuleBaseAddr(_T(moduleName), pid);
	DWORD clientBase2 = dwGetModuleBaseAddr(_T(moduleName2), pid);


	float current;
	float newX = 10.00;
	
	//unitati de milioane sau sute de mii + 112 inainte 1122491136

	int raspuns;

	cout << "1. SpinBot\n2. Trigger: ";
	cin >> raspuns;

	if (raspuns == 1) {
		while (1) {
			newX += 5;
			WriteProcessMemory(pHandle, (LPVOID)(clientBase + 0x10569A8), &newX, sizeof(float), 0);

			if (newX == 360) {
				newX = 0;
			}
			Sleep(1000 / 500);
		}

	}
	DWORD crosshairAddress;

	if (raspuns == 2) {
		while (1) {

			ReadProcessMemory(pHandle, (LPCVOID)(clientBase2 + 0x1211F8), &crosshairAddress, sizeof(crosshairAddress), NULL);



			if (crosshairAddress != 0) {
				cout << "Target: \t" << crosshairAddress << endl;
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(1);
			}
		}

	}
	return 0;

}