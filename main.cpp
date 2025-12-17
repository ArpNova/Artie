#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

class Mate : public QWidget {
public:
    Mate(){
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

        setAttribute(Qt::WA_TranslucentBackground);

        resize(200, 200);
    }
protected:
    void paintEvent(QPaintEvent *event)override{
        QPainter painter(this);

        painter.setBrush(Qt::blue);
        painter.drawEllipse(0, 0, 100, 100);
    }
};

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    Mate myMate;

    myMate.show();

    return app.exec();
}