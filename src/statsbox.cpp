#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   


#include "statsbox.h"
#include "digitalclock.h"
#include "timer.h"
//
StatsBox::StatsBox(QWidget *parent) :
    QFrame(parent)
{

    QGroupBox *clockBox = new QGroupBox(tr("Time"));
    DigitalClock *clock = new DigitalClock(this);
    clock->setHub(hub);
    QVBoxLayout *clockLayout = new QVBoxLayout;
    clockLayout->addWidget(clock);
    clockLayout->setContentsMargins(3,3,3,3);
    clockBox->setLayout(clockLayout);

    QGroupBox *timerBox = new QGroupBox(tr("Timer"));
    Timer *timer = new Timer(this);
    timer->setHub(hub);
    QVBoxLayout *timerLayout = new QVBoxLayout;
    timerLayout->addWidget(timer);
    timerLayout->setContentsMargins(3,3,3,3);
    timerBox->setLayout(timerLayout);




    QGridLayout *base = new QGridLayout;
    base->addWidget(clockBox, 0, 0);;
    base->addWidget(timerBox,1,0);
    setLayout(base);


    //for fullscreen editor :
    connect(timer,SIGNAL(timerSignal(QString)), this, SIGNAL(timerSignal(QString)));
}

