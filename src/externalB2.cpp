#include <iostream>
#include <windows.h>
#include <vector>
#include "proc.h"

int main()
{
	DWORD procId = GetProcId(L"Borderlands2.exe");

	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"Borderlands2.exe");

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x015F7F6C;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	std::vector <unsigned int> coinOffsets = { 0xB14, 0x180, 0x128, 0x2A0 };
	uintptr_t coinAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, coinOffsets);

	std::cout << "coinAddr = " << "0x" << std::hex << coinAddr << std::endl;

	int coinValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)coinAddr, &coinValue, sizeof(coinValue), nullptr);

	std::cout << "Current coin = " << std::dec << coinValue << std::endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_F2))
		{
			std::cout << "Cheat was quit" << std::endl;
			break;
		}
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			int newCoin = 1000000000;
			WriteProcessMemory(hProcess, (BYTE*)coinAddr, &newCoin, sizeof(newCoin), nullptr);
			ReadProcessMemory(hProcess, (BYTE*)coinAddr, &coinValue, sizeof(coinValue), nullptr);

			std::cout << "Current coin = " << std::dec << coinValue << std::endl;
		}
		Sleep(10);
	}

	return 0;
}