// 单生产者-单消费者模型
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
}
void ConsumerTask() // 消费者任务
{
    static int cnt = 0;
    while(1) {
        sleep(1);
        int item = ConsumeItem(&gItemRepository); // 消费一个产品.
        std::cout << "Consume the " << item << "^th item" << std::endl;
        if (++cnt == kItemsToProduce) break; // 如果产品消费个数为 kItemsToProduce, 则退出.
    }
}

int main() {
    std::thread producer(ProducerTask); // 创建生产者线程.
    std::thread consumer(ConsumerTask); // 创建消费之线程.
    producer.join();
    consumer.join();
    return 0;
}
