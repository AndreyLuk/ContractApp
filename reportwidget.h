/*
 * This file is a part of a project "contract" -
 * a Qt Widgets client/server application.
 *
 * It's a working demo, simplified, not a production level.
 *
 * Author: Andrey V. Lukashenko.
 *
 * Anyone can use parts of this program without any warranty
 * and without a reference to author.
 */

#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QtWidgets>
#include <QtSql>
#include <QPrinter>

#include "datamodule.h"

/*
 * A ReportWidget class selects data from a database
 * and creates an HTML document as a report.
 * Then A Print Preview Dialog is used to send the report
 * to a printer or PDF file.
*/
class ReportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReportWidget(QSettings *p_settings,
                          DataModule *p_dm,
                          QWidget *parent = nullptr);
    virtual ~ReportWidget();

private slots:
    void selectType(int p_index);
    void selectPartner(int p_index);
    void selectData();
    void previewButtonClicked();
    void previewReport(QPrinter *printer);
    void clearTextEdit();

private:

    const int BOX_WIDTH {680};

    bool setupFilterBox();
    bool setupDataBox();
    void setupControlBox();

    QVBoxLayout     *mainLayout;

    QGroupBox       *filterBox {nullptr};
    QFormLayout     *filterLayout {nullptr};
    QLabel          *typeLabel {nullptr};
    QComboBox       *typeComboBox {nullptr};
    QLabel          *partnerLabel {nullptr};
    QComboBox       *partnerComboBox {nullptr};

    QGroupBox       *dataBox {nullptr};
    QVBoxLayout     *dataLayout {nullptr};
    QTextEdit       *textEdit {nullptr};

    QGroupBox       *controlBox {nullptr};
    QHBoxLayout     *controlLayout {nullptr};
    QPushButton     *selectDataButton {nullptr};
    QPushButton     *previewButton {nullptr};

    QSettings *settings {nullptr};
    DataModule *dm {nullptr};
};

#endif // REPORTWIDGET_H
