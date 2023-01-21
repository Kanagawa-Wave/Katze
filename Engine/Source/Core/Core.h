#pragma once

#include "Input.h"
#include "Logger/Log.h"
#include "Timer.h"
#include "Timestep.h"

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)