#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "core/RecipeRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshInventoryClicked();

private:
    Ui::MainWindow *ui;
    core::Inventory inventory;
    core::RecipeRepository recipeRepo;

    void loadData();
    void setupInventoryTable();
    void updateInventoryTable();
};


#endif //MAINWINDOW_H