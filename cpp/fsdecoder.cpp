/**
 * @file fsdecoder.cpp
 * 
 * @brief Implementation of the FSDecoder (First Stage Decoder) class
 */
#include "fsdecoder.h"

#include <iostream>
#include <tuple>
#include <memory>
#include <cassert>
#include <string>
#include <vector>
#include <utility>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "encoder.h"
#include "onnx_utils.h"

FSDecoder::FSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& _memory_info)
                    : memory_info(_memory_info) {
    std::cout << "Loading ONNX models from " << onnx_model_path << std::endl;
    Ort::SessionOptions session_options;
    session = std::make_unique<Ort::Session>(env, onnx_model_path.c_str(), session_options);

    std::cout << "Encoder model info: " << std::endl;
    display_model_info(*session);
}

FSDecoderResult FSDecoder::run(EncoderResult& encoder_result) const {
    Ort::Value input_tensor1 {
        Ort::Value::CreateTensor<float>(memory_info, encoder_result.x.data(), encoder_result.x.size(),
            encoder_result.x_shape.data(), encoder_result.x_shape.size())
    };

    Ort::Value input_tensor2 {
        Ort::Value::CreateTensor<int64_t>(memory_info, encoder_result.prompts.data(), encoder_result.prompts.size(),
            encoder_result.prompts_shape.data(), encoder_result.prompts_shape.size())
    };

    print_dims("Input x", encoder_result.x_shape);
    print_dims("Input prompts", encoder_result.prompts_shape);

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

    FSDecoderResult fsdecoder_result;

    fsdecoder_result.y_shape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output y", fsdecoder_result.y_shape);
    assert(outputs[0].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64);
    size_t y_data_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();
    int64_t* y_data = outputs[0].GetTensorMutableData<int64_t>();
    fsdecoder_result.y = std::vector<int64_t>(y_data, y_data + y_data_size);

    fsdecoder_result.k_shape = outputs[1].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output k", fsdecoder_result.k_shape);
    assert(outputs[1].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT);
    size_t k_data_size = outputs[1].GetTensorTypeAndShapeInfo().GetElementCount();
    float* k_data = outputs[1].GetTensorMutableData<float>();
    fsdecoder_result.k = std::vector<float>(k_data, k_data + k_data_size);

    fsdecoder_result.v_shape = outputs[2].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output v", fsdecoder_result.v_shape);
    assert(outputs[2].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT);
    size_t v_data_size = outputs[2].GetTensorTypeAndShapeInfo().GetElementCount();
    float* v_data = outputs[2].GetTensorMutableData<float>();
    fsdecoder_result.v = std::vector<float>(v_data, v_data + v_data_size);

    fsdecoder_result.y_emb_shape = outputs[3].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output y_emb", fsdecoder_result.y_emb_shape);
    assert(outputs[3].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT);
    size_t y_emb_data_size = outputs[3].GetTensorTypeAndShapeInfo().GetElementCount();
    float* y_emb_data = outputs[3].GetTensorMutableData<float>();
    fsdecoder_result.y_emb = std::vector<float>(y_emb_data, y_emb_data + y_emb_data_size);

    fsdecoder_result.x_example_shape = outputs[4].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output x_example", fsdecoder_result.x_example_shape);
    assert(outputs[4].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT);
    size_t x_example_data_size = outputs[4].GetTensorTypeAndShapeInfo().GetElementCount();
    float* x_example_data = outputs[4].GetTensorMutableData<float>();
    fsdecoder_result.x_example = std::vector<float>(x_example_data, x_example_data + x_example_data_size);

    return std::move(fsdecoder_result);
}
