#include "ShaderStorage.hpp"


ShaderStorage* ShaderStorage::Singleton = nullptr;
std::mutex ShaderStorage::Mutex;

ShaderStorage* ShaderStorage::getShaderStorage()
{
	std::lock_guard<std::mutex> Lock(Mutex);

	if (Singleton == nullptr)
		Singleton = new ShaderStorage();

	return Singleton;
}

void ShaderStorage::storeShader(std::string ShaderName, std::vector<uint32_t> ShaderCode)
{
	if (Shaders.contains(ShaderName))
	{
		std::runtime_error("Try to rewrite shader" + ShaderName);
	}

	Shaders.insert({ ShaderName, ShaderCode });
}

const std::vector<uint32_t>& ShaderStorage::getShader(std::string ShaderName)
{
	if (Shaders.contains(ShaderName))
	{
		return Shaders[ShaderName];
	}

	std::runtime_error("Try to access shader ({}) that doesn't stored " + ShaderName);
}

ShaderStorage::ShaderStorage()
{
}

ShaderStorage::~ShaderStorage()
{
}