#include "artie.h"
#include <QApplication>
#include <QRandomGenerator>

artie::artie(QWidget *parent) : QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
  setAttribute(Qt::WA_TranslucentBackground);
  setFocusPolicy(Qt::StrongFocus);
  setFocus();

  bodySprite.load("../assets/body.png");
  headSprite.load("../assets/head.png");
  armSprite.load("../assets/arm.png");
  legSprite.load("../assets/arm.png");

  baseWindowWidth = armSprite.height() * 1.5;
  baseWindowheight =
      legSprite.height() + bodySprite.height() + headSprite.height() - 1.5;
  setFixedSize(baseWindowWidth, baseWindowheight);

  updateSpriteCache();

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &artie::tick);
  timer->start(30);
}

QPixmap artie::tintPixmap(const QPixmap &src, const QColor &color) {
  QPixmap result(src.size());
  result.fill(Qt::transparent);

  QPainter p(&result);
  p.drawPixmap(0, 0, src);
  p.setCompositionMode(QPainter::CompositionMode_SourceIn);
  p.fillRect(result.rect(), color);
  return result;
}

void artie::tick() {
  if (state != Dragged) {
    applyPhysics();
    updateAI();
    updateIdleGestures();
    updateWaveAnimation();
    move(x() + xVelocity, y());

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();

    if (x() <= screenRect.left()) {
      move(screenRect.left(), y());
      walkDirection = 1;
      xVelocity = walkSpeed;
    }

    if (x() + width() >= screenRect.right()) {
      move(screenRect.right() - width(), y());
      walkDirection = -1;
      xVelocity = -walkSpeed;
    }
  }

  updateAnimation();
  update();
}

void artie::applyPhysics() {
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect bounds = screen->availableGeometry();

  yVelocity += gravity;
  int nextY = y() + yVelocity;

  if (nextY + height() >= bounds.bottom()) {
    nextY = bounds.bottom() - height();
    yVelocity = 0;
    state = OnGround;
  } else {
    state = Falling;
  }

  move(x(), nextY);
}

void artie::updateSpriteCache(){
  cachedBody = tintPixmap(bodySprite, armColor);
  cachedArm = tintPixmap(armSprite, armColor);
}

void artie::updateAnimation() {
  time += 0.1;
  breathOffset = std::sin(time * 2.0) * 2.0;

  if (!isWaving) {
    armAngleLeft = -std::sin(time) * 18.0;
  }

  if (state == Dragged) {
    legAngleLeft = 0;
    legAngleRight = 0;
    armAngleRight = 0;
    armAngleLeft = 0;
  } else if (state == OnGround && walkDirection != 0) {
    legAngleLeft = -std::sin(time * 2.0) * 15;
    legAngleRight = std::sin(time * 2.0) * 15;
    armAngleLeft = -std::sin(time * 2.0) * 10;
    armAngleRight = std::sin(time * 2.0) * 10;
  } else {
    legAngleRight = 0;
    legAngleLeft = 0;

    if (!isWaving) {
      armAngleLeft = 0;
    }
    armAngleRight = 0;
  }
}

void artie::updateWindowSize() {
  int baseWidth = baseWindowWidth;
  int baseHeight = baseWindowheight;

  setFixedSize(baseWidth * characterScale, baseHeight * characterScale);
}

void artie::updateAI() {
  if (state != OnGround)
    return;

  decisionTimer++;

  if (decisionTimer >= decisionInterval) {
    decisionTimer = 0;

    walkDirection = (std::rand() % 3) - 1;

    if (walkDirection == 0) {
      xVelocity = 0;
    } else {
      walkSpeed = 1.0 + (std::rand() % 300) / 100.0;
      xVelocity = walkDirection * walkSpeed;
    }
  }
}

void artie::updateIdleGestures() {
  if (state != OnGround || walkDirection != 0)
    return;

  if (isWaving)
    return;

  if (waveCoolDown > 0) {
    waveCoolDown--;
    return;
  }

  int chance = std::rand() % 200;
  if (chance == 0) {
    isWaving = true;
    waveTimer = 0;
  }
}

