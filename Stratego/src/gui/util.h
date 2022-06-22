#ifndef UTIL_H
#define UTIL_H

#include <QWidget>
#include <QApplication>
#include <QScreen>

namespace util{

    /**
     * Centre le widget donné au milieu de l'écran principale.
     *
     * @param widget le widget à centrer
     */
    inline void centerWidget(QWidget* widget){
        QSize mSize = widget -> sizeHint();
        QScreen* screen = QApplication::primaryScreen();
        QRect screenRect = screen -> geometry();
        int newX = screenRect.width() / 2 - mSize.width() / 2;
        int newY = screenRect.height() / 2 - mSize.height() / 2;
        widget -> move(newX, newY);
    }
}

#endif // UTIL_H
