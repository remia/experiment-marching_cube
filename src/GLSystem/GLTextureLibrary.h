#pragma once

#include "GLTexture.h"

#include <map>
#include <string>
#include <memory>

typedef std::string IdT;
typedef std::shared_ptr<GLTexture> GLTextureT;
typedef std::map<IdT, GLTextureT> GLTextureLibrary;
