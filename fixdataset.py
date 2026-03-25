import re

# These are the 17 indices to remove from a 91-column row 
# to result in exactly 74 columns matching your question array.
indices_to_remove = [
    19, 43, 44, 45, 53, 54, 56, 70, 71, 74, 75, 76, 81, 82, 88, 89, 90
]

def fix_dataset_to_74():
    try:
        with open('dataset.c', 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print("Error: dataset.c not found.")
        return

    # Find the properties array with 91 columns
    pattern = re.compile(r'(float properties\[190\]\[91\] = \{.*?^\};)', re.DOTALL | re.MULTILINE)
    match = pattern.search(content)
    
    if not match:
        print("Could not find properties[190][91]. Check the header in your file!")
        return

    block = match.group(1)
    lines = block.split('\n')
    new_lines = []

    for line in lines:
        if '{' in line and '}' in line:
            start = line.find('{') + 1
            end = line.rfind('}')
            # Split by comma and clean whitespace
            data = [v.strip() for v in line[start:end].split(',')]
            
            # Filter out the 17 bad indices
            new_data = [data[i] for i in range(len(data)) if i not in indices_to_remove]
            
            # Rebuild the line string
            line = line[:start] + ", ".join(new_data) + line[end:]
        new_lines.append(line)

    # Change array declaration to [74]
    final_block = "\n".join(new_lines).replace("[91]", "[74]")
    new_content = content.replace(block, final_block)

    with open('dataset_final_74.c', 'w') as f:
        f.write(new_content)
    print(f"Success! Created dataset_final_74.c with {len(new_data)} columns.")

if __name__ == "__main__":
    fix_dataset_to_74()