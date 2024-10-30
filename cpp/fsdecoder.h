#pragma once

#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

struct FSDecoderResult {
    std::vector<float> y;
    std::vector<int64_t> y_shape;
    std::vector<int64_t> k;
    std::vector<int64_t> k_shape;
    std::vector<int64_t> v;
    std::vector<int64_t> v_shape;
    std::vector<int64_t> y_emb;
    std::vector<int64_t> y_emb_shape;
    std::vector<int64_t> x_example;
    std::vector<int64_t> x_example_shape;
};

class FSDecoder {
public:
    FSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info);
    FSDecoderResult run(EncoderResult& encoder_result) const;
private:
    Ort::MemoryInfo& memory_info;
    std::unique_ptr<Ort::Session> session;
};
