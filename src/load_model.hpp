#pragma once

#include <tuple>
#include "types.hpp"

namespace engine
{
	// <vertex, normal, texture, index>
	using model_data = std::tuple<real_buffer, real_buffer, real_buffer, int_buffer>;
	model_data load_model_file(const char* model_file);
}
