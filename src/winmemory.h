#pragma once

#include <string>
#include <windows.h>
#include <psapi.h>

namespace WindowsMemory
{
	typedef uint32_t memloc;

	class MemoryHandler
	{
	private:
		HANDLE pHandle;

	public:
		MemoryHandler(const char *className, const char *windowName);
		MemoryHandler(HWND hWindow);
		void close();

		memloc getBaseAddress(const char *moduleName);
		
		uint32_t readUint32(memloc address);
		float readFloat(memloc address);
		uint8_t *readBytes(memloc address, size_t dataSize);
		
		void writeUint32(memloc address, uint32_t data);
		void writeFloat(memloc address, float data);
		void writeBytes(memloc address, uint8_t data[], size_t dataSize);

	private:
		void openProcessHandle(HWND hWindow);
		template <typename T> T *read(memloc address, size_t dataSize);
		void write(memloc address, void *data, size_t dataSize);
	};
}
