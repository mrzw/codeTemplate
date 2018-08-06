// 单生产者-多消费者模型
// 与单生产者和单消费者模型不同的是，单生产者-多消费者模型中可以允许多个消费者同时从产品库中取走产品。所以除了保护产品库在多个读写线程下互斥之外，还需要维护消费者取走产品的计数器
#include <unistd.h>
#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

static const int kItemRepositorySize  = 10;
static const int kItemsToProduce  = 20;

struct ItemRepository {
    int item_buffer[kItemRepositorySize]; // 产品缓冲区, 配合 read_position 和 write_position 模型环形队列.
    int read_position = 0; // 消费者读取产品位置.
    int write_position = 0; // 生产者写入产品位置.
    std::mutex mtx;
    std::condition_variable repo_not_full; // 条件变量, 指示产品缓冲区不为满
    std::condition_variable repo_not_empty; // 条件变量, 指示产品缓冲区不为空.
    // 相对于单生产者单消费者模型新添加了个计数器
    int item_counter = 0;
    std::mutex ite_counter_mtx;
}gItemRepository;

void ProduceItem(ItemRepository *ir, int item) {
    std::unique_lock<std::mutex> lock(ir->mtx);
    while((ir->write_position+1)%kItemRepositorySize == ir->read_position) {
        std::cout << "Producer is waiting for an empty slot...\n";
        (ir->repo_not_full).wait(lock); // 生产者等待"产品库缓冲区不为满"这一条件发生.
    }
    (ir->item_buffer)[ir->write_position] = item; // 写入产品
    (ir->write_position)++; // 写入位置后移
    if(ir->write_position == kItemRepositorySize) // 写入位置若是在队列最后则重新设置为初始位置.
        ir->write_position = 0;
    (ir->repo_not_empty).notify_all(); // 通知消费者产品库不为空.
}
int ConsumeItem(ItemRepository *ir) {
    int data;
    std::unique_lock<std::mutex> lock(ir->mtx);
    while(ir->write_position == ir->read_position) {
        std::cout << "Consumer is waiting for items...\n";
        (ir->repo_not_empty).wait(lock); // 消费者等待"产品库缓冲区不为空"这一条件发生.
    }
    data = (ir->item_buffer)[ir->read_position]; // 读取某一产品
    (ir->read_position)++; // 读取位置后移
    if (ir->read_position >= kItemRepositorySize) // 读取位置若移到最后，则重新置位.
        ir->read_position = 0;
    (ir->repo_not_full).notify_all(); // 通知消费者产品库不为满.
    return data;
}

void ProducerTask() // 生产者任务
{
    for (int i = 1; i <= kItemsToProduce; ++i) {
        // sleep(1);
        std::cout << "Produce the " << i << "^th item..." << std::endl;
        ProduceItem(&gItemRepository, i); // 循环生产 kItemsToProduce 个产品.
    }
    std::cout << "Producer thread is exiting..." << std::endl;
}
void ConsumerTask() // 消费者任务
{
    while(1) {
        sleep(1);
        std::unique_lock<std::mutex> lock(gItemRepository.ite_counter_mtx);
        if(gItemRepository.item_counter < kItemsToProduce) {
            int item = ConsumeItem(&gItemRepository);
            ++(gItemRepository.item_counter);
            std::cout << "Consumer thread " << std::this_thread::get_id()
                      << " is consuming the " << item << "^th item" << std::endl;
        } else {
            std::cout << "Consumer thread " << std::this_thread::get_id()
                      << " is exiting..." << std::endl;
            break;
        }
    }
}

int main() {
    std::thread producer(ProducerTask); // 创建生产者线程.
    std::thread consumer1(ConsumerTask); // 创建消费之线程.
    std::thread consumer2(ConsumerTask);
    producer.join();
    consumer1.join();
    consumer2.join();
    return 0;
}
