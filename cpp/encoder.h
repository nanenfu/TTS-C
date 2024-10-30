#pragma once

#include <vector>
#include <string>
#include <memory>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

class Encoder {
public:
    Encoder(const std::string& onnx_encoder_path, Ort::Env& env);
    void run(std::vector<int64_t> ref_seq,
                std::vector<int64_t> text_seq,
                std::vector<std::vector<float>> ref_bert,
                std::vector<std::vector<float>> text_bert,
                std::vector<std::vector<std::vector<float>>> ssl_content,
                std::vector<float>& x_result,
                std::vector<int64_t>& x_shape,
                std::vector<int64_t>& prompts_result,
                std::vector<int64_t>& prompts_shape) const;
private:
    std::unique_ptr<Ort::Session> encoder_session;
};
