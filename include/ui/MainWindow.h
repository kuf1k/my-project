#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "core/Inventory.h"
#include "core/RecipeRepository.h"
#include "core/ShoppingList.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void onRefreshInventoryClicked();

    void onAddIngredientClicked();

    void onRemoveExpiredClicked();

    void onExportInventoryClicked();

    void onRecipeSelectionChanged();

    void onFindCookableRecipesClicked();

    void onCookableRecipeSelectionChanged();

    void onGenerateShoppingListClicked();

    void onExportShoppingListClicked();

    void onCookRecipeClicked();

    void onRecipesSortChanged(int index);

    void onMatcherSortChanged(int index);

private:
    Ui::MainWindow *ui;
    core::Inventory inventory;
    core::RecipeRepository recipeRepo;
    core::ShoppingList shoppingList;

    void loadData();

    void setupInventoryTable();

    void setupShoppingTable();

    void setupSortComboBoxes();

    void updateInventoryTable();

    void updateRecipesList();

    void updateShoppingRecipeComboBox();

    void displayRecipeDetails(const core::Recipe &recipe, QTextEdit *targetTextEdit);
};

#endif // MAINWINDOW_H
