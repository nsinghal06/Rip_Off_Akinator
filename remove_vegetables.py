import re
import shutil
from pathlib import Path

INPUT_FILE = "dataset.c"
BACKUP_FILE = "dataset_backup.c"

VEGETABLE_FOODS = {"Carrot", "Corn", "Cucumber", "Peas", "Potato", "Tomato"}
REMOVE_Q_INDICES = {2, *range(50, 67)}  # q2 and q50..q66


def find_block(text: str, start_marker: str):
    start = text.find(start_marker)
    if start == -1:
        raise ValueError(f"Could not find block starting with: {start_marker}")

    brace_start = text.find("{", start)
    if brace_start == -1:
        raise ValueError(f"Could not find opening brace for block: {start_marker}")

    depth = 0
    end_brace = None
    for i in range(brace_start, len(text)):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                end_brace = i
                break

    if end_brace is None:
        raise ValueError(f"Could not find closing brace for block: {start_marker}")

    semi = text.find(";", end_brace)
    if semi == -1:
        raise ValueError(f"Could not find semicolon after block: {start_marker}")

    return start, semi + 1, text[start:semi + 1]


def replace_block(text: str, start_marker: str, new_block: str) -> str:
    start, end, _ = find_block(text, start_marker)
    return text[:start] + new_block + text[end:]


def renumber_q(old_q: int):
    if old_q in REMOVE_Q_INDICES:
        return None
    if old_q < 2:
        return old_q
    if 3 <= old_q <= 49:
        return old_q - 1
    if old_q >= 67:
        return old_q - 18
    raise ValueError(f"Unhandled q index: {old_q}")


def remap_q_labels_in_line(line: str) -> str:
    def repl(m):
        old_q = int(m.group(1))
        new_q = renumber_q(old_q)
        return m.group(0) if new_q is None else f"q{new_q}"
    return re.sub(r"q(\d+)", repl, line)


def update_food_names_block(block: str) -> str:
    lines = block.splitlines()
    out = []

    in_veg_section = False
    for line in lines:
        if "/* VEGETABLES" in line:
            in_veg_section = True
            continue

        if in_veg_section:
            if "/* ASIAN" in line:
                in_veg_section = False
                out.append(line)
            continue

        if any(f'"{food}"' in line for food in VEGETABLE_FOODS):
            continue

        out.append(line)

    result = "\n".join(out)
    result = re.sub(r'food_names\s*\[\s*114\s*\]', 'food_names[108]', result)
    return result


def update_questions_block(block: str) -> str:
    lines = block.splitlines()
    out = []

    in_veg_pool = False
    for line in lines:
        if '"Is it a vegetable?"' in line:
            continue

        if "/* VEGETABLE POOL (q50-q66) */" in line:
            in_veg_pool = True
            continue

        if in_veg_pool:
            if "/* ASIAN POOL (q67-q77) */" in line:
                in_veg_pool = False
                out.append("    /* ASIAN POOL (q49-q59) */")
            continue

        line = remap_q_labels_in_line(line)

        if "/* PHASE 1 (q0-q7) */" in line:
            line = "    /* PHASE 1 (q0-q6) */"
        elif "/* DESSERT POOL (q8-q27) */" in line:
            line = "    /* DESSERT POOL (q7-q26) */"
        elif "/* FRUIT POOL (q28-q49) */" in line:
            line = "    /* FRUIT POOL (q27-q48) */"
        elif "/* SNACK POOL (q78-q94) */" in line:
            line = "    /* SNACK POOL (q60-q76) */"
        elif "/* SOUP POOL (q95-q99) */" in line:
            line = "    /* SOUP POOL (q77-q81) */"
        elif "/* MEAT POOL (q100-q113) */" in line:
            line = "    /* MEAT POOL (q82-q95) */"
        elif "/* WESTERN POOL (q114-q129) */" in line:
            line = "    /* WESTERN POOL (q96-q111) */"

        out.append(line)

    result = "\n".join(out)
    result = re.sub(r'questions\s*\[\s*131\s*\]', 'questions[113]', result)
    return result


