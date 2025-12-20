#include <QApplication>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <qaction.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qscreen.h>
#include <qwindowdefs.h>

class Mate : public QWidget {
private:
  // windowSize
  double baseWindowWidth;
  double baseWindowheight;

  //State
  enum MateState{
    Falling,
    OnGround,
    Dragged
  };

  MateState state = Falling;

  //Horizontal Movement
  int  xVelocity = 0;
  int walkDirection = 0;
  int walkSpeed = 2;

  // AI Timing
  int decisionTimer = 0;
  int decisionInterval = 120;

  // Whole character scale
  double characterScale = 1.0;

  // Sprites
  QPixmap bodySprite;
  QPixmap headSprite;
  QPixmap armSprite;
  QPixmap legSprite;

  // Animation state
  double time = 0.0;
  double breathOffset = 0.0;
  double armAngleRight = 0.0;
  double armAngleLeft = 0.0;
  double legAngleLeft = 0.0;
  double legAngleRight = 0.0;

  // Arm appearance
  double armScaleX = 0.8;
  double armScaleY = 0.54;
  QColor armColor = QColor(220, 180, 140);

  // leg appearence
  double legScaleX = 0.8;
  double legScaleY = 0.6;
  QColor legColor = QColor(220, 180, 140);

  // Physics
  int yVelocity = 0;
  const int gravity = 2;

  // Dragging
  bool isDragging = false;
  QPoint dragOffset;

  QTimer *timer;

public:
  Mate(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                   Qt::Tool);

    setAttribute(Qt::WA_TranslucentBackground);

    bodySprite.load("./assets/body.png");
    headSprite.load("./assets/head.png");
    armSprite.load("./assets/arm.png");
    legSprite.load("./assets/arm.png");

    baseWindowWidth = armSprite.height() * 1.5;
    baseWindowheight =
        legSprite.height() + bodySprite.height() + headSprite.height() - 1.5;
    setFixedSize(baseWindowWidth, baseWindowheight);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Mate::tick);
    timer->start(30);
  }

private:
  // Utility
  QPixmap tintPixmap(const QPixmap &src, const QColor &color) {
    QPixmap result(src.size());
    result.fill(Qt::transparent);

    QPainter p(&result);
    p.drawPixmap(0, 0, src);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(result.rect(), color);
    return result;
  }

  // Update Loop
  void tick() {
    if (state != Dragged) {
      applyPhysics();
      updateAI();
      move(x() + xVelocity, y());

      QScreen *screen = QGuiApplication::primaryScreen();
      QRect screenRect = screen->availableGeometry();

      if(x() <= screenRect.left()){
        move(screenRect.left(), y());
        walkDirection = 1;
        xVelocity = walkSpeed;
      }

      if(x() + width() >= screenRect.right()){
        move(screenRect.right() - width(), y());
        walkDirection = -1;
        xVelocity = -walkSpeed;
      }
    }

    updateAnimation();
    update();
  }

  void applyPhysics() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect bounds = screen->availableGeometry();

    yVelocity += gravity;
    int nextY = y() + yVelocity;

    if (nextY + height() >= bounds.bottom()) {
      nextY = bounds.bottom() - height();
      yVelocity = 0;
      state = OnGround;
    }else{
      state = Falling;
    }

    move(x(), nextY);
  }

  void updateAnimation() {
    time += 0.1;
    breathOffset = std::sin(time * 2.0) * 2.0;
    
    if(state == Dragged){
      legAngleLeft = 0;
      legAngleRight = 0;

      armAngleRight = 0;
      armAngleLeft = 0;
    }else if(state == OnGround && walkDirection != 0){
      legAngleLeft = -std::sin(time * 2.0) * 15;
      legAngleRight = std::sin(time * 2.0) * 15;

      armAngleLeft = -std::sin(time * 2.0) * 10;
      armAngleRight = std::sin(time * 2.0) * 10;
    }else{
      legAngleRight = 0;
      legAngleLeft = 0;

      armAngleLeft = 0;
      armAngleRight = 0;
    }
  }

  void updateWindowSize() {
    int baseWidth = baseWindowWidth;
    int baseHeight = baseWindowheight;

    setFixedSize(baseWidth * characterScale, baseHeight * characterScale);
  }

  void updateAI(){
    if(state != OnGround)return;

    decisionTimer++;

    if(decisionTimer >= decisionInterval){
      decisionTimer = 0;

      walkDirection = (std::rand() % 3) -1;
      xVelocity = walkDirection * walkSpeed;
    }
  }

