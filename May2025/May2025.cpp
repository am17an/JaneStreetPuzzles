#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <tuple>
#include <array>
#include <cstring>
#include <memory>
#include <bitset>

// Hash function for std::pair<int, int>
namespace std {
    template<>
    struct hash<pair<int, int>> {
        size_t operator()(const pair<int, int>& p) const {
            // Combine the hash of both integers
            return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
        }
    };
}

// Function for memoization (similar to @cache in Python)
std::unordered_map<int, long long> fibs_cache;

// Grid initialization
std::vector<std::vector<int>> grid(11, std::vector<int>(11, 0));

// Get Fibonacci numbers
long long getfibs(int n) {
    if (fibs_cache.find(n) != fibs_cache.end()) {
        return fibs_cache[n];
    }
    
    if (n == 0 || n == 1) {
        fibs_cache[n] = 1;
        return 1;
    }
    
    fibs_cache[n] = getfibs(n-1) + getfibs(n-2);
    return fibs_cache[n];
}

// Check if a number is prime
bool is_prime(long long n) {
    // Handle base cases
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    
    // Only check divisors of form 6k ± 1 up to sqrt(n)
    long long i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
        i += 6;
    }
    
    return true;
}

// First 50 Fibonacci numbers
std::unordered_map<long long, int> first_50_fibs;

// Regions definition
std::map<std::string, std::vector<std::tuple<std::pair<int, int>, int>>> regions = {
    {"0", {{{0, 0}, 11}, {{1, 0}, 1}, {{1, 2}, 9}, {{2, 9}, 1}}},
    {"1", {{{1, 1}, 1}, {{2, 0}, 2}, {{3, 0}, 1}, {{4, 0}, 1}, {{5, 0}, 5}, {{6, 0}, 1}, {{6, 4}, 2}, {{7, 0}, 1}, {{3, 3}, 1}, {{4, 3}, 1}}},
    {"2", {{{2, 2}, 4}, {{3, 1}, 2}, {{4, 1}, 2}, {{3, 4}, 1}, {{4, 4}, 1}}},
    {"3", {{{2, 6}, 3}, {{2, 10}, 1}, {{3, 6}, 5}, {{4, 7}, 2}, {{4, 10}, 1}, {{5, 10}, 1}}},
    {"4", {{{3, 5}, 1}, {{4, 5}, 2}, {{5, 5}, 5}, {{6, 6}, 2}, {{6, 9}, 2}, {{4, 9}, 1}}},
    {"5", {{{6, 2}, 2}, {{6, 8}, 1}, {{7, 2}, 7}, {{8, 4}, 1}, {{8, 6}, 1}}},
    {"6", {{{8, 0}, 4}, {{8, 5}, 1}, {{9, 0}, 11}, {{10, 0}, 2}, {{10, 8}, 3}, {{6, 1}, 1}, {{7, 1}, 1}}},
    {"7", {{{10, 2}, 6}}},
    {"8", {{{7, 9}, 2}, {{8, 7}, 4}}}
};

//Solving:0:2 1:4 2:3 3:4 4:1 5:6 6:3 7:7 8:7
// Possible colorings for regions
std::map<std::string, std::vector<int>> possible_colorings = {
    {"0", {2}},
    {"1", {4}},
    {"2", {3}},
    {"3", {4}},
    {"4", {1}},
    {"5", {6}},
    {"6", {3}},
    {"7", {7}},
    {"8", {7}}
};

// Yellow cells
std::vector<std::pair<int, int>> yellow = {
    {1, 3}, {1, 4}, {2, 4}, 
    {2, 9}, {3, 9}, {3, 8},
    {5, 5}, {6, 5}, {6, 6}, 
    {6, 1}, {6, 2}, {7, 1}, {7, 5},
    {8, 4}, {8, 5}, {9, 4}
};

const int GRID_SIZE = 11;

// Map to mark grid cells
std::unordered_map<std::pair<int, int>, std::string> marked;

// Pre-compute squares for binary search
std::vector<long long> squares;

// Function to check if a number is a perfect square
bool is_square(long long num) {
    // For small numbers, check directly
    if (num < 10000) {
        long long root = static_cast<long long>(sqrt(num));
        return root * root == num;
    }
    
    // For larger numbers, use binary search
    int left = 0, right = static_cast<int>(squares.size()) - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        if (squares[mid] == num) {
            return true;
        } else if (squares[mid] < num) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return false;
}

