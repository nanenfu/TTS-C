#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "ssl_content.h"
#include "encoder.h"
#include "fsdecoder.h"
#include "ssdecoder.h"
#include "vits.h"

#include "tts_engine.h"

TTSEngine::TTSEngine(const std::string _onnx_encoder_path,
                const std::string _onnx_fsdec_path,
                const std::string _onnx_sdec_path,
                const std::string _onnx_model_path,
                const std::string ssl_content_path,
                const std::string g2p_dict_file)
                :
                onnx_encoder_path(_onnx_encoder_path),
                onnx_fsdec_path(_onnx_fsdec_path),
                onnx_sdec_path(_onnx_sdec_path),
                onnx_model_path(_onnx_model_path) {

    std::cout << "Initializing TTS Engine with the following options:" << std::endl;
    std::cout << "Encoder ONNX Model Path: " << onnx_encoder_path << std::endl;
    std::cout << "FSDecoder ONNX Model Path: " << onnx_fsdec_path << std::endl;
    std::cout << "SSDecoder ONNX Model Path: " << onnx_sdec_path << std::endl;
    std::cout << "Vits ONNX Model Path: " << onnx_model_path << std::endl;
    std::cout << "SSL Content Path: " << ssl_content_path << std::endl;
    std::cout << "G2P Dict File: " << g2p_dict_file << std::endl;

    std::cout << "OnnxRuntime version: " << Ort::GetVersionString() << std::endl;
    std::cout << "OnnxRuntime Build info: " << Ort::GetBuildInfoString() << std::endl;
    std::cout << "Available Providers: " << std::endl;
    for (const auto& provider : Ort::GetAvailableProviders()) {
        std::cout << provider << std::endl;
    }

    auto [ssl_conent, ssl_content_shape] = load_ssl_content(ssl_content_path);
    this->ssl_conent = ssl_conent;
    this->ssl_content_shape = ssl_content_shape;

    preprocessor = std::make_unique<TextPreprocessor>(g2p_dict_file);
    env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "test");
    memory_info = std::make_unique<Ort::MemoryInfo>(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault));

    encoder = std::make_unique<Encoder>(onnx_encoder_path, *env, *memory_info);
    fsdecoder = std::make_unique<FSDecoder>(onnx_fsdec_path, *env, *memory_info);
    ssdecoder = std::make_unique<SSDecoder>(onnx_sdec_path, *env, *memory_info);
    vits = std::make_unique<Vits>(onnx_model_path, *env, *memory_info);
}

void TTSEngine::reset_model() {
#ifdef VERBOSE
    std::cout << "Resetting model..." << std::endl;
#endif
    fsdecoder = std::make_unique<FSDecoder>(onnx_fsdec_path, *env, *memory_info);
    ssdecoder = std::make_unique<SSDecoder>(onnx_sdec_path, *env, *memory_info);
    vits = std::make_unique<Vits>(onnx_model_path, *env, *memory_info);
}

std::vector<float> TTSEngine::generate_audio(const std::string text) {
#ifdef VERBOSE
    std::cout << "Generating audio for the following text: " << text << std::endl;
#endif

    const std::string lang { "en" };
    const std::string text_split_method { "cut4" };

#ifdef VERBOSE
    std::cout << "Preprocessing text..." << std::endl;
#endif
    std::vector<int64_t> text_seq { preprocessor->preprocess(text, lang, text_split_method) };
#ifdef VERBOSE
    std::cout << "Preprocessing finished..." << std::endl;
#endif

    const std::vector<int64_t> ref_seq {
            10, 64, 26, 75, 42, 1, 64, 68, 1, 64, 68, 1, 55, 80, 75, 68, 61, 42,
            1, 51, 58, 68, 64, 62, 57, 75, 63, 68, 61, 75, 91, 35, 64, 51, 58, 26,
            74, 55, 65, 61, 75, 3, 26, 84, 74, 54, 65, 27, 12, 63, 7, 64, 12, 62, 17,
            50, 1, 91, 13, 80, 22, 62, 26, 88, 55, 93, 22, 62, 60, 13, 75, 80, 80, 16,
            61, 80, 88, 10, 64, 7, 26, 57, 12, 64, 75, 63, 35, 63, 24, 38, 3
    };

    const std::vector<std::vector<float>> ref_bert(ref_seq.size(), std::vector<float>(1024, 0.0f));
    const std::vector<std::vector<float>> text_bert(text_seq.size(), std::vector<float>(1024, 0.0f));

#ifdef VERBOSE
    std::cout << "Run encoder..." << std::endl;
#endif
    EncoderResult encoder_result {
        encoder->run(ref_seq, text_seq, ref_bert, text_bert, ssl_conent, ssl_content_shape)
    };
#ifdef VERBOSE
    std::cout << "Finished." << std::endl;
#endif

#ifdef VERBOSE
    std::cout << "Run decoder..." << std::endl;
#endif
    FSDecoderResult fsdecoder_result {
        fsdecoder->run(encoder_result)
    };
#ifdef VERBOSE
    std::cout << "Finished." << std::endl;
#endif

    const int early_stop_num = 2700;
    const int prefix_len = encoder_result.prompts_shape[1];

#ifdef VERBOSE
    std::cout << "Run ssdecoder..." << std::endl;
#endif
    const std::vector<int64_t> pred_semantic {
        ssdecoder->run(fsdecoder_result, early_stop_num, prefix_len)
    };
#ifdef VERBOSE
    std::cout << "Finished." << std::endl;

    std::cout << "Run vits..." << std::endl;
#endif
    const std::vector<float> audio_output {
        vits->run(text_seq, pred_semantic)
    };
#ifdef VERBOSE
    std::cout << "Finished." << std::endl;

    std::cout << "Audio generated!" << std::endl;
#endif
    return audio_output;
}
