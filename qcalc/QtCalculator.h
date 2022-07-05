#ifndef QTCALCULATOR_H
#define QTCALCULATOR_H

#include <QtCore/QObject>

#include "QtCalcThread.h"
#include "QtThreadSaveQueue.h"

//!
//! \brief The QtCalculator class
//!
class QtCalculator : public QObject
{
    Q_OBJECT

public:
    explicit QtCalculator ();
    ~QtCalculator ();

public slots:
    //! \brief Добавить символ в выражение
    //! Будет выполнена элементарная проверка
    //! на корректность выражения
    void slotAddSymbol (const QVariant &var);
    void slotClear ();
    void setInvoker (QObject *invoker);

    //! \brief Запуск выполнения очередного расчёта
    //! \param timeout Время в течении которого задача будет спать
    void slotExecute (unsigned int timeout);

protected:
    void timerEvent (QTimerEvent *event) override;

    //! \brief Обновление размеров очередей в интерфейсе
    void updateQueueSize ();

    QObject *_invoker {nullptr};
    //! Выражение
    QString _exp;

    //! Очередь запросов
    QtThreadSaveQueue<Request> _requestQueue;
    //! Очередь ответов
    QtThreadSaveQueue<Responce> _responceQueue;

    QThread *_thread {nullptr};
    Worker *_worker {nullptr};

    //! Счётчик запросов
    short _id {0};
    int _timer {0};
};

#endif // QTCALCULATOR_H
