import subprocess
import sys
import os

def get_sha(var_name, fallback):
    return os.environ.get(var_name) or fallback

def get_changed_files(base_sha, head_sha):
    try:
        result = subprocess.run(
            ['git', 'diff', '--name-only', base_sha, head_sha],
            capture_output=True, text=True, check=True
        )
        return [f for f in result.stdout.strip().split('\n') if f]
    except Exception as e:
        print(f"Error getting changed files: {e}")
        sys.exit(2)

def has_endline(file_path):
    try:
        if os.path.getsize(file_path) == 0:
            return True
        with open(file_path, 'rb') as f:
            f.seek(-1, os.SEEK_END)
            last_byte = f.read(1)
            if b'\0' in last_byte:
                return True
            return last_byte == b'\n'
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
        return True  

def main():
    base_ref = os.environ.get('GITHUB_BASE_REF', 'main')
    base_branch = f'origin/{base_ref}'
    head_sha = os.environ.get('GITHUB_SHA', 'HEAD')
    files = get_changed_files(base_branch, head_sha)
    no_newline_files = []
    for file in files:
        if os.path.isfile(file):
            if not has_endline(file):
                no_newline_files.append(file)
    if no_newline_files:
        print("Files without ENDLINE:")
        for file in no_newline_files:
            print(f"- {file}")
        sys.exit(1)
    else:
        print("All files have ENDLINE.")
        sys.exit(0)

if __name__ == "__main__":
    main()
