import matplotlib.pyplot as plt

X, Y = [], []
with open("/home/mseck/Desktop/red_black_tree-1/cbr_function.txt") as f:
    for line in f:
        x, y = map(float, line.strip().split())
        X.append(x)
        Y.append(y)

plt.plot(X, Y, label='f(x)', color='blue')
plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("Fonction linéaire par morceaux")
plt.grid(True)
plt.legend()
plt.show()
