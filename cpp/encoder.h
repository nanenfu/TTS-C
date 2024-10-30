#pragma once

#include <vector>
#include <string>
#include <memory>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>


struct EncoderResult {
    std::vector<float> x;
    std::vector<int64_t> x_shape;
    std::vector<int64_t> prompts;
    std::vector<int64_t> prompts_shape;
};


class Encoder {
public:
    Encoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    EncoderResult run(std::vector<int64_t> ref_seq,
                std::vector<int64_t> text_seq,
                std::vector<std::vector<float>> ref_bert,
                std::vector<std::vector<float>> text_bert,
                std::vector<std::vector<std::vector<float>>> ssl_content) const;
private:
    Ort::MemoryInfo& memory_info;
    std::unique_ptr<Ort::Session> session;
};
