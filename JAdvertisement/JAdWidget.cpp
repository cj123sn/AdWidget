#include "JAdWidget.h"
#include "ui_JAdWidget.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QEventLoop>

static const QColor c_colorSelected = QColor(220,0,0);
static const QColor c_colorUnSelected = QColor(200,200,200);

static const int c_nAdPlayTime = 500;

JAdWidget::JAdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JAdWidget)
{
    ui->setupUi(this);
    initVar();
    initConnect();

    for(int i = 0;i<5;i++)
    {
    }
}

JAdWidget::~JAdWidget()
{
    delete ui;
}

void JAdWidget::addPages(QList<QPixmap> pics)
{
    foreach(QPixmap var,pics)
    {
        addPage(var);
    }
}

void JAdWidget::addPage(QPixmap pic)
{
    m_picList.append(pic);
    addIndicator();
    if(m_currentIndex == -1)
    {
        m_currentIndex =0;
        m_indicators.at(m_currentIndex)->setSelect(true);
    }
    //m_currentIndex = m_picList.size();

    play(m_currentIndex);
}

void JAdWidget::addIndicator()
{
    JAdIndicator * indicator = new JAdIndicator(this);
    connect(indicator,&JAdIndicator::sigIconClick,this,&JAdWidget::slotIndicatorClicked);
    indicator->setFixedSize(18, 5);
    m_indicators.append(indicator);
    ui->hLayout->setSpacing(10);
    ui->hLayout->addWidget(indicator);
}

