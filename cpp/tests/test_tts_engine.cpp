#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "tts_engine.h"

    const std::string project_name { "test3" };
    const std::string onnx_encoder_path { "onnx/" + project_name + "/" + project_name + "_t2s_encoder.onnx" };
    const std::string onnx_fsdec_path { "onnx/" + project_name + "/" + project_name + "_t2s_fsdec.onnx" };
    const std::string onnx_sdec_path { "onnx/" + project_name + "/" + project_name + "_t2s_sdec.onnx" };
    const std::string onnx_model_path { "onnx/" + project_name + "/test3_vits.onnx" };
    const std::string ssl_content_path { "ssl_content.npy" };
    const std::string g2p_dict_file { "g2p_en_dict.json" };

    TTSEngine tts_engine(onnx_encoder_path, onnx_fsdec_path, onnx_sdec_path, onnx_model_path, ssl_content_path, g2p_dict_file);

TEST_CASE("generate_audio should return audio waveform", "[TTSEngine]") {
    const std::vector<float> audio_output = tts_engine.generate_audio("just the two of us, we can make it if we try");
    REQUIRE(audio_output.size() > 0);

    std::string test_str = "Try to understand them, because the English that we speak today is based on what our great, great, great, great grandparents spoke before!";

    BENCHMARK("generate_audio") {
        return tts_engine.generate_audio(test_str);
    };

    BENCHMARK("generate_audio (with reset)") {
        tts_engine.reset_model();
        return tts_engine.generate_audio(test_str);
    };

}
