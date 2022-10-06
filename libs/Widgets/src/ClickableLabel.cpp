#include "Widgets/ClickableLabel.hpp"

ClickableLabel::ClickableLabel(QString txt, QWidget* p)
    : QLabel(txt) {
    this->setParent(p);
}
ClickableLabel::~ClickableLabel() {}
void ClickableLabel::mousePressEvent(QMouseEvent* event) { Q_EMIT clicked(); }
