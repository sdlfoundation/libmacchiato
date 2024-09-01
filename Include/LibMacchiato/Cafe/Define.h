#pragma once

#include "../Utils/Bind.h"

#include <sdl-utils/Types.h>

#define CAFE_FUNCTION(name, res, ...) inline res (*dyn_##name)(__VA_ARGS__)
