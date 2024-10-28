import json
from text.english import read_dict_new

g2p_dict = read_dict_new()

# save to json
with open('g2p_dict.json', 'w') as f:
    json.dump(g2p_dict, f)
