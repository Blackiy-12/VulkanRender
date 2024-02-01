#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <mutex>

class ShaderStorage
{
public:
	static ShaderStorage* getShaderStorage();

	ShaderStorage(ShaderStorage& other) = delete;
	void operator=(const ShaderStorage&) = delete;

	void storeShader(std::string ShaderName, std::vector<uint32_t> ShaderCode);

	const std::vector<uint32_t>& getShader(std::string ShaderName);

private:
	std::unordered_map<std::string, const std::vector<uint32_t>> Shaders{};

	ShaderStorage();
	~ShaderStorage();

	static ShaderStorage* Singleton;
	static std::mutex Mutex;
};

