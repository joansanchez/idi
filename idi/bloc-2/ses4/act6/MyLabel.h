#include <QLabel>

class MyLabel: public QLabel
{
    Q_OBJECT

  public:

    MyLabel(QWidget *parent);

  public slots:
    void changered(int redvalue);
    void changegreen(int greenvalue);
    void changeblue(int bluevalue);
    void send();

  signals:
    void setcolours(int red, int green, int blue);

  private:
    int red, green, blue;
};
