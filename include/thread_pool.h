#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    ThreadPool() {}
    ThreadPool(size_t threadCount, size_t maxQueueSize, int taskTimeoutMs)
        : stop_(false), maxQueueSize_(maxQueueSize),
          taskTimeoutMs_(taskTimeoutMs)
    {
        // 创建指定数量的工作线程
        for (size_t i = 0; i < threadCount; ++i) {
            workers_.emplace_back([this, taskTimeoutMs] {
                while (true) {
                    TaskWrapper task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex_);
                        this->condition_.wait(lock, [this] {
                            return this->stop_ || !this->tasks_.empty();
                        });
                        if (this->stop_ && this->tasks_.empty()) {
                            return;
                        }

                        // 检查任务是否超时
                        auto now = std::chrono::steady_clock::now();
                        task     = std::move(this->tasks_.front());
                        this->tasks_.pop();
                        taskAdded_.notify_one();  // 通知等待添加任务的线程

                        if (std::chrono::duration_cast<
                                std::chrono::milliseconds>(now -
                                                           task.enqueueTime)
                                .count() > taskTimeoutMs) {
                            std::cout << "Task timed out, skipping.\n";
                            continue;
                        }
                    }

                    task.func();  // 执行任务
                }
            });
        }
    }
    void init(size_t threadCount, size_t maxQueueSize, int taskTimeoutMs)
    {
        stop_          = false;
        maxQueueSize_  = maxQueueSize;
        taskTimeoutMs_ = taskTimeoutMs;
        // 创建指定数量的工作线程
        for (size_t i = 0; i < threadCount; ++i) {
            workers_.emplace_back([this, taskTimeoutMs] {
                while (true) {
                    TaskWrapper task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex_);
                        this->condition_.wait(lock, [this] {
                            return this->stop_ || !this->tasks_.empty();
                        });
                        if (this->stop_ && this->tasks_.empty()) {
                            return;
                        }

                        // 检查任务是否超时
                        auto now = std::chrono::steady_clock::now();
                        task     = std::move(this->tasks_.front());
                        this->tasks_.pop();
                        taskAdded_.notify_one();  // 通知等待添加任务的线程

                        if (std::chrono::duration_cast<
                                std::chrono::milliseconds>(now -
                                                           task.enqueueTime)
                                .count() > taskTimeoutMs) {
                            std::cout << "Task timed out, skipping.\n";
                            continue;
                        }
                    }

                    task.func();  // 执行任务
                }
            });
        }
    }
    int get_cur_task_num()
    {
        std::unique_lock<std::mutex> lock(this->queueMutex_);
        return this->tasks_.size();
    }
    // 提交任务到线程池
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex_);

            // 等待队列有空余
            taskAdded_.wait(lock, [this] {
                return this->tasks_.size() < maxQueueSize_ || stop_;
            });

            if (stop_) {
                throw std::runtime_error("ThreadPool is stopped");
            }

            // 将任务包装成带时间戳的任务
            tasks_.emplace(TaskWrapper{
                [task]() { (*task)(); },
                std::chrono::steady_clock::now()  // 记录任务入队时间
            });
        }
        condition_.notify_one();  // 通知工作线程有新任务
        return res;
    }

    // 停止线程池
    void shutdown()
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (std::thread& worker : workers_) {
            worker.join();
        }
    }

    ~ThreadPool()
    {
        if (!stop_) {
            shutdown();
        }
    }

  private:
    struct TaskWrapper
    {
        std::function<void()>                 func;
        std::chrono::steady_clock::time_point enqueueTime;  // 记录任务入队时间
    };

    std::vector<std::thread> workers_;
    std::queue<TaskWrapper>  tasks_;

    std::mutex              queueMutex_;
    std::condition_variable condition_;
    std::condition_variable
           taskAdded_;  // 用于在队列有空余时通知等待添加任务的线程
    bool   stop_;
    size_t maxQueueSize_;
    int    taskTimeoutMs_;  // 任务超时时间
};
