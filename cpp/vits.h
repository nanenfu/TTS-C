#pragma once

#include <string>
#include <vector>
#include <memory>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

class Vits {
public:
    Vits(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    std::vector<float> run(std::vector<int64_t> text_seq, std::vector<int64_t> pred_semantic) const;
private:
    std::unique_ptr<Ort::Session> session;
    Ort::MemoryInfo& memory_info;
};
