#include "QtCalcThread.h"
#include <QtCore/QDebug>
#include <QtCore/QTimerEvent>
#include <QtCore/QStack>
#include <QtCore/QList>

#include <calc.h>

#include <unistd.h>

//! \brief Приоритет операции
int precedence (QChar c)
{
    if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

//! \brief перевод инфиксной записи выражения в постфиксную
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

void Worker::start () {
    calculate ();
}

void Worker::calculate ()
{
    //-- пока есть запросы
    while (auto request = _requestQueue.popSync ())
    {
        //-- преобразование в постфиксную
        QStack<QString> stack;
        auto postfix = infixToPostfix (request._exp);
        if (postfix.isEmpty ()) {
            _responceQueue.push ({request._exp, request._id, 0, -3});
            return;
        }

        short error {0};

        while (!postfix.isEmpty ())
        {
            if (isOperation (postfix.at (0)))
            {
                bool bret {false};
                //-- первый операнд
                auto op2 = stack.pop ().toDouble (&bret);
                if (!bret) {
                    _responceQueue.push ({request._exp, request._id, 0, -3});
                    return;
                }

                //-- второй операнд
                auto op1 = stack.pop ().toDouble (&bret);
                if (!bret) {
                    _responceQueue.push ({request._exp, request._id, 0, -3});
                    return;
                }

                //-- расчитываем и сохраняем
                double result = doIt (operation (postfix.at (0)), op1, op2, &error);
                if (error != 0) {
                    _responceQueue.push ({request._exp, request._id, result, error});
                    return;
                }

                stack.push (QString::number (result));

                //-- удаляем последний
                postfix.removeAt (0);
            }
            else {
                //-- сохраняем операнд
                stack.push (postfix.at (0));
                postfix.removeAt (0);
            }
        }

        //-- спим и отправляем результат
        thread ()->sleep (request._timeout);
        _responceQueue.push ({request._exp, request._id, stack.pop ().toDouble (), error});
    }
}
