#include "ShaderBuilder.hpp"

#include <Shader/ShaderStorage.hpp>

#include <fstream>

void ShaderBuilder::compile(const std::string ShaderName, shaderc_shader_kind Kind, const std::string Filepath, bool Optimize)
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions Options;
    if (Optimize)
    {
        Options.SetOptimizationLevel(shaderc_optimization_level_size);
    }

    std::ifstream file(Filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        std::runtime_error("Can open file" + Filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();


    shaderc::SpvCompilationResult Module = compiler.CompileGlslToSpv(buffer.data(), buffer.size(), Kind, ShaderName.c_str(), Options);
    if (Module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        std::runtime_error("Error compiling module" + Module.GetErrorMessage());
    }

    ShaderStorage::getShaderStorage()->storeShader(ShaderName, std::vector<uint32_t>{ Module.cbegin(), Module.cend() });
}
