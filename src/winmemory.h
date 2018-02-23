#pragma once

#include <string>
#include <windows.h>
#include <psapi.h>

namespace WindowsMemory
{
	class MemoryHandler
	{
	private:
		HANDLE pHandle;

	public:
		MemoryHandler(const char *className, const char *windowName);
		MemoryHandler(HWND hWindow);
		void close();

		uint32_t getBaseAddress(const char *moduleName);
		
		uint32_t readUint32(uint32_t address);
		float readFloat(uint32_t address);
		uint8_t *readBytes(uint32_t address, unsigned int dataSize);
		
		void writeUint32(uint32_t address, uint32_t data);
		void writeFloat(uint32_t address, float data);
		void writeBytes(uint32_t address, uint8_t data[], unsigned int dataSize);

	private:
		void openProcessHandle(HWND hWindow);
		template <typename T> T *read(uint32_t address, uint32_t dataSize);
		void write(uint32_t address, void *data, uint32_t dataSize);
	};
}