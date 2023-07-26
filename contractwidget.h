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

#ifndef CONTRACTWIDGET_H
#define CONTRACTWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPlainTextEdit>
#include <QTableView>
#include <QDataWidgetMapper>

#include "datamodule.h"

/*
 * Класс обработки договоров.
 * Поддерживаются два режима работы с договорами:
 * таблица и форма вставки/правки.
 * Для фиксации данных применяются транзакции.
 */
class ContractWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContractWidget(QSettings *p_settings,
                            DataModule *p_dm,
                            QWidget *p_parent = nullptr);
    virtual ~ContractWidget();

    // режим : таблица или форма
    enum class Mode
    {
        TABLE,
        FORM
    };

private slots:
    void insertRow();
    void editRow();
    void deleteRow();
    void revert();
    void submit();
    void calcSum();
    void dataChanged(const QModelIndex &topLeft,
                     const QModelIndex &bottomRight);

private:
    const int EDIT_DATE_WIDTH {140};
    const int COMBO_WIDTH {300};
    const int LINE_EDIT_WIDTH {200};
    const int TEXT_EDIT_HEIGHT {80};
    const int LIST_EDIT_HEIGHT {650};
    const int CONTROL_BOX_HEIGHT {100};
    const QString GRAY_STYLE {QString("background-color: #456")};

    void createTableView();
    void createEditBox();
    void createControlBox();
    void setWidgets();

    QVBoxLayout     *widgetLayout {nullptr};

    QGroupBox       *tableBox {nullptr};
    QVBoxLayout     *tableLayout {nullptr};
    QTableView      *tableView {nullptr};

    QGroupBox       *editBox {nullptr};
    QFormLayout     *editLayout {nullptr};
    QLabel          *contractIdLabel {nullptr};
    QLineEdit       *contractIdEdit {nullptr};
    QLabel          *typeLabel {nullptr};
    QComboBox       *typeComboBox {nullptr};
    QLabel          *stateLabel {nullptr};
    QComboBox       *stateComboBox {nullptr};
    QLabel          *partnerLabel {nullptr};
    QComboBox       *partnerComboBox {nullptr};
    QLabel          *contractNumLabel {nullptr};
    QLineEdit       *contractNumEdit {nullptr};
    QLabel          *contractDateLabel {nullptr};
    QDateEdit       *contractDateEdit {nullptr};
    QLabel          *startLabel {nullptr};
    QDateEdit       *startEdit {nullptr};
    QLabel          *finishLabel {nullptr};
    QDateEdit       *finishEdit {nullptr};
    QLabel          *vatRateLabel {nullptr};
    QLineEdit       *vatRateEdit {nullptr};
    QLabel          *contractSumLabel {nullptr};
    QLineEdit       *contractSumEdit {nullptr};
    QLabel          *vatSumLabel {nullptr};
    QLineEdit       *vatSumEdit {nullptr};
    QLabel          *totalSumLabel {nullptr};
    QLineEdit       *totalSumEdit {nullptr};
    QLabel          *contractNoteLabel {nullptr};
    QPlainTextEdit  *contractNoteEdit {nullptr};
    QLabel          *updatedDateLabel {nullptr};
    QLineEdit       *updatedDateEdit {nullptr};
    QLabel          *updatedByLabel {nullptr};
    QLineEdit       *updatedByEdit {nullptr};

    QDataWidgetMapper   *mapper {nullptr};

    QGroupBox       *controlBox {nullptr};
    QHBoxLayout     *controlLayout {nullptr};
    QPushButton     *insertButton {nullptr};
    QPushButton     *editButton {nullptr};
    QPushButton     *deleteButton {nullptr};
    QPushButton     *revertButton {nullptr};
    QPushButton     *submitButton {nullptr};

    Mode            m_mode {Mode::TABLE};

    // текущий ID (для возврата к договору в таблице после сохранения правки)
    int             m_currentId {0};

    QSettings                   *settings {nullptr};
    DataModule                  *dm {nullptr};
    QSqlRelationalTableModel    *model {nullptr};
};

#endif // CONTRACTWIDGET_H
