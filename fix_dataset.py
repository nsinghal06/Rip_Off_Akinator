"""
Removes vegetable rows from the properties[] table that are NOT in food_names[].
Keeps only: Carrot, Corn, Cucumber, Peas, Potato, Tomato
Removes:    Garlic, Mushroom, Olive, Onion, Pumpkin, Radish, Spinach, Sweet Potato, Zucchini
"""

import re

KEEP_VEGS = {"Carrot", "Corn", "Cucumber", "Peas", "Potato", "Tomato"}

REMOVE_VEGS = {"Garlic", "Mushroom", "Olive", "Onion", "Pumpkin", "Radish", "Spinach", "Sweet Potato", "Zucchini"}

def should_remove(line):
    """Return True if the line is a properties row for a vegetable we want to remove."""
    for name in REMOVE_VEGS:
        # Match comment pattern like:  /* 65 Garlic           */
        if re.search(r'/\*\s*\d+\s+' + re.escape(name) + r'\s*\*/', line):
            return True
    return False

def fix_properties_table(input_path, output_path):
    with open(input_path, 'r') as f:
        lines = f.readlines()

    output_lines = []
    skip_next = False  # used to skip the data line after the comment line

    i = 0
    while i < len(lines):
        line = lines[i]

        if should_remove(line):
            # This is the comment+data line for a veg we want to remove
            # The row is entirely on this one line (comment and array data together)
            print(f"  REMOVING: {line.strip()[:80]}")
            i += 1
            continue

        output_lines.append(line)
        i += 1

    with open(output_path, 'w') as f:
        f.writelines(output_lines)

    print(f"\nDone. Written to {output_path}")
    print(f"Removed {len(lines) - len(output_lines)} line(s).")

if __name__ == "__main__":
    import sys

    input_file  = sys.argv[1] if len(sys.argv) > 1 else "dataset.c"
    output_file = sys.argv[2] if len(sys.argv) > 2 else "dataset_fixed.c"

    print(f"Reading:  {input_file}")
    print(f"Writing:  {output_file}")
    print(f"Removing rows for: {', '.join(sorted(REMOVE_VEGS))}\n")

    fix_properties_table(input_file, output_file)