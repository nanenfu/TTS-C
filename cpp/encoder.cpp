#include <iostream>
#include <tuple>
#include <memory>
#include <tuple>
#include <cassert>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "encoder.h"
#include "onnx_utils.h"

Encoder::Encoder(const std::string& onnx_encoder_path, Ort::Env& env) {
    std::cout << "Loading ONNX models from " << onnx_encoder_path << std::endl;
    Ort::SessionOptions encoder_session_options;
    encoder_session = std::make_unique<Ort::Session>(env, onnx_encoder_path.c_str(), encoder_session_options);

    std::cout << "Encoder model info: " << std::endl;
    display_model_info(*encoder_session);
}

void Encoder::run(std::vector<int64_t> ref_seq,
                    std::vector<int64_t> text_seq,
                    std::vector<std::vector<float>> ref_bert,
                    std::vector<std::vector<float>> text_bert,
                    std::vector<std::vector<std::vector<float>>> ssl_content,
                    std::vector<float>& x_result,
                    std::vector<int64_t>& x_shape,
                    std::vector<int64_t>& prompts_result,
                    std::vector<int64_t>& prompts_shape) const
{
    auto memory_info { Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault) };

    std::vector<int64_t> input_dims1 { 1, static_cast<int64_t>(ref_seq.size()) };
    Ort::Value input_tensor1 {
        Ort::Value::CreateTensor<int64_t>(memory_info, ref_seq.data(), ref_seq.size(),
            input_dims1.data(), input_dims1.size())
    };


    std::vector<int64_t> input_dims2 {1, static_cast<int64_t>(text_seq.size())};
    Ort::Value input_tensor2 {
        Ort::Value::CreateTensor<int64_t>(memory_info, text_seq.data(), text_seq.size(),
            input_dims2.data(), input_dims2.size())
    };


    std::vector<int64_t> input_dims3 {
        static_cast<int64_t>(ref_bert.size()),
        static_cast<int64_t>(ref_bert[0].size())
    };
    std::vector<float> ref_bert_flattened_data { flatten(ref_bert) };
    Ort::Value input_tensor3 {
        Ort::Value::CreateTensor<float>(memory_info, ref_bert_flattened_data.data(),
            ref_bert_flattened_data.size(), input_dims3.data(), input_dims3.size())
    };


    std::vector<int64_t> input_dims4 {
        static_cast<int64_t>(text_bert.size()),
        static_cast<int64_t>(text_bert[0].size())
    };
    std::vector<float> text_bert_flattened_data { flatten(text_bert) };
    Ort::Value input_tensor4 {
        Ort::Value::CreateTensor<float>(memory_info, text_bert_flattened_data.data(),
            text_bert_flattened_data.size(), input_dims4.data(), input_dims4.size())
    };


    std::vector<int64_t> input_dims5 {
        1,
        static_cast<int64_t>(ssl_content[0].size()),
        static_cast<int64_t>(ssl_content[0][0].size())
    };
    std::vector<float> ssl_content_flattened_data { flatten(ssl_content) };
    Ort::Value input_tensor5 {
        Ort::Value::CreateTensor<float>(memory_info, ssl_content_flattened_data.data(),
            ssl_content_flattened_data.size(), input_dims5.data(), input_dims5.size())
    };

    print_dims("Input ref_seq", input_dims1);
    print_dims("Input text_seq", input_dims2);
    print_dims("Input ref_bert", input_dims3);
    print_dims("Input text_bert", input_dims4);
    print_dims("Input ssl_content", input_dims5);

    std::vector<const char*> input_names { "ref_seq", "text_seq", "ref_bert", "text_bert", "ssl_content" };
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(input_tensor1));
    inputs.push_back(std::move(input_tensor2));
    inputs.push_back(std::move(input_tensor3));
    inputs.push_back(std::move(input_tensor4));
    inputs.push_back(std::move(input_tensor5));

    std::vector<const char*> output_names { "x", "prompts" };

    std::cout << "Running encoder..." << std::endl;
    auto outputs = encoder_session->Run(Ort::RunOptions{nullptr},
                                        input_names.data(), inputs.data(), inputs.size(),
                                        output_names.data(), output_names.size());
    std::cout << "Encoder run successfully" << std::endl;

    x_shape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output x", x_shape);
    assert(outputs[0].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT);

    prompts_shape = outputs[1].GetTensorTypeAndShapeInfo().GetShape();
    print_dims("Output prompts", prompts_shape);
    assert(outputs[1].GetTensorTypeAndShapeInfo().GetElementType() == ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64);

    float* x_data = outputs[0].GetTensorMutableData<float>();
    int64_t* prompts_data = outputs[1].GetTensorMutableData<int64_t>();

    size_t x_data_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();
    size_t prompts_data_size = outputs[1].GetTensorTypeAndShapeInfo().GetElementCount();

    x_result = std::vector<float>(x_data, x_data + x_data_size);
    prompts_result = std::vector<int64_t>(prompts_data, prompts_data + prompts_data_size);
}
