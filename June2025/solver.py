import matplotlib.pyplot as plt
import matplotlib.patches as patches

import sys
import json
sys.setrecursionlimit(30000)

grid = [[0 for i in range(45)] for j in range(45)]

file_name = ""

GRID_SIZE = 45

colors = {
    9: 'lightsteelblue',
    8: 'brown',
    7: 'yellow',
    6: 'royalblue',
    5: 'maroon',
    4: 'blue',
    3: 'darkorange',
    2: 'green',
    1: 'red'
}

def plot_grid(squares):
    fig, ax = plt.subplots(figsize=(9, 9))

    for x in range(GRID_SIZE+1):
        ax.axhline(x, color='lightgray', linewidth=0.1)
        ax.axvline(x, color='lightgray', linewidth=0.1)

    for size, positions in squares.items():
        for (y, x) in positions:
            rect = patches.Rectangle((x, y), size, size, linewidth=1, facecolor=colors[size], edgecolor='black')
            ax.add_patch(rect)

    ax.set_xlim(0, GRID_SIZE)
    ax.set_ylim(0, GRID_SIZE)
    ax.set_aspect('equal')
    plt.gca().invert_yaxis()

    #plt.savefig(f'output/{file_name}.png')
    plt.show()


def can_fit(x, y, sz):
    for i in range(x, x+sz):
        for j in range(y, y+sz):
            if grid[i][j] != 0:
                return False
    return True

def set_occupied(x, y, sz, val):
    for i in range(x, x+sz):
        for j in range(y, y+sz):
            grid[i][j] = val

def solve(x, y, squares, remaining_squares):

    if sum(remaining_squares) == 0 and x >= GRID_SIZE: 
        row_x, row_y = squares[1][0][0], squares[1][0][1]
        gridnum = int(file_name[-1]) - 1
        grid_x = gridnum//3
        grid_y = gridnum%3

        char_x = (row_x+grid_x*45)%26 + ord('a') 
        char_y = (row_y+grid_y*45)%26 + ord('a')

        print(chr(char_x), chr(char_y))
        #plot_grid(squares)

        return

    if x >= GRID_SIZE:
        return

    if y >= GRID_SIZE:
        solve(x+1, 0, squares, remaining_squares)
        return 

    if grid[x][y] == 1:
        solve(x, y+1, squares, remaining_squares)
        return


    for i in range(len(remaining_squares)):
        if remaining_squares[i] > 0:
            if x + i <= GRID_SIZE and y + i <= GRID_SIZE and can_fit(x, y, i):
                remaining_squares[i] -= 1
                squares[i].append((x, y))
                set_occupied(x, y, i, 1)
                solve(x,y+i,squares, remaining_squares)
                set_occupied(x, y, i, 0)
                remaining_squares[i] += 1
                squares[i].pop()




"""
squares = {
    9: [(9, 0), (18, 0), (27, 0), (36, 0), (0, 15), (0, 24), (13,29)],
    8: [(0, 37), (22,37), (30,37), (30, 29), (29, 9), (37, 9)],
    7: [(8, 38), (15, 38),(38, 31), (38, 38)],
    6: [(13, 9)],
    5: [(8, 33),(19, 9), (24, 9)],
    4: [(0, 11), (0, 33), (4, 33)],
    3: [(4, 12)],
    2: [],
    1: []
}
"""





with open(sys.argv[1]) as squares_json:
    squares_data = json.load(squares_json)

    file_name = sys.argv[1].split(".")[0]
    
    # Convert string keys to integer keys
    squares = {int(k): v for k, v in squares_data.items()}

    if len(sys.argv) == 3:
        plot_grid(squares)
        sys.exit(1)
        

    for size, pos in squares.items():
        for (x, y) in pos:
            for sx in range(x, x + size):
                for sy in range(y, y + size):
                    grid[sx][sy] = 1


    #plot_grid(squares)

    remaining_squares = [(i) for i in range(0, 10)]

    for x, y in squares.items():
        remaining_squares[x] -= len(y)

    solve(0, 0, squares, remaining_squares)
