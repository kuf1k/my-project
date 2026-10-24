#ifndef ADDRECIPEDIALOG_H
#define ADDRECIPEDIALOG_H

#include <QDialog>
#include "core/Recipe.h"

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
    void onAddRequirementClicked();

    void onRemoveRequirementClicked();

    void onSaveClicked();

private:
    Ui::AddRecipeDialog *ui;
    core::Recipe createdRecipe;
};
#endif //ADDRECIPEDIALOG_H