// Check if a number is divisible by each of its digits
bool divisible_by_digits(long long num) {
    if (num == 0) {
        return false;
    }
    
    long long original_num = num;
    while (num > 0) {
        int digit = num % 10;
        if (digit == 0 || original_num % digit != 0) {
            return false;
        }
        num /= 10;
    }
    
    return true;
}

// Check if a number is odd and a palindrome
bool odd_palindrome(long long num) {
    if (num % 2 != 1) {
        return false;
    }
    
    std::string num_str = std::to_string(num);
    std::string reversed = num_str;
    std::reverse(reversed.begin(), reversed.end());
    
    return num_str == reversed;
}

// Product of digits
long long product_of_digits(long long number) {
    if (number == 0) {
        return 0;
    }
    
    long long product = 1;
    while (number > 0) {
        product *= number % 10;
        number /= 10;
    }
    
    return product;
}

// Check if a number satisfies row-specific conditions
bool row_condition(int row, long long number) {
    if (number < 10) {  // Numbers must be at least 2 digits
        return false;
    }
    
    if (row == 0) {
        return is_square(number);  // Row 0: Perfect square
    } else if (row == 1) {
        return product_of_digits(number) == 20;  // Row 1: Product of digits = 20
    } else if (row == 2) {
        return number % 13 == 0;  // Row 2: Multiple of 13
    } else if (row == 3) {
        return number % 32 == 0;  // Row 3: Multiple of 32
    } else if (row == 4) {
        return divisible_by_digits(number);  // Row 4: Divisible by each of its digits
    } else if (row == 5) {
        return product_of_digits(number) == 25;  // Row 5: Product of digits = 25
    } else if (row == 6) {
        return divisible_by_digits(number);  // Row 6: Divisible by each of its digits
    } else if (row == 7) {
        return odd_palindrome(number);  // Row 7: Odd and a palindrome
    } else if (row == 8) {
        return first_50_fibs.find(number) != first_50_fibs.end();  // Row 8: Fibonacci
    } else if (row == 9) {
        return product_of_digits(number) == 2025;  // Row 9: Product of digits = 2025
    } else if (row == 10) {
        return is_prime(number);  // Row 10: Prime
    }
    return false;
}

// Adjacency list for regions
std::map<std::string, std::set<std::string>> adjacency_list;

// Check if assigning a color to a region is consistent with current assignment
bool is_consistent(const std::string& region_id, int color, const std::map<std::string, int>& assignment) {
    for (const auto& neighbor_id : adjacency_list[region_id]) {
        if (assignment.find(neighbor_id) != assignment.end() && assignment.at(neighbor_id) == color) {
            return false;
        }
    }
    return true;
}

// Variables for solutions
int num_solutions = 0;
std::vector<std::map<std::string, int>> all_solutions;
std::vector<std::string> region_ids;

// Solve function for finding valid colorings
void solve(int region_index, std::map<std::string, int>& assignment) {
    // Base case: All regions have been assigned a color
    if (region_index == static_cast<int>(region_ids.size())) {
        num_solutions++;
        // Store a copy of the solution
        all_solutions.push_back(assignment);
        return;
    }
    
    std::string current_region_id = region_ids[region_index];
    std::vector<int> possible_colors = possible_colorings[current_region_id];
    
    // Try each possible color for the current region
    for (int color : possible_colors) {
        // Check if the assignment is valid with respect to already colored neighbors
        if (is_consistent(current_region_id, color, assignment)) {
            // Make the assignment
            assignment[current_region_id] = color;
            
            // Recurse to the next region
            solve(region_index + 1, assignment);
            
            // Backtrack: Remove the assignment to explore other colors for the current region
            assignment.erase(current_region_id);
        }
    }
}

// Collect regions from the marked map
std::map<int, std::vector<std::pair<int, int>>> collect_regions(const std::unordered_map<std::pair<int, int>, std::string>& marked) {
    std::map<int, std::vector<std::pair<int, int>>> region_grid;
    
    for (const auto& [pos, region] : marked) {
        int region_int = std::stoi(region);
        if (region_grid.find(region_int) == region_grid.end()) {
            region_grid[region_int] = std::vector<std::pair<int, int>>();
        }
        region_grid[region_int].push_back(pos);
    }
    
    return region_grid;
}

