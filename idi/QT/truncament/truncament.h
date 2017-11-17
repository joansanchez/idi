#include  <QLabel>

class truncament: public QLabel
{
  Q_OBJECT
public:
  truncament(QWidget *parent);
  void reescriu();
public slots:
  void enviartext(QString text);
  void enviarvalor(int val);
private:
  QString noustring;
  int valoratruncar;
};
