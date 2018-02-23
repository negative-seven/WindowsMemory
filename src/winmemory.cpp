#include "winmemory.h"

namespace WindowsMemory
{
	MemoryHandler::MemoryHandler(const char *className, const char *windowName)
	{
		HWND hWindow = FindWindow(className, windowName);

		if (!hWindow)
		{
			throw std::runtime_error("Could not find window.");
		}

		openProcessHandle(hWindow);
	}

	MemoryHandler::MemoryHandler(HWND hWindow)
	{
		if (!hWindow)
		{
			throw std::runtime_error("Invalid handle.");
		}

		openProcessHandle(hWindow);
	}

	void MemoryHandler::close()
	{
		CloseHandle(pHandle);
	}

	uint32_t MemoryHandler::getBaseAddress(const char *moduleName)
	{
		HMODULE hModules[1024];
		DWORD cbNeeded;
		TCHAR modulePath[MAX_PATH];

		if (EnumProcessModulesEx(pHandle, hModules, 1024, &cbNeeded, LIST_MODULES_ALL))
		{
			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				if (GetModuleFileNameEx(pHandle, hModules[i], modulePath, sizeof(modulePath) / sizeof(TCHAR)))
				{
					if (strstr(modulePath, moduleName) != NULL)
					{
						return (uint32_t)hModules[i];
					}
				}
			}
		}

		throw std::runtime_error("Module not found.");
	}

	uint32_t MemoryHandler::readUint32(uint32_t address)
	{
		uint32_t *dataPtr = read<uint32_t>(address, sizeof(uint32_t));
		uint32_t data = *dataPtr;
		delete dataPtr;
		return data;
	}

	float MemoryHandler::readFloat(uint32_t address)
	{
		float *dataPtr = read<float>(address, sizeof(float));
		float data = *dataPtr;
		delete dataPtr;
		return data;
	}

	uint8_t *MemoryHandler::readBytes(uint32_t address, unsigned int dataSize)
	{
		uint8_t *value = read<uint8_t>(address, dataSize);
		return value;
	}

	void MemoryHandler::writeUint32(uint32_t address, uint32_t data)
	{
		write(address, &data, sizeof(uint32_t));
	}

	void MemoryHandler::writeFloat(uint32_t address, float data)
	{
		write(address, &data, sizeof(float));
	}

	void MemoryHandler::writeBytes(uint32_t address, uint8_t data[], unsigned int dataSize)
	{
		write(address, data, dataSize);
	}

	void MemoryHandler::openProcessHandle(HWND hWindow)
	{
		DWORD pId;
		GetWindowThreadProcessId(hWindow, &pId);
		pHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, false, pId);

		if (!pHandle)
		{
			throw std::runtime_error("Could not open handle for process.");
		}
	}

	template <typename T>
	T *MemoryHandler::read(uint32_t address, uint32_t dataSize)
	{
		T *value = new T[dataSize / sizeof(T)];

		if (!ReadProcessMemory(pHandle, (LPVOID)address, value, dataSize, NULL))
		{
			throw std::runtime_error("Could not read from memory.");
		}

		return value;
	}
	
	void MemoryHandler::write(uint32_t address, void *data, uint32_t dataSize)
	{
		if (!WriteProcessMemory(pHandle, (LPVOID)address, data, dataSize, NULL))
		{
			throw std::runtime_error("Could not write to memory.");
		}
	}
}
