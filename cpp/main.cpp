/**
 * This is an entry point for ./tts_cpp CLI command.
 */
#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <string>
#include <complex>
#include <iomanip>

#include <sndfile.h>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

// preprocessor and tokenizer
#include "text_preprocessor.h"

// data from ssl_content.npy
#include "ssl_content.h"

// TTS Engine
#include "tts_engine.h"

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

int main() {
    // Run T2S model to generate y
    const std::string project_name { "test3" };
    const std::string onnx_encoder_path { "onnx/" + project_name + "/" + project_name + "_t2s_encoder.onnx" };
    const std::string onnx_fsdec_path { "onnx/" + project_name + "/" + project_name + "_t2s_fsdec.onnx" };
    const std::string onnx_sdec_path { "onnx/" + project_name + "/" + project_name + "_t2s_sdec.onnx" };
    const std::string onnx_model_path { "onnx/" + project_name + "/test3_vits.onnx" };
    const std::string ssl_content_path { "ssl_content.npy" };

    // Step 1: Use the TextPreprocessor class to get text_seq
    const std::string text { "just the two of us, we can make it if we try" };
    const std::string lang { "en" };
    const std::string text_split_method { "cut4" };
    TextPreprocessor text_preprocessor;
    std::vector<int64_t> text_seq { text_preprocessor.preprocess(text, lang, text_split_method) };

    const std::vector<int64_t> ref_seq {
            10, 64, 26, 75, 42, 1, 64, 68, 1, 64, 68, 1, 55, 80, 75, 68, 61, 42,
            1, 51, 58, 68, 64, 62, 57, 75, 63, 68, 61, 75, 91, 35, 64, 51, 58, 26,
            74, 55, 65, 61, 75, 3, 26, 84, 74, 54, 65, 27, 12, 63, 7, 64, 12, 62, 17,
            50, 1, 91, 13, 80, 22, 62, 26, 88, 55, 93, 22, 62, 60, 13, 75, 80, 80, 16,
            61, 80, 88, 10, 64, 7, 26, 57, 12, 64, 75, 63, 35, 63, 24, 38, 3
    };

    TTSEngine tts_engine(onnx_encoder_path, onnx_fsdec_path, onnx_sdec_path, onnx_model_path, ssl_content_path);
    const std::vector<float> audio_output = tts_engine.generate_audio(text_seq, ref_seq);

    // Save the audio
    const std::string output_file_path { "output.wav" };
    save_audio(audio_output, output_file_path, 32000);
    std::cout << "Audio saved as '" << output_file_path << "'" << std::endl;

    return 0;
}
