#ifndef ADDINGREDIENTDIALOG_H
#define ADDINGREDIENTDIALOG_H
#include <QDialog>
#include <memory>
#include "core/Ingredient.h"

namespace Ui {
    class AddIngredientDialog;
}

class AddIngredientDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddIngredientDialog(QWidget *parent = nullptr);

    ~AddIngredientDialog();

    std::unique_ptr<core::Ingredient> getCreatedIngredient();

private slots:
    void onTypeChanged(int index);

    void onAddClicked();

private:
    Ui::AddIngredientDialog *ui;
    std::unique_ptr<core::Ingredient> createdIngredient;
};
#endif // ADDINGREDIENTDIALOG_H
