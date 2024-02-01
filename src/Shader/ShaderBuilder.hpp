#pragma once

#include <vector>
#include <shaderc/shaderc.hpp>
#include <string>

class ShaderBuilder
{
public:
	static void compile(const std::string ShaderName, shaderc_shader_kind Kind, const std::string Filepath, bool Optimize);
};