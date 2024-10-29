#include <iostream>
#include <vector>
#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "model.h"
#include "onnx_utils.h"


std::vector<int64_t> run_t2s_onnx_model(std::string onnx_encoder_path,
                                        std::string onnx_fsdec_path,
                                        std::string onnx_sdec_path,
                                        std::vector<int64_t> ref_seq,
                                        std::vector<int64_t> text_seq,
                                        std::vector<std::vector<float>> ref_bert,
                                        std::vector<std::vector<float>> text_bert,
                                        std::vector<std::vector<std::vector<float>>> ssl_content,
                                        int early_stop_num)
{
    std::cout << "Loading ONNX models from " << onnx_encoder_path << " " << onnx_fsdec_path << " " << onnx_sdec_path << std::endl;
    // set seed to 65
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions encoder_session_options;
    Ort::Session encoder_session(env, onnx_encoder_path.c_str(), encoder_session_options);
    std::cout << "Encoder model info: " << std::endl;
    display_model_info(encoder_session);

    Ort::SessionOptions fsdec_session_options;
    Ort::Session fsdec_session(env, onnx_fsdec_path.c_str(), fsdec_session_options);
    std::cout << "Fsdec model info: " << std::endl;
    display_model_info(fsdec_session);

    Ort::SessionOptions sdec_session_options;
    Ort::Session sdec_session(env, onnx_sdec_path.c_str(), sdec_session_options);
    std::cout << "Sdec model info: " << std::endl;
    display_model_info(sdec_session);

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


    display_input_dims(input_dims1, "ref_seq");
    display_input_dims(input_dims2, "text_seq");
    display_input_dims(input_dims3, "ref_bert");
    display_input_dims(input_dims4, "text_bert");
    display_input_dims(input_dims5, "ssl_content");

    std::vector<const char*> input_names {"ref_seq", "text_seq", "ref_bert", "text_bert", "ssl_content"};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(input_tensor1));
    inputs.push_back(std::move(input_tensor2));
    inputs.push_back(std::move(input_tensor3));
    inputs.push_back(std::move(input_tensor4));
    inputs.push_back(std::move(input_tensor5));

    std::vector<const char*> output_names {"x", "prompts"};

    std::cout << "Running encoder model" << std::endl;
    auto outputs = encoder_session.Run(Ort::RunOptions{nullptr}, input_names.data(), inputs.data(),
                                        inputs.size(), output_names.data(), output_names.size());
    std::cout << "Encoder model run successfully" << std::endl;

    return std::vector<int64_t>();
}

// Adapted from the Enfu's code
std::vector<float> run_vits_onnx_model(std::string onnx_model_path, std::vector<int64_t> text_seq,
                                        std::vector<int64_t> pred_semantic) {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    Ort::Session session(env, onnx_model_path.c_str(), session_options);

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::AllocatedStringPtr input_name_ptr1 = session.GetInputNameAllocated(0, allocator);
    Ort::AllocatedStringPtr input_name_ptr2 = session.GetInputNameAllocated(1, allocator);
    Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(0, allocator);

    const char* input_name1 = input_name_ptr1.get();
    const char* input_name2 = input_name_ptr2.get();
    const char* output_name = output_name_ptr.get();

    std::cout << "Number of inputs: " << session.GetInputCount() << std::endl;
    std::cout << "Input 0 name: " << input_name1 << std::endl;
    std::cout << "Input 1 name: " << input_name2 << std::endl;
    std::cout << "Output name: " << output_name << std::endl;

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

    auto outputs = session.Run(Ort::RunOptions{nullptr}, input_names.data(), inputs.data(), inputs.size(), &output_name, 1);
    float* audio_output = outputs[0].GetTensorMutableData<float>();
    size_t audio_output_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();

    return std::vector<float>(audio_output, audio_output + audio_output_size);
}
