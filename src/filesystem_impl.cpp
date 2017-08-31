/*
filesystem_impl.cpp - xash filesystem_stdio
Copyright (C) 2016-2017 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/
#include "filesystem.h"
#define XASH_DEDICATED
#include "engine/common/common.h"
#include "engine/common/filesystem.h"

class CXashFileSystem : public IFileSystem
{
public:
	void Mount( void );
	void Unmount( void );

	void RemoveAllSearchPaths( void );

	void AddSearchPath( const char *pPath, const char *pathID );
	bool RemoveSearchPath( const char *pPath );

	void RemoveFile( const char *pRelativePath, const char *pathID );

	void CreateDirHierarchy( const char *path, const char *pathID );

	bool FileExists( const char *pFileName );
	bool IsDirectory( const char *pFileName );

	FileHandle_t Open( const char *pFileName, const char *pOptions, const char *pathIDL );
	void Close( FileHandle_t file );

	void Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType );
	unsigned int Tell( FileHandle_t file );

	unsigned int Size( FileHandle_t file );
	unsigned int Size( const char *pFileName );

	long GetFileTime( const char *pFileName );
	void FileTimeToString( char* pStrip, int maxCharsIncludingTerminator, long fileTime );

	bool IsOk( FileHandle_t file );

	void Flush( FileHandle_t file );
	bool EndOfFile( FileHandle_t file );

	int	  Read( void* pOutput, int size, FileHandle_t file );
	int	  Write( void const* pInput, int size, FileHandle_t file );
	char* ReadLine( char *pOutput, int maxChars, FileHandle_t file );
	int   FPrintf( FileHandle_t file, const char *pFormat, ... );

	void* GetReadBuffer( FileHandle_t file, int *outBufferSize, bool failIfNotInCache );
	void  ReleaseReadBuffer( FileHandle_t file, void *readBuffer );

	const char* FindFirst( const char *pWildCard, FileFindHandle_t *pHandle, const char *pathIDL );
	const char* FindNext( FileFindHandle_t handle );
	bool        FindIsDirectory( FileFindHandle_t handle );
	void        FindClose( FileFindHandle_t handle );

	void        GetLocalCopy( const char *pFileName );

	const char* GetLocalPath( const char *pFileName, char *pLocalPath, int localPathBufferSize );

	char*       ParseFile( char* pFileBytes, char* pToken, bool* pWasQuoted );

	bool FullPathToRelativePath( const char *pFullpath, char *pRelative );

	bool GetCurrentDirectory( char* pDirectory, int maxlen );

	void PrintOpenedFiles( void );

	void SetWarningFunc( void (*pfnWarning)( const char *fmt, ... ) );
	void SetWarningLevel( FileWarningLevel_t level );

	void LogLevelLoadStarted( const char *name );
	void LogLevelLoadFinished( const char *name );
	int HintResourceNeed( const char *hintlist, int forgetEverything );
	int PauseResourcePreloading( void );
	int	ResumeResourcePreloading( void );
	int	SetVBuf( FileHandle_t stream, char *buffer, int mode, long size );
	void GetInterfaceVersion( char *p, int maxlen );
	bool IsFileImmediatelyAvailable(const char *pFileName);

	WaitForResourcesHandle_t WaitForResources( const char *resourcelist );

	bool GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */ , bool *complete /* out */ );

	void CancelWaitForResources( WaitForResourcesHandle_t handle );

	bool IsAppReadyForOfflinePlay( int appID );

	bool AddPackFile( const char *fullpath, const char *pathID );

	FileHandle_t OpenFromCacheForRead( const char *pFileName, const char *pOptions, const char *pathIDL );

	void AddSearchPathNoWrite( const char *pPath, const char *pathID );

	CXashFileSystem();

private:
	bool IsGameDir( const char *pathID );


	bool m_bMounted;
};

// =====================================
// interface singletons
static CXashFileSystem fs;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CXashFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION, fs )

CXashFileSystem *XashFileSystem( void )
{
	return &fs;
}

