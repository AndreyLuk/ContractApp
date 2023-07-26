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

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>

#include "reportwidget.h"

// the constructor
ReportWidget::ReportWidget(QSettings *p_settings,
                         DataModule *p_dm,
                         QWidget *p_parent)
    : QWidget(p_parent),
      settings(p_settings),
      dm(p_dm)
{
    setFixedWidth(p_parent->width() * 0.95);
    setFixedHeight(p_parent->height() * 0.92);

    mainLayout = new QVBoxLayout;
    if (!setupFilterBox()) {
        QMessageBox::critical(this, tr("Form creation error"),
                              "Filter Box creation error.");
        return;
    }
    if (!setupDataBox()) {
        QMessageBox::critical(this, tr("Form creation error"),
                              "Data Box creation error.");
        return;
    }
    setupControlBox();
    setLayout(mainLayout);

    // connect signals to slots using an old style (with macros SIGNAL/SLOT)
    connect(selectDataButton, SIGNAL(clicked(bool)), SLOT(selectData()));
    connect(previewButton, SIGNAL(clicked(bool)), SLOT(previewButtonClicked()));
    connect(typeComboBox, SIGNAL(currentIndexChanged(int)),
            SLOT(selectType(int)));
    connect(partnerComboBox, SIGNAL(currentIndexChanged(int)),
            SLOT(selectPartner(int)));

} // ReportWidget::ReportWidget()

// the destructor
ReportWidget::~ReportWidget()
{

} // ~ReportWidget()

// create & set a filter box
bool ReportWidget::setupFilterBox()
{
    filterBox = new QGroupBox;
    filterBox->setTitle(tr("Filters"));
    filterLayout = new QFormLayout;
    typeLabel = new QLabel(tr("Contract type"));
    typeComboBox = new QComboBox;
    typeComboBox->addItem(tr("-- All contract types --"));
    typeComboBox->setCurrentIndex(0);

    // fill typeComboBox
    QSqlQuery query(dm->database());
    bool result = query.exec(
            "select type_id, type_name from " +
                dm->schema() + ".contract_type order by type_id");
    if (!result) {
        QMessageBox::critical(this, tr("Query error"),
                              query.lastError().text());
        return false;
    }
    while (query.next()) {
        typeComboBox->addItem(query.record().field(1).value().toString());
    }
    filterLayout->addRow(typeLabel, typeComboBox);

    partnerLabel = new QLabel(tr("Partner"));
    partnerComboBox = new QComboBox;

    // fill partnerComboBox
    partnerComboBox->addItem(tr("-- All partners --"));
    typeComboBox->setCurrentIndex(0);

    result = query.exec(
            "select partner_id, partner_name from partner order by partner_id");
    if (!result) {
        QMessageBox::critical(this, tr("Query error"),
                              query.lastError().text());
        return false;
    }
    while (query.next()) {
        partnerComboBox->addItem(query.record().field(1).value().toString());
    }

    filterLayout->addRow(partnerLabel, partnerComboBox);
    filterBox->setLayout(filterLayout);
    filterBox->setFixedWidth(BOX_WIDTH);
    mainLayout->addWidget(filterBox);

    return true;

} // setupFilterBox()

// create & set a data box
bool ReportWidget::setupDataBox()
{
    dataBox = new QGroupBox;
    dataBox->setTitle(tr("Data"));
    dataLayout = new QVBoxLayout;
    textEdit = new QTextEdit;
    selectData();
    dataLayout->addWidget(textEdit);
    dataBox->setLayout(dataLayout);
    mainLayout->addWidget(dataBox);

    return true;

} // setupDataBox()

// create & set a button's box
void ReportWidget::setupControlBox()
{
    controlBox = new QGroupBox;
    controlLayout = new QHBoxLayout;
    selectDataButton = new QPushButton(tr("Select data"));
    previewButton = new QPushButton(tr("Preview"));
    controlLayout->addWidget(selectDataButton);
    controlLayout->addWidget(previewButton);
    controlBox->setLayout(controlLayout);
    controlBox->setFixedWidth(BOX_WIDTH);
    mainLayout->addWidget(controlBox);
} // setupControlBox()

