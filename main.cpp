#include <QApplication>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <qguiapplication.h>
#include <qnamespace.h>
#include <qpixmap.h>
#include <qscreen.h>
#include <qtransform.h>

class Mate : public QWidget {
private:
  QPixmap spriteRight;
  QPixmap spriteLeft;
  QPixmap *currentSprite;

  QTimer *timer;
  QPoint dragPosition;
  bool isDragging = false;

public:
  Mate(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::Tool);

    setAttribute(Qt::WA_TranslucentBackground);

    bool success = spriteRight.load("./assets/Eren.png");

    if (success) {
      spriteLeft = spriteRight.transformed(QTransform().scale(-1, 1));

      currentSprite = &spriteRight;

      resize(spriteRight.size());
    } else {
      resize(100, 100);
    }

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Mate::updatePosition);

    timer->start(30);
  }

  void updatePosition() {
    if (isDragging)
      return;

    QScreen *screen = QGuiApplication::primaryScreen();
    int screenWidth = screen->geometry().width();

    int currentX = this->x();
    int currentY = this->y();

    // screen collision
    if (currentX >= screenWidth - this->width()) {

      currentSprite = &spriteLeft;
    }
    if (currentX <= 0) {

      currentSprite = &spriteRight;
    }

    this->move(currentX, currentY);

    update();
  }

protected:
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      isDragging = true;

      dragPosition =
          event->globalPosition().toPoint() - frameGeometry().topLeft();
      event->accept();
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    if (event->buttons() & Qt::LeftButton && isDragging) {
      move(event->globalPosition().toPoint() - dragPosition);
      event->accept();
    }
  }

  void mouseReleaseEvent(QMouseEvent *event) override { isDragging = false; }

  void paintEvent(QPaintEvent *event) override {
    QPainter painter(this);

    if (currentSprite) {
      painter.drawPixmap(0, 0, *currentSprite);
    }
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Mate myMate;

  myMate.show();

  return app.exec();
}