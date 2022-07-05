#include "QtCalculator.h"
#include <QtCore/QDebug>
#include <QtCore/QTimerEvent>

QtCalculator::QtCalculator ()
    : QObject (nullptr)
{
    _worker = new Worker (_requestQueue, _responceQueue, nullptr);
    _thread = new QThread (this);
    _worker->moveToThread (_thread);
    connect (_thread, &QThread::started, _worker, &Worker::start);
    connect (_thread, &QThread::finished, _worker, &Worker::finish);
    _thread->start ();
}

QtCalculator::~QtCalculator ()
{
    if (_thread) {
        if (_thread->isRunning ()) {
            _thread->quit ();
            if (!_thread->wait (10))
                _thread->terminate ();
        }

        if (_worker)
            delete _worker;
        delete _thread;
    }
}

void QtCalculator::slotAddSymbol (const QVariant &var)
{
    QChar ch = var.toString ().isEmpty () ? QChar () : var.toString ().at (0);
    if (ch.isNull ())
        return;

    if (_exp.isEmpty ()) {
        if (ch == '-' || ch.isDigit ()) {
            _exp = ch;

            if (_invoker)
                QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                           Q_ARG(QVariant, _exp));
            return;
        } else {
            return;
        }
    }

    auto lch = _exp[_exp.size () - 1];
    if (lch.isDigit ()) {
        if (ch == '.' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch.isDigit ()) {
            _exp.append (ch);

            if (_invoker)
                QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                           Q_ARG(QVariant, _exp));
        }
    }
    else if (lch == '+' || lch == '-' || lch == '*' || lch == '/') {
        if (ch.isDigit ()) {
            _exp.append (ch);

            if (_invoker)
                QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                           Q_ARG(QVariant, _exp));
        }
    }
    else if (lch == '.') {
        if (ch.isDigit ()) {
            _exp.append (ch);

            if (_invoker)
                QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                           Q_ARG(QVariant, _exp));
        }
    }
}

void QtCalculator::slotClear () {
    _exp = "";

    if (_invoker)
        QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                   Q_ARG(QVariant, _exp));
}

void QtCalculator::setInvoker (QObject *invoker) {
    _invoker = invoker;
}

void QtCalculator::slotExecute (const QString &exp, unsigned int timeout)
{
    if (exp.isEmpty () ||
            exp.at (exp.size () - 1) == '.' ||
            exp.at (exp.size () - 1) == '+' ||
            exp.at (exp.size () - 1) == '*' ||
            exp.at (exp.size () - 1) == '/' ||
            exp.at (exp.size () - 1) == '-')
    {
        if (_invoker)
            QMetaObject::invokeMethod (_invoker, "appendErrorMessage",
                                       Q_ARG(QVariant, "Invalid input string: " + _exp));

        _exp.clear ();
        if (_invoker)
            QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                       Q_ARG(QVariant, _exp));
    }
    else
    {
        if (_exp.at (0) == '-')
            _exp = '0' + _exp;

        QMetaObject::invokeMethod (_invoker, "appendRequestMessage",
                                   Q_ARG(QVariant, QString ("push request (id %1) to the queue: %2").arg (_id).arg (_exp)));

        if (_timer == 0)
            _timer = startTimer (100);
        _requestQueue.push ({_exp, _id++, timeout});
    }
}

void QtCalculator::timerEvent (QTimerEvent *event)
{
    if (event->timerId () != _timer)
        return;

    auto responce = _responceQueue.pop ();
    if (!responce)
        return;

    QMetaObject::invokeMethod (_invoker, "appendResponceMessage",
                               Q_ARG(QVariant, QString ("pop responce (id %1) from the queue: %2").arg (responce->_id).arg (_exp)));
}
