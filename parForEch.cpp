#include <iostream>
#include <vector>
#include <algorithm>
#include <future>

template<typename Iterator, typename Func>
void parallel_for_each(Iterator begin, Iterator end, Func&& func, size_t threshold) {
    parallel_for_each_rec(begin, end, std::forward<Func>(func), threshold);
}



template<typename Iterator, typename Func>
void parallel_for_each_rec(Iterator begin, Iterator end, Func&& func, size_t threshold) {
    const size_t total_elements = std::distance(begin, end);

    if (total_elements <= threshold) {
        std::for_each(begin, end, func);
    }
    else {
        Iterator mid = begin;
        std::advance(mid, total_elements / 2);

        // Запускаем асинхронную версию для первой половины
        std::future<void> async_task = std::async(std::launch::async, [begin, mid, &func, threshold]() {
            parallel_for_each_rec(begin, mid, func, threshold);
            });

        // Запускаем синхронную версию для второй половины
        parallel_for_each_rec(mid, end, func, threshold);

        // Ждем завершения асинхронной задачи
        async_task.wait();
    }
}



int main() {
    std::vector<int> numbers(1000, 3);


    parallel_for_each(numbers.begin(), numbers.end(), [](int& num) {
        num *= 2;
        }, 125);


    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
