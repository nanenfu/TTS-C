#include <vector>
#include <cassert>
#include <tuple>
#include <utility>
#include <iostream>

#include "cnpy.h"

#include "ssl_content.h"

/**
 * Load the SSL content from the ssl_content.npy file
 */
std::tuple<std::vector<float>, std::vector<int64_t>> load_ssl_content() {
    std::cout << "Loading SSL content from " << ssl_content_path << std::endl;
    const cnpy::NpyArray ssl_content_numpy { cnpy::npy_load(ssl_content_path) };
    assert(ssl_content_numpy.word_size == sizeof(float));

    const float* ssl_content_data = ssl_content_numpy.data<float>();

    std::vector<float> ssl_content(ssl_content_data, ssl_content_data + ssl_content_numpy.num_vals);
    const std::vector<int64_t> ssl_content_shape(ssl_content_numpy.shape.begin(), ssl_content_numpy.shape.end());

    // transpose if Fortran order
    if (ssl_content_numpy.fortran_order) {
        std::cout << ssl_content_path << " is in Fortran order" << std::endl;

        // Transpose the matrix
        std::vector<float> ssl_content_transposed(ssl_content.size());
        const int64_t num_rows = ssl_content_shape[1];
        const int64_t num_cols = ssl_content_shape[2];
        for (int64_t i = 0; i < num_rows; i++) {
            for (int64_t j = 0; j < num_cols; j++) {
                ssl_content_transposed[i * num_cols + j] = ssl_content[j * num_rows + i];
            }
        }

        return std::move(std::make_tuple(ssl_content_transposed, ssl_content_shape));
    }

    return std::move(std::make_tuple(ssl_content, ssl_content_shape));
}