IFileSystem *filesystem( void )
{
	return (IFileSystem *)XashFileSystem();
}
// =====================================
// interface implementation

#define STUBCALL( format, ... ) printf( "FS_Stdio_Xash: called a stub: %s  ->(" format ")\n" , __PRETTY_FUNCTION__, __VA_ARGS__ );
#define STUBCALL_VOID			printf( "FS_Stdio_Xash: called a stub: %s  ->(void)\n", __PRETTY_FUNCTION__ );
#define LOGCALL( format, ... )	printf( "FS_Stdio_Xash: called %s     ->(" format ")\n" , __PRETTY_FUNCTION__, __VA_ARGS__ )
#define LOGCALL_VOID			printf( "FS_Stdio_Xash: called %s     ->(void)\n", __PRETTY_FUNCTION__ );

#define LOGRETVAL( format, ret ) printf( "FS_Stdio_Xash:             \-> " format "\n", ret );

void CXashFileSystem::Mount()
{
	LOGCALL_VOID;
	m_bMounted = true;
}

void CXashFileSystem::Unmount()
{
	LOGCALL_VOID;
	m_bMounted = false;
}

void CXashFileSystem::RemoveAllSearchPaths( void )
{
	STUBCALL_VOID;
}

void CXashFileSystem::AddSearchPath(const char *pPath, const char *pathID)
{
	STUBCALL("%s,%s", pPath, pathID );;
}

bool CXashFileSystem::RemoveSearchPath(const char *pPath)
{
	STUBCALL("%s", pPath);
	return false;
}

void CXashFileSystem::RemoveFile(const char *pRelativePath, const char *pathID)
{
	STUBCALL( "%s, %s", pRelativePath, pathID );
}

void CXashFileSystem::CreateDirHierarchy(const char *path, const char *pathID)
{
	char *pPath = strdup(path);
	FS_CreatePath(pPath);

	free(pPath);
}

bool CXashFileSystem::FileExists(const char *pFileName)
{
	return FS_FileExists( pFileName, false );
}

bool CXashFileSystem::IsDirectory(const char *pFileName)
{
	struct stat buf;
	if( stat( pFileName, &buf ) != -1 )
		return S_ISDIR( buf.st_mode );
	return false;
}

FileHandle_t CXashFileSystem::Open(const char *pFileName, const char *pOptions, const char *pathID)
{
	return FS_Open( pFileName, pOptions, IsGameDir( pathID ));
}

void CXashFileSystem::Close( FileHandle_t file )
{
	FS_Close( (file_t*)file );
}

void CXashFileSystem::Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType )
{
	FS_Seek( (file_t*)file, pos, seekType );
}

unsigned int CXashFileSystem::Tell(FileHandle_t file)
{
	return FS_Tell( (file_t*)file );
}

unsigned int CXashFileSystem::Size(FileHandle_t file)
{
	fs_offset_t orig = FS_Tell((file_t*)file);

	FS_Seek( (file_t*)file, 0, SEEK_END );
	fs_offset_t size = FS_Tell( (file_t*)file );
	FS_Seek( (file_t*)file, orig, SEEK_SET );

	return size;
}

unsigned int CXashFileSystem::Size(const char *pFileName)
{
	return FS_FileSize( pFileName, false );
}

long CXashFileSystem::GetFileTime(const char *pFileName)
{
	return FS_FileTime( pFileName, false );
}

void CXashFileSystem::FileTimeToString(char *pStrip, int maxCharsIncludingTerminator, long fileTime)
{
	STUBCALL("out, %i, %li", maxCharsIncludingTerminator, fileTime );
	pStrip[0] = 0;
}

bool CXashFileSystem::IsOk(FileHandle_t file)
{
	file_t *nativeFile = (file_t*)file;
	if( !file )
	{
		MsgDev( D_WARN, "Tried to IsOk NULL");
		return false;
	}

	return true;
}

void CXashFileSystem::Flush(FileHandle_t file)
{
	Seek( file, 0, FILESYSTEM_SEEK_HEAD );
}

