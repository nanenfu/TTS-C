#include <tuple>
#include <string>
#include <vector>

namespace NLP {
    /**
     * @brief Segment text into chunks by its language.
     * 
     * @param text The text to segment.
     * @param lang The language of the text.
     * 
     * @return A tuple containing the segmented text and the language of the text.
     * 
     * Currently, this function does nothing and returns the input text and language.
     */
    std::tuple<std::vector<std::string>, std::vector<std::string>> seg_text(const std::string& text, const std::string& lang) {
        return std::make_tuple(std::vector<std::string>{text}, std::vector<std::string>{lang});
    }
}
