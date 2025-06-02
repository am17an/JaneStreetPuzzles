import multiprocessing as mp
import subprocess
import os
import sys
import math

def run_jane_with_range(start_percentile, end_percentile, process_id):
    """Run Jane_backward.py with a specific percentile range of solutions"""
    # Create a log file for this process
    log_file = f"jane_backprocess_{process_id}.log"
    
    # Construct the command to run Jane_backward.py with the range arguments
    command = [
        "./jane_cpp",
        "--start-percentile", str(start_percentile),
        "--end-percentile", str(end_percentile),
        "--process-id", str(process_id)
    ]
    
    # Run the process and redirect output to the log file
    with open(log_file, 'w') as log:
        process = subprocess.Popen(
            command,
            stdout=log,
            stderr=subprocess.STDOUT,
            text=True
        )
        return process

def parallelize_jane(num_processes=None):
    """Launch multiple instances of Jane_backward.py with different percentile ranges"""
    if num_processes is None:
        num_processes = mp.cpu_count()
    
    print(f"Launching {num_processes} processes to run Jane_backward.py")
    
    # Calculate percentile ranges for each process
    percentile_step = 100.0 / num_processes
    ranges = []
    
    for i in range(num_processes):
        start = i * percentile_step
        end = (i + 1) * percentile_step if i < num_processes - 1 else 100.0
        ranges.append((start, end))
    
    # Launch all processes
    processes = []
    for i, (start, end) in enumerate(ranges):
        process = run_jane_with_range(start, end, i)
        processes.append(process)
        print(f"Launched process {i} to handle percentile range {start:.2f}-{end:.2f}")
    
    # Wait for all processes to complete
    for i, process in enumerate(processes):
        return_code = process.wait()
        print(f"Process {i} completed with return code {return_code}")
    
    # Combine results (if needed)
    print("All processes completed. Check individual log files for results.")

if __name__ == "__main__":
    # Get number of processes from command-line argument, if provided
    num_processes = None
    if len(sys.argv) > 1:
        try:
            num_processes = int(sys.argv[1])
        except ValueError:
            print(f"Invalid number of processes: {sys.argv[1]}")
            sys.exit(1)
    
    # Run Jane_backward.py in parallel
    parallelize_jane(num_processes) 