bool CXashFileSystem::EndOfFile(FileHandle_t file)
{
	return FS_Eof( (file_t*) file );
}

int CXashFileSystem::Read( void *pOutput, int size, FileHandle_t file )
{
	return FS_Read( (file_t*)file, pOutput, size );
}

int CXashFileSystem::Write(const void *pInput, int size, FileHandle_t file)
{
	return FS_Write( (file_t*)file, pInput, size );
}

char *CXashFileSystem::ReadLine(char *pOutput, int maxChars, FileHandle_t file)
{
	file_t *nativeFile = (file_t*)file;

	if( FS_Eof( nativeFile ) )
		return NULL;

	char *orig = pOutput;
	for( int i = 0; i < maxChars && !FS_Eof( nativeFile ); i++, pOutput++ )
	{
		FS_Read( nativeFile, pOutput, 1 );

		if( *pOutput == '\n' )
			return orig;
	}

	return orig;
}

int CXashFileSystem::FPrintf(FileHandle_t file, const char *pFormat, ...)
{
	int	result;
	va_list	args;

	va_start( args, pFormat );
	result = FS_VPrintf( (file_t*)file, pFormat, args );
	va_end( args );

	return result;
}

void *CXashFileSystem::GetReadBuffer(FileHandle_t file, int *outBufferSize, bool failIfNotInCache)
{
	// FS_LoadFile?
	STUBCALL_VOID;
	return NULL;
}

void CXashFileSystem::ReleaseReadBuffer(FileHandle_t file, void *readBuffer)
{
	// FS_CloseFile?
	STUBCALL_VOID
	return;
}

struct findData_t
{
	search_t *search;
	int iter;
};

const char *CXashFileSystem::FindFirst(const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID)
{
	if( !pHandle )
		return NULL;

	findData_t *ptr = new findData_t;
	if( pWildCard[0] == '/' ) pWildCard++;
	ptr->search = FS_Search( pWildCard, false, IsGameDir( pathID) );

	if( !ptr->search )
	{
		delete ptr;
		return NULL;
	}
	ptr->iter = 0;

	*pHandle = (int)ptr; // I am so sorry
	return FindNext( *pHandle );
}

const char *CXashFileSystem::FindNext(FileFindHandle_t handle)
{
	findData_t *ptr = (findData_t *)handle;
	return ptr->search->filenames[ptr->iter++];
}

bool CXashFileSystem::FindIsDirectory(FileFindHandle_t handle)
{
	findData_t *ptr = (findData_t *)handle;
	return IsDirectory( ptr->search->filenames[ptr->iter] );
}

void CXashFileSystem::FindClose(FileFindHandle_t handle)
{
	findData_t *ptr = (findData_t *)handle;

	if( ptr->search ) Mem_Free( ptr->search );
	delete ptr;
	return;
}

void CXashFileSystem::GetLocalCopy(const char *pFileName)
{
	STUBCALL("%s", pFileName );
	return;
}

const char* CXashFileSystem::GetLocalPath(const char *pFileName, char *pLocalPath, int localPathBufferSize)
{
	// Is it an absolute path?
#ifdef _WIN32
	if ( strchr( pFileName, ':' ) )
#else
	if ( pFileName && pFileName[0] == '/' )
#endif
	{
		strncpy( pLocalPath, pFileName, localPathBufferSize );
		pLocalPath[localPathBufferSize-1] = 0;

		COM_FixSlashes( pLocalPath );
		return pLocalPath;
	}

	const char *diskPath = FS_GetDiskPath( pFileName, false );

	if( diskPath )
	{
		strncpy( pLocalPath, diskPath, localPathBufferSize );
		pLocalPath[localPathBufferSize-1] = 0;

		return pLocalPath;
	}
	return NULL;
}

char *CXashFileSystem::ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted)
{
	STUBCALL_VOID; // nothing safe to read
	return 0;
}

