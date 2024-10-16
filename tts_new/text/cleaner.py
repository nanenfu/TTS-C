from text import english, cleaned_text_to_sequence, symbols

language_module_map = {"en": english}
special = [
    ## List for handling special characters, you can add special characters and their corresponding handling as needed
    ("$", "en", "SP2"),
    ("$", "en", "SP2"),
    ("^", "en", "SP3"),
]

def clean_text(text, language):
    language = "en"  
    for special_s, special_l, target_symbol in special:
        if special_s in text and language == special_l:
            return clean_special(text, language, special_s, target_symbol)
    language_module = language_module_map[language]
    norm_text = language_module.text_normalize(text)
    phones = language_module.g2p(norm_text)
    word2ph = None

    for ph in phones:
        assert ph in symbols
    return phones, word2ph, norm_text


def clean_special(text, language, special_s, target_symbol):
    """
    Handle special silence symbols
    """
    text = text.replace(special_s, ",")
    language_module = language_module_map[language]
    norm_text = language_module.text_normalize(text)
    phones = language_module.g2p(norm_text)
    new_ph = []
    for ph in phones[0]:
        assert ph in symbols
        if ph == ",":  # Replace commas with the specified special symbol
            new_ph.append(target_symbol)
        else:
            new_ph.append(ph)
    return new_ph, phones[1], norm_text


def text_to_sequence(text):
    phones = clean_text(text)
    return cleaned_text_to_sequence(phones)


if __name__ == "__main__":
    print(clean_text("Hello, how are you?", "en"))

