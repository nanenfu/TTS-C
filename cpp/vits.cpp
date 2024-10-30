#include <iostream>
#include <vector>
#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "vits.h"
#include "onnx_utils.h"

Vits::Vits(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& memory_info) {
    std::cout << "Loading ONNX models from " << onnx_model_path << std::endl;
    Ort::SessionOptions encoder_session_options;
    session = std::make_unique<Ort::Session>(env, onnx_model_path.c_str(), encoder_session_options);

    std::cout << "VITS model info: " << std::endl;
    display_model_info(*session);
}

// Adapted from the Enfu's code
std::vector<float> Vits::run(std::string onnx_model_path, std::vector<int64_t> text_seq,
                                        std::vector<int64_t> pred_semantic) const
{
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::AllocatedStringPtr input_name_ptr1 = session->GetInputNameAllocated(0, allocator);
    Ort::AllocatedStringPtr input_name_ptr2 = session->GetInputNameAllocated(1, allocator);
    Ort::AllocatedStringPtr output_name_ptr = session->GetOutputNameAllocated(0, allocator);

    const char* input_name1 = input_name_ptr1.get();
    const char* input_name2 = input_name_ptr2.get();
    const char* output_name = output_name_ptr.get();

    std::vector<int64_t> text_seq_modifiable = text_seq;
    std::vector<int64_t> input_dims1 = {1, static_cast<int64_t>(text_seq_modifiable.size())};

    Ort::Value input_tensor1 = Ort::Value::CreateTensor<int64_t>(
        memory_info, text_seq_modifiable.data(), text_seq_modifiable.size(), input_dims1.data(), input_dims1.size());

    std::vector<int64_t> pred_semantic_modifiable = pred_semantic;
    std::vector<int64_t> input_dims2 = {1, 1, static_cast<int64_t>(pred_semantic_modifiable.size())};

    Ort::Value input_tensor2 = Ort::Value::CreateTensor<int64_t>(
        memory_info, pred_semantic_modifiable.data(), pred_semantic_modifiable.size(), input_dims2.data(), input_dims2.size());

    std::vector<const char*> input_names = {input_name1, input_name2};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(input_tensor1));
    inputs.push_back(std::move(input_tensor2));

    auto outputs = session->Run(Ort::RunOptions{nullptr}, input_names.data(), inputs.data(), inputs.size(), &output_name, 1);
    float* audio_output = outputs[0].GetTensorMutableData<float>();
    size_t audio_output_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();

    return std::vector<float>(audio_output, audio_output + audio_output_size);
}
