#include <QApplication>
#include "bookletforepsonxp100.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  //Для правильного отображения кириллицы в приложении
  QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(utfCodec);
//  QTextCodec::setCodecForTr(utfCodec);
//  QTextCodec::setCodecForCStrings(utfCodec);

  BookletForEpsonXP100 mBooklet;
  mBooklet.show();

  return a.exec();
}

//tr("Итоговое количество страниц для печати: ");
//tr("Необходимое количество листов для печати: ");
//tr("Середина брошюры: страницы ");
