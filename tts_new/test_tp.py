from get_ph import TextPreprocessor
from pprint import pprint

tp = TextPreprocessor()

text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer sit amet massa mi.\n" \
        "\tDonec ac ipsum non lacus dignissim volutpat... Mauris sollicitudin est lacus,\n" \
        "iaculis pretium dolor finibus at? Donec et vestibulum dolor. Duis ultricies\n\n\n" \
        "\tjusto vel nisi posuere feugiat, suspendisse pharetra arcu quis ante vehicula\n" \
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n" \
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n" \
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n" \
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n" \
        "dignissim, pellentesque tincidunt ut nisl ac aliquam, integer dignissim, elit\n" \
        "ac ultrices auctor, tortor odio tempor justo, sed hendrerit sapien neque pretium dui.\n" \
        "\tNunc in enim tellus...                   In eleifend sagittis dui id maximus。\n" \
        "Duis condimentum risus eu urna porta, sed mattis sem molestie." \
        "Sed elit nulla, hendrerit sed quam quis, aliquam pharetra sem.\n\n\n" \
        "Maecenas condimentum libero ac nibh tincidunt, consectetur cursus justo hendrerit.\n" \
        "Phasellus bibendum odio in augue pulvinar, non blandit est ultrices. Duis commodo ve lectus nec dignissim.\n"

lang = "en"
text_split_method = "cut4"

output = tp.pre_seg_text(text, lang, text_split_method)

# Expected output:
pprint(output)
