#pragma once

#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

class FSDecoder {
public:
    FSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    void run(std::vector<float>& x_result, std::vector<int64_t>& x_shape, std::vector<int64_t>& prompts_result,
            std::vector<int64_t>& prompts_shape);
private:
    Ort::MemoryInfo& memory_info;
    std::unique_ptr<Ort::Session> session;
};
