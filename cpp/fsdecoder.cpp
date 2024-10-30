#include <iostream>
#include <tuple>
#include <memory>
#include <tuple>
#include <cassert>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "fsdecoder.h"
#include "onnx_utils.h"

FSDecoder::FSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& _memory_info)
                    : memory_info(_memory_info)
{
    std::cout << "Loading ONNX models from " << onnx_model_path << std::endl;
    Ort::SessionOptions encoder_session_options;
    session = std::make_unique<Ort::Session>(env, onnx_model_path.c_str(), encoder_session_options);

    std::cout << "Encoder model info: " << std::endl;
    display_model_info(*session);
}

void FSDecoder::run(std::vector<float>& x_result, std::vector<int64_t>& x_shape, std::vector<int64_t>& prompts_result,
            std::vector<int64_t>& prompts_shape)
{
    Ort::Value input_tensor1 {
        Ort::Value::CreateTensor<float>(memory_info, x_result.data(), x_result.size(),
            x_shape.data(), x_shape.size())
    };

    Ort::Value input_tensor2 {
        Ort::Value::CreateTensor<int64_t>(memory_info, prompts_result.data(), prompts_result.size(),
            prompts_shape.data(), prompts_shape.size())
    };

    print_dims("Input x", x_shape);
    print_dims("Input prompts", prompts_shape);

    std::vector<const char*> input_names {"x", "prompts"};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(input_tensor1));
    inputs.push_back(std::move(input_tensor2));

    std::vector<const char*> output_names { "y", "k", "v", "y_emb", "x_example" };

    std::cout << "Running first stage decoder..." << std::endl;
    auto outputs = session->Run(Ort::RunOptions{nullptr},
                                        input_names.data(), inputs.data(), inputs.size(),
                                        output_names.data(), output_names.size());
    std::cout << "First stage decoder run successfully" << std::endl;
}