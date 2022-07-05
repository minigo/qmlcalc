#ifndef QTTHREADSAVEQUEUE_H
#define QTTHREADSAVEQUEUE_H

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QQueue>

#include <optional>

template<typename T>
class QtThreadSaveQueue
{
public:
    QtThreadSaveQueue () = default;
    virtual ~QtThreadSaveQueue () = default;

    QtThreadSaveQueue (const QtThreadSaveQueue &) = delete;
    QtThreadSaveQueue (QtThreadSaveQueue &&) = delete;
    QtThreadSaveQueue &operator = (const QtThreadSaveQueue&) = delete;
    QtThreadSaveQueue &operator = (QtThreadSaveQueue&&) = delete;

    void push (T&& item) {
        QMutexLocker locker (&_mutex);
        _queue.push_back (std::move (item));
        _wc.notify_all ();
    }

    size_t size () const {
        QMutexLocker locker (&_mutex);
        return _queue.size ();
    }

    bool empty () const {
        QMutexLocker locker (&_mutex);
        return _queue.empty ();
    }

    T popSync () {
        QMutexLocker locker (&_mutex);

        if (_queue.empty ())
            _wc.wait (&_mutex);

        auto item = std::move (_queue.front ());
        _queue.pop_front ();
        return item;
    }

    std::optional<T> pop () {
        QMutexLocker locker (&_mutex);
        if (_queue.empty ())
            return std::nullopt;
        auto item = std::move (_queue.front ());
        _queue.pop_front ();
        return item;
    }

protected:
    mutable QMutex _mutex;
    mutable QWaitCondition _wc;

    QQueue<T> _queue;
};

#endif // QTTHREADSAVEQUEUE_H
