#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

template <typename T>
class BlockingQueue
{
public:
    void enqueue(const T &item)
    {
        QMutexLocker locker(&mutex);
        queue.enqueue(item);
        cond.wakeOne();
    }

    T dequeue()
    {
        QMutexLocker locker(&mutex);
        while ( queue.isEmpty() )
        {
            cond.wait(&mutex);
        }
        return queue.dequeue();
    }

    bool isEmpty()
    {
        QMutexLocker locker(&mutex);
        return queue.isEmpty();
    }

    QQueue<T> queue;
private:

    QMutex mutex;
    QWaitCondition cond;
};

#endif // BLOCKINGQUEUE_H
