#ifndef QTCALCULATOR_H
#define QTCALCULATOR_H

#include <QtCore/QObject>

#include "QtCalcThread.h"
#include "QtThreadSaveQueue.h"

class QtCalculator : public QObject
{
    Q_OBJECT

public:
    explicit QtCalculator ();
    ~QtCalculator ();

public slots:
    void slotAddSymbol (const QVariant &var);
    void slotClear ();
    void setInvoker (QObject *invoker);
    void slotExecute (const QString &exp, unsigned int timeout);

protected:
    void timerEvent (QTimerEvent *event) override;

    QObject *_invoker {nullptr};
    QString _exp;

    QtThreadSaveQueue<Request> _requestQueue;
    QtThreadSaveQueue<Responce> _responceQueue;

    QThread *_thread {nullptr};
    Worker *_worker {nullptr};

    short _id {0};
    int _timer {0};
};

#endif // QTCALCULATOR_H
