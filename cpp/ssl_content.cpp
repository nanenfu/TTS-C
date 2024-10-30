#include <vector>
#include <cassert>

#include "cnpy.h"

/**
 * Load the SSL content from the ssl_content.npy file
 */
void load_ssl_content(std::vector<std::vector<std::vector<float>>>& ssl_content) {
    const cnpy::NpyArray ssl_content_numpy { cnpy::npy_load("ssl_content.npy") };
    assert(ssl_content_numpy.word_size == sizeof(float));

    const float* ssl_content_data = ssl_content_numpy.data<float>();

    // fill ssl_content from loaded_data
    for (size_t i = 0; i < ssl_content_numpy.shape[0]; ++i) {
        std::vector<std::vector<float>> ssl_content_row;
        for (size_t j = 0; j < ssl_content_numpy.shape[1]; ++j) {
            std::vector<float> ssl_content_col;
            for (size_t k = 0; k < ssl_content_numpy.shape[2]; ++k) {
                ssl_content_col.push_back(ssl_content_data[i * ssl_content_numpy.shape[1] * ssl_content_numpy.shape[2] + j * ssl_content_numpy.shape[2] + k]);
            }
            ssl_content_row.push_back(ssl_content_col);
        }
        ssl_content.push_back(ssl_content_row);
    }
}
