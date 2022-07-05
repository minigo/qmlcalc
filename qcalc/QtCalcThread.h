#ifndef QTCALCTHREAD_H
#define QTCALCTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include "QtThreadSaveQueue.h"

struct Request;
struct Responce;

//!
//! \brief Класс непосредственно расчёта.
//!
class Worker : public QObject
{
public:
    //! \param requestQueue Очередь запросов
    //! \param responceQueue Очередь ответов
    explicit Worker (QtThreadSaveQueue<Request> &requestQueue,
                     QtThreadSaveQueue<Responce> &responceQueue,
                     QObject *parent = nullptr);
    ~Worker ();

public slots:
    //! Запуск процесса обработки
    void start ();

protected:
    void calculate ();

    //! \brief Очередь запросов
    QtThreadSaveQueue<Request> &_requestQueue;
    //! \brief Очередь ответов
    QtThreadSaveQueue<Responce> &_responceQueue;
};

struct Request {
    QString _exp;
    short _id {0};
    unsigned int _timeout {0};

    explicit operator bool () const {
        return _id >= 0;
    }
};

struct Responce {
    QString _exp;
    short _id {0};
    double _result {0.0};

    //! -1 - division by zero
    //! -2 - Unknown operation
    //! -3 - convert error
    //! 0  - no error
    int _error {0};
};

#endif // QTCALCTHREAD_H
