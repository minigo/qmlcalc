#include "QtCalculator.h"
#include <QtCore/QVariant>
#include <QtCore/QTimerEvent>

QtCalculator::QtCalculator ()
    : QObject (nullptr)
{
    _worker = new Worker (_requestQueue, _responceQueue, nullptr);
    _thread = new QThread (this);
    _worker->moveToThread (_thread);
    connect (_thread, &QThread::started, _worker, &Worker::start);
    _thread->start ();
}

QtCalculator::~QtCalculator ()
{
    _requestQueue.push ({"", -1, 0});

    if (_thread) {
        if (_thread->isRunning ()) {
            _thread->quit ();
            _thread->wait ();
        }
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

void QtCalculator::slotExecute (unsigned int timeout)
{
    if (_exp.isEmpty () ||
            _exp.at (_exp.size () - 1) == '.' ||
            _exp.at (_exp.size () - 1) == '+' ||
            _exp.at (_exp.size () - 1) == '*' ||
            _exp.at (_exp.size () - 1) == '/' ||
            _exp.at (_exp.size () - 1) == '-')
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

        if (_timer == 0)
            _timer = startTimer (100);
        _requestQueue.push ({_exp, _id, timeout});

        QMetaObject::invokeMethod (_invoker, "appendRequestMessage",
                                   Q_ARG(QVariant, QString ("send request (id=%1) to the queue: %2")
                                         .arg (_id).arg (_exp)));

        _id++;

        _exp.clear ();
        if (_invoker)
            QMetaObject::invokeMethod (_invoker, "updateResultArea",
                                       Q_ARG(QVariant, _exp));
    }
}

void QtCalculator::timerEvent (QTimerEvent *event)
{
    if (event->timerId () != _timer)
        return;

    updateQueueSize ();

    if (_responceQueue.empty ())
        return;

    auto responce = _responceQueue.popSync ();
    if (responce._error == 0)
        QMetaObject::invokeMethod (_invoker, "appendResponceMessage",
                                   Q_ARG(QVariant, QString ("receive result for %1 (id=%2) from the queue: %3")
                                         .arg (responce._exp).arg (responce._id).arg (responce._result)));
    else
        QMetaObject::invokeMethod (_invoker, "appendErrorMessage",
                                   Q_ARG(QVariant, QString ("receive result for %1 (id=%1) with error: %2")
                                         .arg (responce._exp).arg (responce._id).arg (responce._error)));
}

void QtCalculator::updateQueueSize ()
{
    QMetaObject::invokeMethod (_invoker, "updateRequestQueueSize",
                               Q_ARG(QVariant, QString::number (_requestQueue.size ())));
    QMetaObject::invokeMethod (_invoker, "updateResponceQueueSize",
                               Q_ARG(QVariant, QString::number (_responceQueue.size ())));
}
