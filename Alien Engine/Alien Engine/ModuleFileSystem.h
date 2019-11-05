#pragma once

#include "Module.h"
#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

struct aiFileIO;
#include "Bass/include/bass.h"
//struct BASS_FILEPROCS;


#define ASSETS_FOLDER "Assets/"
#define LIBRARY_FOLDER "Library/"
#define LIBRARY_MODELS_FOLDER "Library/Models/"
#define LIBRARY_MESHES_FOLDER "Library/Meshes/"
#define LIBRARY_TEXTURES_FOLDER "Library/Textures/"
#define CONFIGURATION_FOLDER "Configuration/"
#define CONFIGURATION_LAYOUTS_FOLDER "Configuration/Layouts/"
#define MODELS_FOLDER "Assets/Models/"
#define TEXTURES_FOLDER "Assets/Textures/"
#define SCRIPTS_FOLDER "Assets/Scripts/"

#include "Resource_.h"

class ResourceTexture;

enum class FileDropType {
	MODEL3D,
	TEXTURE,
	FOLDER,
	SCRIPT,

	UNKNOWN
};

struct FileNode {

	FileNode(){}
	FileNode(std::string name, bool is_file, FileNode* parent);
	FileNode(const std::string& path, const std::string& name, bool is_file, FileNode* parent);
	
public:

	std::string name;
	std::string path;

	bool is_file = true;
	bool is_base_file = false;

	bool changing_name = false;

	FileNode* parent = nullptr;
	std::vector<FileNode*> children;

	ResourceTexture* icon = nullptr;

	FileDropType type = FileDropType::UNKNOWN;

private:

	void SetIcon();

};



class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(const char* game_path = nullptr);

	// Destructor
	~ModuleFileSystem();

	// Called before render is available
	bool Init();

	// Called before quitting
	bool CleanUp() override;

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	void CreateDirectory(const char* directory);
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	void DiscoverEverythig(FileNode* node);
	void DiscoverFolders(FileNode* node);
	bool CopyFromOutsideFS(const char* full_path, const char* destination);
	bool Copy(const char* source, const char* destination);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;
	void NormalizePath(std::string& full_path) const;

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;
	void* BassLoad(const char* file) const;

	// IO interfaces for other libs to handle files via PHYSfs
	aiFileIO* GetAssimpIO();
	BASS_FILEPROCS* GetBassIO();

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool SaveUnique(std::string& output, const void* buffer, uint size, const char* path, const char* prefix, const char* extension);
	bool Remove(const char* file);

	const char* GetBasePath() const;
	const char* GetWritePath() const;
	const char* GetReadPaths() const;

	void ManageNewDropFile(const char* extern_path);

	const FileDropType& SearchExtension(const std::string& extern_path);

	std::string GetBaseFileName(const char* file_name);
	std::string GetBaseFileNameWithExtension(const char* file_name);
	std::string GetCurrentFolder(const std::string & path);
	std::string GetCurrentHolePathFolder(const std::string& path);
	void GetPreviousNames(std::string& previous, FileNode* node);
private:

	void CreateAssimpIO();
	void CreateBassIO();


private:

	aiFileIO* AssimpIO = nullptr;
	BASS_FILEPROCS* BassIO = nullptr;
};

