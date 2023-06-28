import os
import sys
from pathlib import Path

out_dir = Path("out")
if len(sys.argv) > 3:
    out_dir = sys.argv[2]

input_dir_root = out_dir / "default"

input_dirs = [
    (input_dir_root / "crashes"),
    (input_dir_root / "queue")
]

for input_dir in input_dirs:
    for input_file in input_dir.iterdir():
        os.system(f"./susan {str(input_file)}")
