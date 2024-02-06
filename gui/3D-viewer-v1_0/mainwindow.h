#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QtOpenGL>

extern "C" {
#include "../../affine_transformation.h"
#include "../../parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void pushButton_File_clicked();
  void pushButton_min_clicked();
  void pushButton_plus_clicked();
  void pushButton_Draw_clicked();
  void on_horizontalSlider_X_valueChanged(int value);
  void on_horizontalSlider_Y_valueChanged(int value);
  void on_horizontalSlider_Z_valueChanged(int value);
  void on_dial_X_valueChanged(int value);
  void on_dial_Y_valueChanged(int value);
  void on_dial_Z_valueChanged(int value);
  void pushButton_res_clicked();
  void on_line_Xcoor_valueChanged();
  void on_line_Ycoor_valueChanged();
  void on_line_Zcoor_valueChanged();
  void on_line_Xrot_valueChanged();
  void on_line_Yrot_valueChanged();
  void on_line_Zrot_valueChanged();
  void on_line_Scale_valueChanged();

 private:
  Ui::MainWindow *ui;
  QString file_name;
};
#endif  // MAINWINDOW_H
