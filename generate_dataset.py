# generate_dataset.py
# run command: python3 generate_dataset.py
# Output: dataset.c and dataset.h 

# 289 FOODS - ALPHABETICALLY SORTED
foods = sorted([
    # DESSERTS & SWEETS
    "Apple Pie", "Baklava", "Biscotti", "Biscuit", "Black Forest Cake",
    "Bread Pudding", "Brownie", "Butter Tart", "Cannoli", "Caramel",
    "Carrot Cake", "Cheesecake", "Churro Ice Cream", "Churros",
    "Cinnamon Roll", "Cookie", "Cotton Candy", "Crepes", "Creme Brulee",
    "Croissant Sweet", "Cupcake", "Custard", "Danish Pastry",
    "Donuts", "Eclair", "Frozen Yogurt", "Fudge", "Fruit Cake",
    "Funnel Cake", "Gelato", "Gingerbread", "Ice Cream", "Key Lime Pie",
    "Lemon Tart", "Macarons", "Marshmallow", "Meringue", "Mochi",
    "Muffin", "Nanaimo Bar", "Pain au Chocolat", "Pancakes",
    "Peach Cobbler", "Pecan Pie", "Popsicle", "Pudding",
    "Pumpkin Pie", "Red Velvet Cake", "Rice Pudding", "Scone",
    "Shortbread", "Sorbet", "Sponge Cake", "Sticky Toffee Pudding",
    "Strawberry Shortcake", "Strudel", "Tiramisu", "Tart",
    "Toffee", "Trifle", "Turkish Delight", "Wafer", "Waffle Cone",
    "Waffles", "Chocolate Fondue", "Chocolate Mousse",

    # ITALIAN
    "Lasagna", "Pasta", "Pizza",

    # JAPANESE
    "Ramen", "Sushi",

    # CHINESE
    "Dim Sum", "Dumplings", "Fried Rice", "Spring Rolls",

    # SOUTHEAST ASIAN
    "Mango Sticky Rice", "Pad Thai",

    # INDIAN
    "Butter Chicken", "Samosa",

    # KOREAN
    "Kimchi",

    # AMERICAN
    "BBQ Ribs", "Buffalo Wings", "Burger", "Chili", "Corn Dog",
    "Corn on the Cob", "Fries", "Fried Chicken", "Grilled Cheese",
    "Hot Dog", "Mac and Cheese", "Mashed Potatoes", "Meatloaf",
    "Smoked Salmon",

    # MEXICAN
    "Burrito", "Guacamole", "Nachos", "Quesadilla", "Salsa", "Tacos",

    # MIDDLE EASTERN
    "Hummus", "Kebab", "Shawarma",

    # EUROPEAN
    "Baguette", "Croissant", "Fish and Chips", "French Onion Soup",
    "Pierogi", "Pretzel", "Quiche", "Ratatouille",

    # BREAKFAST
    "Acai Bowl", "Avocado Toast", "Bagel", "Breakfast Burrito",
    "Breakfast Sandwich", "Chia Pudding", "Eggs Benedict",
    "French Toast", "Granola", "Hash Browns", "Oatmeal",
    "Omelette", "Overnight Oats", "Scrambled Eggs", "Smoothie Bowl",
    "Yogurt Parfait",

    # SOUPS
    "Chicken Noodle Soup", "Lentil Soup", "Miso Soup", "Tomato Soup",

    # SEAFOOD
    "Calamari", "Clams", "Crab", "Fish Tacos", "Grilled Octopus",
    "Grilled Salmon", "Lobster", "Oysters", "Sashimi",
    "Scallops", "Shrimp Cocktail",

    # SNACKS & CANDY
    "Almonds", "Animal Crackers", "Beef Jerky", "Cashews",
    "Cheese Puffs", "Cheez Its", "Chips Ahoy", "Crackers",
    "Doritos", "Ferrero Rocher", "Fruit Roll Up", "Goldfish Crackers",
    "Graham Crackers", "Granola Bar", "Gummy Bears", "Hershey Bar",
    "Kit Kat", "Lindt Chocolate", "M&Ms", "Milky Way", "Mixed Nuts",
    "Nutter Butter", "Oreos", "Peanuts", "Pistachios", "Pocky",
    "Popcorn", "Popcorn Chicken", "Popcorn Shrimp", "Pork Rinds",
    "Potato Chips", "Prawn Crackers", "Pringles", "Pumpkin Seeds",
    "Reese's Pieces", "Rice Cakes", "Rice Krispie Treat", "Ritz Crackers",
    "Seaweed Snack", "Skittles", "Snickers", "Sour Patch Kids",
    "Sunflower Seeds", "Toblerone", "Trail Mix", "Twix",

    # FRUITS
    "Acai", "Apple", "Apricot", "Avocado", "Banana", "Blackberry",
    "Blueberry", "Cantaloupe", "Cherry", "Clementine", "Coconut",
    "Cranberry", "Date", "Dragon Fruit", "Durian", "Fig",
    "Gooseberry", "Grape", "Grapefruit", "Guava", "Honeydew",
    "Jackfruit", "Kiwi", "Kumquat", "Lemon", "Lime", "Lychee",
    "Mandarin", "Mango", "Mulberry", "Nectarine", "Olive",
    "Orange", "Papaya", "Passion Fruit", "Peach", "Pear",
    "Persimmon", "Pineapple", "Plantain", "Plum", "Pomegranate",
    "Rambutan", "Raspberry", "Starfruit", "Strawberry", "Tamarind",
    "Tangerine", "Tomato", "Watermelon",

    # VEGETABLES
    "Acorn Squash", "Arugula", "Artichoke", "Asparagus",
    "Bamboo Shoots", "Bean Sprouts", "Beet", "Bell Pepper",
    "Bok Choy", "Broccoli", "Brussels Sprouts", "Butternut Squash",
    "Cabbage", "Carrot", "Cauliflower", "Celery", "Collard Greens",
    "Corn", "Cucumber", "Eggplant", "Endive", "Fennel",
    "Garlic", "Green Beans", "Kale", "Leek", "Lettuce",
    "Mushroom", "Mustard Greens", "Okra", "Onion",
    "Parsnip", "Peas", "Potato", "Pumpkin", "Radish",
    "Radicchio", "Shallot", "Snow Peas", "Spinach", "Squash",
    "Sugar Snap Peas", "Sweet Potato", "Swiss Chard", "Turnip",
    "Watercress", "Yam", "Zucchini",
])

