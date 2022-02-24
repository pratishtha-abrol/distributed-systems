import sys

arr=[]
for line in sys.stdin:
    for i in line.split():
        arr.append(int(i))

m = arr[0]
n = arr[1]
p = arr[m*n + 3]
X = arr[2:m*n+2]
Y = arr[m*n+4:]

A = [[0 for i in range(m)] for j in range(n)]
B = [[0 for i in range(n)] for j in range(p)]
i=0
for row in range(m):
    for col in range(n):
        A[row][col] = X[i]
        i+=1
i=0
for row in range(n):
    for col in range(p):
        B[row][col] = Y[i]
        i+=1

for l in range(p):
    for j in range(m):
        for k in range(n):
            print('{0} {1} {2}\t A {3}'.format(j, l, k, A[j][k]))
for j in range(m):
    for k in range(n):
        for l in range(p):
            print('{0} {1} {2}\t B {3}'.format(j, l, k, B[k][l]))