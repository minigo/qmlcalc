#ifndef QTCALCTHREAD_H
#define QTCALCTHREAD_H

#include <QtCore/QThread>
#include "QtThreadSaveQueue.h"

struct Request {
    QString _exp;
    short _id {0};
    unsigned int _timeout {0};
};

struct Responce {
    short _id {0};
    double _result {0.0};

    //! -1 - division by zero
    //! -2 - Unknown operation
    //! -3 - convert error
    //! 0  - no error
    int _error {0};
};

class Worker : public QObject
{
public:
    explicit Worker (QtThreadSaveQueue<Request> &requestQueue,
                     QtThreadSaveQueue<Responce> &responceQueue,
                     QObject *parent = nullptr);
    ~Worker ();

public slots:
    void start ();
    void finish ();

protected:
    void calculate ();
    void timerEvent (QTimerEvent *event) override;

    QtThreadSaveQueue<Request> &_requestQueue;
    QtThreadSaveQueue<Responce> &_responceQueue;

    int _timer {0};
};

#endif // QTCALCTHREAD_H
