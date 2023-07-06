a = int(input())
print(a)
f = open("test_input.txt")
a = f.readlines()
print(len(a))
for i in range(len(a)) :
    print(a[i],end="")