// Mark a region with a specific number
void mark_region(const std::map<int, std::vector<std::pair<int, int>>>& region_dict, int region, int number) {
    for (const auto& pos : region_dict.at(region)) {
        grid[pos.first][pos.second] = number;
    }
}

// Print the grid
void print_grid() {
    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

// Set to keep track of numbers
std::unordered_set<long long> c_set;

// Add numbers to the set
void add_to_set(const std::vector<long long>& numbers) {
    for (long long num : numbers) {
        c_set.insert(num);
    }
}

// Remove numbers from the set
void remove_from_set(const std::vector<long long>& numbers) {
    for (long long num : numbers) {
        c_set.erase(num);
    }
}

// Check if none of the numbers are in the set
bool none_in_set(const std::vector<long long>& numbers) {
    for (long long num : numbers) {
        if (c_set.find(num) != c_set.end()) {
            return false;
        }
    }
    return true;
}

// Collect numbers from a row
std::vector<long long> collect_numbers(int row) {
    long long current_number = 0;
    std::vector<long long> result;
    
    for (int number : grid[row]) {
        if (number == 0) {
            if (current_number != 0) {
                result.push_back(current_number);
            }
            current_number = 0;
        } else {
            current_number = current_number * 10 + number;
        }
    }
    
    if (current_number != 0) {
        result.push_back(current_number);
    }
    
    return result;
}

// Check if a number can potentially have its digits increased to reach a product of 20
bool can_become_product_20(long long number) {
    // If product is already 20, return true
    long long current_product = product_of_digits(number);
    if (current_product == 20) {
        return true;
    }
    
    // If product > 20, we can't decrease it to get 20
    if (current_product > 20) {
        return false;
    }
    
    // Extract digits
    std::vector<int> digits;
    long long temp = number;
    while (temp > 0) {
        digits.push_back(temp % 10);
        temp /= 10;
    }
    
    // Count digits greater than 4
    for (int digit : digits) {
        if (digit >= 6 && digit <= 9) {
            return false;
        }
    }
    
    return true;
}

// Extract a number that ends at the specified position
long long extract_number_ending_at(int row, int end_col) {
    // Find the start of the number (tile or start of row)
    int start_col = end_col;
    while (start_col > 0 && grid[row][start_col-1] != 0) {
        start_col--;
    }
    
    // Extract and convert the number
    long long number = 0;
    for (int i = start_col; i < end_col; i++) {
        number = number * 10 + grid[row][i];
    }
    
    return number;
}

// Forward declaration for backtrack function
void backtrack(int row, int col);

// Function for distributing value in the backtrack function
void distribute_value(int value, std::vector<std::pair<int, int>>& cells, int index = 0, int row = 0, int col = 0) {
    if (index == static_cast<int>(cells.size())) {
        if (value == 0) {  // All value has been distributed
            backtrack(row, col+1);
        }
        return;
    }
    
    int r = cells[index].first;
    int c = cells[index].second;
    
    // Try different amounts to add to this cell
    for (int amount = 0; amount <= value; amount++) {
        if (grid[r][c] + amount <= 9) {
            grid[r][c] += amount;
            distribute_value(value - amount, cells, index + 1, row, col);
            grid[r][c] -= amount;  // Restore original value
        }
    }
}

// Check if a position is in the yellow cells
bool is_yellow(int row, int col) {
    return std::find(yellow.begin(), yellow.end(), std::make_pair(row, col)) != yellow.end();
}

// Backtrack function for solving the grid
void backtrack(int row, int col) {
    if (col >= GRID_SIZE && row == 1) {
        std::vector<long long> numbers = collect_numbers(0);
        std::vector<long long> numbers_2 = collect_numbers(1);
        
        // Check if all numbers are squares
        bool all_squares = true;
        for (long long num : numbers) {
            if (!row_condition(0, num)) {
                all_squares = false;
                break;
            }
        }
        
        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }
        
        // Check for product 20 condition
        bool valid_product_20 = true;
        for (long long num : numbers_2) {
            if (!can_become_product_20(num)) {
                valid_product_20 = false;
                break;
            }
        }
        
        // Check a specific condition
        if (grid[1][2] == 0 && grid[1][3] == 2 && grid[1][4] == 2 && grid[1][5] == 0) {
            return;
        }
        
        if (all_squares && valid_product_20) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }

    if (col >= GRID_SIZE && row == 2) {
        std::vector<long long> numbers = collect_numbers(1);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
            return;
        }
        return;
    }

    if (col >= GRID_SIZE && row == 3) {
        std::vector<long long> numbers = collect_numbers(2);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }

    if (col >= GRID_SIZE && row == 4) {
        std::vector<long long> numbers = collect_numbers(row-1);

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }
    
    if (col >= GRID_SIZE && row == 5) {
        std::vector<long long> numbers = collect_numbers(row-1);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
            return;
        }
        return;
    }

    if (col >= GRID_SIZE && row == 6) {
        std::vector<long long> numbers = collect_numbers(row-1);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (product_of_digits(num) != 25) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }

    if (col >= GRID_SIZE && row == 7) {
        std::vector<long long> numbers = collect_numbers(row-1);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }

    if (col >= GRID_SIZE && row == 8) {
        std::vector<long long> numbers = collect_numbers(row-1);

        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            // Print grid for debugging
            //print_grid();
            
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }
    
    if (col >= GRID_SIZE && row == 9) {
        std::vector<long long> numbers = collect_numbers(row-1);
        
        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            //print_grid();
            //std::exit(0);  // Exit program when solution is found
            
            add_to_set(numbers);
            backtrack(row+1, 0);
            remove_from_set(numbers);
        }
        return;
    }

    if (col >= GRID_SIZE && row == 10) {
        std::vector<long long> numbers = collect_numbers(row-1);

        std::vector<long long> numbers_2 = collect_numbers(row);
        
        // Check for duplicates
        std::unordered_set<long long> unique_numbers(numbers.begin(), numbers.end());
        if (unique_numbers.size() != numbers.size()) {
            return;
        }

        bool valid = true;
        for (long long num : numbers) {
            if (!row_condition(row-1, num)) {
                valid = false;
                break;
            }
        }

        if (valid && none_in_set(numbers)) {
            std::cout << "Reached row 10, numbers: ";

            for (long long num : numbers_2) {
                if (!row_condition(row, num)) {
                    valid = false;
                    break;
                }
            }

            if(valid) {
                add_to_set(numbers);
                add_to_set(numbers_2);
                std::cout << "Solution found" << std::endl;
                int ans = 0;
                for(auto number: c_set) {
                    ans += number;
                    std::cout << number << " ";
                }
                std::cout << "Sum: " << ans << std::endl;
                print_grid();
            }
        }
        return;
    }
    
    if (row == 11) {
        return;
    }

    if (col >= GRID_SIZE) {
        backtrack(row+1, 0);
        return;
    }

    if (row > 0 && grid[row-1][col] == 0) {
        long long number = extract_number_ending_at(row-1, col);
            
        if (number != 0 && (!row_condition(row-1, number) || c_set.find(number) != c_set.end())) {
            return;
        }
    }

    backtrack(row, col+1);
        
    if (!is_yellow(row, col)) {
        bool can = true;
        if (col > 0 && grid[row][col-1] == 0) {
            can = false;
        }

        if (col > 1 && grid[row][col-2] == 0) {
            can = false;
        }

        if (row > 0 && grid[row-1][col] == 0) {
            can = false;
        }

        if (col == 1 || col == 9) {
            can = false;
        }

        if (can) {
            int prev_value = grid[row][col];
            grid[row][col] = 0;
            
            // Get all valid orthogonally adjacent cells
            std::vector<std::pair<int, int>> adjacent_cells;
            
            // Check up
            if (row > 0 && grid[row-1][col] != 0 && !is_yellow(row-1, col)) {
                adjacent_cells.push_back({row-1, col});
            }
            
            // Check down
            if (row < GRID_SIZE-1 && grid[row+1][col] != 0 && !is_yellow(row+1, col)) {
                adjacent_cells.push_back({row+1, col});
            }
            
            // Check left
            if (col > 0 && grid[row][col-1] != 0 && !is_yellow(row, col-1)) {
                adjacent_cells.push_back({row, col-1});
            }
            
            // Check right
            if (col < GRID_SIZE-1 && grid[row][col+1] != 0 && !is_yellow(row, col+1)) {
                adjacent_cells.push_back({row, col+1});
            }
            
            // Distribute the previous value in all possible ways
            if (!adjacent_cells.empty()) {
                distribute_value(prev_value, adjacent_cells, 0, row, col);
            }
            
            grid[row][col] = prev_value;
        }
    }
}

