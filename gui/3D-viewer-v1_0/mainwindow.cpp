#include "mainwindow.h"

#include "./ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->pushButton_File, SIGNAL(clicked()), this,
          SLOT(pushButton_File_clicked()));
  connect(ui->pushButton_min, SIGNAL(clicked()), this,
          SLOT(pushButton_min_clicked()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this,
          SLOT(pushButton_plus_clicked()));
  connect(ui->pushButton_Draw, SIGNAL(clicked()), this,
          SLOT(pushButton_Draw_clicked()));
  connect(
      ui->horizontalSlider_X, SIGNAL(valueChanged), this,
      SLOT(on_horizontalSlider_X_valueChanged(ui->horizontalSlider_X->value)));
  connect(
      ui->horizontalSlider_Y, SIGNAL(valueChanged), this,
      SLOT(on_horizontalSlider_Y_valueChanged(ui->horizontalSlider_Y->value)));
  connect(
      ui->horizontalSlider_Z, SIGNAL(valueChanged), this,
      SLOT(on_horizontalSlider_Z_valueChanged(ui->horizontalSlider_Z->value)));
  connect(ui->dial_X, SIGNAL(valueChanged), this,
          SLOT(on_dial_X_valueChanged(ui->dial_X->value)));
  connect(ui->dial_Y, SIGNAL(valueChanged), this,
          SLOT(on_dial_Y_valueChanged(ui->dial_Y->value)));
  connect(ui->dial_Z, SIGNAL(valueChanged), this,
          SLOT(on_dial_Z_valueChanged(ui->dial_Z->value)));

  connect(ui->line_Scale, SIGNAL(valueChanged), this,
          SLOT(on_line_Scale_valueChanged()));
  connect(ui->line_Xcoor, SIGNAL(valueChanged), this,
          SLOT(on_line_Xcoor_valueChanged()));
  connect(ui->line_Ycoor, SIGNAL(valueChanged), this,
          SLOT(on_line_Ycoor_valueChanged()));
  connect(ui->line_Zcoor, SIGNAL(valueChanged), this,
          SLOT(on_line_Zcoor_valueChanged()));
  connect(ui->line_Xrot, SIGNAL(valueChanged), this,
          SLOT(on_line_Xrot_valueChanged()));
  connect(ui->line_Yrot, SIGNAL(valueChanged), this,
          SLOT(on_line_Yrot_valueChanged()));
  connect(ui->line_Zrot, SIGNAL(valueChanged), this,
          SLOT(on_line_Zrot_valueChanged()));
  connect(ui->pushButton_res, SIGNAL(clicked()), this,
          SLOT(pushButton_res_clicked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::pushButton_File_clicked() {
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                                                  tr("Object files (*.obj)"));
  if (filePath.isEmpty()) {
    ui->textBrowser->setText("Error!\nФайл не выбран");
    ui->openGLWidget->flag_open_file = 0;
  } else {
    int error = 0;
    t_object object;
    const char *cLine;
    QByteArray inArr = filePath.toUtf8();
    cLine = inArr.constData();
    // START PARSER
    error = parser(cLine, &object);
    if (error == 1) {
      ui->textBrowser->setText("Error!\nОшибка при открытии файла");
      ui->openGLWidget->flag_open_file = 0;
    } else {
      ui->openGLWidget->flag_open_file = 1;
      ui->openGLWidget->ogObj = object;
      // Вычленение имени файла из пути файла
      std::string utf8_text = filePath.toUtf8().constData();
      std::string file_name_std = utf8_text.substr(utf8_text.rfind('/') + 1);
      QString fileName = QString::fromStdString(file_name_std);
      // Запись текста в поле textBrowser
      ui->textBrowser->setText("Имя файла: " + fileName +
                               "\nПуть к файлу: " + filePath);
      char numStr[20] = {'\0'};
      sprintf(numStr, "%d", object.vAmount);
      ui->label_Dots->setText(numStr);
      sprintf(numStr, "%d", object.eAmount);
      ui->label_Lines->setText(numStr);
      ui->openGLWidget->draw();
    }
  }
}

void MainWindow::pushButton_min_clicked() {
  double scale = ui->line_Scale->text().toDouble();
  char outStr[20] = {'\0'};
  if (scale > 0.1) scale -= 0.1;
  sprintf(outStr, "%.1lf", scale);
  ui->line_Scale->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::pushButton_plus_clicked() {
  double scale = ui->line_Scale->text().toDouble();
  char outStr[20] = {'\0'};
  if (scale < 10) scale += 0.1;
  sprintf(outStr, "%.1lf", scale);
  ui->line_Scale->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_horizontalSlider_X_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Xcoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_horizontalSlider_Y_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Ycoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_horizontalSlider_Z_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Zcoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_dial_X_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Xrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_dial_Y_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Yrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_dial_Z_valueChanged(int value) {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", value);
  ui->line_Zrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::pushButton_Draw_clicked() {
  ui->openGLWidget->scale = ui->line_Scale->text().toDouble();
  ui->openGLWidget->movX = ui->line_Xcoor->text().toDouble();
  ui->openGLWidget->movY = ui->line_Ycoor->text().toDouble();
  ui->openGLWidget->movZ = ui->line_Zcoor->text().toDouble();
  ui->openGLWidget->rotX = ui->line_Xrot->text().toDouble();
  ui->openGLWidget->rotY = ui->line_Yrot->text().toDouble();
  ui->openGLWidget->rotZ = ui->line_Zrot->text().toDouble();
  ui->openGLWidget->draw();
  ui->openGLWidget->update();
}

void MainWindow::pushButton_res_clicked() {
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  ui->line_Xcoor->setText(outStr);
  ui->line_Ycoor->setText(outStr);
  ui->line_Zcoor->setText(outStr);
  ui->line_Xrot->setText(outStr);
  ui->line_Yrot->setText(outStr);
  ui->line_Zrot->setText(outStr);
  sprintf(outStr, "%.1lf", 1.0);
  ui->line_Scale->setText(outStr);
  pushButton_Draw_clicked();
}
void MainWindow::on_line_Xcoor_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Xcoor->text().toDouble(&ok);
  if (!ok) ui->line_Xcoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Ycoor_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Ycoor->text().toDouble(&ok);
  if (!ok) ui->line_Ycoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Zcoor_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Zcoor->text().toDouble(&ok);
  if (!ok) ui->line_Zcoor->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Xrot_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Xrot->text().toDouble(&ok);
  if (!ok) ui->line_Xrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Yrot_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Yrot->text().toDouble(&ok);
  if (!ok) ui->line_Yrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Zrot_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%d", 0);
  double x = ui->line_Zrot->text().toDouble(&ok);
  if (!ok) ui->line_Zrot->setText(outStr);
  pushButton_Draw_clicked();
}

void MainWindow::on_line_Scale_valueChanged() {
  bool ok;
  char outStr[20] = {'\0'};
  sprintf(outStr, "%0.1lf", 1.0);
  double x = ui->line_Scale->text().toDouble(&ok);
  if (!ok) ui->line_Scale->setText(outStr);
  pushButton_Draw_clicked();
}