#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtWidgets>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ExportDialog(QWidget *parent = nullptr);
  ~ExportDialog();

  int fromBlock() const;
  int toBlock() const;
  QString fileName() const;

public slots:
  void setBlockCount(int count);
  void showFileSelectDialog();


private:
  Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
