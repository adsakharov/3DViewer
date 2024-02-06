#ifndef WINOPENGL_H
#define WINOPENGL_H

#include <math.h>

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWidget>

#include "mainwindow.h"

typedef struct colorStruct {
  QColor colorVertex;
  QColor colorLine;
  QColor colorBG;

  double colorVertexRed;
  double colorVertexGreen;
  double colorVertexBlue;
  double colorVertexAlpha;

  double colorLineRed;
  double colorLineGreen;
  double colorLineBlue;
  double colorLineAlpha;
} colorData;

class WinOpenGL : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  WinOpenGL(QWidget *parent = 0);
  ~WinOpenGL();

  colorData allColor;
  int sizeWinWidgetW;
  int sizeWinWidgetH;
  double sizeX;
  double sizeY;
  double sizeZ;
  double fieldView;
  t_object obj, ogObj;
  int flag_open_file;
  double scale;
  double movX, movY, movZ;
  double rotX, rotY, rotZ;

  void draw();

  double sizeToAxis(double min,
                    double max);  // Определение размера модели по заданной оси
  double maxSizeToAxis(
      double x, double y,
      double z);  // Выбор максимально размера модели по одной из осей

 private:
  void newObj(t_object *object);
  void cpObj(t_object a, t_object *b);
  void initializeGL()
      override;  // используется для глобальных настроек построения изображения,
                 // которые нет необходимости указывать при построении кадра.
  void resizeGL(int w, int h) override;
  void paintGL() override;  // этот метод будет выстраивать каждый наш кадр для
                            // отображения.
  void paint_model();
};

#endif  // WINOPENGL_H
