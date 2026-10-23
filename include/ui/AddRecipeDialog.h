#ifndef ADDRECIPEDIALOG_H
#define ADDRECIPEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QSpinBox>
#include "core/Recipe.h"

class AddRecipeDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddRecipeDialog(QWidget *parent = nullptr);
    core::Recipe getCreatedRecipe() const;

private slots:
    void onAddRequirementClicked();
    void onRemoveRequirementClicked();
    void onSaveClicked();

private:
    QLineEdit *titleLineEdit;
    QSpinBox *prepTimeSpinBox;
    QDoubleSpinBox *caloriesSpinBox;
    QTextEdit *instructionsTextEdit;

    QTableWidget *requirementsTable;
    QPushButton *addReqButton;
    QPushButton *removeReqButton;

    QLineEdit *reqNameEdit;
    QDoubleSpinBox *reqAmountSpinBox;
    QComboBox *reqUnitComboBox;

    core::Recipe createdRecipe;

    void setupUi();
};
#endif //ADDRECIPEDIALOG_H