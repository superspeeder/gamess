
#include <gtest/gtest.h>

#include <game/utils/lru_cache.hpp>

/**
 * TODO: implement tests
 * I need to manage the fact that most of my operations hinge on OpenGL state, which I need to both mock so I can test
 * that things *would* work in valid opengl contexts, and also I need a valid opengl context for some tests (to ensure
 * that I am properly using OpenGL).
 *
 * There are several things I can implement tests for without this though. For example, I can test the lru cache
 */
