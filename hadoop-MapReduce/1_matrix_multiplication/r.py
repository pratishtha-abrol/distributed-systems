from operator import itemgetter
import sys

curr_key = None
curr_val = 0
row = []
col = []
mat = []
temp = []
index=0


for line in sys.stdin:
    index+=1
    key, val = line.split('\t', 1)
    val = val.split()
    key = key.split()
    try:
        key[0] = int(key[0])
        key[1] = int(key[1])
        key[2] = int(key[2])
        val[1] = int(val[1])
    except ValueError:
        continue
    # print("key: ", key)
    # print("val: ", val)

    if index >1:
        if curr_key[1] != key[1]:
            if curr_key[0] not in row:
                row.append(curr_key[0])
            if curr_key[1] not in col:
                col.append(curr_key[1])
            mat.append([curr_key[0], curr_key[1], sum(temp)])
            temp=[]

    if curr_key == key:
        curr_val*=val[1]
        temp.append(curr_val)
    else:
        curr_key = key
        curr_val = val[1]

if curr_key[0] not in row:
    row.append(curr_key[0])
if curr_key[1] not in col:
    col.append(curr_key[1])
mat.append([curr_key[0], curr_key[1], sum(temp)])
# print(mat)

rows=[]
for i in range(len(row)):
    temp = []
    for j in range(len(col)):
        for val in mat:
            if val[0] == i and val[1] == j:
                temp.append(val[2])
    rows.append(temp)

# print(rows)
for i in rows:
    print(*i)