#include <QLCDNumber>

class disco: public QLCDNumber {
  Q_OBJECT
public:
  disco(QWidget *parent);
  void reescriu();
public slots:
  void canviarvalor(int val);
  void valor0();
private:
  int valor;

};
