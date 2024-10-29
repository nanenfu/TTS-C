#pragma once

#include <string>
#include <vector>

std::vector<int64_t> run_t2s_onnx_model(std::string onnx_encoder_path,
                                            std::string onnx_fsdec_path,
                                            std::string onnx_sdec_path,
                                            std::vector<int64_t> ref_seq,
                                            std::vector<int64_t> text_seq,
                                            std::vector<std::vector<int64_t>> ref_bert,
                                            std::vector<std::vector<int64_t>> text_bert,
                                            std::vector<std::vector<float>> ssl_content,
                                            int early_stop_num);

std::vector<float> run_vits_onnx_model(std::string onnx_model_path,
                                            std::vector<int64_t> text_seq,
                                            std::vector<int64_t> pred_semantic);

