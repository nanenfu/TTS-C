# A script to generate symbols.h file for C++ implementation

from text import symbols

# save to json
with open('symbols.h', 'w') as f:
    f.write("#pragma once\n\n")
    f.write("#include <vector>\n")
    f.write("#include <string>\n\n\n")

    f.write("namespace NLP {\n")
    f.write("\t std::vector<std::string> symbols {")
    for symbol in symbols:
        f.write("\"" + symbol + "\",")
    f.write("};\n")
    f.write("}\n")
