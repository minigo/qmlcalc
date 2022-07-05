#include "QtCalcThread.h"
#include <QtCore/QDebug>
#include <QtCore/QTimerEvent>
#include <QtCore/QStack>
#include <QtCore/QList>

#include <calc.h>

int precedence (QChar c)
{
    if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

QStringList infixToPostfix (const QString &s)
{
    QStringList result;

    QStack<QChar> operators;
    QString operand;

    if (s.size () == 0)
        return {};
    if (s[0] == '-')
        operand += '0';

    for (int i = 0; i < s.length (); i++) {
        auto c = s[i];

        if (c.isDigit () || c == '.') {
            operand += c;
        } else {
            if (!operand.isEmpty ()) {
                result.push_back (operand);
                operand.clear ();
            }

            while (!operators.empty () && precedence (c) <= precedence (operators.top ())) {
                result.push_back (operators.top ());
                operators.pop ();
            }

            operators.push (c);
        }
    }

    if (!operand.isEmpty ()) {
        result.push_back (operand);
        operand.clear ();
    }

    while (!operators.empty ()) {
        result.push_back (operators.top ());
        operators.pop ();
    }

    return result;
}

bool isOperation (const QString &value) {
    if (value == "+" || value == "-" || value == "*" || value == "/")
        return true;
    return false;
}

Operation operation (const QString &value) {
    if (value == "+")
        return Addition;
    if (value == "-")
        return Subtractions;
    if (value == "*")
        return Multiplication;
    if (value == "/")
        return Division;
    return Unknown;
}

Worker::Worker (QtThreadSaveQueue<Request> &requestQueue,
                QtThreadSaveQueue<Responce> &responceQueue,
                QObject *parent)
    : QObject (parent)
    , _requestQueue (requestQueue)
    , _responceQueue (responceQueue)
{}

Worker::~Worker ()
{}

void Worker::start ()
{
    if (_timer == 0)
        _timer = startTimer (100);
    calculate ();
}

void Worker::finish () {
    if (_timer != 0)
        killTimer (_timer);
}

void Worker::calculate ()
{
    auto request = _requestQueue.pop (); //popSync ();
    if (!request)
        return;

    QStack<QString> stack;
    auto postfix = infixToPostfix (request->_exp);
    if (postfix.isEmpty ()) {
        thread ()->sleep (request->_timeout);
        _responceQueue.push ({request->_id, 0, -3});
        return;
    }

    while (!postfix.isEmpty ())
    {
        if (isOperation (postfix.at (postfix.size () - 1)))
        {
            bool bret {false};
            //-- первый операнд
            auto op1 = stack.pop ().toDouble (&bret);
            if (!bret) {
                thread ()->sleep (request->_timeout);
                _responceQueue.push ({request->_id, 0, -3});
                return;
            }

            //-- второй операнд
            auto op2 = stack.pop ().toDouble (&bret);
            if (!bret) {
                thread ()->sleep (request->_timeout);
                _responceQueue.push ({request->_id, 0, -3});
                return;
            }

            //-- расчитываем
            short error;
            auto result = doIt (operation (postfix.at (postfix.size () - 1)), op1, op2, &error);

            //-- спим и отправляем результат
            thread ()->sleep (request->_timeout);
            _responceQueue.push ({request->_id, result, error});
        }
        else {
            //-- сохраняем операнд
            stack.push (postfix.at (postfix.size () - 1));
            postfix.removeAt (postfix.size () - 1);
        }
    }
}

void Worker::timerEvent (QTimerEvent *event)
{
    if (event->timerId () != _timer)
        return;
    calculate ();
}
