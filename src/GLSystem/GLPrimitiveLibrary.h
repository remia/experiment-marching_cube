#pragma once

#include "GLPrimitive3D.h"

#include <map>
#include <string>
#include <memory>

typedef std::string IdT;
typedef std::shared_ptr<GLPrimitive3D> GLPrimitiveT;
typedef std::map<IdT, GLPrimitiveT> GLPrimitiveLibrary;