// select data from the database and show it in a QTextEdit widget
void ReportWidget::selectData()
{
    textEdit->clear();
    if (previewButton != nullptr)
        previewButton->setEnabled(false);

    /* full SQL query
     *
    select c.contract_id, t.type_name, s.state_name, p.partner_name,
        c.contract_num, c.contract_date, c.start_date, c.finish_date,
        c.vat_rate, c.contract_sum, c.vat_sum, c.total_sum
      from contracts.contract as c, contracts.contract_type as t,
        contracts.contract_state as s, contracts.partner as p
      where (t.type_id = c.type_id) and (s.state_id = c.state_id)
        and (p.partner_id = c.partner_id)
        and (c.partner_id = 1)
        and (c.type_id = 1)
      order by c.contract_id desc
    */

    // get data from a database
    QString selectString, fromString, whereString, orderString, queryString;

    selectString +=
        " select c.contract_id, t.type_name, s.state_name, p.partner_name, ";
    selectString +=
        " c.contract_num, c.contract_date, c.start_date, c.finish_date, ";
    selectString +=
        " c.vat_rate, c.contract_sum, c.vat_sum, c.total_sum ";
    fromString +=
        " from contracts.contract as c, contracts.contract_type as t, ";
    fromString +=
        " contracts.contract_state as s, contracts.partner as p ";
    whereString +=
        " where (t.type_id = c.type_id) and (s.state_id = c.state_id) ";
    whereString += " and (p.partner_id = c.partner_id) ";
    if (typeComboBox->currentIndex() > 0) {
        whereString += " and (c.type_id = " +
                    QVariant(typeComboBox->currentIndex()).toString() + " ) ";
    }
    if (partnerComboBox->currentIndex() > 0) {
        whereString += " and (c.partner_id = " +
                QVariant(partnerComboBox->currentIndex()).toString() + " ) ";
    }
    orderString += " order by c.contract_id desc ";

    queryString = selectString + fromString + whereString + orderString;

    QSqlQuery query(dm->database());

    bool result = query.exec(queryString);

    if (!result) {
        QMessageBox::critical(this, tr("Select data error"),
                              query.lastError().text());
        return;
    }

    QString(tr("Contract report"));

    QString topString(
            "<!DOCTYPE html>"
            "<html>"
            "    <head>"
            "        <title>Contracts report</title>"
            "        <meta charset=""UTF-8"">"
            "        <style>"
            "            table, td, th {"
            "             border: black solid 1px;   "
            "            }"
            "        </style>"
            "    </head>"
            "    <body>");
    topString += QString("<div id=""top-div""><h1>");
    topString += QString(tr("Contract report"));
    topString += QString("</h1></div>");

    topString += QString(
            "        <div id=""content-div"">"
            "            <table border=1>"
            "                <tr>"
            "                <th>");
    topString += QString(tr("Id"));
    topString += QString("</th><th>");
    topString += QString(tr("Type"));
    topString += QString("</th><th>");
    topString += QString(tr("State"));
    topString += QString("</th><th>");
    topString += QString(tr("Partner"));
    topString += QString("</th><th>");
    topString += QString(tr("Number"));
    topString += QString("</th><th>");
    topString += QString(tr("Date"));
    topString += QString("</th><th>");
    topString += QString(tr("Start"));
    topString += QString("</th><th>");
    topString += QString(tr("Finish"));
    topString += QString("</th><th>");
    topString += QString(tr("VAT(%)"));
    topString += QString("</th><th>");
    topString += QString(tr("Contract Sum"));
    topString += QString("</th><th>");
    topString += QString(tr("VAT Sum"));
    topString += QString("</th><th>");
    topString += QString(tr("Total Sum"));
    topString += QString("</th></tr>");

    QString midString;

    while (query.next()) {
        midString += "<tr>";
        for (int i = 0; i < 13; ++i) {
            midString += "<td>";
            midString += query.record().field(i).value().toString();
            midString += "</td>";
        }
        midString += "</tr>";
    }

    QString rows = QVariant(query.size()).toString();
    QString bottomString(
                "            </table>"
                "        </div>"
                "        <div id=""bottom-div""><h3> " +rows+ " " +
                QString(tr("rows")) +
                "</h3></div>"
                "    </body>"
                "</html>");

    textEdit->setHtml(topString + midString + bottomString);
    if (previewButton != nullptr)
        previewButton->setEnabled(true);

} // selectData()

// set "contract type" filter
void ReportWidget::selectType(int p_index)
{
    textEdit->clear();
    if (previewButton != nullptr)
        previewButton->setEnabled(false);
    typeComboBox->setCurrentIndex(p_index);
} // selectType()

// set "partner" filter
void ReportWidget::selectPartner(int p_index)
{
    textEdit->clear();
    if (previewButton != nullptr)
        previewButton->setEnabled(false);
    partnerComboBox->setCurrentIndex(p_index);
} // selectPartner()

// previewButtonClicked slot
void ReportWidget::previewButtonClicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
            this, SLOT(previewReport(QPrinter*)));
    preview.exec();
} // previewButtonClicked()

// preview a report
void ReportWidget::previewReport(QPrinter *printer)
{
    textEdit->print(printer);
} // previewReport()

// clear a QTextEdit widget
void ReportWidget::clearTextEdit()
{
    textEdit->clear();
    if (previewButton != nullptr)
        previewButton->setEnabled(false);
} // clearTextEdit()
