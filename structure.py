import os

def print_directory_structure(root_dir):
    for root, dirs, files in os.walk(root_dir):
        # Print current directory path
        level = root.replace(root_dir, '').count(os.sep)
        indent = ' ' * 4 * level
        print(f"{indent}[{os.path.basename(root)}/]")
        
        # Print files in the current directory
        sub_indent = ' ' * 4 * (level + 1)
        for file in files:
            print(f"{sub_indent}{file}")

if __name__ == "__main__":
    cwd = os.getcwd()  # Get the current working directory
    print_directory_structure(cwd)
    