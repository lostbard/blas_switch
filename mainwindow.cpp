#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showCurrentBlas(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCurrentBlas(bool updateoption)
{
  //ui->currentBlasText->setText(QString("Current Blas: %1").arg(name));
  QString path = qApp->applicationDirPath();

  QFile refblas(QString("%1/refblas.dll").arg(path));
  QFile openblas(QString("%1/openblas.dll").arg(path));
  QFile blas(QString("%1/blas.dll").arg(path));
  const char * name;

  if (refblas.size() == blas.size())
    name = "refblas";
  else if (openblas.size() == blas.size())
    name = "openblas";
  else
    name = "Unknown";

  ui->currentBlasText->setText(QString("Current Blas: %1").arg(name));

  if(updateoption)
  {
      if(QString(name) == "refblas")
          ui->refBlasOption->setChecked(true);
      if(QString(name) == "openblas")
          ui->openBlasOption->setChecked(true);
  }
}

void MainWindow::setCurrentBlas(const char *name)
{
  ui->currentBlasText->setText(QString("Current Blas: %1").arg(name));
  QString path = qApp->applicationDirPath();

  QFile newblas(QString("%1/%2.dll").arg(path, name));
  QFile blas(QString("%1/blas.dll").arg(path));
  if(!newblas.exists())
  {
     QMessageBox msgBox;
     msgBox.setText("Failed to find blas files");
     msgBox.setIcon(QMessageBox::Critical);
     msgBox.setInformativeText(QString("%1.dll").arg(name));
     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.exec();
  }
  else if (blas.open(QIODevice::WriteOnly|QIODevice::Truncate) == false ||
     newblas.open(QIODevice::ReadOnly) == false)
  {
     QMessageBox msgBox;
     msgBox.setText("Failed to open blas files");
     msgBox.setIcon(QMessageBox::Critical);
     if(blas.error() != QFileDevice::NoError)
        msgBox.setInformativeText(blas.errorString());
     else
        msgBox.setInformativeText(newblas.errorString());
     msgBox.setStandardButtons(QMessageBox::Ok);
     msgBox.exec();
  }
  else
  {
    char buff[1024];
    qint64 sz = newblas.read(buff, sizeof(buff));
    qint64 wsz = 0;
    while (sz > 0 && wsz >= 0)
    {
        wsz = blas.write(buff, sz);

        sz = newblas.read(buff, sizeof(buff));
    }
    if(wsz < 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to write to blas file");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setInformativeText(blas.errorString());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
  }

  showCurrentBlas(true);
}

void MainWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    qDebug("pressed %s", button->text().toUtf8().data());
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
      ui->currentBlasText->setText("Apply");
      if(ui->refBlasOption->isChecked())
      {
        setCurrentBlas("refblas");
      }
      else if(ui->openBlasOption->isChecked())
      {
        setCurrentBlas("openblas");
      }
    }
    else if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
    {
      close();
    }
}
