"""Apply the translator's defined local EBP seed to an existing generated tree.

This retains the current local-EBP/SEH-bridge model. It is not a replacement
for a complete architectural EBP register model. Existing explicit inheritance
and SEH readbacks still run as before.
"""
from pathlib import Path
import argparse
import re


def initialize(source):
    return re.sub(r"^(    uint32_t ebp);$", r"\1 = g_seh_ebp;", source, flags=re.M)


if __name__ == "__main__":
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--gen",type=Path,required=True)
    args=parser.parse_args()
    count=0
    for file in sorted(args.gen.glob("recomp_*.c")):
        source=file.read_text()
        fixed=initialize(source)
        if fixed!=source:
            count+=source.count("    uint32_t ebp;\n")
            file.write_text(fixed)
    print(f"Initialized {count} translated frame-pointer locals")
