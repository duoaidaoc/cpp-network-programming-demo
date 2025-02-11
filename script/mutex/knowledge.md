1. std::unique_lock<std::mutex> lock(tasks_mtx); 的作用是通过构造函数 传递引用 来将 tasks_mtx 锁定，而不是拷贝 tasks_mtx。 
2. std::unique_lock 不能直接用于锁定非同步类型的对象。如果你想要锁定其他类型的资源，你需要确保这些资源是支持锁定的类型，通常是类似互斥量的同步对象。例如： 
std::mutex：最常见的锁定类型。
std::recursive_mutex：允许同一线程多次锁定的互斥量。
std::timed_mutex：可以进行超时等待的互斥量。
std::shared_mutex：用于共享锁，允许多个读锁和一个写锁的同步机制（C++17+）。
3. 线程阻塞：当 cv.wait() 被调用时，线程会被阻塞，直到条件满足（即 stop 为 true 或者 tasks 队列不为空）。在等待期间，线程会释放锁 (lock) 以允许其他线程访问共享资源（如任务队列）。

4. std::condition_variable 中 notify_one 和 notify_all 的区别
cv.notify_one()：唤醒一个等待线程，适用于你只希望唤醒一个线程进行处理的情况。
cv.notify_all()：唤醒所有等待线程，适用于你希望所有等待线程都能继续执行的情况。
选择使用哪种方法
性能考虑：如果你知道只有一个线程需要继续执行，使用 cv.notify_one() 可以避免不必要的上下文切换，提升性能。
多线程协作：如果多个线程都依赖于相同的条件，并且你希望所有线程都有机会继续执行，那么 cv.notify_all() 更合适。