// Find adjacent regions to a given region
std::set<std::string> find_adjacent_regions(const std::string& region_id) {
    int region_id_int = std::stoi(region_id);
    auto region_dict = collect_regions(marked);
    const auto& region_cells = region_dict[region_id_int];
    
    std::set<std::string> adjacent_regions;
    
    for (const auto& [x, y] : region_cells) {
        // Check all four directions
        const std::array<std::pair<int, int>, 4> directions = {{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
        
        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < 11 && ny >= 0 && ny < 11) {
                std::pair<int, int> new_pos = {nx, ny};
                
                if (marked.find(new_pos) != marked.end() && marked[new_pos] != region_id) {
                    adjacent_regions.insert(marked[new_pos]);
                }
            }
        }
    }
    
    return adjacent_regions;
}

// Get valid colorings for a region based on adjacency constraints
std::vector<int> get_valid_colorings(const std::string& region_id, const std::map<std::string, int>& colored_regions) {
    std::set<std::string> adjacent_regions = find_adjacent_regions(region_id);
    std::unordered_set<int> used_digits;
    
    for (const auto& adj_region : adjacent_regions) {
        if (colored_regions.find(adj_region) != colored_regions.end()) {
            // Get the first digit of the coloring
            int first_digit = colored_regions.at(adj_region);
            used_digits.insert(first_digit);
        }
    }
    
    // Return valid colorings that don't conflict with adjacent regions
    std::vector<int> valid_colors;
    for (int color : possible_colorings[region_id]) {
        if (used_digits.find(color) == used_digits.end()) {
            valid_colors.push_back(color);
        }
    }
    
    return valid_colors;
}