void JAdWidget::setPagePixmap(JAdPage* page,QPixmap pixmap)
{
    if(page)
    {
        page->setPixmap(pixmap.scaled(m_pageSize,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }


}

void JAdWidget::play(int index)
{
    if(index<0) return;
    int count = m_picList.size();
    if(count < 2) return;

    int leftIndex = index -1;
    int rightIndex = index +1;

    // 小于最小,播放最大index
    if(leftIndex<0)
    {
        leftIndex = count-1;
    }
    // 大于最大,播放最小
    if(rightIndex>=count)
    {
        rightIndex = 0;
    }

    this->setPagePixmap(m_pLeftPage,m_picList.at(leftIndex));
    this->setPagePixmap(m_pRightPage,m_picList.at(rightIndex));
    this->setPagePixmap(m_pCentrePage,m_picList.at(index));
}

void JAdWidget::startAnimation(int direction)
{
    QRect leftRect = m_pLeftPage->geometry();
    QRect centerRect = m_pCentrePage->geometry();
    QRect rightRect = m_pRightPage->geometry();

    QRect tmpRect = QRect(centerRect.x(), centerRect.y()+20,
                          centerRect.width(), centerRect.height()-20);


//    QRect temRightToCentre = QRect(centerRect.x(),centerRect.y(),
//                                   centerRect.width()-rightRect.width(),centerRect.height());


    QRect tmpLeftToRight = QRect(rightRect.x()+rightRect.width(),rightRect.y(),
                                 0,rightRect.height());

    QRect tmpRightToLeft = QRect(leftRect.x() + leftRect.width(),leftRect.y(),
                                 0,leftRect.height());

    if(direction >0)
    {
        m_pLeftAnimation->setStartValue(tmpRect);
        m_pLeftAnimation->setEndValue(leftRect);
        m_pCentreAnimation->setStartValue(rightRect);
        m_pCentreAnimation->setEndValue(centerRect);
        m_pRightAnimation->setStartValue(tmpLeftToRight);
        m_pRightAnimation->setEndValue(rightRect);
    }
    else
    {
        m_pLeftAnimation->setStartValue(tmpRightToLeft);
        m_pLeftAnimation->setEndValue(leftRect);
        m_pCentreAnimation->setStartValue(leftRect);
        m_pCentreAnimation->setEndValue(centerRect);
        m_pRightAnimation->setStartValue(tmpRect);
        m_pRightAnimation->setEndValue(rightRect);
    }



    m_pRightAnimation->start();
    m_pLeftAnimation->start();
    m_pCentreAnimation->start();

}

void JAdWidget::initVar()
{
    qDebug()<<"1234"<<this->width()<<"1111"<<this->height();
    m_pageSize = QSize(400,200);


    m_pLeftPage = new JAdPage(this);
    m_pCentrePage = new JAdPage(this);
    m_pRightPage = new JAdPage(this);
    m_pCentrePage->setActive(true);

    m_pLeftAnimation = new QPropertyAnimation(m_pLeftPage,"geometry");
    m_pLeftAnimation->setDuration(c_nAdPlayTime);
    m_pLeftAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pCentreAnimation = new QPropertyAnimation(m_pCentrePage,"geometry");
    m_pCentreAnimation->setDuration(c_nAdPlayTime);
    m_pCentreAnimation->setEasingCurve(QEasingCurve::Linear);
    m_pRightAnimation = new QPropertyAnimation(m_pRightPage,"geometry");
    m_pRightAnimation->setDuration(c_nAdPlayTime);
    m_pRightAnimation->setEasingCurve(QEasingCurve::Linear);

    m_pCentrePage->setFixedSize(m_pageSize);
    m_pLeftPage->setMinimumWidth(m_pageSize.width()/4);
    m_pLeftPage->setMaximumWidth(m_pageSize.width()*2/3);
    m_pLeftPage->setFixedHeight(m_pageSize.height()-20);
    m_pRightPage->setMinimumWidth(m_pageSize.width()/4);
    m_pRightPage->setMaximumWidth(m_pageSize.width()*2/3);
    m_pRightPage->setFixedHeight(m_pageSize.height()-20);


    ui->centreHLayout->addWidget(m_pLeftPage);
    ui->centreHLayout->addWidget(m_pCentrePage);
    ui->centreHLayout->addWidget(m_pRightPage);

}

void JAdWidget::initConnect()
{
    connect(m_pLeftPage,&JAdPage::sigClicked,this,&JAdWidget::slotAdPageClicked);
    connect(m_pCentrePage,&JAdPage::sigClicked,this,&JAdWidget::slotAdPageClicked);
    connect(m_pRightPage,&JAdPage::sigClicked,this,&JAdWidget::slotAdPageClicked);

}

void JAdWidget::slotIndicatorClicked()
{
    if(m_pRightAnimation->state()!= QAbstractAnimation::Stopped)
        return;

    JAdIndicator* pIndicator = dynamic_cast<JAdIndicator*>(sender());
    foreach(JAdIndicator* var,m_indicators)
    {
        var->setSelect(false);
    }
    pIndicator->setSelect(true);
    int index = m_indicators.indexOf(pIndicator);
    startAnimation(m_currentIndex<index);
    play(index);
    m_currentIndex = index;

}

void JAdWidget::slotAdPageClicked()
{

}

JAdIndicator::JAdIndicator(QWidget *parent)
{

}

JAdIndicator::~JAdIndicator()
{

}

void JAdIndicator::setSelect(bool select)
{
    m_bSelected = select;
    update();
}

void JAdIndicator::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if (m_bEnable)
    {
        emit sigIconClick();
    }
}

void JAdIndicator::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    update();
    if (m_bEnable)
    {
        emit sigIconClick();
    }
}

void JAdIndicator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if(m_bSelected){
        painter.setBrush(QBrush(c_colorSelected));
    }else{
        painter.setBrush(QBrush(c_colorUnSelected));
    }

    painter.drawRect(rect());
}

JAdPage::JAdPage(QWidget *parent)
{

}

JAdPage::~JAdPage()
{

}

void JAdPage::setActive(bool active)
{
    m_bActive = active;
}

void JAdPage::mousePressEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button()== Qt::LeftButton)
    {
        emit sigClicked();
    }
}

void JAdPage::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    if(!m_bActive){
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(10,10,10, 100));
        painter.drawRect(rect());
    }
}
