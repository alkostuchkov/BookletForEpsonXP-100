#include "bookletforepsonxp100.h"
#include "ui_bookletforepsonxp100.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSettings>
#include <QKeyEvent>

BookletForEpsonXP100::BookletForEpsonXP100(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookletForEpsonXP100)
{
    ui->setupUi(this);

    //Регистрация фильтров событий для spbxFirstPage и spbxLastPage
    ui->spbxFirstPage->installEventFilter(this);
    ui->spbxLastPage->installEventFilter(this);

    //Загружаем настройки приложения из .ini файла
    readSettings();

    //На изменения RadioButtons вызываем наш слот on_radioButtons_toggled()
    connect(ui->rbtnAllPages, SIGNAL(clicked()), this, SLOT(on_radioButtons_toggled()));
    connect(ui->rbtnRange, SIGNAL(clicked()), this, SLOT(on_radioButtons_toggled()));

    //Set id for Radiobuttons in ButtonGroup
    ui->btngrWhatPrint->setId(ui->rbtnAllPages, 0);
    ui->btngrWhatPrint->setId(ui->rbtnRange, 1);
    //  ui->rbtnAllPages->click();//Имитируем клик по rbtnAllPages при запуске
    ui->spbxLastPage->clear();
    ui->spbxLastPage->setFocus();
    ui->spbxFirstPage->setValue(1);
    this->windowToCenterScreen(); //Окно приложения по центру экрана
}

BookletForEpsonXP100::~BookletForEpsonXP100()
{
    //Сохраняем настройки приложения в .ini файл
    writeSettings();

    delete ui;
}


void BookletForEpsonXP100::on_radioButtons_toggled()
{//Мой слот на изменения RadioButtons
    //Очищаем поля вывода результатов расчетов
    ui->ledFacePages->clear();
    ui->ledVersoPages->clear();
    //Очищаем выведенные результаты расчетов в Labels
    ui->lblTotalPagesForPrint->clear();
    ui->lblAmountSheetsForPrint->clear();
    ui->lblMidOfBooklet->clear();
    ui->spbxLastPage->setValue(0);
    ui->spbxLastPage->clear();

    //Подготавливаем поля ввода в зависимости от выбранного вида печати
    switch (ui->btngrWhatPrint->checkedId()) {
    case 0: //rbtnAllPages
        ui->spbxFirstPage->setValue(1);
        ui->spbxFirstPage->setEnabled(false);
        ui->spbxLastPage->setFocus();
        break;
    case 1: //rbtnRange
        ui->spbxFirstPage->setValue(0);//Чтобы сработала проверка на корректность
        ui->spbxFirstPage->clear();    //ввода при отсутствии значения в spbxFirstPage
        ui->spbxFirstPage->setEnabled(true);
        ui->spbxFirstPage->setFocus();
        break;
    default:
        break;
    }
}


void BookletForEpsonXP100::windowToCenterScreen()
{//Ф-ция для размещения окна по центру экрана
    int newX, newY;
    int widthScreen, heightScreen;
    //Получаем размер текущего экрана
    widthScreen = QApplication::desktop()->screenGeometry().width();
    heightScreen = QApplication::desktop()->screenGeometry().height();
    //Рассчет новых координат верхней левой точки окна приложения
    newX = (widthScreen - QWidget::width()) / 2;
    newY = (heightScreen - QWidget::height()) / 2;
    //Передаем новые X, Y и размеры окна приложения, т.е. смещаем наше прямоугольное окно
    QWidget::setGeometry(newX, newY, QWidget::width(), QWidget::height());
}

void BookletForEpsonXP100::on_pbtnCalculate_clicked()
{
    int firstPage, lastPage;
    firstPage = ui->spbxFirstPage->value();
    lastPage = ui->spbxLastPage->value();
    if(firstPage <= 0) {  //Проверка корректности ввода номера первой страницы
        QMessageBox::warning(this, this->windowTitle(), tr("Amount of pages must be more than 0")); //++from here
        ui->spbxFirstPage->setFocus();
        return;
    }
    if((lastPage - firstPage) < 0) { //Проверка, чтобы первая страница не была больше последней
        QMessageBox::warning(this, this->windowTitle(), tr("The last page cannot be less than the first one"));
        ui->spbxLastPage->setFocus();
        return;
    }
    calculatePages(firstPage, lastPage);
}

