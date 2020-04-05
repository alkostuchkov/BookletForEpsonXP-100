#ifndef BOOKLETFOREPSONXP100_H
#define BOOKLETFOREPSONXP100_H

#include <QWidget>
#include <QTranslator>
#include <QKeyEvent>
//#include <QEvent>

namespace Ui {
  class BookletForEpsonXP100;
}

class BookletForEpsonXP100 : public QWidget
{
  Q_OBJECT


public:
  explicit BookletForEpsonXP100(QWidget *parent = 0);
  ~BookletForEpsonXP100();

private slots:
  //Мой слот на изменение RadioButtons
  void on_radioButtons_toggled();
  void on_pbtnClose_clicked();
  void on_pbtnCalculate_clicked();
  void on_cbxLanguages_currentIndexChanged(int index);

private:
  void windowToCenterScreen();
  void calculatePages(int firstPage, int lastPage);
  void writeSettings();
  void readSettings();
  void retranslateUi();
  void changeEvent(QEvent *event);
  void keyPressEvent(QKeyEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);

  Ui::BookletForEpsonXP100 *ui;
  int amountPages, amountSheets, facePages, versoPages;
  int leftFacePage, rightFacePage, leftVersoPage, rightVersoPage;
  QString tmpString;
  QTranslator *mTranslator;
  QTranslator *mqTranslator;
};

#endif // BOOKLETFOREPSONXP100_H
