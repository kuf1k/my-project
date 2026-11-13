#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "core/Inventory.h"
#include "core/RecipeRepository.h"
#include "core/ShoppingList.h"
#include "core/MealPlanner.h"

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

    void onDeleteIngredientClicked();

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

    void onAddRecipeClicked();

    void onAddMealPlanClicked();

    void onRemoveMealPlanClicked();

    void onClearMealPlanClicked();

    void onGenerateWeeklyShoppingClicked();

    void onApplySmartFilterClicked();

    void onResetSmartFilterClicked();

    void onFilteredRecipeSelectionChanged();

    void onQuickAddToMealPlanClicked();

private:
    Ui::MainWindow *ui;
    core::Inventory inventory;
    core::RecipeRepository recipeRepo;
    core::ShoppingList shoppingList;
    core::MealPlanner mealPlanner;

    void loadData();

    void setupInventoryTable();

    void setupShoppingTable();

    void setupMealPlanTable();

    void setupSortComboBoxes();

    void setupMealPlannerControls();

    void setupSmartFilterControls();

    void updateInventoryTable();

    void updateRecipesList();

    void updateShoppingRecipeComboBox();

    void updateMealPlanTable();

    void displayRecipeDetails(const core::Recipe &recipe, QTextEdit *targetTextEdit);

    QString formatDayWithDate(const QString &dayName) const;
};

#endif // MAINWINDOW_H