def update_pool_defs_block(block: str) -> str:
    lines = block.splitlines()
    out = []

    for line in lines:
        if "Vegetable q50-q66" in line:
            continue
        elif "{8,   28,  0, 0}" in line or "{8," in line and "Dessert" in line:
            out.append("    {7,   27,  0, 0},  /* Pool 0: Dessert   q7-q26   unlocks when q0=YES  */")
        elif "{28,  50,  1, 0}" in line or "{28," in line and "Fruit" in line:
            out.append("    {27,  49,  1, 0},  /* Pool 1: Fruit     q27-q48  unlocks when q1=YES  */")
        elif "{67,  78,  3, 0}" in line or "{67," in line and "Asian" in line:
            out.append("    {49,  60,  2, 0},  /* Pool 2: Asian     q49-q59  unlocks when q2=YES  */")
        elif "{78,  95,  4, 0}" in line or "{78," in line and "Snack" in line:
            out.append("    {60,  77,  3, 0},  /* Pool 3: Snack     q60-q76  unlocks when q3=YES  */")
        elif "{95,  100, 5, 0}" in line or "{95," in line and "Soup" in line:
            out.append("    {77,  82,  4, 0},  /* Pool 4: Soup      q77-q81  unlocks when q4=YES  */")
        elif "{100, 114, 6, 0}" in line or "{100," in line and "Meat" in line:
            out.append("    {82,  96,  5, 0},  /* Pool 5: Meat      q82-q95  unlocks when q5=YES */")
        elif "{114, 131, 7, 0}" in line or "{114," in line and "Western" in line:
            out.append("    {96,  113, 6, 0}   /* Pool 6: Western   q96-q112 unlocks when q6=YES */")
        else:
            out.append(line)

    result = "\n".join(out)
    result = re.sub(r'pool_defs\s*\[\s*8\s*\]\s*\[\s*4\s*\]', 'pool_defs[7][4]', result)
    return result


def split_top_level_rows(text: str):
    rows = []
    depth = 0
    row_start = None
    for i, ch in enumerate(text):
        if ch == "{":
            if depth == 0:
                row_start = i
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0 and row_start is not None:
                rows.append((row_start, i + 1))
    return rows


def parse_row_numbers(row_text: str):
    return re.findall(r'-?\d+\.\d+|-?\d+', row_text)


def update_properties_block(block: str) -> str:
    header_match = re.search(r'float\s+properties\s*\[\s*114\s*\]\s*\[\s*131\s*\]\s*=\s*\{', block)
    if not header_match:
        raise ValueError("Could not locate properties header.")

    brace_start = block.find("{", header_match.start())
    brace_end = block.rfind("}")
    inner = block[brace_start + 1:brace_end]

    lines = inner.splitlines()
    out_lines = []

    in_veg_header = False
    for line in lines:
        if "/* VEGETABLES (q2=0.95 for all, veg pool q50-q66) */" in line:
            in_veg_header = True
            continue

        if in_veg_header:
            if "/* 60 Carrot" in line:
                in_veg_header = False
            else:
                continue

        if any(food in line for food in VEGETABLE_FOODS):
            continue

        if line.strip().startswith("/*") and "q0" in line and "q130" in line:
            q_labels = "  ".join(f"q{i}" for i in range(113))
            out_lines.append(f"    /*                          {q_labels} */")
            continue

        out_lines.append(line)

    filtered_inner = "\n".join(out_lines)

    row_spans = split_top_level_rows(filtered_inner)
    rebuilt_parts = []
    last = 0

    for start, end in row_spans:
        rebuilt_parts.append(filtered_inner[last:start])
        row_text = filtered_inner[start:end]

        line_start = filtered_inner.rfind("\n", 0, start) + 1
        prefix = filtered_inner[line_start:start]
        comment_match = re.search(r'/\*.*?\*/', prefix)

        if comment_match and any(food in comment_match.group(0) for food in VEGETABLE_FOODS):
            last = end
            continue

        nums = parse_row_numbers(row_text)
        if len(nums) == 131:
            kept = [nums[i] for i in range(131) if i not in REMOVE_Q_INDICES]
            rebuilt_parts.append("{" + ", ".join(kept) + "}")
        else:
            rebuilt_parts.append(row_text)

        last = end

    rebuilt_parts.append(filtered_inner[last:])
    new_inner = "".join(rebuilt_parts)

    result = block[:brace_start + 1] + new_inner + block[brace_end:]
    result = re.sub(
        r'float\s+properties\s*\[\s*114\s*\]\s*\[\s*131\s*\]',
        'float properties[108][113]',
        result
    )
    return result


def main():
    input_path = Path(INPUT_FILE)
    if not input_path.exists():
        raise FileNotFoundError(f"Could not find {INPUT_FILE}")

    shutil.copyfile(INPUT_FILE, BACKUP_FILE)

    text = input_path.read_text(encoding="utf-8")

    _, _, food_block = find_block(text, 'const char *food_names[')
    _, _, question_block = find_block(text, 'const char *questions[')
    _, _, pool_block = find_block(text, 'int pool_defs[')
    _, _, properties_block = find_block(text, 'float properties[')

    new_food_block = update_food_names_block(food_block)
    new_question_block = update_questions_block(question_block)
    new_pool_block = update_pool_defs_block(pool_block)
    new_properties_block = update_properties_block(properties_block)

    out = text
    out = replace_block(out, 'const char *food_names[', new_food_block)
    out = replace_block(out, 'const char *questions[', new_question_block)
    out = replace_block(out, 'int pool_defs[', new_pool_block)
    out = replace_block(out, 'float properties[', new_properties_block)

    input_path.write_text(out, encoding="utf-8")

    print("Done.")
    print(f"Backup created: {BACKUP_FILE}")
    print("Edited in place: dataset.c")
    print("Final foods: 108")
    print("Final questions: 113")
    print("Final properties shape: [108][113]")


if __name__ == "__main__":
    main()