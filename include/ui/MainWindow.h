#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "core/RecipeRepository.h"

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

    void onRecipeSelectionChanged();

    void onFindCookableRecipesClicked();

    void onCookableRecipeSelectionChanged();

    void onRecipesSortChanged(int index);

    void onMatcherSortChanged(int index);

private:
    Ui::MainWindow *ui;
    core::Inventory inventory;
    core::RecipeRepository recipeRepo;

    void loadData();

    void setupInventoryTable();

    void updateInventoryTable();

    void updateRecipesList();

    void setupSortComboBoxes();

    void sortRecipes(std::vector<core::Recipe> &recipes, int sortOption);

    void displayRecipeDetails(const core::Recipe &recipe, class QTextEdit *targetTextEdit);

};


#endif //MAINWINDOW_H
