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
#include <qwindowdefs.h>

class Mate : public QWidget {
private:
  QPixmap spriteRight;
  QPixmap spriteLeft;
  QPixmap *currentSprite;

  QTimer *timer;
  QPoint dragPosition;
  bool isDragging = false;

  int y_velocity = 0;
  const int gravity = 2;

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
    if (isDragging){
      y_velocity = 0;
      return;
    }
      

    QScreen *screen = QGuiApplication::primaryScreen();

    QRect screenRect = screen->availableGeometry();

    int screenWidth = screen->geometry().width();

    int screenBottom = screenRect.bottom();

    y_velocity += gravity;

    int currentX = this->x();
    int nextY = this->y() + y_velocity;

    if(nextY + this->height() >= screenBottom){
      nextY = screenBottom - this->height();
      
      y_velocity = -y_velocity * 0.5;
    }

    // screen collision
    if (currentX >= screenWidth - this->width()) {

      currentSprite = &spriteLeft;
    }
    if (currentX <= 0) {

      currentSprite = &spriteRight;
    }

    this->move(currentX, nextY);

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