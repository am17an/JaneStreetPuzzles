from functools import cache
import sys
import argparse

grid = [[0]*11 for _ in range(11)]

@cache
def getfibs(n):
    if n == 0 or n == 1:
        return 1
    
    return getfibs(n-1) + getfibs(n-2)

def is_prime(n):
    """Check if a number is prime using trial division with optimizations."""
    # Handle base cases
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    
    # Only check divisors of form 6k ± 1 up to sqrt(n)
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
        
    return True

first_50_fibs = {getfibs(i): 1 for i in range(50)}


regions = {
    '0': [[(0, 0), 11], [(1, 0), 1], [(1, 2), 9],[(2, 9), 1]],
    '1': [[(1, 1), 1], [(2, 0), 2], [(3, 0), 1], [(4, 0), 1], [(5,0), 5], [(6,0), 1], [(6, 4), 2], [(7, 0), 1], [(3, 3), 1], [(4, 3), 1]],
    '2': [[(2,2), 4], [(3,1), 2], [(4,1), 2], [(3,4), 1], [(4,4), 1]],
    '3': [[(2,6), 3], [(2,10), 1], [(3,6), 5], [(4,7), 2], [(4,10), 1], [(5, 10), 1]],
    '4': [[(3, 5), 1], [(4, 5), 2], [(5,5), 5], [(6,6), 2], [(6, 9), 2], [(4, 9), 1]],
    '5': [[(6,2), 2], [(6,8), 1], [(7,2), 7],[(8,4), 1],[(8, 6), 1]],
    '6': [[(8, 0), 4], [(8,5), 1], [(9, 0), 11], [(10,0), 2], [(10,8), 3], [(6, 1), 1], [(7, 1), 1]],
    '7': [[(10, 2), 6]],
    '8': [[(7,9), 2], [(8, 7), 4]]
}

"""
possible_colorings = {
    '0': [2],
    '1': [5],
    '2': [i for i in range(1, 10)],
    '3': [i for i in range(1, 10)],
    '4': [1,5], 
    '5': [i for i in range(1, 10)],
    '6': [1, 3, 5, 9], 
    '7': [i for i in range(1, 10)],
    '8': [i for i in range(1, 10)]
}
"""

possible_colorings = {
    '0': [2],
    '1': [4],
    '2': [3],
    '3': [4],
    '4': [1], 
    '5': [6],
    '6': [3], 
    '7': [7],
    '8': [7]
}

yellow = [
     (1, 3), (1, 4), (2, 4), 
     (2, 9), (3, 9), (3, 8),
     (5,5), (6, 5), (6, 6), 
     (6,1),(6,2),(7,1),
     (8, 4), (8, 5), (9, 4)
]
    
GRID_SIZE = 11

