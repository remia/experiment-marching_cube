#pragma once

#include "GLDrawable.h"

#include <map>
#include <string>
#include <memory>

typedef std::string IdT;
typedef std::shared_ptr<GLDrawable> GLEntityT;
typedef std::map<IdT, GLEntityT> GLPrimitiveLibrary;
