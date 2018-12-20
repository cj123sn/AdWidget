#ifndef JADWIDGET_H
#define JADWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>

class QPropertyAnimation;

class JAdIndicator :public QWidget
{
    Q_OBJECT

public:
    explicit JAdIndicator(QWidget *parent = 0);
    ~JAdIndicator();

    void setSelect(bool select);

protected:
    void enterEvent(QEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    bool m_bSelected = false;
    bool m_bEnable = true;

signals:
    void sigIconClick();
};

class JAdPage :public QLabel
{
    Q_OBJECT

public:
    explicit JAdPage(QWidget *parent = 0);
    ~JAdPage();

    void setActive(bool active);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_bActive = false;

signals:
    void sigClicked();
};

namespace Ui {
class JAdWidget;
}

class JAdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JAdWidget(QWidget *parent = 0);
    ~JAdWidget();

    void addPages(QList<QPixmap> pics);
    void addPage(QPixmap pic);
    void addIndicator();
    void setPagePixmap(JAdPage* page, QPixmap pixmap);
    void play(int index);
    void startAnimation(int direction);
private:
    void initVar();
    void initConnect();

private:
    Ui::JAdWidget *ui;
    QList<JAdIndicator*> m_indicators;
    JAdPage* m_pLeftPage = NULL;
    JAdPage* m_pCentrePage = NULL;
    JAdPage* m_pRightPage = NULL;
    QPropertyAnimation* m_pLeftAnimation = NULL;
    QPropertyAnimation* m_pCentreAnimation = NULL;
    QPropertyAnimation* m_pRightAnimation = NULL;
    QSize m_pageSize;
    QList<QPixmap> m_picList;
    int m_currentIndex = 0;

public slots:
    void slotIndicatorClicked();
    void slotAdPageClicked();
};




#endif // JADWIDGET_H
