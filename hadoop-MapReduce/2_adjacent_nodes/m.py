import sys

for line in sys.stdin:
    nodes = list(map(int,line.strip().split()))
    if len(nodes) == 2:
        m,n = min(nodes[0], nodes[1]), max(nodes[0], nodes[1])
        print('{0} \t {1} {2}'.format(m,m,n))
        print('{0} \t {1}'.format(n,n))
    else:
        print('{0} \t {1}'.format(nodes[0],nodes[0]))