bool CXashFileSystem::FullPathToRelativePath(const char *pFullpath, char *pRelative)
{
	bool success = false;

	if( !pFullpath[0] )
	{
		pRelative[0] = 0;
		return success;
	}

	char *fullpath = realpath( pFullpath, NULL );
	searchpath_t *sp = FS_GetSearchPaths();
	char *real = NULL;

	for( sp; sp; sp = sp->next )
	{
		if( sp->wad || sp->pack )
			continue;

		real = realpath( sp->filename, 0 );

		if( !real )
			continue; // is this possible?

		if( !strncmp( fullpath, real, strlen( real )))
		{
			break;
		}
		else
		{
			free( real );
			real = NULL;
		}
	}

	if( real )
	{
		int slen = strlen( real );

		int i;
		for( i = 0; i < slen; i++ )
		{
			if( real[i] != fullpath[i] )
				break;
		}

		strcpy( pRelative, fullpath + i + 1 );

		free( real );
	}

	free( fullpath );

	return true;
}

bool CXashFileSystem::GetCurrentDirectory(char *pDirectory, int maxlen)
{
	STUBCALL_VOID;
	return false;
}

void CXashFileSystem::PrintOpenedFiles()
{
	STUBCALL_VOID;
}

void CXashFileSystem::SetWarningFunc(void (*pfnWarning)(const char *, ...))
{
	STUBCALL_VOID;
}

void CXashFileSystem::SetWarningLevel(FileWarningLevel_t level)
{
	STUBCALL("%i", level);
}

void CXashFileSystem::LogLevelLoadStarted(const char *name)
{
	STUBCALL("%s", name);
}

void CXashFileSystem::LogLevelLoadFinished(const char *name)
{
	STUBCALL("%s",name);
}

int CXashFileSystem::HintResourceNeed(const char *hintlist, int forgetEverything)
{
	STUBCALL("%s, %i", hintlist, forgetEverything );
	return 0;
}

int CXashFileSystem::PauseResourcePreloading()
{
	STUBCALL_VOID;
	return 0;
}

int CXashFileSystem::ResumeResourcePreloading()
{
	STUBCALL_VOID;
	return 0;
}

int CXashFileSystem::SetVBuf(FileHandle_t stream, char *buffer, int mode, long size)
{
	STUBCALL("%p", stream);
	return 0;
}

void CXashFileSystem::GetInterfaceVersion(char *p, int maxlen)
{
	STUBCALL_VOID;
}

bool CXashFileSystem::IsFileImmediatelyAvailable(const char *pFileName)
{
	STUBCALL("%s", pFileName);
	return false;
}

WaitForResourcesHandle_t CXashFileSystem::WaitForResources(const char *resourcelist)
{
	STUBCALL("%s", resourcelist);
	return 0;
}

bool CXashFileSystem::GetWaitForResourcesProgress(WaitForResourcesHandle_t handle, float *progress, bool *complete)
{
	STUBCALL_VOID;
	return false;
}

void CXashFileSystem::CancelWaitForResources(WaitForResourcesHandle_t handle)
{
	STUBCALL_VOID;
	return;
}

bool CXashFileSystem::IsAppReadyForOfflinePlay(int appID)
{
	STUBCALL("%i", appID);
	return true;
}

bool CXashFileSystem::AddPackFile(const char *fullpath, const char *pathID)
{
	STUBCALL("%s, %s", fullpath, pathID );
	return false;
}

FileHandle_t CXashFileSystem::OpenFromCacheForRead(const char *pFileName, const char *pOptions, const char *pathID)
{
	STUBCALL( "%s, %s, %s", pFileName, pOptions, pathID );
	return NULL;
}

void CXashFileSystem::AddSearchPathNoWrite(const char *pPath, const char *pathID)
{
	STUBCALL("%s, %s", pPath, pathID)
	return;
}


// =====================================
// private

CXashFileSystem::CXashFileSystem()
{
	m_bMounted = false;
}

bool CXashFileSystem::IsGameDir(const char *pathID)
{
	bool gamedironly = false;

	if( pathID && (strstr( pathID, "GAME" ) || strstr( pathID, "BASE" )))
		gamedironly = true;

	return gamedironly;
}





