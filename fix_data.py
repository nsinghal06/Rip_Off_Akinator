import re
from pathlib import Path

INPUT_FILE = "dataset.c"
OUTPUT_FILE = "dataset_new.c"

NEW_QUESTION_LINE = '"Is it covered in a cheese sauce?",               /* q134 */'


def replace_first(pattern, repl, text, flags=0):
    new_text, count = re.subn(pattern, repl, text, count=1, flags=flags)
    if count != 1:
        raise ValueError(f"Expected exactly 1 match for pattern:\n{pattern}")
    return new_text


def main():
    text = Path(INPUT_FILE).read_text(encoding="utf-8")

    # 1) questions array size: [134] -> [135]
    text = replace_first(
        r'(const\s+char\s*\*\s*questions\s*\[\s*)134(\s*\]\s*=\s*\{)',
        r'\g<1>135\g<2>',
        text
    )

    # 2) insert new western question after q133, before the questions closing };
    tail_pattern = r'(^\s*"Is it made using potatoes\?",\s*/\*\s*q133\s*\*/\s*$)'
    m = re.search(tail_pattern, text, flags=re.MULTILINE)
    if not m:
        raise ValueError("Could not find the q133 question line.")

    indent = re.match(r'^(\s*)', m.group(1)).group(1)
    insertion = m.group(1) + "\n" + indent + NEW_QUESTION_LINE
    text = re.sub(tail_pattern, insertion, text, count=1, flags=re.MULTILINE)

    # 3) western pool end: {115, 134, 7, 0} -> {115, 135, 7, 0}
    text = replace_first(
        r'(\{\s*115\s*,\s*)134(\s*,\s*7\s*,\s*0\s*\})',
        r'\g<1>135\g<2>',
        text
    )

    # 4) western pool comment q115-q133 -> q115-q134
    text = text.replace('q115-q133', 'q115-q134', 1)

    # 5) properties dimensions: [107][134] -> [107][135]
    text = replace_first(
        r'(float\s+properties\s*\[\s*107\s*\]\s*\[\s*)134(\s*\]\s*=\s*\{)',
        r'\g<1>135\g<2>',
        text
    )

    # 6) append 0.05 to every properties row, preserving existing row comments/spacing
    row_pattern = re.compile(r'^(\s*/\*.*?\*/\s*\{)(.*?)(\}\s*,?\s*)$', re.MULTILINE)

    def extend_row(match):
        row_start, values, row_end = match.groups()
        return f"{row_start}{values}, 0.05{row_end}"

    text, row_count = row_pattern.subn(extend_row, text)

    if row_count == 0:
        raise ValueError("Did not find any properties rows to extend.")

    Path(OUTPUT_FILE).write_text(text, encoding="utf-8")

    print(f"Done. Wrote {OUTPUT_FILE}")
    print("Added q134: Is it covered in a cheese sauce?")
    print(f"Extended {row_count} properties rows with trailing 0.05")


if __name__ == "__main__":
    main()