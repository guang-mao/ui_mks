// ThreadSafeQueue.h
#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
//#include <QWaitCondition>

template <typename T>
class ThreadSafeQueue
{
public:
    void enqueue(const T &value)
    {
        QMutexLocker locker(&mutex);
        queue.enqueue(value);
        //cond.wakeOne();
    }

    bool dequeue(T &value)
    {
        QMutexLocker locker(&mutex);
        //while ( queue.isEmpty() )
        if ( !queue.isEmpty() )
        {
            value = queue.dequeue();
            return true;
            //cond.wait(&mutex);
        }
        return false;
    }

    bool isEmpty() const
    {
        QMutexLocker locker(&mutex);
        return queue.isEmpty();
    }


private:
    mutable QMutex mutex;
    QQueue<T> queue;
    //QWaitCondition cond;
};

#endif // THREADSAFEQUEUE_H