void artie::updateWaveAnimation() {
  if (!isWaving)
    return;

  waveTimer++;

  armAngleLeft = 115 + std::sin(waveTimer * 0.5) * 25;

  if (waveTimer >= waveDuration) {
    isWaving = false;
    waveCoolDown = waveCoolDownMax;
    armAngleLeft = 0;
  }
}

void artie::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isDragging = true;
    state = Dragged;
    isWaving = false;
    waveCoolDown = waveCoolDownMax;
    xVelocity = 0;
    yVelocity = 0;

    dragOffset = event->globalPosition().toPoint() - frameGeometry().topLeft();
  }
}

void artie::mouseMoveEvent(QMouseEvent *event) {
  if (isDragging) {
    move(event->globalPosition().toPoint() - dragOffset);
  }
}

void artie::mouseReleaseEvent(QMouseEvent *) {
  isDragging = false;
  state = Falling;
}

void artie::contextMenuEvent(QContextMenuEvent *event) {
  QMenu menu(this);

  QAction *smallartie = menu.addAction("Small artie");
  QAction *normalartie = menu.addAction("Normal artie");
  QAction *largeartie = menu.addAction("Large artie");
  menu.addSeparator();

  connect(smallartie, &QAction::triggered, this, [&]() {
    characterScale = 0.7;
    updateWindowSize();
  });

  connect(normalartie, &QAction::triggered, this, [&]() {
    characterScale = 1.0;
    updateWindowSize();
  });

  connect(largeartie, &QAction::triggered, this, [&]() {
    characterScale = 1.3;
    updateWindowSize();
  });

  QAction *thinArms = menu.addAction("Thin Arms");
  QAction *thickArms = menu.addAction("Thick Arms");
  QAction *redArtie = menu.addAction("Red Artie");

  connect(thinArms, &QAction::triggered, this, [&]() { armScaleX = 0.8; });
  connect(thickArms, &QAction::triggered, this, [&]() { armScaleX = 1.3; });
  connect(redArtie, &QAction::triggered, this,
          [&]() { 
            armColor = QColor(180, 60, 60);
            updateSpriteCache();
          });

  menu.exec(event->globalPos());
}

void artie::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_W) {
    if (!isWaving && state == OnGround && walkDirection == 0) {
      isWaving = true;
      waveTimer = 0;
      waveCoolDown = waveCoolDownMax;
    }
  }
}

void artie::paintEvent(QPaintEvent *) {
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


  drawLimb(painter, rightHip, legAngleRight, cachedArm, false, legScaleX,
           legScaleY);
  drawLimb(painter, leftHip, legAngleLeft, cachedArm, true, legScaleX,
           legScaleY);

  // Body
  painter.drawPixmap(bodyX, bodyY, cachedBody);

  // Head
  int headX = bodyX + (bodySprite.width() - headSprite.width()) / 2;
  int headY = bodyY - headSprite.height() / 2;
  painter.drawPixmap(headX, headY, headSprite);

  // Arms
  drawArm(painter, QPoint(bodyX + 12, bodyY + 20), armAngleRight, cachedArm,
          false);
  drawArm(painter, QPoint(bodyX + bodySprite.width() - 12, bodyY + 20),
          armAngleLeft, cachedArm, true);

  painter.restore();
}

void artie::drawArm(QPainter &painter, const QPoint &shoulder, double angle,
                    const QPixmap &arm, bool flip) {
  painter.save();
  painter.translate(shoulder);
  painter.rotate(angle);
  painter.scale(flip ? -armScaleX : armScaleX, armScaleY);

  painter.drawPixmap(-arm.width() / 2, 0, arm);
  painter.restore();
}

void artie::drawLimb(QPainter &painter, const QPoint &joint, double angle,
                     const QPixmap &limb, bool flip, double scaleX,
                     double scaleY) {
  painter.save();
  painter.translate(joint);
  painter.rotate(angle);
  painter.scale(flip ? -scaleX : scaleX, scaleY);

  painter.drawPixmap(-limb.width() / 2, 0, limb);
  painter.restore();
}
