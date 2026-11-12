#ifndef ADDRECIPEDIALOG_H
#define ADDRECIPEDIALOG_H

#include <QDialog>
#include "core/Recipe.h"
#include "core/IngredientValidator.h"

namespace Ui {
    class AddRecipeDialog;
}

class AddRecipeDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddRecipeDialog(QWidget *parent = nullptr);

    ~AddRecipeDialog();


    core::Recipe getCreatedRecipe() const;

private slots:
    void onReqNameChanged(const QString &text);

    void onAddRequirementClicked();

    void onRemoveRequirementClicked();

    void onSaveClicked();

private:
    Ui::AddRecipeDialog *ui;
    core::Recipe createdRecipe;
    core::IngredientValidator validator;
};
#endif //ADDRECIPEDIALOG_H
