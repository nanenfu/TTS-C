import os
import sys

now_dir = os.getcwd()
sys.path.append(now_dir)
import re
import LangSegment
from typing import Dict, List, Tuple
from text.cleaner import clean_text
from text import cleaned_text_to_sequence
from text_segmentation_method import split_big_text, splits, get_method as get_seg_method
from i18n.i18n import I18nAuto
import time

i18n = I18nAuto()

def get_first(text: str) -> str:
    pattern = "[" + "".join(re.escape(sep) for sep in splits) + "]"
    text = re.split(pattern, text)[0].strip()
    return text

def merge_short_text_in_array(texts: str, threshold: int) -> list:
    if len(texts) < 2:
        return texts
    result = []
    text = ""
    for ele in texts:
        text += ele
        if len(text) >= threshold:
            result.append(text)
            text = ""
    if len(text) > 0:
        if len(result) == 0:
            result.append(text)
        else:
            result[len(result) - 1] += text
    return result

class TextPreprocessor:
    def __init__(self):
        pass

    def preprocess(self, text: str, lang: str, text_split_method: str) -> List[Dict]:
        start_time_seg = time.time()  
        texts = self.pre_seg_text(text, lang, text_split_method)
        end_time_seg = time.time()  
        result = []

        for text in texts:
            phones, norm_text = self.segment_and_extract_feature_for_text(text, lang)
            if phones is None:
                continue
            res = {
                "phones": phones,
                "norm_text": norm_text,
            }
            result.append(res)
        return result

    def pre_seg_text(self, text: str, lang: str, text_split_method: str):
        text = text.strip("\n")
        if text[0] not in splits and len(get_first(text)) < 4:
            text = "." + text if lang == "en" else "。" + text

        seg_method = get_seg_method(text_split_method)
        text = seg_method(text)

        while "\n\n" in text:
            text = text.replace("\n\n", "\n")

        _texts = text.split("\n")
        _texts = merge_short_text_in_array(_texts, 5)
        texts = []

        for text in _texts:
            if len(text.strip()) == 0:
                continue
            if text[-1] not in splits:
                text += "." if lang == "en" else "。"
            if len(text) > 510:
                texts.extend(split_big_text(text))
            else:
                texts.append(text)

        return texts

    def segment_and_extract_feature_for_text(self, text: str, language: str) -> Tuple[list, str]:
        textlist, langlist = self.seg_text(text, language)
        if len(textlist) == 0:
            return None, None

        phones, norm_text = self.extract_features(textlist, langlist)
        return phones, norm_text

    def seg_text(self, text: str, language: str) -> Tuple[list, list]:
        textlist = []
        langlist = []
        if language == "en":
            LangSegment.setfilters(["en"])
            formattext = " ".join(tmp["text"] for tmp in LangSegment.getTexts(text))
            while "  " in formattext:
                formattext = formattext.replace("  ", " ")
            if formattext != "":
                textlist.append(formattext)
                langlist.append("en")
        else:
            raise ValueError(f"language {language} not supported")

        return textlist, langlist

    def extract_features(self, textlist: list, langlist: list):
        phones_list = []
        norm_text_list = []
        for i in range(len(textlist)):
            lang = langlist[i]
            phones, word2ph, norm_text = self.clean_text_inf(textlist[i], lang)
            phones_list.extend(phones)
            norm_text_list.append(norm_text)

        norm_text = ''.join(norm_text_list)
        return phones_list, norm_text

    def clean_text_inf(self, text: str, language: str):
        phones, word2ph, norm_text = clean_text(text, language)
        phones = cleaned_text_to_sequence(phones)
        return phones, word2ph, norm_text


