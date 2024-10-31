#include <iostream>
#include <string>
#include <vector>

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include "onnx_utils.h"

std::string tensor_type_conv(ONNXTensorElementDataType type) {
    switch (type) {
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
            return "float";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
            return "uint8";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
            return "int8";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16:
            return "uint16";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
            return "int16";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
            return "int32";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
            return "int64";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
            return "string";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
            return "bool";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
            return "float16";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
            return "double";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32:
            return "uint32";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
            return "uint64";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
            return "complex64";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
            return "complex128";
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
            return "bfloat16";
        default:
            return "unknown";
    }
}

/**
 * This function display model information with all input and output names and shapes
 * 
 * @param session The ONNX session
 */
void display_model_info(Ort::Session& session) {
    // print input and output names
    std::cout << "Number of inputs: " << session.GetInputCount() << std::endl;
    std::cout << "Number of outputs: " << session.GetOutputCount() << std::endl;

    Ort::AllocatorWithDefaultOptions allocator;

    for (int i = 0; i < session.GetInputCount(); i++) {
        Ort::TypeInfo input_type_info = session.GetInputTypeInfo(i);
        auto tensor_info = input_type_info.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> input_dims = tensor_info.GetShape();
        Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(i, allocator);

        std::cout << "Input " << i << " name: " << input_name_ptr.get() << std::endl;
        input_name_ptr.release();

        std::cout << "Input " << i << " shape: ";
        for (int j = 0; j < input_dims.size(); j++) {
            std::cout << input_dims[j] << " ";
        }
        std::cout << std::endl;

        std::cout << "Input " << i << " type: " << tensor_type_conv(tensor_info.GetElementType()) << std::endl;
        std::cout << std::endl;
    }

    for (int i = 0; i < session.GetOutputCount(); i++) {
        Ort::TypeInfo output_type_info = session.GetOutputTypeInfo(i);
        auto tensor_info = output_type_info.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> output_dims = tensor_info.GetShape();
        Ort::AllocatedStringPtr outpit_name_ptr = session.GetOutputNameAllocated(i, allocator);

        std::cout << "Output " << i << " name: " << outpit_name_ptr.get() << std::endl;
        outpit_name_ptr.release();

        std::cout << "Output " << i << " shape: ";
        for (int j = 0; j < output_dims.size(); j++) {
            std::cout << output_dims[j] << " ";
        }
        std::cout << std::endl;

        std::cout << "Output " << i << " type: " << tensor_type_conv(tensor_info.GetElementType()) << std::endl;
        std::cout << std::endl;
    }
}

/**
 * This function display dimensions
 * 
 * @param input_name The input name
 * @param dims The  dimensions
 */
void print_dims(const std::string& name, const std::vector<int64_t>& dims) {
    std::cout << name << " dims: ";
    for (int j = 0; j < dims.size(); j++) {
        std::cout << dims[j] << " ";
    }
    std::cout << std::endl;
}
