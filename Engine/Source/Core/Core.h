#pragma once

#define BIT(x) (1 << x)

#ifndef BIND_EVENT_FN
    #define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#endif