// Function for first row search
void first_row_search(float start_percentile = 0.0, float end_percentile = 100.0, int process_id = -1) {
    std::string log_prefix = (process_id != -1) ? "[Process " + std::to_string(process_id) + "]" : "";
    
    std::cout << log_prefix << " Solutions size: " << all_solutions.size() << std::endl;
    std::cout << log_prefix << " Processing solutions from " << start_percentile << "% to " << end_percentile << "%" << std::endl;
    
    // Calculate indices based on percentiles
    int start_idx = static_cast<int>(all_solutions.size() * start_percentile / 100.0);
    int end_idx = static_cast<int>(all_solutions.size() * end_percentile / 100.0);
    
    // Get the slice of solutions to process
    std::vector<std::map<std::string, int>> solutions_to_process(
        all_solutions.begin() + start_idx,
        all_solutions.begin() + end_idx
    );
    
    std::cout << log_prefix << " Processing " << solutions_to_process.size() << " solutions" << std::endl;
    
    auto region_dict = collect_regions(marked);
    
    // Process each solution in the range
    for (size_t i = 0; i < solutions_to_process.size(); i++) {
        auto& solution = solutions_to_process[i];

        std::cout << "Solving:";
        for(auto& [region, digit] : solution) {
            std::cout << region << ":" << digit << " ";
        };
        std::cout << std::endl;
        
        if (i % 10 == 0) {  // Print progress every 10 solutions
            std::cout << log_prefix << " Processed " << i << "/" << solutions_to_process.size() << " solutions" << std::endl;
        }
        
        // Reset the grid to all zeros before applying this solution
        for (int r = 0; r < GRID_SIZE; r++) {
            for (int c = 0; c < GRID_SIZE; c++) {
                grid[r][c] = 0;
            }
        }
        
        // Apply the current solution to the grid
        for (const auto& [region, digit] : solution) {
            mark_region(region_dict, std::stoi(region), digit);
        }
        
        c_set.clear();
        backtrack(0, 0);
    }
}

