
import time

t = time.time()

with open("test.txt", "w") as f:
    for i in range(1000000):
        f.write("hello world! {}".format(i))

with open("test.txt") as f:
    res = f.readlines()

print(time.time() - t)

# print(res)
