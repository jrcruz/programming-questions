

for j in range(1,100):
    if j % 15 == 0:
        print("fizzbuzz")
    elif j % 5 == 0:
        print("buzz")
    elif j % 3 == 0:
        print("fizz")
    else:
        print(j)
