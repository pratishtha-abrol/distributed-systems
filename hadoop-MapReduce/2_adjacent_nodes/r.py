import sys

def dfs(visited, graph, node):
    print(node, end=" ")
    visited.add(int(node))
    for n in graph[int(node)]:
        if n not in visited:
            dfs(visited, graph, n)
            visited.add(n)

adv = {}
curr_key = None
for line in sys.stdin:
    key, val = line.strip().split('\t', 1)
    key = int(key)
    val = val.split()
    # for i in val:
    #     # i = int(i)
    #     print(i)
    if curr_key == key:
        for i in val[1:]:
            if i not in adv[key]:
                adv[key].append(i)
    else:
        adv[key] = []
        curr_key = key
        for i in val[1:]:
            if i not in adv[key]:
                adv[key].append(i)

visited = set()
for key in sorted(adv):
    if key not in visited:
        dfs(visited, adv, key)
        print('\n')

