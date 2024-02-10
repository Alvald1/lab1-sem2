import random

def generate_random_numbers(num_numbers, min_value, max_value):
    random_numbers = [random.randint(min_value, max_value) for _ in range(num_numbers)]
    return random_numbers

def save_numbers_to_file(numbers, filename):
    with open(filename, 'w') as file:
        for number in numbers:
            file.write(str(number) + '\n')

if __name__ == "__main__":
    random_numbers = generate_random_numbers(50_000_000, -100, 100)
    save_numbers_to_file(random_numbers, 'test_50_000_000.txt')
    print(f"Случайные числа сохранены в файл test_50_000_000.txt")
