#pragma once

#include <string>
#include <vector>
#include <memory>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "encoder.h"
#include "fsdecoder.h"
#include "ssdecoder.h"
#include "vits.h"

class TTSEngine {
public:
    TTSEngine(const std::string& onnx_encoder_path,
                const std::string& onnx_fsdec_path,
                const std::string& onnx_sdec_path,
                const std::string& onnx_model_path,
                const std::string& ssl_content_path);

    std::unique_ptr<Encoder> encoder;
    std::unique_ptr<FSDecoder> fsdecoder;
    std::unique_ptr<SSDecoder> ssdecoder;
    std::unique_ptr<Vits> vits;
    std::vector<float> ssl_conent;
    std::vector<int64_t> ssl_content_shape;

    std::unique_ptr<Ort::Env> env;
    std::unique_ptr<Ort::MemoryInfo> memory_info;

    std::vector<float> generate_audio(std::vector<int64_t> text_seq,
                                        std::vector<int64_t> ref_seq);
};
