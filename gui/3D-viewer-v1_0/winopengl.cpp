#define GL_SILENCE_DEPRECATION
#include "winopengl.h"

#include <QDebug>
#include <iostream>

#include "../../parser.h"
#include "mainwindow.h"

WinOpenGL::WinOpenGL(QWidget *parent) : QOpenGLWidget(parent) {
  sizeWinWidgetW = 1101;
  sizeWinWidgetH = 941;
  sizeX = 0;
  sizeY = 0;
  sizeZ = 0;
  fieldView = 0;
  setNulls(&obj);
  setNulls(&ogObj);
  flag_open_file = 0;
  this->scale = 1;
  this->movX = 0;
  this->movY = 0;
  this->movZ = 0;
  this->rotX = 0;
  this->rotY = 0;
  this->rotZ = 0;

  allColor.colorVertexRed = 0.971832;
  allColor.colorVertexGreen = 0.171923;
  allColor.colorVertexBlue = 0.27657;
  allColor.colorVertexAlpha = 1.0;
  allColor.colorVertex =
      QColor(allColor.colorVertexRed, allColor.colorVertexGreen,
             allColor.colorVertexBlue, allColor.colorVertexAlpha);
  allColor.colorLineRed = 0.735;
  allColor.colorLineGreen = 0.83;
  allColor.colorLineBlue = 0.98;
  allColor.colorLineAlpha = 1.0;
  allColor.colorLine = QColor(allColor.colorLineRed, allColor.colorLineGreen,
                              allColor.colorLineBlue, allColor.colorLineAlpha);
}

WinOpenGL::~WinOpenGL() {
  free(obj.v);
  free(obj.e);
  free(ogObj.v);
  free(ogObj.e);
}

void WinOpenGL::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void WinOpenGL::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Настройка отображения линии:
  glLineWidth(4);
  // Настройка отображения вершин:
  glEnable(GL_POINT_SMOOTH);
  glPointSize(6);

  resizeGL(sizeWinWidgetW, sizeWinWidgetH);
  paint_model();
}

void WinOpenGL::resizeGL(int w, int h) {
  sizeX = sizeToAxis(ogObj.minMaxX[0], ogObj.minMaxX[1]);
  sizeY = sizeToAxis(ogObj.minMaxY[0], ogObj.minMaxY[1]);
  sizeZ = sizeToAxis(ogObj.minMaxZ[0], ogObj.minMaxZ[1]);
  fieldView = maxSizeToAxis(sizeX, sizeY, sizeZ);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-sizeX - 5, sizeX + 5, -sizeY - 5, sizeY + 5, -sizeZ - 5, sizeZ + 5);
}

// Определение размера модели в доль рассчитываемой оси
double WinOpenGL::sizeToAxis(double min, double max) {
  double size = 0;
  if (max < 0 && min < 0) {
    size = abs(min) - abs(max);
  } else if (max > 0 && min > 0) {
    size = max - min;
  } else {
    size = max + abs(min);
  }
  return size;
}

// Определение максимального размера модели по одной из осей
double WinOpenGL::maxSizeToAxis(double x, double y, double z) {
  double result = 0;
  if (x > y) {
    if (x > z) {
      result = x;
    } else {
      result = y;
    }
  } else {
    if (y > z) {
      result = y;
    } else {
      result = z;
    }
  }
  return result;
}

// Выбор модели, аффинные преобразования, отрисовка линий и вершин
void WinOpenGL::paint_model() {
  if (flag_open_file == 1) {
    // Отрисовка вершин
    glColor3d(this->allColor.colorVertexRed, this->allColor.colorVertexGreen,
              this->allColor.colorVertexBlue);  // цвет вершин - RGB
    for (int i = 0; i < obj.vAmount; i++) {
      glBegin(GL_POINTS);
      glVertex3d(obj.v[i].x, obj.v[i].y, obj.v[i].z);
      glEnd();
    }

    // Отрисовка линий
    glColor3d(this->allColor.colorLineRed, this->allColor.colorLineGreen,
              this->allColor.colorLineBlue);  // цвет линии - RGB
    for (int i = 0; i < obj.eAmount; i++) {
      glBegin(GL_LINES);
      glVertex3d(obj.v[obj.e[i].vertIndex[0]].x, obj.v[obj.e[i].vertIndex[0]].y,
                 obj.v[obj.e[i].vertIndex[0]].z);
      glVertex3d(obj.v[obj.e[i].vertIndex[1]].x, obj.v[obj.e[i].vertIndex[1]].y,
                 obj.v[obj.e[i].vertIndex[1]].z);
      glEnd();
    }

    glDisable(GL_BLEND);
  }
}

void WinOpenGL::draw() {
  glFinish();
  setNulls(&obj);
  cpObj(ogObj, &obj);
  newObj(&obj);
  paintGL();
  glFinish();
}

void WinOpenGL::newObj(t_object *object) {
  scale_increase(scale, object);
  move_x(movX, object);
  move_y(movY, object);
  move_z(movZ, object);
  rotate_x(rotX, object);
  rotate_y(rotY, object);
  rotate_z(rotZ, object);
}

void WinOpenGL::cpObj(t_object a, t_object *b) {
  b->vAmount = a.vAmount;
  b->eAmount = a.eAmount;
  b->v = (t_vertex *)malloc(sizeof(t_vertex) * b->vAmount);
  b->e = (t_edge *)malloc(sizeof(t_edge) * b->eAmount);
  for (int i = 0; i < a.vAmount; i++) {
    b->v[i].x = a.v[i].x;
    b->v[i].y = a.v[i].y;
    b->v[i].z = a.v[i].z;
  }
  for (int i = 0; i < a.eAmount; i++) {
    b->e[i].vertIndex[0] = a.e[i].vertIndex[0];
    b->e[i].vertIndex[1] = a.e[i].vertIndex[1];
  }
}