# 20 QUESTIONS
questions = [
    "Is it a dessert?", # Q00
    "Is it served hot?", # Q01
    "Does it contain meat?", # Q02
    "Is it a fruit?", # Q03
    "Is it a vegetable?", # Q04
    "Is it naturally sweet?", # Q05
    "Does it grow on a tree or vine?", # Q06
    "Does it grow underground?", # Q07
    "Is it green in colour?", # Q08
    "Is it red or orange in colour?", # Q09
    "Is it white or yellow in colour?", # Q10
    "Can you eat it with your hands?", # Q11
    "Is it fried or deep fried?", # Q12
    "Does it contain bread or dough?", # Q13
    "Does it contain cheese?", # Q14
    "Is it from Asian cuisine?", # Q15
    "Is it from European or Western cuisine?", # Q16
    "Is it a liquid or soup?", # Q17
    "Is it a snack or candy?", # Q18
    "Is it typically eaten for breakfast?", # Q19
]

# CATEGORY TAGS PER FOOD
# each food gets a set of tags, tags are used to assign weights to questions
food_tags = {
    # DESSERTS
    "Apple Pie":            ["dessert", "baked", "sweet", "dough", "fruit_based"],
    "Baklava":              ["dessert", "baked", "sweet", "dough", "middle_eastern", "nutty"],
    "Biscuit":              ["dessert", "baked", "sweet", "handheld", "dough"],
    "Black Forest Cake":    ["dessert", "baked", "sweet", "chocolate"],
    "Bread Pudding":        ["dessert", "baked", "sweet", "dough"],
    "Brownie":              ["dessert", "baked", "sweet", "chocolate", "handheld"],
    "Butter Tart":          ["dessert", "baked", "sweet", "dough"],
    "Cannoli":              ["dessert", "fried", "sweet", "italian", "dough"],
    "Caramel":              ["dessert", "sweet", "candy", "snack"],
    "Carrot Cake":          ["dessert", "baked", "sweet"],
    "Cheesecake":           ["dessert", "baked", "sweet", "cheese", "cold"],
    "Churros":              ["dessert", "fried", "sweet", "handheld", "dough"],
    "Cinnamon Roll":        ["dessert", "baked", "sweet", "dough"],
    "Cookie":               ["dessert", "baked", "sweet", "handheld"],
    "Cotton Candy":         ["dessert", "sweet", "candy", "handheld", "snack"],
    "Crepes":               ["dessert", "sweet", "pan_cooked", "dough", "french"],
    "Creme Brulee":         ["dessert", "sweet", "cold", "french", "creamy"],
    "Croissant Sweet":      ["dessert", "baked", "sweet", "dough", "french", "handheld"],
    "Cupcake":              ["dessert", "baked", "sweet"],
    "Custard":              ["dessert", "sweet", "creamy", "cold"],
    "Danish Pastry":        ["dessert", "baked", "sweet", "dough", "handheld"],
    "Donuts":               ["dessert", "fried", "sweet", "dough", "handheld"],
    "Eclair":               ["dessert", "baked", "sweet", "dough", "french"],
    "Frozen Yogurt":        ["dessert", "sweet", "frozen", "cold", "dairy"],
    "Fudge":                ["dessert", "sweet", "candy", "chocolate", "handheld"],
    "Fruit Cake":           ["dessert", "baked", "sweet", "fruit_based"],
    "Funnel Cake":          ["dessert", "fried", "sweet", "dough", "handheld"],
    "Gelato":               ["dessert", "sweet", "frozen", "cold", "italian", "dairy"],
    "Gingerbread":          ["dessert", "baked", "sweet", "handheld"],
    "Ice Cream":            ["dessert", "sweet", "frozen", "cold", "dairy"],
    "Key Lime Pie":         ["dessert", "baked", "sweet", "cold"],
    "Lemon Tart":           ["dessert", "baked", "sweet", "french"],
    "Macarons":             ["dessert", "baked", "sweet", "french", "handheld"],
    "Marshmallow":          ["dessert", "sweet", "candy", "soft", "handheld", "snack"],
    "Meringue":             ["dessert", "baked", "sweet"],
    "Mochi":                ["dessert", "sweet", "asian", "handheld", "chewy"],
    "Muffin":               ["dessert", "baked", "sweet", "handheld"],
    "Nanaimo Bar":          ["dessert", "sweet", "chocolate", "cold", "handheld"],
    "Pain au Chocolat":     ["dessert", "baked", "sweet", "dough", "chocolate", "french", "handheld"],
    "Pancakes":             ["dessert", "sweet", "pan_cooked", "dough", "breakfast"],
    "Peach Cobbler":        ["dessert", "baked", "sweet", "fruit_based"],
    "Pecan Pie":            ["dessert", "baked", "sweet", "nutty"],
    "Popsicle":             ["dessert", "sweet", "frozen", "cold", "handheld"],
    "Pudding":              ["dessert", "sweet", "creamy", "cold"],
    "Pumpkin Pie":          ["dessert", "baked", "sweet"],
    "Red Velvet Cake":      ["dessert", "baked", "sweet", "chocolate"],
    "Rice Pudding":         ["dessert", "sweet", "creamy", "cold"],
    "Scone":                ["dessert", "baked", "sweet", "handheld", "dough", "british"],
    "Shortbread":           ["dessert", "baked", "sweet", "handheld", "british"],
    "Sorbet":               ["dessert", "sweet", "frozen", "cold"],
    "Sponge Cake":          ["dessert", "baked", "sweet"],
    "Sticky Toffee Pudding":["dessert", "baked", "sweet", "british", "hot"],
    "Strawberry Shortcake": ["dessert", "baked", "sweet", "fruit_based"],
    "Tiramisu":             ["dessert", "sweet", "cold", "italian", "creamy", "layered"],
    "Tart":                 ["dessert", "baked", "sweet", "dough"],
    "Toffee":               ["dessert", "sweet", "candy", "handheld", "snack"],
    "Turkish Delight":      ["dessert", "sweet", "candy", "middle_eastern", "chewy", "handheld"],
    "Wafer":                ["dessert", "baked", "sweet", "handheld", "snack"],
    "Waffle Cone":          ["dessert", "baked", "sweet", "handheld", "dough"],
    "Waffles":              ["dessert", "sweet", "pan_cooked", "dough", "breakfast"],
    "Chocolate Cake":       ["dessert", "sweet", "cold", "chocolate", "creamy", "layered"],

    # ITALIAN
    "Lasagna":              ["italian", "hot", "baked", "meat", "cheese", "dough"],
    "Pasta":                ["italian", "hot", "european"],
    "Pizza":                ["italian", "hot", "baked", "cheese", "dough", "handheld"],

    # JAPANESE
    "Ramen":                ["asian", "japanese", "hot", "soup", "meat"],
    "Sushi":                ["asian", "japanese", "cold", "seafood", "handheld"],

    # CHINESE
    "Dim Sum":              ["asian", "chinese", "hot", "meat", "handheld", "dough"],
    "Dumplings":            ["asian", "chinese", "hot", "meat", "dough", "handheld"],
    "Fried Rice":           ["asian", "chinese", "hot", "fried", "meat"],
    "Spring Rolls":         ["asian", "chinese", "fried", "handheld", "dough"],

    # SOUTHEAST ASIAN
    "Mango Sticky Rice":    ["asian", "sweet", "cold", "fruit_based"],
    "Pad Thai":             ["asian", "hot", "fried", "seafood", "noodles"],

    # INDIAN
    "Butter Chicken":       ["asian", "indian", "hot", "meat", "spicy"],
    "Samosa":               ["asian", "indian", "fried", "handheld", "dough", "spicy"],

    # KOREAN
    "Kimchi":               ["asian", "korean", "cold", "spicy", "vegetable_dish"],

    # AMERICAN
    "BBQ Ribs":             ["american", "hot", "meat", "western"],
    "Buffalo Wings":        ["american", "hot", "fried", "meat", "handheld", "spicy", "western"],
    "Burger":               ["american", "hot", "meat", "handheld", "dough", "western"],
    "Chili":                ["american", "hot", "meat", "soup", "spicy", "western"],
    "Corn Dog":             ["american", "hot", "fried", "meat", "handheld", "dough", "western"],
    "Corn on the Cob":      ["american", "hot", "vegetable_dish", "handheld", "western"],
    "Fries":                ["american", "hot", "fried", "handheld", "western", "snack"],
    "Fried Chicken":        ["american", "hot", "fried", "meat", "handheld", "western"],
    "Grilled Cheese":       ["american", "hot", "cheese", "handheld", "dough", "western"],
    "Hot Dog":              ["american", "hot", "meat", "handheld", "dough", "western"],
    "Mac and Cheese":       ["american", "hot", "cheese", "western"],
    "Mashed Potatoes":      ["american", "hot", "western", "vegetable_dish"],

    # MEXICAN
    "Burrito":              ["mexican", "hot", "meat", "handheld", "dough", "western", "spicy"],
    "Guacamole":            ["mexican", "cold", "western", "handheld", "vegetarian"],
    "Nachos":               ["mexican", "hot", "cheese", "handheld", "western", "snack"],
    "Quesadilla":           ["mexican", "hot", "cheese", "handheld", "dough", "western"],
    "Salsa":                ["mexican", "cold", "western", "spicy", "liquid"],
    "Tacos":                ["mexican", "hot", "meat", "handheld", "dough", "western", "spicy"],

    # MIDDLE EASTERN
    "Hummus":               ["middle_eastern", "cold", "vegetarian"],
    "Kebab":                ["middle_eastern", "hot", "meat", "handheld"],
    "Shawarma":             ["middle_eastern", "hot", "meat", "handheld", "dough"],

    # EUROPEAN
    "Baguette":             ["french", "baked", "handheld", "dough", "european"],
    "Croissant":            ["french", "baked", "handheld", "dough", "european"],
    "Fish and Chips":       ["british", "hot", "fried", "seafood", "handheld", "european"],
    "French Onion Soup":    ["french", "hot", "soup", "cheese", "european"],
    "Pierogi":              ["polish", "hot", "dough", "cheese", "european"],
    "Pretzel":              ["german", "baked", "handheld", "dough", "european"],
    "Quiche":               ["french", "baked", "cheese", "european"],
    "Ratatouille":          ["french", "hot", "vegetarian", "european"],

    # BREAKFAST
    "Acai Bowl":            ["breakfast", "cold", "sweet", "fruit_based"],
    "Avocado Toast":        ["breakfast", "handheld", "dough", "vegetarian"],
    "Bagel":                ["breakfast", "baked", "handheld", "dough"],
    "Boiled eggs":          ["breakfast", "hot", "pan_cooked", "western"],
    "Breakfast Burrito":    ["breakfast", "hot", "meat", "handheld", "dough", "western"],
    "Breakfast Sandwich":   ["breakfast", "hot", "meat", "handheld", "dough", "western"],
    "Chia Pudding":         ["breakfast", "cold", "sweet", "creamy"],
    "French Toast":         ["breakfast", "hot", "sweet", "dough", "pan_cooked"],
    "Granola":              ["breakfast", "sweet", "snack", "handheld"],
    "Hash Browns":          ["breakfast", "hot", "fried", "western"],
    "Oatmeal":              ["breakfast", "hot", "sweet"],
    "Omelette":             ["breakfast", "hot", "pan_cooked", "meat", "cheese", "western"],
    "Overnight Oats":       ["breakfast", "cold", "sweet"],
    "Scrambled Eggs":       ["breakfast", "hot", "pan_cooked", "western"],
    "Smoothie Bowl":        ["breakfast", "cold", "sweet", "fruit_based"],
    "Yogurt Parfait":       ["breakfast", "cold", "sweet", "dairy"],

    # SOUPS
    "Chicken Noodle Soup":  ["hot", "soup", "meat", "western"],
    "Lentil Soup":          ["hot", "soup", "vegetarian"],
    "Miso Soup":            ["asian", "japanese", "hot", "soup"],
    "Tomato Soup":          ["hot", "soup", "vegetarian", "western"],

    # SEAFOOD
    "Calamari":             ["seafood", "hot", "fried", "handheld"],
    "Clams":                ["seafood", "hot"],
    "Crab":                 ["seafood", "hot"],
    "Fish Tacos":           ["seafood", "hot", "handheld", "dough", "western"],
    "Grilled Octopus":      ["seafood", "hot"],
    "Grilled Salmon":       ["seafood", "hot"],
    "Lobster":              ["seafood", "hot", "red"],
    "Oysters":              ["seafood", "cold"],
    "Sashimi":              ["seafood", "cold", "asian", "japanese"],
    "Scallops":             ["seafood", "hot"],
    "Shrimp":             ["seafood", "cold", "handheld"],

    # SNACKS & CANDY
    "Almonds":              ["snack", "handheld", "nutty"],
    "Animal Crackers":      ["snack", "sweet", "handheld", "baked"],
    "Beef Jerky":           ["snack", "meat", "handheld", "western"],
    "Cashews":              ["snack", "handheld", "nutty"],
    "Cheese Puffs":         ["snack", "handheld", "cheese", "western"],
    "Cheez Its":            ["snack", "baked", "handheld", "cheese", "western"],
    "Chips Ahoy":           ["snack", "sweet", "handheld", "baked", "chocolate"],
    "Crackers":             ["snack", "baked", "handheld", "dough"],
    "Doritos":              ["snack", "handheld", "western", "spicy"],
    "Ferrero Rocher":       ["snack", "sweet", "handheld", "chocolate", "candy", "nutty"],
    "Fruit Roll Up":        ["snack", "sweet", "handheld", "candy", "fruit_based"],
    "Goldfish Crackers":    ["snack", "baked", "handheld", "cheese", "western"],
    "Graham Crackers":      ["snack", "baked", "handheld", "sweet", "dough"],
    "Granola Bar":          ["snack", "sweet", "handheld", "baked"],
    "Gummy Bears":          ["snack", "sweet", "handheld", "candy", "chewy"],
    "Hershey Bar":          ["snack", "sweet", "handheld", "chocolate", "candy"],
    "Kit Kat":              ["snack", "sweet", "handheld", "chocolate", "candy"],
    "Lindt Truffle":        ["snack", "sweet", "handheld", "chocolate", "candy"],
    "M&Ms":                 ["snack", "sweet", "handheld", "chocolate", "candy"],
    "Milky Way":            ["snack", "sweet", "handheld", "chocolate", "candy"],
    "Mixed Nuts":           ["snack", "handheld", "nutty"],
    "Nutter Butter":        ["snack", "sweet", "handheld", "baked"],
    "Oreos":                ["snack", "sweet", "handheld", "baked", "chocolate"],
    "Peanuts":              ["snack", "handheld", "nutty"],
    "Pistachios":           ["snack", "handheld", "nutty"],
    "Pocky":                ["snack", "sweet", "handheld", "asian", "chocolate"],
    "Popcorn":              ["snack", "handheld", "western"],
    "Popcorn Chicken":      ["snack", "hot", "fried", "meat", "handheld", "western"],
    "Popcorn Shrimp":       ["snack", "hot", "fried", "seafood", "handheld"],
    "Pork Rinds":           ["snack", "handheld", "meat", "fried", "western"],
    "Potato Chips":         ["snack", "handheld", "western", "fried"],
    "Prawn Crackers":       ["snack", "handheld", "asian", "seafood", "fried"],
    "Pringles":             ["snack", "handheld", "western"],
    "Pumpkin Seeds":        ["snack", "handheld"],
    "Reese's Pieces":       ["snack", "sweet", "handheld", "candy"],
    "Rice Cakes":           ["snack", "handheld"],
    "Rice Krispie Treat":   ["snack", "sweet", "handheld"],
    "Ritz Crackers":        ["snack", "baked", "handheld", "dough"],
    "Seaweed Snack":        ["snack", "handheld", "asian"],
    "Skittles":             ["snack", "sweet", "handheld", "candy"],
    "Snickers":             ["snack", "sweet", "handheld", "chocolate", "candy", "nutty"],
    "Sour Patch Kids":      ["snack", "sweet", "handheld", "candy"],
    "Sunflower Seeds":      ["snack", "handheld"],
    "Toblerone":            ["snack", "sweet", "handheld", "chocolate", "candy", "nutty"],
    "Trail Mix":            ["snack", "handheld", "nutty"],
    "Twix":                 ["snack", "sweet", "handheld", "chocolate", "candy"],

    # FRUITS
    "Apple":                ["fruit", "sweet", "tree_fruit", "handheld"],
    "Apricot":              ["fruit", "sweet", "tree_fruit", "handheld"],
    "Avocado":              ["fruit", "tree_fruit", "handheld"],
    "Banana":               ["fruit", "sweet", "tropical", "handheld"],
    "Blackberry":           ["fruit", "sweet", "berry", "handheld"],
    "Blueberry":            ["fruit", "sweet", "berry", "handheld"],
    "Cantaloupe":           ["fruit", "sweet", "melon"],
    "Cherry":               ["fruit", "sweet", "tree_fruit", "handheld"],
    "Clementine":           ["fruit", "sweet", "citrus", "handheld"],
    "Coconut":              ["fruit", "tropical"],
    "Cranberry":            ["fruit", "berry", "sour"],
    "Date":                 ["fruit", "sweet", "tropical", "handheld"],
    "Dragon Fruit":         ["fruit", "sweet", "tropical", "handheld"],
    "Fig":                  ["fruit", "sweet", "tree_fruit", "handheld"],
    "Gooseberry":           ["fruit", "berry", "sour"],
    "Grape":                ["fruit", "sweet", "vine_fruit", "handheld"],
    "Grapefruit":           ["fruit", "citrus", "sour", "handheld"],
    "Guava":                ["fruit", "sweet", "tropical", "handheld"],
    "Honeydew":             ["fruit", "sweet", "melon"],
    "Jackfruit":            ["fruit", "sweet", "tropical"],
    "Kiwi":                 ["fruit", "sweet", "handheld"],
    "Kumquat":              ["fruit", "citrus", "sour", "handheld"],
    "Lemon":                ["fruit", "citrus", "sour", "handheld"],
    "Lime":                 ["fruit", "citrus", "sour", "handheld"],
    "Lychee":               ["fruit", "sweet", "tropical", "handheld"],
    "Mandarin":             ["fruit", "sweet", "citrus", "handheld"],
    "Mango":                ["fruit", "sweet", "tropical", "handheld"],
    "Mulberry":             ["fruit", "sweet", "berry"],
    "Nectarine":            ["fruit", "sweet", "tree_fruit", "handheld"],
    "Olive":                ["fruit", "sour", "handheld"],
    "Orange":               ["fruit", "sweet", "citrus", "handheld"],
    "Papaya":               ["fruit", "sweet", "tropical", "handheld"],
    "Passion Fruit":        ["fruit", "sweet", "tropical"],
    "Peach":                ["fruit", "sweet", "tree_fruit", "handheld"],
    "Pear":                 ["fruit", "sweet", "tree_fruit", "handheld"],
    "Persimmon":            ["fruit", "sweet", "tree_fruit", "handheld"],
    "Pineapple":            ["fruit", "sweet", "tropical"],
    "Plantain":             ["fruit", "tropical"],
    "Plum":                 ["fruit", "sweet", "tree_fruit", "handheld"],
    "Pomegranate":          ["fruit", "sweet", "handheld"],
    "Raspberry":            ["fruit", "sweet", "berry", "handheld"],
    "Starfruit":            ["fruit", "sweet", "tropical", "handheld"],
    "Strawberry":           ["fruit", "sweet", "berry", "handheld"],
    "Tamarind":             ["fruit", "sour", "tropical"],
    "Tangerine":            ["fruit", "sweet", "citrus", "handheld"],
    "Tomato":               ["fruit", "vegetable_dish", "sour"],
    "Watermelon":           ["fruit", "sweet", "melon"],

    # VEGETABLES
    "Arugula":              ["vegetable", "green", "raw"],
    "Artichoke":            ["vegetable", "hot"],
    "Asparagus":            ["vegetable", "hot", "green"],
    "Bean Sprouts":         ["vegetable", "asian", "raw"],
    "Beet":                 ["vegetable", "underground", "red_orange"],
    "Bell Pepper":          ["vegetable", "raw", "red_orange"],
    "Bok Choy":             ["vegetable", "asian", "green"],
    "Broccoli":             ["vegetable", "hot", "green"],
    "Brussels Sprouts":     ["vegetable", "hot", "green"],
    "Butternut Squash":     ["vegetable", "hot", "baked", "red_orange"],
    "Cabbage":              ["vegetable", "green", "raw"],
    "Carrot":               ["vegetable", "underground", "red_orange", "raw"],
    "Cauliflower":          ["vegetable", "hot", "white_yellow"],
    "Celery":               ["vegetable", "green", "raw"],
    "Corn":                 ["vegetable", "hot", "white_yellow", "handheld"],
    "Cucumber":             ["vegetable", "green", "raw"],
    "Eggplant":             ["vegetable", "hot", "purple"],
    "Garlic":               ["vegetable", "underground", "white_yellow"],
    "Green Beans":          ["vegetable", "hot", "green"],
    "Kale":                 ["vegetable", "hot", "green"],
    "Leek":                 ["vegetable", "hot", "white_yellow", "underground"],
    "Lettuce":              ["vegetable", "green", "raw"],
    "Mushroom":             ["vegetable", "hot"],
    "Mustard Greens":       ["vegetable", "hot", "green"],
    "Okra":                 ["vegetable", "hot", "green"],
    "Onion":                ["vegetable", "underground", "white_yellow"],
    "Peas":                 ["vegetable", "hot", "green"],
    "Potato":               ["vegetable", "hot", "underground", "white_yellow"],
    "Pumpkin":              ["vegetable", "hot", "baked", "red_orange"],
    "Radish":               ["vegetable", "underground", "red_orange", "raw"],
    "Shallot":              ["vegetable", "underground", "white_yellow"],
    "Snow Peas":            ["vegetable", "green", "raw"],
    "Spinach":              ["vegetable", "green", "raw"],
    "Squash":               ["vegetable", "hot", "white_yellow"],
    "Sugar Snap Peas":      ["vegetable", "green", "raw", "handheld"],
    "Sweet Potato":         ["vegetable", "hot", "underground", "red_orange"],
    "Turnip":               ["vegetable", "underground", "white_yellow"],
    "Yam":                  ["vegetable", "hot", "underground"],
    "Zucchini":             ["vegetable", "hot", "green"],
}

