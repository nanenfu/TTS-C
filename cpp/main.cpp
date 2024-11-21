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
    const std::string g2p_dict_file { "g2p_en_dict.json" };

    TTSEngine tts_engine(onnx_encoder_path, onnx_fsdec_path, onnx_sdec_path, onnx_model_path, ssl_content_path, g2p_dict_file);
    const std::vector<float> audio_output = tts_engine.generate_audio("just the two of us, we can make it if we try");

    // Save the audio
    const std::string output_file_path { "output.wav" };
    save_audio(audio_output, output_file_path, 32000);
    std::cout << "Audio saved as '" << output_file_path << "'" << std::endl;

    return 0;
}
