// parForEch.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <iterator>

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <iterator>

template<typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end,
	Function&& func, size_t num_threads = std::thread::hardware_concurrency()) {
	const size_t total_elements = std::distance(begin, end);
	const size_t block_size = (total_elements + num_threads - 1) / num_threads;

	std::vector<std::thread> threads;
	threads.reserve(num_threads);

	for (size_t i = 0; i < num_threads; ++i) {
		Iterator block_begin = std::next(begin, i * block_size);
		Iterator block_end = std::next(block_begin, std::min(block_size, static_cast<size_t>
			(std::distance(block_begin, end)
				)
		));
		threads.emplace_back([block_begin, block_end, &func]() {
			std::for_each(block_begin, block_end, func);
			});
	}

	for (std::thread& t : threads) {
		t.join();
	}
}

int main() {
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	parallel_for_each(vec.begin(), vec.end(), [](int& num) {
		num *= 2;
		std::cout << std::endl;
		});

	for (int num : vec) {
		std::cout << num << " ";
	}
	std::cout << std::endl;

	return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
