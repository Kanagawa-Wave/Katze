#pragma once

#include <iostream>
#include <cstdio>
#include <memory>
#include <functional>
#include <limits>
#include <chrono>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <array>

#include "Logger/Log.h"
#include "Event/Event.h"


#ifdef _PLATFORM_WINDOWS
    #include "Windows.h"
#endif