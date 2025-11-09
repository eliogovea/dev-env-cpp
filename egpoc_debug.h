#pragma once

#ifndef EGPOC_DEBUG_H_
#define EGPOC_DEBUG_H_

void egpoc_debug(char const* file, int line, char const* format, ...);

#define EGPOC_DEBUG(...) egpoc_debug(__FILE__, __LINE__, __VA_ARGS__)

#endif  // EGPOC_DEBUG_H_
