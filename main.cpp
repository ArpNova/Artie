#include <QApplication>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <qaction.h>
#include <qevent.h>
#include <qguiapplication.h>
#include <qmenu.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qscreen.h>
#include <qsize.h>
#include <qtransform.h>
#include <qwindowdefs.h>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <math.h>

class Mate : public QWidget {
private:
  QPixmap originalSprite; 
  QPixmap spriteRight;
  QPixmap spriteLeft;
  QPixmap *currentSprite;

  QPixmap bodySprite, headSprite, armSprite;

  //Animation state
  double time = 0.0;

  //physics output
  double breathOffset = 0.0;
  double armAngle = 0.0;


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

    bodySprite.load("./assets/body.png");
    headSprite.load("./assets/head.png");
    armSprite.load("./assets/arm.png");

    int canvasWidth = 150;
    int canvasHeight = 200;

    setFixedSize(canvasWidth, canvasHeight);
    // bool success = originalSprite.load("./assets/Eren.png");

    // if (success) {
    //   changeSize(1.0);
    // } else {
    //   resize(100, 100);
    // }

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

    time += 0.1;

    breathOffset = std::sin(time * 2.0) * 2.0;
    armAngle = std::sin(time) * 5.0;

    update();

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

  void changeSize(float scale){
    if(originalSprite.isNull())return;

    QSize newSize = originalSprite.size() * scale;

    spriteRight = originalSprite.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    spriteLeft = spriteRight.transformed(QTransform().scale(-1, 1));

    int heightDifference = newSize.height() - this->height();
    int newY = this->y() - heightDifference;

    resize(newSize);
    move(this->x(), newY);

    if(currentSprite == &spriteLeft){
      currentSprite = &spriteLeft;
    }else{
      currentSprite = &spriteRight;
    }

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

  void contextMenuEvent(QContextMenuEvent *event) override {
    QMenu menu(this);

    QAction *smallAction = menu.addAction("Small (50%)");
    QAction *normalAction = menu.addAction("Normal (100%)");

    connect(smallAction, &QAction::triggered, this, [this](){
      changeSize(0.5);
    });

    connect(normalAction, &QAction::triggered, this, [this](){
      changeSize(1.0);
    });

    menu.exec(event->globalPos());
  }

  void paintEvent(QPaintEvent *event) override {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //draw
    int bodyX = (this->width() - bodySprite.width()) / 2;
    int bodyY = 80;
    bodyY += (int)breathOffset;

    painter.drawPixmap(bodyX, bodyY, bodySprite);

    int neckX = bodyX + (bodySprite.width() / 2) - (headSprite.width() / 2);
    int neckY = bodyY - (headSprite.height() / 2);

    painter.drawPixmap(neckX, neckY, headSprite);

    painter.save();

    painter.translate(bodyX + 9, bodyY + 19);

    painter.rotate(armAngle);

    painter.drawPixmap(-armSprite.width()/2, 0, armSprite);

    painter.restore();
 
    // if (currentSprite) {
    //   painter.drawPixmap(0, 0, *currentSprite);
    // }
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Mate myMate;

  myMate.show();

  return app.exec();
}