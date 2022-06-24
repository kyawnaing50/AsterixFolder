#include "exportDialog.h"
#include "ui_exportDialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ExportDialog)
{
  ui->setupUi(this);
  connect(ui->browseButton, &QPushButton::clicked, this, &ExportDialog::showFileSelectDialog);
}

ExportDialog::~ExportDialog()
{
  delete ui;
}


int ExportDialog::fromBlock() const
{
  return ui->fromBlockSpin->value();
}


int ExportDialog::toBlock() const
{
  return ui->toBlockSpin->value();
}


QString ExportDialog::fileName() const
{
  return ui->fileNameEdit->text();
}


void ExportDialog::setBlockCount(int count)
{
  ui->fromBlockSpin->setRange(1, count);
  ui->fromBlockSpin->setValue(1);
  ui->toBlockSpin->setRange(1, count);
  ui->toBlockSpin->setValue(count);
}


void ExportDialog::showFileSelectDialog()
{
  QString name = QFileDialog::getSaveFileName(this, "Specify output file");
  ui->fileNameEdit->setText(name);
}