int main(int argc, char* argv[]) {
    // Initialize the first 50 Fibonacci numbers
    for (int i = 0; i < 50; i++) {
        first_50_fibs[getfibs(i)] = 1;
    }
    
    // Initialize squares for binary search
    for (long long i = 1; i < 1000000; i++) {
        squares.push_back(i * i);
    }
    
    // Process command line arguments
    float start_percentile = 0.0;
    float end_percentile = 100.0;
    int process_id = -1;
    
    // Simple argument parsing
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--start-percentile" && i + 1 < argc) {
            start_percentile = std::stof(argv[++i]);
        } else if (arg == "--end-percentile" && i + 1 < argc) {
            end_percentile = std::stof(argv[++i]);
        } else if (arg == "--process-id" && i + 1 < argc) {
            process_id = std::stoi(argv[++i]);
        }
    }
    
    // Map grid cells to regions
    for (const auto& [region_id, segments] : regions) {
        for (const auto& segment : segments) {
            auto [pos, l] = segment;
            int r = pos.first;
            int c = pos.second;
            
            if (!(0 <= r && r < GRID_SIZE)) {
                std::cout << "Warning: Region " << region_id << " row " << r << " out of bounds." << std::endl;
                continue;
            }
            if (!(0 <= c && c < GRID_SIZE)) {
                std::cout << "Warning: Region " << region_id << " col " << c << " out of bounds." << std::endl;
                continue;
            }
            if (!(c + l - 1 < GRID_SIZE)) {
                std::cout << "Warning: Region " << region_id << " segment starts at (" << r << "," << c 
                          << ") with length " << l << ", exceeds col boundary " << (GRID_SIZE-1) << "." << std::endl;
                continue;
            }
            
            for (int i = 0; i < l; i++) {
                grid[r][c + i] = 0;  // Initialize grid
                marked[{r, c + i}] = region_id;  // Mark the region
            }
        }
    }
    
    // Determine adjacency between regions
    for (const auto& [region_id, _] : regions) {
        adjacency_list[region_id] = std::set<std::string>();
    }
    
    // Define directions for adjacency check
    std::array<int, 4> dr = {-1, 1, 0, 0};  // up, down, left, right
    std::array<int, 4> dc = {0, 0, -1, 1};
    
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            std::pair<int, int> pos = {r, c};
            if (marked.find(pos) != marked.end()) {
                std::string region1_id = marked[pos];
                
                for (int i = 0; i < 4; i++) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    std::pair<int, int> new_pos = {nr, nc};
                    
                    if (0 <= nr && nr < GRID_SIZE && 0 <= nc && nc < GRID_SIZE) {
                        if (marked.find(new_pos) != marked.end()) {
                            std::string region2_id = marked[new_pos];
                            if (region1_id != region2_id) {
                                adjacency_list[region1_id].insert(region2_id);
                                adjacency_list[region2_id].insert(region1_id);  // Ensure symmetry
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Initialize region IDs list
    for (const auto& [region_id, _] : regions) {
        region_ids.push_back(region_id);
    }
    
    // Run the solver
    std::cout << "Starting search for configurations..." << std::endl;
    std::map<std::string, int> initial_assignment;
    solve(0, initial_assignment);  // Start with the first region (index 0) and an empty assignment
    
    std::cout << "Number of solutions found: " << num_solutions << std::endl;
    
    // Print marked regions for verification
    for (const auto& [region_id, segments] : regions) {
        for (const auto& segment : segments) {
            auto [pos, l] = segment;
            int sx = pos.first;
            int sy = pos.second;
            
            for (int i = 0; i < l; i++) {
                std::pair<int, int> grid_pos = {sx, sy + i};
                if (marked.find(grid_pos) == marked.end()) {
                    marked[grid_pos] = region_id;
                } else if (marked[grid_pos] != region_id) {
                    std::cout << sx << " " << sy + i << " repeated by " << marked[grid_pos] << std::endl;
                }
            }
        }
    }
    
    // Check for unassigned cells
    for (int row = 0; row < static_cast<int>(grid.size()); row++) {
        for (int col = 0; col < static_cast<int>(grid[0].size()); col++) {
            if (grid[row][col] == 0 && marked.find({row, col}) == marked.end()) {
                std::cout << "Left unassigned " << row << " " << col << std::endl;
            }
        }
    }
    
    // Print the grid
    for (const auto& row : grid) {
        for (int i = 0; i < static_cast<int>(row.size()); i++) {
            std::cout << row[i];
            if (i < static_cast<int>(row.size()) - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
    
    // Run first_row_search with specified percentile range
    std::cout << "[Process " << process_id << "] Starting Jane with solution range " 
              << start_percentile << "%-" << end_percentile << "%" << std::endl;
    
    // Process all solutions through first_row_search
    if (!all_solutions.empty()) {
        first_row_search(start_percentile, end_percentile, process_id);
    } else {
        std::cout << "No solutions found to process!" << std::endl;
    }
    
    return 0;
}


