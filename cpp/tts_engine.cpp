#include <string>
#include <memory>
#include <vector>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "ssl_content.h"
#include "encoder.h"
#include "fsdecoder.h"
#include "ssdecoder.h"
#include "vits.h"

#include "tts_engine.h"

TTSEngine::TTSEngine(const std::string& onnx_encoder_path,
                const std::string& onnx_fsdec_path,
                const std::string& onnx_sdec_path,
                const std::string& onnx_model_path,
                const std::string& ssl_content_path) {

    auto [ssl_conent, ssl_content_shape] = load_ssl_content(ssl_content_path);
    this->ssl_conent = ssl_conent;
    this->ssl_content_shape = ssl_content_shape;

    env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "test");
    memory_info = std::make_unique<Ort::MemoryInfo>(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault));

    encoder = std::make_unique<Encoder>(onnx_encoder_path, *env, *memory_info);
    fsdecoder = std::make_unique<FSDecoder>(onnx_fsdec_path, *env, *memory_info);
    ssdecoder = std::make_unique<SSDecoder>(onnx_sdec_path, *env, *memory_info);
    vits = std::make_unique<Vits>(onnx_model_path, *env, *memory_info);
}

std::vector<float> TTSEngine::generate_audio(std::vector<int64_t> text_seq,
                                    std::vector<int64_t> ref_seq) {
    const std::vector<std::vector<float>> ref_bert(ref_seq.size(), std::vector<float>(1024, 0.0f));
    const std::vector<std::vector<float>> text_bert(text_seq.size(), std::vector<float>(1024, 0.0f));

    EncoderResult encoder_result {
        encoder->run(ref_seq, text_seq, ref_bert, text_bert, ssl_conent, ssl_content_shape)
    };

    FSDecoderResult fsdecoder_result {
        fsdecoder->run(encoder_result)
    };

    const int early_stop_num = 2700;
    const int prefix_len = encoder_result.prompts_shape[1];

    const std::vector<int64_t> pred_semantic {
        ssdecoder->run(fsdecoder_result, early_stop_num, prefix_len)
    };

    const std::vector<float> audio_output {
        vits->run(text_seq, pred_semantic)
    };

    return std::move(audio_output);
}