# QUESTION RULES
# each tag maps to float weights for each of the 20 questions
# Format: tag -> [Q00, Q01, Q02, Q03, Q04, Q05, Q06, Q07, Q08, Q09, Q10, Q11, Q12, Q13, Q14, Q15, Q16, Q17, Q18, Q19]
tag_weights = {
    #                        Q00   Q01   Q02   Q03   Q04   Q05   Q06   Q07   Q08   Q09   Q10   Q11   Q12   Q13   Q14   Q15   Q16   Q17   Q18   Q19
    # FOOD TYPE
    "dessert":              [1.00, 0.30, 0.00, 0.00, 0.00, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.50, 0.20, 0.30, 0.10, 0.00, 0.00, 0.00, 0.20, 0.00],
    "fruit":                [0.10, 0.00, 0.00, 1.00, 0.00, 0.70, 0.60, 0.00, 0.20, 0.20, 0.20, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00],
    "vegetable":            [0.00, 0.40, 0.00, 0.00, 1.00, 0.10, 0.00, 0.20, 0.40, 0.20, 0.20, 0.40, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00],
    "snack":                [0.10, 0.10, 0.10, 0.00, 0.00, 0.20, 0.00, 0.00, 0.00, 0.00, 0.00, 0.90, 0.20, 0.10, 0.00, 0.20, 0.10, 0.00, 0.10, 1.00],
    "candy":                [0.20, 0.00, 0.00, 0.00, 0.00, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.90, 0.00, 0.00, 0.00, 0.10, 0.00, 0.00, 0.00, 1.00],
    "seafood":              [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.40, 0.10, 0.00, 0.00, 0.10, 0.20, 0.00, 0.00, 1.00],

    # COOKING METHOD
    "baked":                [0.30, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 0.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00],
    "fried":                [0.10, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.70, 1.00, 0.20, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00],
    "pan_cooked":           [0.20, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.50, 0.00, 0.20, 0.00, 0.00, 0.00, 0.00, 0.30, 0.00],
    "raw":                  [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # TEMPERATURE
    "hot":                  [0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "cold":                 [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "frozen":               [0.50, 0.00, 0.00, 0.00, 0.00, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # TASTE
    "sweet":                [0.60, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "sour":                 [0.00, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "spicy":                [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "nutty":                [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # TEXTURE
    "creamy":               [0.30, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.50, 0.00, 0.00],
    "chewy":                [0.10, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "soft":                 [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "layered":              [0.30, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # INGREDIENTS
    "meat":                 [0.00, 0.80, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "cheese":               [0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "dough":                [0.20, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "dairy":                [0.30, 0.00, 0.00, 0.00, 0.00, 0.30, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00],
    "noodles":              [0.00, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "chocolate":            [0.60, 0.00, 0.00, 0.00, 0.00, 0.90, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "fruit_based":          [0.30, 0.00, 0.00, 0.00, 0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # GROW LOCATION
    "tree_fruit":           [0.00, 0.00, 0.00, 0.90, 0.00, 0.70, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "vine_fruit":           [0.00, 0.00, 0.00, 0.90, 0.00, 0.70, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "berry":                [0.00, 0.00, 0.00, 0.90, 0.00, 0.80, 0.60, 0.00, 0.00, 0.00, 0.00, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "tropical":             [0.00, 0.00, 0.00, 0.90, 0.00, 0.70, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00],
    "melon":                [0.00, 0.00, 0.00, 0.90, 0.00, 0.80, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "citrus":               [0.00, 0.00, 0.00, 0.90, 0.00, 0.50, 1.00, 0.00, 0.00, 0.00, 0.50, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "underground":          [0.00, 0.00, 0.00, 0.00, 0.80, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # COLOUR
    "green":                [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "red_orange":           [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "white_yellow":         [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "purple":               [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # CUISINE
    "italian":              [0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "french":               [0.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "european":             [0.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "british":              [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "german":               [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "polish":               [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 1.00, 0.00, 0.00, 0.00],
    "asian":                [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "japanese":             [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "chinese":              [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "korean":               [0.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "indian":               [0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "american":             [0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00],
    "western":              [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00],
    "mexican":              [0.00, 0.60, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00],
    "middle_eastern":       [0.00, 0.50, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.20, 0.50, 0.00, 0.00, 0.00],

    # SERVING STYLE
    "handheld":             [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "soup":                 [0.00, 0.80, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00],
    "liquid":               [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00],
    "vegetarian":           [0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],
    "vegetable_dish":       [0.00, 0.50, 0.00, 0.00, 0.30, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00],

    # MEAL TIME
    "breakfast":            [0.00, 0.70, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00],
}

# COMPUTE PROPERTY TABLE
# for each food, average the weights of all its tags
NUM_FOODS = len(foods)
NUM_QUESTIONS = len(questions)

def compute_properties(food):
    tags = food_tags.get(food, [])
    if not tags:
        return [0.0] * NUM_QUESTIONS
    totals = [0.0] * NUM_QUESTIONS
    counts = [0]   * NUM_QUESTIONS
    for tag in tags:
        if tag in tag_weights:
            for q in range(NUM_QUESTIONS):
                totals[q] += tag_weights[tag][q]
                counts[q] += 1
    result = []
    for q in range(NUM_QUESTIONS):
        if counts[q] > 0:
            val = totals[q] / counts[q]
        else:
            val = 0.0
        result.append(round(min(1.0, max(0.0, val)), 2))
    return result

properties = {}
for food in foods:
    properties[food] = compute_properties(food)

# OUTPUT dataset.c
c_output = []
c_output.append("/* ----------------------------------------------------------")
c_output.append(" * dataset.c")
c_output.append(" * AUTO GENERATED by generate_dataset.py")
c_output.append(f" * {NUM_FOODS} foods x {NUM_QUESTIONS} questions")
c_output.append(" * ----------------------------------------------------------*/")
c_output.append("")
c_output.append('#include "dataset.h"')
c_output.append("")
c_output.append(f"#define NUM_FOODS     {NUM_FOODS}")
c_output.append(f"#define NUM_QUESTIONS {NUM_QUESTIONS}")
c_output.append("")

# food names array
c_output.append(f"const char *food_names[{NUM_FOODS}] = {{")
for i, food in enumerate(foods):
    comma = "," if i < NUM_FOODS - 1 else ""
    c_output.append(f'    "{food}"{comma}')
c_output.append("};")
c_output.append("")

# questions array
c_output.append(f"const char *questions[{NUM_QUESTIONS}] = {{")
for i, q in enumerate(questions):
    comma = "," if i < NUM_QUESTIONS - 1 else ""
    c_output.append(f'    "{q}"{comma}')
c_output.append("};")
c_output.append("")

# property table
c_output.append(f"float properties[{NUM_FOODS}][{NUM_QUESTIONS}] = {{")
for i, food in enumerate(foods):
    props = properties[food]
    props_str = ", ".join(f"{v:.2f}" for v in props)
    comma = "," if i < NUM_FOODS - 1 else ""
    c_output.append(f"    /* {i:3d} {food:<30s} */ {{{props_str}}}{comma}")
c_output.append("};")

dataset_c = "\n".join(c_output)

# OUTPUT dataset.h
h_output = []
h_output.append("/* ----------------------------------------------------------")
h_output.append(" * dataset.h")
h_output.append(" * AUTO GENERATED by generate_dataset.py")
h_output.append(" * ----------------------------------------------------------*/")
h_output.append("")
h_output.append("#ifndef DATASET_H")
h_output.append("#define DATASET_H")
h_output.append("")
h_output.append(f"#define NUM_FOODS     {NUM_FOODS}")
h_output.append(f"#define NUM_QUESTIONS {NUM_QUESTIONS}")
h_output.append("")
h_output.append(f"extern const char *food_names[{NUM_FOODS}];")
h_output.append(f"extern const char *questions[{NUM_QUESTIONS}];")
h_output.append(f"extern float properties[{NUM_FOODS}][{NUM_QUESTIONS}];")
h_output.append("")
h_output.append("#endif")

dataset_h = "\n".join(h_output)

# WRITE FILES
with open("dataset.c", "w") as f:
    f.write(dataset_c)

with open("dataset.h", "w") as f:
    f.write(dataset_h)

print(f"Done! Generated dataset.c and dataset.h")
print(f"  {NUM_FOODS} foods")
print(f"  {NUM_QUESTIONS} questions")
print(f"  {NUM_FOODS * NUM_QUESTIONS} total property values")
print()
print("First 5 foods and their properties:")
for food in foods[:5]:
    props = properties[food]
    print(f"  {food}: {props}")