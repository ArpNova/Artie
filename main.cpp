#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <qpixmap.h>
#include <qtransform.h>

class Mate : public QWidget
{
private:
  QPixmap spriteRight;
  QPixmap spriteLeft;
  QPixmap *currentSprite;

  QTimer *timer;
  int x_velocity = 2;
  QPoint dragPosition;
  bool isDragging = false;

public:
  Mate(QWidget *parent = nullptr) : QWidget(parent)
  {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::Tool);

    setAttribute(Qt::WA_TranslucentBackground);

    bool success = spriteRight.load("./assets/car.png");

    if (success)
    {
      spriteLeft = spriteRight.transformed(QTransform().scale(-1, 1));

      currentSprite = &spriteRight;

      resize(spriteRight.size());
    }
    else
    {
      resize(100, 100);
    }

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Mate::updatePosition);

    timer->start(30);
  }

  void updatePosition()
  {
    if (isDragging)
      return;

    int currentX = this->x();
    int currentY = this->y();

    int newX = currentX + x_velocity;

    // screen collision
    if (newX > 1080)
    {
      x_velocity = -2;
      currentSprite = &spriteLeft;
    }
    if (newX < 0)
    {
      x_velocity = 2;
      currentSprite = &spriteRight;
    }

    this->move(newX, currentY);

    update();
  }

protected:
  void mousePressEvent(QMouseEvent *event) override
  {
    if (event->button() == Qt::LeftButton)
    {
      isDragging = true;

      dragPosition =
          event->globalPosition().toPoint() - frameGeometry().topLeft();
      event->accept();
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override
  {
    if (event->buttons() & Qt::LeftButton && isDragging)
    {
      move(event->globalPosition().toPoint() - dragPosition);
      event->accept();
    }
  }

  void mouseReleaseEvent(QMouseEvent *event) override { isDragging = false; }

  void paintEvent(QPaintEvent *event) override
  {
    QPainter painter(this);

    if (currentSprite)
    {
      painter.drawPixmap(0, 0, *currentSprite);
    }
  }
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  Mate myMate;

  myMate.show();

  return app.exec();
}