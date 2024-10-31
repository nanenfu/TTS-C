#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "ssdecoder.h"
#include "onnx_utils.h"

SSDecoder::SSDecoder(const std::string& onnx_model_path, Ort::Env& env, Ort::MemoryInfo& _memory_info)
                    : memory_info(_memory_info)
{
    std::cout << "Loading ONNX models from " << onnx_model_path << std::endl;
    Ort::SessionOptions encoder_session_options;
    session = std::make_unique<Ort::Session>(env, onnx_model_path.c_str(), encoder_session_options);

    std::cout << "SSDecoder model info: " << std::endl;
    display_model_info(*session);
}

std::vector<int64_t> SSDecoder::run(FSDecoderResult& fsdecoder_result, int early_stop_num, int prefix_len) const
{
    std::vector<int64_t> iy = fsdecoder_result.y;
    std::vector<int64_t> iy_shape = fsdecoder_result.y_shape;

    std::vector<float> ik = fsdecoder_result.k;
    std::vector<int64_t> ik_shape = fsdecoder_result.k_shape;

    std::vector<float> iv = fsdecoder_result.v;
    std::vector<int64_t> iv_shape = fsdecoder_result.v_shape;

    std::vector<float> iy_emb = fsdecoder_result.y_emb;
    std::vector<int64_t> iy_emb_shape = fsdecoder_result.y_emb_shape;

    std::vector<float> ix_example = fsdecoder_result.x_example;
    std::vector<int64_t> ix_example_shape = fsdecoder_result.x_example_shape;

    std::vector<float> logits;
    std::vector<int32_t> samples;

    print_dims("Input y", iy_shape);
    print_dims("Input k", ik_shape);
    print_dims("Input v", iv_shape);
    print_dims("Input y_emb", iy_emb_shape);
    print_dims("Input x_example", ix_example_shape);

    std::vector<const char*> input_names {"iy", "ik", "iv", "iy_emb", "ix_example"};
    std::vector<const char*> output_names { "y", "k", "v", "y_emb", "logits", "samples" };
    int idx = 1;
    for (; idx < 1500; ++idx) {
        Ort::Value input_tensor1 {
            Ort::Value::CreateTensor<int64_t>(memory_info, iy.data(), iy.size(), iy_shape.data(), iy_shape.size())
        };

        Ort::Value input_tensor2 {
            Ort::Value::CreateTensor<float>(memory_info, ik.data(), ik.size(), ik_shape.data(), ik_shape.size())
        };

        Ort::Value input_tensor3 {
            Ort::Value::CreateTensor<float>(memory_info, iv.data(), iv.size(), iv_shape.data(), iv_shape.size())
        };

        Ort::Value input_tensor4 {
            Ort::Value::CreateTensor<float>(memory_info, iy_emb.data(), iy_emb.size(), iy_emb_shape.data(), iy_emb_shape.size())
        };

        Ort::Value input_tensor5 {
            Ort::Value::CreateTensor<float>(memory_info, ix_example.data(), ix_example.size(), ix_example_shape.data(), ix_example_shape.size())
        };

        std::vector<Ort::Value> inputs;
        inputs.push_back(std::move(input_tensor1));
        inputs.push_back(std::move(input_tensor2));
        inputs.push_back(std::move(input_tensor3));
        inputs.push_back(std::move(input_tensor4));
        inputs.push_back(std::move(input_tensor5));

        auto outputs = session->Run(Ort::RunOptions{nullptr},
                                            input_names.data(), inputs.data(), inputs.size(),
                                            output_names.data(), output_names.size());

        iy_shape = outputs[0].GetTensorTypeAndShapeInfo().GetShape();
        size_t y_data_size = outputs[0].GetTensorTypeAndShapeInfo().GetElementCount();
        int64_t* y_data = outputs[0].GetTensorMutableData<int64_t>();
        iy = std::vector<int64_t>(y_data, y_data + y_data_size);

        ik_shape = outputs[1].GetTensorTypeAndShapeInfo().GetShape();
        size_t k_data_size = outputs[1].GetTensorTypeAndShapeInfo().GetElementCount();
        float* k_data = outputs[1].GetTensorMutableData<float>();
        ik = std::vector<float>(k_data, k_data + k_data_size);

        iv_shape = outputs[2].GetTensorTypeAndShapeInfo().GetShape();
        size_t v_data_size = outputs[2].GetTensorTypeAndShapeInfo().GetElementCount();
        float* v_data = outputs[2].GetTensorMutableData<float>();
        iv = std::vector<float>(v_data, v_data + v_data_size);

        iy_emb_shape = outputs[3].GetTensorTypeAndShapeInfo().GetShape();
        size_t y_emb_size = outputs[3].GetTensorTypeAndShapeInfo().GetElementCount();
        float* y_emb_data = outputs[3].GetTensorMutableData<float>();
        iy_emb = std::vector<float>(y_emb_data, y_emb_data + y_emb_size);


        if (early_stop_num != -1 && (iy_shape[1] - prefix_len) > early_stop_num) {
            std::cout << "early_stop_num != -1 && (iy_shape[1] - prefix_len)  > early_stop_num" << std::endl;
            break;
        }

        size_t samples_data_size = outputs[5].GetTensorTypeAndShapeInfo().GetElementCount();
        int32_t* samples_data = outputs[5].GetTensorMutableData<int32_t>();
        samples = std::vector<int32_t>(samples_data, samples_data + samples_data_size);

        if (samples[0] == 1024) {
            std::cout << "samples[0] == 1024" << std::endl;
            break;
        }

        size_t logits_size = outputs[4].GetTensorTypeAndShapeInfo().GetElementCount();
        float* logits_data = outputs[4].GetTensorMutableData<float>();
        logits = std::vector<float>(logits_data, logits_data + logits_size);

        auto max_it = std::max_element(logits.begin(), logits.end());
        int max_idx = std::distance(logits.begin(), max_it);
        if (max_idx == 1024) {
            std::cout << "np.argmax(logits, axis=-1)[0] == 1024" << std::endl;
            break;
        }
    }

    std::cout << "stop at " << idx << std::endl;

    iy[iy.size() - 1] = 0;
    // Perform slicing
    std::vector<int64_t> y_sliced(iy.begin() + iy.size() - idx, iy.end());
    std::cout << "Final y shape after stopping and slicing: " << y_sliced.size() << std::endl;

    return y_sliced;
}
