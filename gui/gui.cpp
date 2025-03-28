#include "gui.hpp"
#include "config.hpp"

#include <QApplication>
#include <QKeySequence>
#include <QKeySequenceEdit>
#include <QWidget>
#include <QTabWidget>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QColorDialog>
#include <QColor>
#include <QLayout>
#include <QKeyEvent>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include "../xutil.hpp"

void gui(int argc, char* argv[]) { 
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("GNU/Linux CSS Hack");
  window.setGeometry(100, 100, 300, 200);
  window.setFixedSize(300, 200);
  
  QTabWidget* tabs = new QTabWidget(&window);
  tabs->setGeometry(0, 0, 300, 200);
  
  /* ESP begin */
  QWidget* esp_tab = new QWidget;
  
  QVBoxLayout* esp_layout = new QVBoxLayout(esp_tab);

    
  QCheckBox* esp_master = new QCheckBox("Master", esp_tab);
  esp_master->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_master);
  QObject::connect(esp_master, &QPushButton::clicked, []() {
    config.esp.master = !config.esp.master;
  });
  
  /* Box begin */
  QHBoxLayout* box_layout = new QHBoxLayout();
  QCheckBox* esp_box = new QCheckBox("Box", esp_tab);
  esp_box->setCheckState(Qt::CheckState::Checked);
  box_layout->addWidget(esp_box);
  QObject::connect(esp_box, &QPushButton::clicked, []() {
    config.esp.box = !config.esp.box;
  });

  QWidget box_color_window;
  box_color_window.setWindowTitle("Select Box Color");
  box_color_window.setGeometry(100, 100, 520, 450);
  box_color_window.setFixedSize(520, 450);

  QVBoxLayout* box_color_dialog_layout = new QVBoxLayout(&box_color_window);

  QColorDialog* box_color_dialog = new QColorDialog(QColor(255, 0, 50), esp_tab);
  QPushButton* box_color_button = new QPushButton();
  box_color_button->setAutoFillBackground(true);
  box_color_button->setPalette(QColor(255, 0, 50));
  box_color_button->update();
  box_color_dialog_layout->addWidget(box_color_dialog);
  box_color_dialog->setWindowFlags(Qt::Widget);
  box_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  box_layout->addWidget(box_color_button);
  QObject::connect(box_color_button, &QPushButton::clicked, [&]() {
    box_color_window.show();
  });

  QObject::connect(box_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.box_color[0], &config.esp.box_color[1], &config.esp.box_color[2]);
    box_color_button->setPalette(color);
    box_color_button->update();
  });
 
  esp_layout->addLayout(box_layout);
  /* Box end */
  
  QCheckBox* esp_health = new QCheckBox("Health", esp_tab);
  esp_health->setCheckState(Qt::CheckState::Checked);
  esp_layout->addWidget(esp_health);
  QObject::connect(esp_health, &QPushButton::clicked, []() {
    config.esp.health = !config.esp.health;
  });

  /* Skeleton begin */
  QHBoxLayout* skeleton_layout = new QHBoxLayout();
  QCheckBox* esp_skeleton = new QCheckBox("Skeleton", esp_tab);
  esp_skeleton->setCheckState(Qt::CheckState::Unchecked);
  skeleton_layout->addWidget(esp_skeleton);
  QObject::connect(esp_skeleton, &QPushButton::clicked, []() {
    config.esp.skeleton = !config.esp.skeleton;
  });

  
  QWidget skeleton_color_window;
  skeleton_color_window.setWindowTitle("Select Skeleton Color");
  skeleton_color_window.setGeometry(100, 100, 520, 450);
  skeleton_color_window.setFixedSize(520, 450);

  QVBoxLayout* skeleton_color_dialog_layout = new QVBoxLayout(&skeleton_color_window);

  QColorDialog* skeleton_color_dialog = new QColorDialog(QColor(255, 255, 255), esp_tab);
  QPushButton* skeleton_color_button = new QPushButton();
  skeleton_color_button->setAutoFillBackground(true);
  skeleton_color_button->setPalette(QColor(255, 255, 255));
  skeleton_color_button->update();
  skeleton_color_dialog_layout->addWidget(skeleton_color_dialog);
  skeleton_color_dialog->setWindowFlags(Qt::Widget);
  skeleton_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  skeleton_layout->addWidget(skeleton_color_button);
  QObject::connect(skeleton_color_button, &QPushButton::clicked, [&]() {
    skeleton_color_window.show();
  });

  QObject::connect(skeleton_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.skeleton_color[0], &config.esp.skeleton_color[1], &config.esp.skeleton_color[2]);
    skeleton_color_button->setPalette(color);
    skeleton_color_button->update();
  });
 
  esp_layout->addLayout(skeleton_layout);
  /* Skeleton end */

  /* Snap Lines begin */
  QHBoxLayout* snap_lines_layout = new QHBoxLayout();
  QCheckBox* snap_lines = new QCheckBox("Snap lines", esp_tab);
  snap_lines->setCheckState(Qt::CheckState::Unchecked);
  snap_lines_layout->addWidget(snap_lines);
  QObject::connect(snap_lines, &QPushButton::clicked, []() {
    config.esp.snap_lines = !config.esp.snap_lines;
  });

  QWidget snap_lines_color_window;
  snap_lines_color_window.setWindowTitle("Select Snap Lines Color");
  snap_lines_color_window.setGeometry(100, 100, 520, 450);
  snap_lines_color_window.setFixedSize(520, 450);
  
  QVBoxLayout* snap_lines_color_dialog_layout = new QVBoxLayout(&snap_lines_color_window);

  QColorDialog* snap_lines_color_dialog = new QColorDialog(QColor(0, 255, 0), esp_tab);
  QPushButton* snap_lines_color_button = new QPushButton();
  snap_lines_color_button->setAutoFillBackground(true);
  snap_lines_color_button->setPalette(QColor(0, 255, 0));
  snap_lines_color_button->update();
  snap_lines_color_dialog_layout->addWidget(snap_lines_color_dialog);
  snap_lines_color_dialog->setWindowFlags(Qt::Widget);
  snap_lines_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  snap_lines_layout->addWidget(snap_lines_color_button);
  QObject::connect(snap_lines_color_button, &QPushButton::clicked, [&]() {
    snap_lines_color_window.show();
  });

  QObject::connect(snap_lines_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.esp.snap_lines_color[0], &config.esp.snap_lines_color[1], &config.esp.snap_lines_color[2]);
    snap_lines_color_button->setPalette(color);
    snap_lines_color_button->update();
  });
 
  esp_layout->addLayout(snap_lines_layout);
  /* Snap Lines end */

  tabs->addTab(esp_tab, "ESP");
  /* ESP end */

  /* Visuals begin */
  QWidget* visuals_tab = new QWidget;
  
  QVBoxLayout* visuals_layout = new QVBoxLayout(visuals_tab);
  
  QCheckBox* visuals_master = new QCheckBox("Master", visuals_tab);
  visuals_master->setCheckState(Qt::CheckState::Checked);
  visuals_layout->addWidget(visuals_master);
  QObject::connect(visuals_master, &QPushButton::clicked, []() {
    config.visuals.master = !config.visuals.master;
  });

  /* Crosshair begin */
  QHBoxLayout* crosshair_layout = new QHBoxLayout();
  QCheckBox* crosshair = new QCheckBox("Crosshair", visuals_tab);
  crosshair->setCheckState(Qt::CheckState::Checked);
  crosshair_layout->addWidget(crosshair);
  QObject::connect(crosshair, &QPushButton::clicked, []() {
    config.visuals.crosshair = !config.visuals.crosshair;
  });
  
  QWidget crosshair_color_window;
  crosshair_color_window.setWindowTitle("Select Crosshair Color");
  crosshair_color_window.setGeometry(100, 100, 520, 450);
  crosshair_color_window.setFixedSize(520, 450);

  QVBoxLayout* crosshair_color_dialog_layout = new QVBoxLayout(&crosshair_color_window);

  QColorDialog* crosshair_color_dialog = new QColorDialog(QColor(255, 255, 255), visuals_tab);
  QPushButton* crosshair_color_button = new QPushButton();
  crosshair_color_button->setAutoFillBackground(true);
  crosshair_color_button->setPalette(QColor(255, 255, 255));
  crosshair_color_button->update();
  crosshair_color_dialog_layout->addWidget(crosshair_color_dialog);
  crosshair_color_dialog->setWindowFlags(Qt::Widget);
  crosshair_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  crosshair_layout->addWidget(crosshair_color_button);
  QObject::connect(crosshair_color_button, &QPushButton::clicked, [&]() {
    crosshair_color_window.show();
  });

  QObject::connect(crosshair_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.visuals.crosshair_color[0], &config.visuals.crosshair_color[1], &config.visuals.crosshair_color[2]);
    crosshair_color_button->setPalette(color);
    crosshair_color_button->update();
  });
 
  visuals_layout->addLayout(crosshair_layout);
  /* Crosshair end */

  /* Crosshair Recoil begin */
  QHBoxLayout* crosshair_recoil_layout = new QHBoxLayout();
  QCheckBox* crosshair_recoil = new QCheckBox("Recoil Crosshair", visuals_tab);
  crosshair_recoil->setCheckState(Qt::CheckState::Unchecked);
  crosshair_recoil_layout->addWidget(crosshair_recoil);
  QObject::connect(crosshair_recoil, &QPushButton::clicked, []() {
    config.visuals.crosshair_recoil = !config.visuals.crosshair_recoil;
  });
  
  QWidget crosshair_recoil_color_window;
  crosshair_recoil_color_window.setWindowTitle("Select Crosshair Recoil Color");
  crosshair_recoil_color_window.setGeometry(100, 100, 520, 450);
  crosshair_recoil_color_window.setFixedSize(520, 450);
  
  QVBoxLayout* crosshair_recoil_color_dialog_layout = new QVBoxLayout(&crosshair_recoil_color_window);

  QColorDialog* crosshair_recoil_color_dialog = new QColorDialog(QColor(255, 0, 0), visuals_tab);
  QPushButton* crosshair_recoil_color_button = new QPushButton();
  crosshair_recoil_color_button->setAutoFillBackground(true);
  crosshair_recoil_color_button->setPalette(QColor(255, 0, 0));
  crosshair_recoil_color_button->update();
  crosshair_recoil_color_dialog_layout->addWidget(crosshair_recoil_color_dialog);
  crosshair_recoil_color_dialog->setWindowFlags(Qt::Widget);
  crosshair_recoil_color_dialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
  crosshair_recoil_layout->addWidget(crosshair_recoil_color_button);
  QObject::connect(crosshair_recoil_color_button, &QPushButton::clicked, [&]() {
    crosshair_recoil_color_window.show();
  });

  QObject::connect(crosshair_recoil_color_dialog, &QColorDialog::currentColorChanged, [&](const QColor &color) {
    color.getRgb(&config.visuals.crosshair_recoil_color[0], &config.visuals.crosshair_recoil_color[1], &config.visuals.crosshair_recoil_color[2]);
    crosshair_recoil_color_button->setPalette(color);
    crosshair_recoil_color_button->update();
  });
 
  visuals_layout->addLayout(crosshair_recoil_layout);
  /* Crosshair Recoil end */
  
  tabs->addTab(visuals_tab, "Visuals");
  /* Visuals end */
  
  /* AIMBOT begin */
  QWidget* aim_tab = new QWidget;
  
  QVBoxLayout* aim_layout = new QVBoxLayout(aim_tab);
    
  QCheckBox* aim_master = new QCheckBox("Master", aim_tab);
  aim_master->setCheckState(Qt::CheckState::Unchecked);
  aim_layout->addWidget(aim_master);
  QObject::connect(aim_master, &QPushButton::clicked, []() {
    config.aim.master = !config.aim.master;
  });

  QKeySequenceEdit* key = new QKeySequenceEdit;
  key->setKeySequence(QKeySequence("c"));
  aim_layout->addWidget(key);
  QObject::connect(key, &QKeySequenceEdit::keySequenceChanged, [](const QKeySequence &keySequence) {
    config.aim.key = keySequence.toString().toStdString()[0];
  });
  
  QCheckBox* aim_recoil = new QCheckBox("Recoil control", aim_tab);
  aim_recoil->setCheckState(Qt::CheckState::Checked);
  aim_layout->addWidget(aim_recoil);
  QObject::connect(aim_recoil, &QPushButton::clicked, []() {
    config.aim.recoil = !config.aim.recoil;
  });  
  
  tabs->addTab(aim_tab, "AIMBOT");
  /* AIMBOT end */

  /* MISC begin */
  QWidget* misc_tab = new QWidget;
  
  QVBoxLayout* misc_layout = new QVBoxLayout(misc_tab);
  
  QCheckBox* misc_bhop = new QCheckBox("Bhop", misc_tab);
  misc_bhop->setCheckState(Qt::CheckState::Checked);
  misc_layout->addWidget(misc_bhop);
  QObject::connect(misc_bhop, &QPushButton::clicked, []() {
    config.misc.bhop = !config.misc.bhop;
  });

  QCheckBox* misc_spin_yaw = new QCheckBox("Spin yaw", misc_tab);
  misc_spin_yaw->setCheckState(Qt::CheckState::Unchecked);
  misc_layout->addWidget(misc_spin_yaw);
  QObject::connect(misc_spin_yaw, &QPushButton::clicked, []() {
    config.misc.spin_yaw = !config.misc.spin_yaw;
  });

  
  QCheckBox* misc_spin_pitch = new QCheckBox("Spin pitch", misc_tab);
  misc_spin_pitch->setCheckState(Qt::CheckState::Unchecked);
  misc_layout->addWidget(misc_spin_pitch);
  QObject::connect(misc_spin_pitch, &QPushButton::clicked, []() {
    config.misc.spin_pitch = !config.misc.spin_pitch;
  });
  
  
  tabs->addTab(misc_tab, "MISC");
  /* MISC end */
  
  window.show();

  app.exec();

  return;  
}