void BookletForEpsonXP100::calculatePages(int firstPage, int lastPage)
{ //Расчет страниц
    //Рассчитываем сколько страниц нужно напечатать
    amountPages = (lastPage - firstPage) + 1;

    //Проверка количества страниц кратности 4 (4 страницы на 1 листе!)
    //Если не кратно 4, то добавляем по 1, пока не будет кратно 4
    while((amountPages % 4) != 0) {
        amountPages++; //теперь последняя страница = amountPages
    }

    //Вывод итогового количества страниц и листов
    ui->lblTotalPagesForPrint->setText(tr("Total pages for printing: ") + QString::number(amountPages));
    ui->lblTotalPagesForPrint->setVisible(true);
    amountSheets = amountPages / 4; //Необходимое количество листов для печати
    ui->lblAmountSheetsForPrint->setText(tr("Necessary sheets for printing: ") + QString::number(amountSheets));
    ui->lblAmountSheetsForPrint->setVisible(true);

    //Находим левую страницу середины брошюры leftFacePage
    leftFacePage = firstPage + (amountPages / 2 - 1);
    //Находим правую страницу середины брошюры rightFacePage
    rightFacePage = leftFacePage + 1;
    //Вывод страниц середины брошюры
    ui->lblMidOfBooklet->setText(tr("The middle of the booklet: pages ")
                                 + QString::number(leftFacePage) + ", " + QString::number(rightFacePage));
    ui->lblMidOfBooklet->setVisible(true);
    //Находим левую оборотную страницу leftVersoPage
    leftVersoPage = leftFacePage - 1;
    //Находим правую оборотную страницу rightVersoPage
    rightVersoPage = rightFacePage + 1;

    //Вычисляем и печатаем левые и правые лицевые страницы
    while((leftFacePage >= (firstPage + 1)) || (rightFacePage <= (amountPages - 1))) {
        if(rightFacePage != (firstPage + (amountPages - 2))) {
            tmpString += QString::number(leftFacePage) + "," + QString::number(rightFacePage) + ",";
        }
        else { //после последней правой страницы не ставим запятую
            tmpString += QString::number(leftFacePage) + "," + QString::number(rightFacePage);
        }
        leftFacePage = leftFacePage - 2;
        rightFacePage = rightFacePage + 2;
    }
    ui->ledFacePages->setText(tmpString);
    tmpString = ""; //обнуляем временную строку

    //Вычисляем и печатаем левые и правые оборотные страницы
    while((leftVersoPage >= firstPage) || (rightVersoPage <= amountPages)) {
        if(rightVersoPage != (firstPage + (amountPages - 1))) {
            tmpString += QString::number(rightVersoPage) + "," + QString::number(leftVersoPage) + ",";
        }
        else { //после последней правой страницы не ставим запятую
            tmpString += QString::number(rightVersoPage) + "," + QString::number(leftVersoPage);
        }
        leftVersoPage = leftVersoPage - 2;
        rightVersoPage = rightVersoPage + 2;
    }
    ui->ledVersoPages->setText(tmpString);
    tmpString = ""; //обнуляем временную строку
}

void BookletForEpsonXP100::on_cbxLanguages_currentIndexChanged(int index)
{
    QApplication::removeTranslator(mTranslator);
    mTranslator = new QTranslator(this);
    mqTranslator = new QTranslator(this);

    switch(index) {
    case 0:
        mTranslator->load(QString(":/lang/booklet_en.qm")); //default
        mqTranslator->load(QString(":/lang/qtbase_en.qm"));
        break;
    case 1:
        mTranslator->load(QString(":/lang/booklet_ru.qm"));
        mqTranslator->load(QString(":/lang/qtbase_ru.qm"));
        break;
    default:
        break;
    }
    QApplication::installTranslator(mqTranslator);
    QApplication::installTranslator(mTranslator);
}

void BookletForEpsonXP100::changeEvent(QEvent *event)
{ //Переопределяем метод события changeEvent
    if(event->type() == QEvent::LanguageChange) {
        retranslateUi(); //при installTranslator присваиваем новые значения
        //строкам виджетов приложения
        ui->rbtnAllPages->click(); //Очищаем все поля ввода
    }
    else {
        QWidget::changeEvent(event);
    }
}

void BookletForEpsonXP100::retranslateUi()
{
    setWindowTitle(tr("Booklet for Epson XP-100"));
    ui->lblLanguages->setText(tr("Language:"));
    ui->cbxLanguages->setItemText(0, tr("English"));
    ui->cbxLanguages->setItemText(1, tr("Russian"));
    ui->grbxWhatPrint->setTitle(tr("Print:"));
    ui->rbtnAllPages->setText(tr("All pages"));
    ui->rbtnRange->setText(tr("The range"));
    ui->lblFirstPage->setText(tr("The first page: "));
    ui->lblLastPage->setText(tr("The last page: "));
    ui->pbtnCalculate->setText(tr("Calculate"));
    ui->lblFacePages->setText(tr("Face pages:"));
    ui->lblVersoPages->setText(tr("Verso pages:"));
    ui->pbtnClose->setText(tr("Close"));
}

void BookletForEpsonXP100::writeSettings()
{   //Сохраняем настройки приложения в .ini файла
    QSettings settings(QApplication::applicationDirPath() + "/config.ini",
                       QSettings::IniFormat);
    settings.setValue("cbxLanguages/currentIndex", ui->cbxLanguages->currentIndex());
}

void BookletForEpsonXP100::readSettings()
{
    //Загружаем настройки приложения из .ini файла
    QSettings settings(QApplication::applicationDirPath() + "/config.ini",
                       QSettings::IniFormat);
    ui->cbxLanguages->setCurrentIndex(settings.value("cbxLanguages/currentIndex", 0).toInt());
}

bool BookletForEpsonXP100::eventFilter(QObject *obj, QEvent *event)
{//Отслеживаем нажатие клавиши "Enter" для spbxFirstPage и spbxLastPage
    if(obj == ui->spbxFirstPage){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
                ui->spbxLastPage->setFocus();
                return true;
            }
            else{
                return false;
            }
        }
    }
    if(obj == ui->spbxLastPage){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
                ui->pbtnCalculate->setFocus();
                ui->pbtnCalculate->click();
                return true;
            }
            else{
                return false;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void BookletForEpsonXP100::keyPressEvent(QKeyEvent *event)
{//Переопределяем для закрытия приложения по нажатию Escape
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
    else{
        QWidget::keyPressEvent(event);
    }
}

void BookletForEpsonXP100::on_pbtnClose_clicked()
{
    this->close();
}
