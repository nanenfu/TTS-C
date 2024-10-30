#pragma once

#include <string>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

std::string tensor_type_conv(ONNXTensorElementDataType type);
void display_model_info(Ort::Session& session);
void print_dims(const std::string& name, const std::vector<int64_t>& dims);

/**
 * This function flatten a 3D vector to a 1D vector
 */
template <typename T>
std::vector<T> flatten(const std::vector<std::vector<std::vector<T>>>& nested_vector) {
    std::vector<T> flattened;
    
    // Reserve space if you know the total size, or you can resize as you go
    for (const auto& matrix : nested_vector) {
        for (const auto& row : matrix) {
            flattened.insert(flattened.end(), row.begin(), row.end());
        }
    }
    
    return flattened;
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& nested_vector) {
    std::vector<T> flattened;
    
    // Reserve space if you know the total size, or you can resize as you go
    for (const auto& row : nested_vector) {
        flattened.insert(flattened.end(), row.begin(), row.end());
    }
    
    return flattened;
}
