import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib import colors

neighbourhood = ((-1,-1), (-1,0), (-1,1), (0,-1), (0, 1), (1,-1), (1,0), (1,1)  )
EMPTY, TREE, FIRE, WATER= 0, 1, 2, 3

colors_list = [(0.2,0,0), (0,0.5,0), (1,0,0), 'orange', 'blue']
cmap = colors.ListedColormap(colors_list)
bounds = [0, 1,2,3,4]
norm = colors.BoundaryNorm(bounds, cmap.N)



def iterate(X):
    X1 = np.zeros((ny, nx))
    for ix in range(1,nx-1):
        for iy in range(1,ny-1):
            if X[iy,ix] == TREE:
                X1[iy,ix] = TREE;
                for dx,dy in neighbourhood:
                    if X[iy+dy,ix+dx] == FIRE:
                        if np.random.random() > 0.20:
                            X1[iy,ix] = FIRE
                        break
            elif X[iy, ix] == WATER:
                X1[iy, ix] = WATER
                for dx,dy in neighbourhood:
                    if X[iy+dy,ix+dx] == FIRE:
                        if np.random.random() > 0.5:
                            X1[iy,ix] = EMPTY
                        X[iy+dy, ix+dx] = EMPTY

    return X1

# The initial fraction of the forest occupied by trees.
forest_fraction = 0.5
# Forest size (number of cells in x and y directions).
nx, ny = 150, 150
# Initialize the forest grid.
X  = np.zeros((ny, nx))


X[1:ny-1, 1:nx-1] = np.random.random(size=(ny-2, nx-2)) < forest_fraction


for k in range(0, 10):
    radius = k
    center = nx // 2
    for i in range(-radius,+radius):
            if np.random.random() > 0.5:
                X[center + int((radius**2 - i**2)**0.5), center + i] = FIRE
                X[center - int((radius**2 - i**2)**0.5), center + i] = FIRE
                X[ center + i, center + int((radius**2 - i**2)**0.5)] = FIRE
                X[ center + i, center - int((radius**2 - i**2)**0.5)] = FIRE



for k in range(20, 22):
    radius = k
    center = nx // 2
    for i in range(-radius,+radius):
        if np.random.random() > 0.5:
            X[center + int((radius**2 - i**2)**0.5), center + i] = WATER
            X[center - int((radius**2 - i**2)**0.5), center + i] = WATER
            X[ center + i, center + int((radius**2 - i**2)**0.5)] = WATER
            X[ center + i, center - int((radius**2 - i**2)**0.5)] = WATER


fig = plt.figure(figsize=(25/3, 6.25))
ax = fig.add_subplot(111)
ax.set_axis_off()
im = ax.imshow(X, cmap=cmap, norm=norm)

# The animation function: called to produce a frame for each generation.
def animate(i):
    im.set_data(animate.X)
    animate.X = iterate(animate.X)
# Bind our grid to the identifier X in the animate function's namespace.
animate.X = X

# Interval between frames (ms).
interval = 100
anim = animation.FuncAnimation(fig, animate, interval=interval)
plt.show()