protected:
  // Mouse
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      isDragging = true;
      state = Dragged;
      xVelocity = 0;
      yVelocity = 0;
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    if (isDragging) {
      move(event->globalPosition().toPoint() - dragOffset);
    }
  }

  void mouseReleaseEvent(QMouseEvent *) override { 
    isDragging = false;
    state = Falling;
  }

  // Context Menu
  void contextMenuEvent(QContextMenuEvent *event) override {

    QMenu menu(this);

    QAction *smallMate = menu.addAction("Small Mate");
    QAction *normalMate = menu.addAction("Normal Mate");
    QAction *largeMate = menu.addAction("Large Mate");
    menu.addSeparator();

    connect(smallMate, &QAction::triggered, this, [&]() {
      characterScale = 0.7;
      updateWindowSize();
    });

    connect(normalMate, &QAction::triggered, this, [&]() {
      characterScale = 1.0;
      updateWindowSize();
    });

    connect(largeMate, &QAction::triggered, this, [&]() {
      characterScale = 1.3;
      updateWindowSize();
    });

    QAction *thinArms = menu.addAction("Thin Arms");
    QAction *thickArms = menu.addAction("Thick Arms");
    QAction *redArms = menu.addAction("Red Arms");
    

    connect(thinArms, &QAction::triggered, this, [&]() { armScaleX = 0.8; });

    connect(thickArms, &QAction::triggered, this, [&]() { armScaleX = 1.3; });

    connect(redArms, &QAction::triggered, this,
            [&]() { armColor = QColor(180, 60, 60); });

    

    menu.exec(event->globalPos());
  }

  // Rendering
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.save();
    painter.scale(characterScale, characterScale);

    int bodyX = (width() - bodySprite.width()) / 2;
    int bodyY = 80 + static_cast<int>(breathOffset);

    int bodyBottomY = bodyY + bodySprite.height();

    // LEG JOINTS
    QPoint leftHip(bodyX + bodySprite.width() * 0.45, bodyBottomY);

    QPoint rightHip(bodyX + bodySprite.width() * 0.55, bodyBottomY);

    QPixmap coloredLimb = tintPixmap(armSprite, armColor);

    drawLimb(painter, leftHip, legAngleRight, coloredLimb, false, legScaleX,
             legScaleY);
    drawLimb(painter, leftHip, legAngleLeft, coloredLimb, true, legScaleX,
             legScaleY);

    // Body
    QPixmap coloredBody = tintPixmap(bodySprite, armColor);
    painter.drawPixmap(bodyX, bodyY, coloredBody);

    // Head
    int headX = bodyX + (bodySprite.width() - headSprite.width()) / 2;
    int headY = bodyY - headSprite.height() / 2;
    painter.drawPixmap(headX, headY, headSprite);

    // Arms
    QPixmap coloredArm = tintPixmap(armSprite, armColor);

    drawArm(painter, QPoint(bodyX + 12, bodyY + 20), armAngleRight, coloredArm,
            false);

    drawArm(painter, QPoint(bodyX + bodySprite.width() - 12, bodyY + 20),
            armAngleLeft, coloredArm, true);

    painter.restore();
  }

  void drawArm(QPainter &painter, const QPoint &shoulder, double angle,
               const QPixmap &arm, bool flip) {

    painter.save();
    painter.translate(shoulder);
    painter.rotate(angle);
    painter.scale(flip ? -armScaleX : armScaleX, armScaleY);

    painter.drawPixmap(-arm.width() / 2, 0, arm);
    painter.restore();
  }

  void drawLimb(QPainter &painter, const QPoint &joint, double angle,
                const QPixmap &limb, bool flip, double scaleX, double scaleY) {

    painter.save();
    painter.translate(joint);
    painter.rotate(angle);
    painter.scale(flip ? -scaleX : scaleX, scaleY);

    painter.drawPixmap(-limb.width() / 2, 0, limb);
    painter.restore();
  }
};

// Main
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Mate mate;
  mate.show();
  return app.exec();
}
