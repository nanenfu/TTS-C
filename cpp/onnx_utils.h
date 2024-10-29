#pragma once

#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

std::string tensor_type_conv(ONNXTensorElementDataType type);
void display_model_info(Ort::Session& session);
