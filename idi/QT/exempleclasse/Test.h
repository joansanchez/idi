 #include "ui_slottest.h" 

class Test : public QWidget
{
    Q_OBJECT
    
public:
    Test(QWidget *parent = 0);
    
private:
    Ui::Test ui;
};