# --- Step 1: Map grid cells to regions ---
grid = [[None for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]

squares = [i*i for i in range(1, int(1e6))]

def is_square(num):
    """Check if a number is a perfect square using binary search."""
    # For small numbers, check directly
    if num < 10000:
        root = int(num ** 0.5)
        return root * root == num
        
    # For larger numbers, use binary search
    left, right = 0, len(squares) - 1
    
    while left <= right:
        mid = (left + right) // 2
        if squares[mid] == num:
            return True
        elif squares[mid] < num:
            left = mid + 1
        else:
            right = mid - 1
    
    return False

def divisible_by_digits(num):
    """Check if a number is divisible by each of its digits."""
    if num == 0:
        return False
    
    original_num = num
    while num > 0:
        digit = num % 10
        if digit == 0 or original_num % digit != 0:
            return False
        num //= 10

    return True

def odd_palindrome(num):
    """Check if a number is odd and a palindrome."""
    if num%2 != 1:
        return False
    num_str = str(num)
    
    
    # Check if the string reads the same forward and backward
    return num_str == num_str[::-1]

def row_condition(row, number):
    """Check if a number satisfies the condition for a specific row."""
    if number < 10:  # Numbers must be at least 2 digits
        return False
        
    if row == 0:
        return is_square(number)  # Row 0: Perfect square
    elif row == 1:
        return product_of_digits(number) == 20  # Row 1: Product of digits = 20
    elif row == 2:
        return number % 13 == 0  # Row 2: Multiple of 13
    elif row == 3:
        return number % 32 == 0  # Row 3: Multiple of 32
    elif row == 4:
        return divisible_by_digits(number)  # Row 4: Divisible by each of its digits
    elif row == 5:
        return product_of_digits(number) == 25  # Row 5: Product of digits = 25
    elif row == 6:
        return divisible_by_digits(number)  # Row 6: Divisible by each of its digits
    elif row == 7:
        return odd_palindrome(number)  # Row 7: Odd and a palindrome
    elif row == 8:
        return is_fib(number)  # Row 8: Fibonacci
    elif row == 9:
        return product_of_digits(number) == 2025  # Row 9: Product of digits = 2025
    elif row == 10:
        return is_prime(number)  # Row 10: Prime
    return False


for region_id, segments in regions.items():
    for (r, c), l in segments:
        if not (0 <= r < GRID_SIZE):
            print(f"Warning: Region {region_id} row {r} out of bounds.")
            continue
        if not (0 <= c < GRID_SIZE):
            print(f"Warning: Region {region_id} col {c} out of bounds.")
            continue
        if not (c + l - 1 < GRID_SIZE):
             print(f"Warning: Region {region_id} segment starts at ({r},{c}) with length {l}, exceeds col boundary {GRID_SIZE-1}.")
             continue
        for i in range(l):
            grid[r][c + i] = region_id

# --- Step 2: Determine adjacency between regions ---
adjacency_list = {region_id: set() for region_id in regions}

dr = [-1, 1, 0, 0] # up, down, left, right
dc = [0, 0, -1, 1]

for r in range(GRID_SIZE):
    for c in range(GRID_SIZE):
        region1_id = grid[r][c]
        if region1_id is not None:
            for i in range(4):
                nr, nc = r + dr[i], c + dc[i]
                if 0 <= nr < GRID_SIZE and 0 <= nc < GRID_SIZE:
                    region2_id = grid[nr][nc]
                    assert region2_id is not None
                    if region2_id is not None and region1_id != region2_id:
                        adjacency_list[region1_id].add(region2_id)
                        adjacency_list[region2_id].add(region1_id) # Ensure symmetry


region_ids = list(regions.keys())
num_solutions = 0
all_solutions = []

def is_consistent(region_id, color, assignment):
    """Checks if assigning color to region_id is consistent with current assignment."""
    for neighbor_id in adjacency_list[region_id]:
        # If neighbor is already colored and has the same color, it's inconsistent
        if neighbor_id in assignment and assignment[neighbor_id] == color:
            return False
    return True

def solve(region_index, assignment):
    """Backtracking function to find valid colorings."""
    global num_solutions, all_solutions

    # Base case: All regions have been assigned a color
    if region_index == len(region_ids):
        num_solutions += 1
        # Store a copy of the solution (assignment is modified by backtracking)
        all_solutions.append(assignment.copy())
        return

    current_region_id = region_ids[region_index]
    possible_colors = possible_colorings[current_region_id]

    # Try each possible color for the current region
    for color in possible_colors:
        # Check if the assignment is valid with respect to already colored neighbors
        if is_consistent(current_region_id, color, assignment):
            # Make the assignment
            assignment[current_region_id] = color

            # Recurse to the next region
            solve(region_index + 1, assignment)

            # Backtrack: Remove the assignment to explore other colors for the current region
            del assignment[current_region_id]

# --- Step 4: Run the solver and print results ---
print("Starting search for configurations...")
solve(0, {}) # Start with the first region (index 0) and an empty assignment


marked = {}
for region, values in regions.items():
    for value in values:
        sx, sy = value[0]
        length = value[1]

        for l in range(length):
            grid[sx][sy + l] = region

            if (sx, sy+l) not in marked:
                marked[(sx, sy+l)] = region 
            else:
                print(sx, sy+l, "repeated by", marked[(sx, sy+l)])

for row in range(len(grid)):
    for col in range(len(grid[0])):
        if grid[row][col] == 0:
            print("Left unassigned", row, col)

for row in grid:
    print(','.join(row))


def collect_regions(marked):
    region_grid = {}
    for (x, y), region in marked.items():
        if int(region) not in region_grid:
            region_grid[int(region)] = []
        region_grid[int(region)].append((x, y))

    return region_grid


region_dict = collect_regions(marked)

def mark_region(region_dict, region, number):
    for (x, y) in region_dict[region]:
        grid[x][y] = number

def print_grid():
    for row in grid:
        print(row)



import random
def first_row_search(start_percentile=0, end_percentile=100, process_id=None):
    """
    Process a specific percentile range of solutions.
    
    Args:
        start_percentile: Starting percentile of solutions to process (0-100)
        end_percentile: Ending percentile of solutions to process (0-100)
        process_id: Optional ID for logging purposes
    """
    log_prefix = f"[Process {process_id}]" if process_id is not None else ""
    print(f"{log_prefix} Solutions size: {len(all_solutions)}")
    print(f"{log_prefix} Processing solutions from {start_percentile}% to {end_percentile}%")
    
    # Calculate indices based on percentiles
    start_idx = int(len(all_solutions) * start_percentile / 100)
    end_idx = int(len(all_solutions) * end_percentile / 100)
    
    # Get the slice of solutions to process
    solutions_to_process = all_solutions[start_idx:end_idx]
    print(f"{log_prefix} Processing {len(solutions_to_process)} solutions")
    
    # Process each solution in the range
    for i, solution in enumerate(solutions_to_process):
        print(f"{log_prefix} starting {solution}")

        if i % 10 == 0:  # Print progress every 10 solutions
            print(f"{log_prefix} Processed {i}/{len(solutions_to_process)} solutions")
        
        for region, digit in solution.items():
            mark_region(region_dict, int(region), digit)

        print_grid()
        
        c_set.clear()
        backtrack(0, 0)


#def algorithm
def collect_numbers(row):
    current_number = 0
    result = []
    for number in grid[row]:
        if number == 0:
            if current_number != 0:
                result.append(current_number)
            current_number = 0 
        else:
            current_number = current_number*10 + number


    if current_number != 0:
        result.append(current_number)
    return result

def satisfy_condition(row, f):
    return all(map(lambda x: f(x), collect_numbers(row)))

def product_of_digits(number):
    """Calculate the product of all digits in a number."""
    if number == 0:
        return 0
    
    product = 1
    while number > 0:
        product *= number % 10
        number //= 10
    
    return product

def can_become_product_20(number):
    """
    Check if a number can potentially have its digits increased to reach a product of 20.
    Filters out numbers that can't possibly become product 20 through any digit increase.
    """
    # If product is already 20, return True
    current_product = product_of_digits(number)
    if current_product == 20:
        return True
    
    # If product > 20, we can't decrease it to get 20
    if current_product > 20:
        return False
    
    # Extract digits
    digits = []
    temp = number
    while temp > 0:
        digits.append(temp % 10)
        temp //= 10
    
    # Count digits greater than 4
    if 6 in digits or 7 in digits or 8 in digits or 9 in digits:
        return False
    
    return True

# Store valid configurations from Phase 1
phase1_seeds = []
# Pre-compute squares for binary search
squares = [i*i for i in range(1, int(1e6))]

def is_square(num):
    """
    Check if a number is a perfect square using binary search.
    """
    left, right = 0, len(squares) - 1
    
    while left <= right:
        mid = (left + right) // 2
        if squares[mid] == num:
            return True
        elif squares[mid] < num:
            left = mid + 1
        else:
            right = mid - 1
    
    return False

def copy_grid():
    """Create a deep copy of the current grid."""
    return [row[:] for row in grid]

def divisible_by_digits(num):
    digits = []
    number = num
    while num:
        digits.append(num%10)
        num //= 10

    for digit in digits:
        if number%digit != 0:
            return False
    return True

def is_fib(num):
    if num in first_50_fibs:
        return True
    return False

c_set = set()

def add_to_set(numbers):
    for num in numbers:
        c_set.add(num)

def remove_from_set(numbers):
    for num in numbers:
        c_set.remove(num)

def none_in_set(numbers):
    return all(num not in c_set for num in numbers)

#tiles = [[1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
#         [0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1],
#         [1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0],
#         [0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1]]

tiles = []

def extract_number_ending_at(row, end_col):
    """Extract a number that ends at the specified position."""
    # Find the start of the number (tile or start of row)
    start_col = end_col
    while start_col > 0 and grid[row][start_col-1] != 0:
        start_col -= 1
        
    # Extract and convert the number
    number = 0
    for i in range(start_col, end_col):
        number = number * 10 + grid[row][i]
            
    return number

def backtrack(row, col):
    global current_numbers_set
    global tiles

    if col >= len(grid) and row == 1:
        numbers = collect_numbers(0)
        numbers_2 = collect_numbers(1)
        
        # Check if all numbers are squares using binary search
        all_squares = all(row_condition(0, num) for num in numbers)

        if len(numbers) != len(set(numbers)):
            return

        # Check if all numbers in numbers_2 are at most one digit change away from having product 20
        valid_product_20 = all(can_become_product_20(num) for num in numbers_2)

        # can't make this on a result because it's yellow
        if grid[1][2:6] == [0, 2, 2, 0]:
            return

        if all_squares and valid_product_20:
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return

    if col >= len(grid) and row == 2:
        numbers = collect_numbers(1)

        if len(numbers) != len(set(numbers)):
            return

        if all(row_condition(1, num) for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            #print(grid[0], grid[1], grid[2], "Seed2")
            backtrack(row+1,0)
            remove_from_set(numbers)
            return
        return

    if col >= len(grid) and row == 3:
        numbers = collect_numbers(2)

        if len(numbers) != len(set(numbers)):
            return

        if all(row_condition(row-1, num) for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
            #return
        return

    if col >= len(grid) and row == 4:
        numbers = collect_numbers(row-1)


        if all(row_condition(row-1, num) for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            #print(grid[0], grid[1], grid[2], grid[3], grid[4], "Seed4")
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return
    
    if col >= len(grid) and row == 5:
        numbers = collect_numbers(row-1)

        if len(numbers) != len(set(numbers)):
            return

        valid = True
        if all(row_condition(row-1, num) for num in numbers) and valid and none_in_set(numbers):
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
            #print(grid, "Seed5")
            #sys.exit()
            return
        return

    if col >= len(grid) and row == 6:
        numbers = collect_numbers(row-1)

        if len(numbers) != len(set(numbers)):
            return

        #if all(product_of_digits(num) == 25 for num in numbers):
        #    print("Found duplicate solution ", grid[5], grid[6])

        if all(product_of_digits(num) == 25 for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return

    if col >= len(grid) and row == 7:
        numbers = collect_numbers(row-1)

        if len(numbers) != len(set(numbers)):
            return

        if all(row_condition(row-1,num) for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return

    if col >= len(grid) and row == 8:
        numbers = collect_numbers(row-1)

        if len(numbers) != len(set(numbers)):
            return

        if all(row_condition(row-1,num) for num in numbers) and none_in_set(numbers):
            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return
    
    if col >= len(grid) and row == 9:
        numbers = collect_numbers(row-1)
        if len(numbers) != len(set(numbers)):
            return

        #print("Valid numbers", [row_condition(row-1, num) for num in numbers], numbers)

        if all(row_condition(row-1, num) for num in numbers) and none_in_set(numbers):

            add_to_set(numbers)
            backtrack(row+1, 0)
            remove_from_set(numbers)
        return

    if col >= len(grid) and row == 10:
        numbers = collect_numbers(row-1)
        numbers2 = collect_numbers(row)

        if len(numbers) != len(set(numbers)):
            return

        if all(row_condition(row-1, num) for num in numbers) and all(row_condition(row, num) for num in numbers2) and none_in_set(numbers):
            add_to_set(numbers)
            add_to_set(numbers2)
            backtrack(row+1, 0)
            print("Found answer", c_set)
        return
    
    if row == 11:
        return


    if col >= len(grid):
        backtrack(row+1, 0)
        return

    if row > 0 and grid[row-1][col] == 0:
        number = extract_number_ending_at(row-1, col)
            
        if number != 0 and (not row_condition(row-1, number) or number in c_set):
            #print("Number at", row-1, number, col, grid[row-1][col])
            return

    backtrack(row, col+1)
        
    if (row, col) not in yellow:
        can = True
        if col > 0 and grid[row][col-1] == 0:
            can = False

        if col > 1 and grid[row][col-2] == 0:
            can = False

        if row > 0 and grid[row-1][col] == 0:
            can = False

        if col == 1 or col == 9: 
            can = False

        if row < len(tiles):
            if tiles[row][col] == 0:
                can = False

        if can:
            prev_value = grid[row][col]
            grid[row][col] = 0
            
            # Get all valid orthogonally adjacent cells
            adjacent_cells = []
            # Check up
            if row > 0 and grid[row-1][col] != 0 and (row-1, col) not in yellow:
                adjacent_cells.append((row-1, col))
            # Check down
            if row < len(grid)-1 and grid[row+1][col] != 0 and (row+1, col) not in yellow:
                adjacent_cells.append((row+1, col))
            # Check left
            if col > 0 and grid[row][col-1] != 0 and (row, col-1) not in yellow:
                adjacent_cells.append((row, col-1))
            # Check right
            if col < len(grid[0])-1 and grid[row][col+1] != 0 and (row, col+1) not in yellow:
                adjacent_cells.append((row, col+1))
            
            # Store original values of adjacent cells
            # Distribute the previous value in all possible ways
            def distribute_value(value, cells, index=0):
                if index == len(cells):
                    if value == 0:  # All value has been distributed
                        backtrack(row, col+1)
                    return
                
                r, c = cells[index]
                # Try different amounts to add to this cell
                for amount in range(value + 1):
                    if grid[r][c] + amount <= 9:
                        grid[r][c] += amount
                        distribute_value(value - amount, cells, index + 1)
                        grid[r][c] -= amount  # Restore original value
            
            # Start distribution if there are adjacent cells
            if adjacent_cells:
                distribute_value(prev_value, adjacent_cells)
                # If no adjacent cells, just proceed with backtracking
            
            grid[row][col] = prev_value
                

def find_adjacent_regions(region_id):
    """Find all regions adjacent to the given region."""
    region_cells = region_dict[int(region_id)]
    adjacent_regions = set()
    
    for x, y in region_cells:
        # Check all four directions
        for dx, dy in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
            nx, ny = x + dx, y + dy
            if 0 <= nx < 11 and 0 <= ny < 11:
                if (nx, ny) in marked and marked[(nx, ny)] != region_id:
                    adjacent_regions.add(marked[(nx, ny)])
    
    return adjacent_regions

def get_valid_colorings(region_id, colored_regions):
    """
    Get valid colorings for a region based on adjacency constraints.
    No adjacent region can have the same starting digit.
    """
    adjacent_regions = find_adjacent_regions(region_id)
    used_digits = set()
    
    for adj_region in adjacent_regions:
        if adj_region in colored_regions:
            # Get the first digit of the coloring
            first_digit = colored_regions[adj_region]
            used_digits.add(first_digit)
    
    # Return valid colorings that don't conflict with adjacent regions
    return [color for color in possible_colorings[region_id] if color not in used_digits]


# Run Phase 1 to collect seeds
print(len(all_solutions))

if __name__ == "__main__":
    # Set up command-line argument parsing
    parser = argparse.ArgumentParser(description='Process a range of solutions for Jane')
    parser.add_argument('--start-percentile', type=float, default=0.0,
                        help='Starting percentile of solutions to process (0-100)')
    parser.add_argument('--end-percentile', type=float, default=100.0,
                        help='Ending percentile of solutions to process (0-100)')
    parser.add_argument('--process-id', type=int, default=None,
                        help='Process ID for logging purposes')
    
    # Parse arguments
    args = parser.parse_args()
    
    # Print regions, adjacency list, and other initialization info
    print(f"[Process {args.process_id}] Starting Jane with solution range {args.start_percentile}%-{args.end_percentile}%")
    
    # Run first_row_search with specified percentile range
    first_row_search(args.start_percentile, args.end_percentile, args.process_id)

