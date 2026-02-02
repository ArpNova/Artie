#ifndef artie_H
#define artie_H

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <qaction.h>
#include <qcolor.h>
#include <qevent.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qscreen.h>
#include <qwindowdefs.h>

class artie : public QWidget {
  Q_OBJECT

public:
  explicit artie(QWidget *parent = nullptr);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private slots:
  void tick();

private:
  // windowSize
  double baseWindowWidth;
  double baseWindowheight;

  // State
  enum artieState { Falling, OnGround, Dragged };

  artieState state = Falling;

  // Horizontal Movement
  int xVelocity = 0;
  int walkDirection = 0;
  double walkSpeed = 0.0;

  // AI Timing
  int decisionTimer = 0;
  int decisionInterval = 120;

  // Gesture: Wave
  bool isWaving = false;
  int waveTimer = 0;
  int waveDuration = 60;
  int waveCoolDown = 0;
  int waveCoolDownMax = 300;

  // Whole character scale
  double characterScale = 1.0;

  // Sprites
  QPixmap bodySprite;
  QPixmap headSprite;
  QPixmap armSprite;
  QPixmap legSprite;

  //Cached colored sprites
  QPixmap cachedBody;
  QPixmap cachedArm;

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
  QColor armColor = QColor(100, 149, 237);

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

  // Helper methods
  QPixmap tintPixmap(const QPixmap &src, const QColor &color);
  void applyPhysics();
  void updateAnimation();
  void updateWindowSize();
  void updateAI();
  void updateIdleGestures();
  void updateWaveAnimation();
  void updateSpriteCache();
  void drawArm(QPainter &painter, const QPoint &shoulder, double angle,
               const QPixmap &arm, bool flip);
  void drawLimb(QPainter &painter, const QPoint &joint, double angle,
                const QPixmap &limb, bool flip, double scaleX, double scaleY);
};

#endif // artie_H
