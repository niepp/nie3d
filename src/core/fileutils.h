#pragma once

#include "unzip/unzip.h"

class CORE_API FileUtils
{
public:
	static FileUtils* Instance();

	// pSize is bufsize in Byte
	inline Int8* ReadTextFile(const char *pszFileName, Uint32 *pSize);
	inline Uint8* ReadBinaryFile(const char *pszFileName, Uint32 *pSize);

	inline Bool WriteTextFile(const char *pszFileName, const Int8 *pData, Uint32 pSize);
	inline Bool WriteBinaryFile(const char *pszFileName, const Uint8 *pData, Uint32 pSize);

	inline void SetResourceRoot(const char *res_root) {
		m_res_root = res_root;
	}

	inline const Filepath& GetResourceRoot() const { return m_res_root; }

protected:
	FileUtils() : m_res_root("") { }

private:
	// m_res_root is the apk path in android platform, all file path is related to m_res_root
	Filepath m_res_root;
};

Int8* FileUtils::ReadTextFile(const char *pszFileName, Uint32 *pSize)
{
	Int8 *pBuffer = NULL;
	assert(pszFileName != NULL && pSize != NULL);

#if PLATFORM == PLATFORM_WIN32 || PLATFORM == PLATFORM_IOS
	Filepath path = m_res_root.AsString() + pszFileName;
	*pSize = 0;
	do {
		// read the file from hardware
		FILE *fp = fopen(path.AsCharPtr(), "r");
		if(!fp) { break; }

		fseek(fp,0,SEEK_END);
		*pSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		pBuffer = new Int8[*pSize + 1];
		memset(pBuffer, 0, *pSize + 1);
		*pSize = fread(pBuffer, sizeof(Int8), *pSize, fp);
		pBuffer[*pSize] = '\0';  // add '\0' at text end
		fclose(fp);
	} while (0);
#elif PLATFORM == PLATFORM_ANDROID
	do {
		unzFile pFile = unzOpen(m_res_root.AsCharPtr());

		if (!pFile) { break; }

		String filePath("assets/");
		filePath += pszFileName;
		int nRet = unzLocateFile(pFile, filePath.AsCharPtr(), 1);
		if (nRet != UNZ_OK) { break; }

		char szFilePathA[MAX_PATH_LEN];
		unz_file_info FileInfo;
		nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
		if (nRet != UNZ_OK) { break; }

		nRet = unzOpenCurrentFile(pFile);
		if (nRet != UNZ_OK) { break; }

		pBuffer = new Int8[FileInfo.uncompressed_size + 1];
		memset(pBuffer, 0, FileInfo.uncompressed_size + 1);
		int nSize = 0;
		nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
		
		assert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size);

		*pSize = FileInfo.uncompressed_size;
		pBuffer[*pSize] = '\0';

		unzCloseCurrentFile(pFile);
		
		if (pFile) {
			unzClose(pFile);
		}

	} while (0);
#endif
	return pBuffer;
}

Uint8* FileUtils::ReadBinaryFile(const char *pszFileName, Uint32 *pSize)
{
	Uint8 *pBuffer = NULL;
	assert(pszFileName != NULL && pSize != NULL);

#if PLATFORM == PLATFORM_WIN32 || PLATFORM == PLATFORM_IOS
	Filepath path = m_res_root.AsString() + pszFileName;
	*pSize = 0;
	do {
		// read the file from hardware
		FILE *fp = fopen(path.AsCharPtr(), "rb");
		if(!fp) {
			break;
		}

		fseek(fp,0,SEEK_END);
		*pSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		pBuffer = new Uint8[*pSize];
		*pSize = fread(pBuffer, sizeof(Uint8), *pSize, fp);
		fclose(fp);
	} while (0);
#elif PLATFORM == PLATFORM_ANDROID
	do {
		unzFile pFile = unzOpen(m_res_root.AsCharPtr());
		if (!pFile) { break; }

		String filePath("assets/");
		filePath += pszFileName;
		int nRet = unzLocateFile(pFile, filePath.AsCharPtr(), 1);
		if (nRet != UNZ_OK) { break; }

		char szFilePathA[MAX_PATH_LEN];
		unz_file_info FileInfo;
		nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
		if (nRet != UNZ_OK) { break; }

		nRet = unzOpenCurrentFile(pFile);
		if (nRet != UNZ_OK) { break; }

		pBuffer = new Uint8[FileInfo.uncompressed_size];
		int nSize = 0;
		nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);

		assert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size);

		*pSize = FileInfo.uncompressed_size;
		unzCloseCurrentFile(pFile);

		if (pFile) {
			unzClose(pFile);
		}

	} while (0);
#endif
	return pBuffer;
}

Bool FileUtils::WriteTextFile(const char *pszFileName, const Int8 *pData, Uint32 pSize)
{
	assert(pszFileName != NULL && pData != NULL);
	Filepath path = m_res_root.AsString() + pszFileName;
	FILE *fp = ::fopen(path.AsCharPtr(), "w");
	if (!fp) {
		return false;
	}

	fwrite(pData, sizeof(Int8), pSize, fp);
	fclose(fp);
	return true;
}

Bool FileUtils::WriteBinaryFile(const char *pszFileName, const Uint8 *pData, Uint32 pSize)
{
	assert(pszFileName != NULL && pData != NULL);
	Filepath path = m_res_root.AsString() + pszFileName;
	FILE *fp = ::fopen(path.AsCharPtr(), "wb");
	if (!fp) {
		return false;
	}

	fwrite(pData, sizeof(Uint8), pSize, fp);
	fclose(fp);
	return true;
}

