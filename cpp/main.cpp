#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <vector>
#include <iostream>
#include <sndfile.h>

// Adapted from the Enfu's code
void save_audio(const std::vector<float>& audio_data, const std::string& file_path, int sample_rate) {
    SF_INFO sfinfo;
    sfinfo.frames = audio_data.size();
    sfinfo.samplerate = sample_rate;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(file_path.c_str(), SFM_WRITE, &sfinfo);
    if (!outfile) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }

    sf_write_float(outfile, audio_data.data(), audio_data.size());
    sf_close(outfile);
}

// Adapted from the Enfu's code
int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    std::string onnx_model_path = "/Users/dima/dev/Text-to-speech-new/tts_new/onnx/test3/test3_vits.onnx";
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

    std::vector<int64_t> text_seq = {60, 13, 75, 80, 27, 12, 80, 88, 13, 90, 13, 75, 1, 91, 58, 61,
                                        10, 64, 63, 42, 61, 55, 80, 55, 49, 91, 58, 80, 74, 22, 3};
    std::vector<int64_t> text_seq_modifiable = text_seq;
    std::vector<int64_t> input_dims1 = {1, static_cast<int64_t>(text_seq_modifiable.size())};

    Ort::Value input_tensor1 = Ort::Value::CreateTensor<int64_t>(
        memory_info, text_seq_modifiable.data(), text_seq_modifiable.size(), input_dims1.data(), input_dims1.size());

    std::vector<int64_t> pred_semantic = {361, 643, 479, 150, 753, 760, 644, 45, 996, 42, 744, 785, 790, 619, 251, 319, 488,
                                        783, 127, 660, 331, 637, 32, 528, 934, 303, 740, 9, 908, 837, 325, 201, 55, 385,
                                        921, 256, 369, 887, 827, 355, 145, 293, 989, 988, 84, 378, 727, 180, 180, 263, 496,
                                        175, 902, 278, 748, 748, 52, 0};
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

    std::vector<float> output = std::vector<float>(audio_output, audio_output + audio_output_size);
    save_audio(output, "ntout.wav", 32000);
    std::cout << "Audio saved as 'ntout.wav'" << std::endl;
    return 0;
}
