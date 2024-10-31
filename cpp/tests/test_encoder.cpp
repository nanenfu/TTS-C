#include <string>
#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "ssl_content.h"
#include "encoder.h"

TEST_CASE("run should run inference", "[Encoder]") {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    const std::string project_name { "test3" };
    const std::string onnx_encoder_path { "onnx/" + project_name + "/" + project_name + "_t2s_encoder.onnx" };
    auto memory_info { Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault) };
    Encoder encoder(onnx_encoder_path, env, memory_info);

    const std::vector<int64_t> text_seq {
        60, 13, 75, 80, 27, 12, 80, 88, 13, 90, 13, 75, 1, 91, 58, 61, 10,
        64, 63, 42, 61, 55, 80, 55, 49, 91, 58, 80, 74, 22, 3
    };

    const std::vector<int64_t> ref_seq {
        10, 64, 26, 75, 42, 1, 64, 68, 1, 64, 68, 1, 55, 80, 75, 68, 61, 42,
        1, 51, 58, 68, 64, 62, 57, 75, 63, 68, 61, 75, 91, 35, 64, 51, 58, 26,
        74, 55, 65, 61, 75, 3, 26, 84, 74, 54, 65, 27, 12, 63, 7, 64, 12, 62, 17,
        50, 1, 91, 13, 80, 22, 62, 26, 88, 55, 93, 22, 62, 60, 13, 75, 80, 80, 16,
        61, 80, 88, 10, 64, 7, 26, 57, 12, 64, 75, 63, 35, 63, 24, 38, 3
    };

    const std::vector<std::vector<float>> ref_bert(ref_seq.size(), std::vector<float>(1024, 0.0f));
    const std::vector<std::vector<float>> text_bert(text_seq.size(), std::vector<float>(1024, 0.0f));

    auto [ssl_content, ssl_content_shape] = load_ssl_content();

    EncoderResult encoder_result {
        encoder.run(ref_seq, text_seq, ref_bert, text_bert, ssl_content, ssl_content_shape)
    };

    // check model output
    REQUIRE(encoder_result.x.size() == 62464);
    REQUIRE(encoder_result.x_shape.size() == 3);
    REQUIRE(encoder_result.prompts.size() == 159);
    REQUIRE(encoder_result.prompts_shape.size() == 2);

    // check a few values
    REQUIRE(encoder_result.prompts[0] == 55);
    REQUIRE(encoder_result.prompts[10] == 256);
}
