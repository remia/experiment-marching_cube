#pragma once

#include <map>
#include <string>
#include <memory>

namespace GLRenderer
{
	class Actor;
	class Mesh;
	class Texture;
	class ShaderProgram;

	typedef std::string IdT;

	// actor library
	typedef std::shared_ptr<Actor> ActorT;
	typedef std::map<IdT, ActorT> ActorLib;

	// mesh library
	typedef std::shared_ptr<Mesh> MeshT;
	typedef std::map<IdT, MeshT> MeshLib;

	// texture library
	typedef std::shared_ptr<Texture> TextureT;
	typedef std::map<IdT, TextureT> TextureLib;

	// shader library
	typedef std::shared_ptr<ShaderProgram> ShaderProgramT;
	typedef std::map<IdT, ShaderProgramT> ShaderProgramLib;
}
