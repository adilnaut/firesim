f = open("../data/labels.txt", "r")
text = f.read()
X = []
Y = []
hash = {}
last_words = {}
for row in text.split("\n")[:-1]:
    tokens = row.split("\t")
    X.append(int(tokens[0]))
    Y.append(tokens[1])
    for _ in tokens[1].split(" "):
        if _ in hash:
            hash[_] += 1
        else:
            hash[_] = 1
    last_word = tokens[1].split(" ")[-1]
    if last_word in last_words:
        last_words[last_word] += 1
    else:
        last_words[last_word] = 1
print(len(X))
print(len(Y))
sorted_hash = sorted(hash.items(), key=lambda item: item[1])
sorted_last = sorted(last_words.items(), key=lambda item: item[1])
for key, value in sorted_hash:
    print("%s: %s" % (key, value))
for i in range(10):
    print("!______________Last__words_______________!")
for key, value in sorted_last:
    print("%s: %s" % (key, value))

classes = { "Forest" : ["Forest"],
            "Woodland": ["Woodland"],
            "Grassland": ["Grassland", "Herbaceous",  "Scrub"] ,
             "Soil": ["Steppe"],
             "Water": ["Wetlands","Wetland" ] ,
             "Other": [" "]
             }

label_hash = {}
for i in range(len(X)):
    x = X[i]
    y = Y[i]
    break_label = 0
    for key,value in classes:
        for _ in value:
            if _ in y:
                label_hash[x] = key
                break_label = 1
                break;
        if break_label == 1:
            break_label = 0
            break

print(label_hash)
