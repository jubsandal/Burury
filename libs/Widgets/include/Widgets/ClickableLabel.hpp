#ifndef CLICKABLELABEL_HPP_M04Y1DXJ
#define CLICKABLELABEL_HPP_M04Y1DXJ

#include <QLabel>
#include <QObject>

class ClickableLabel : public QLabel {
    Q_OBJECT

   public:
        ClickableLabel(QString txt, QWidget *p = nullptr);
        virtual ~ClickableLabel();

   signals:
        void clicked();

   protected:
        virtual void mousePressEvent(QMouseEvent* event);
};

#endif /* end of include guard: CLICKABLELABEL_HPP_M04Y1DXJ */
