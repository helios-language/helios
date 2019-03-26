
a = [
    [1,2],
    [3,4],
    [5,6]
]

b = [
    [1,2,3],
    [4,5,6]
]

def matmul(a,b):
    if len(a) != len(b[0]):
        raise ValueError

    resultrows = len(a[0])
    resultcols = len(b)

    res = []

    for i in range(resultrows):
        temprow = []
        for j in range(resultcols):
            tempsum = 0
            for k in range(len(a)):
                tempsum += a[k][i]*b[j][k]
            temprow.append(tempsum)
        res.append(temprow)
    return res

print(matmul(a,b))