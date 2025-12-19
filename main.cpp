#include <QApplication>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpoint.h>

class Mate : public QWidget {
private:
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

  //leg appearence
  double legScaleX = 0.8;
  double legScaleY = 0.6;
  QColor legColor = QColor(220,180,140);

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
    setFixedSize(150, 200);

    bodySprite.load("./assets/body.png");
    headSprite.load("./assets/head.png");
    armSprite.load("./assets/arm.png");
    legSprite.load("./assets/arm.png");

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
    if (!isDragging) {
      applyPhysics();
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
      yVelocity = -yVelocity * 0.5;
    }

    move(x(), nextY);
  }

  void updateAnimation() {
    time += 0.1;
    breathOffset = std::sin(time * 2.0) * 2.0;
    armAngleRight = std::sin(time) * 18.0;
    armAngleLeft = -std::sin(time) * 18.0;
    legAngleLeft = -std::sin(time) * 10;
    legAngleRight = std::sin(time) * 10;
  }

protected:
  // Mouse
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      isDragging = true;
      yVelocity = 0;
      dragOffset =
          event->globalPosition().toPoint() - frameGeometry().topLeft();
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    if (isDragging) {
      move(event->globalPosition().toPoint() - dragOffset);
    }
  }

  void mouseReleaseEvent(QMouseEvent *) override { isDragging = false; }

  // Context Menu
  void contextMenuEvent(QContextMenuEvent *event) override {
    QMenu menu(this);

    QAction *thinArms = menu.addAction("Thin Arms");
    QAction *thickArms = menu.addAction("Thick Arms");
    QAction *redArms = menu.addAction("Red Arms");
    QAction *normalArms = menu.addAction("Normal Arms");

    connect(thinArms, &QAction::triggered, this, [&]() { armScaleX = 0.8; });

    connect(thickArms, &QAction::triggered, this, [&]() { armScaleX = 1.3; });

    connect(redArms, &QAction::triggered, this,
            [&]() { armColor = QColor(180, 60, 60); });

    connect(normalArms, &QAction::triggered, this, [&]() {
      armScaleX = 1.0;
      armScaleY = 1.2;
      armColor = QColor(220, 180, 140);
    });

    menu.exec(event->globalPos());
  }

  // Rendering
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int bodyX = (width() - bodySprite.width()) / 2;
    int bodyY = 80 + static_cast<int>(breathOffset);

    int bodyBottomY = bodyY + bodySprite.height();

    //LEG JOINTS
    QPoint leftHip(
      bodyX + bodySprite.width() * 0.45,
      bodyBottomY
    );

    QPoint rightHip(
      bodyX + bodySprite.width() * 0.55,
      bodyBottomY
    );

    QPixmap coloredLimb = tintPixmap(armSprite, armColor);

    drawLimb(painter, leftHip, legAngleRight, coloredLimb, false, legScaleX, legScaleY);
    drawLimb(painter, leftHip, legAngleLeft, coloredLimb, true, legScaleX, legScaleY);

    // Body
    painter.drawPixmap(bodyX, bodyY, bodySprite);

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

  void drawLimb(QPainter &painter, 
                const QPoint &joint, 
                double angle, 
                const QPixmap &limb,
                bool flip,
                double scaleX,
                double scaleY){
        
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
