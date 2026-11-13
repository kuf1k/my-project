#include "ui/TranslationManager.h"

TranslationManager::TranslationManager() {

    ukTexts["app_title"] = "🥗 NutriMesh — Розумний кухонний помічник";
    enTexts["app_title"] = "🥗 NutriMesh — Smart Kitchen Assistant";

    ukTexts["tab_pantry"] = "🧊 Комора та Холодильник";
    enTexts["tab_pantry"] = "🧊 Pantry & Fridge";

    ukTexts["tab_matcher"] = "🍳 Що Я можу приготувати?";
    enTexts["tab_matcher"] = "🍳 What Can I Cook?";

    ukTexts["tab_recipes"] = "📖 Книга рецептів";
    enTexts["tab_recipes"] = "📖 Recipe Book";

    ukTexts["tab_shopping"] = "🛒 Список покупок";
    enTexts["tab_shopping"] = "🛒 Shopping List";

    ukTexts["tab_meal"] = "📅 Планувальник";
    enTexts["tab_meal"] = "📅 Meal Planner";

    ukTexts["tab_filter"] = "🎛️ Розумний фільтрувальник";
    enTexts["tab_filter"] = "🎛️ Smart Filter";

    ukTexts["tab_stats"] = "📊 Статистика";
    enTexts["tab_stats"] = "📊 Statistics";

    ukTexts["btn_add_ingredient"] = "➕ Додати інгредієнт";
    enTexts["btn_add_ingredient"] = "➕ Add Ingredient";

    ukTexts["btn_delete_ingredient"] = "❌ Видалити вибране";
    enTexts["btn_delete_ingredient"] = "❌ Delete Selected";

    ukTexts["btn_remove_expired"] = "🗑️ Видалити протерміновані";
    enTexts["btn_remove_expired"] = "🗑️ Remove Expired";

    ukTexts["btn_export_inventory"] = "📄 Експортувати звіт інвентарю";
    enTexts["btn_export_inventory"] = "📄 Export Inventory Report";

    ukTexts["btn_refresh_inventory"] = "🔄 Оновити інвентар";
    enTexts["btn_refresh_inventory"] = "🔄 Refresh Inventory";

    ukTexts["inv_header_name"] = "Назва";
    enTexts["inv_header_name"] = "Name";

    ukTexts["inv_header_amount"] = "Кількість";
    enTexts["inv_header_amount"] = "Amount";

    ukTexts["inv_header_unit"] = "Одиниця";
    enTexts["inv_header_unit"] = "Unit";

    ukTexts["inv_header_exp"] = "Термін придатності";
    enTexts["inv_header_exp"] = "Expiration Date";

    ukTexts["btn_find_recipes"] = "🔍 Знайти рецепти";
    enTexts["btn_find_recipes"] = "🔍 Find Recipes";

    ukTexts["lbl_sort_by"] = "Сортування:";
    enTexts["lbl_sort_by"] = "Sort by:";

    ukTexts["btn_cook_recipe"] = "🍳 Приготувати (Спожити інгредієнти)";
    enTexts["btn_cook_recipe"] = "🍳 Cook This Recipe (Consume Ingredients)";

    ukTexts["lbl_select_recipe"] = "Оберіть рецепт:";
    enTexts["lbl_select_recipe"] = "Select Recipe:";

    ukTexts["btn_gen_shopping"] = "⚡ Згенерувати відсутні інгредієнти";
    enTexts["btn_gen_shopping"] = "⚡ Generate Missing Ingredients";

    ukTexts["btn_export_shopping"] = "💾 Експортувати список покупок";
    enTexts["btn_export_shopping"] = "💾 Export Shopping List to File";

    ukTexts["lbl_day"] = "День:";
    enTexts["lbl_day"] = "Day:";

    ukTexts["lbl_time"] = "Час:";
    enTexts["lbl_time"] = "Time:";

    ukTexts["lbl_recipe"] = "Рецепт:";
    enTexts["lbl_recipe"] = "Recipe:";

    ukTexts["btn_add_schedule"] = "➕ Додати до розкладу";
    enTexts["btn_add_schedule"] = "➕ Add to Schedule";

    ukTexts["btn_clear_schedule"] = "🧹 Очистити розклад";
    enTexts["btn_clear_schedule"] = "🧹 Clear Schedule";

    ukTexts["btn_weekly_shopping"] = "🛒 Згенерувати тижневий список";
    enTexts["btn_weekly_shopping"] = "🛒 Generate Weekly Shopping List";

    ukTexts["lbl_max_calories"] = "Макс. калорії (0 = будь-які):";
    enTexts["lbl_max_calories"] = "Max Calories (0 = any):";

    ukTexts["lbl_max_time"] = "Макс. час (0 = будь-який):";
    enTexts["lbl_max_time"] = "Max Time (0 = any):";

    ukTexts["btn_apply_filter"] = "🔍 Застосувати фільтр";
    enTexts["btn_apply_filter"] = "🔍 Apply Filter";

    ukTexts["btn_reset"] = "🔄 Скинути";
    enTexts["btn_reset"] = "🔄 Reset";

    ukTexts["btn_refresh_stats"] = "🔄 Оновити аналітику";
    enTexts["btn_refresh_stats"] = "🔄 Refresh Analytics";

    ukTexts["dlg_add_ing_title"] = "Додати інгредієнт";
    enTexts["dlg_add_ing_title"] = "Add Ingredient";

    ukTexts["lbl_type"] = "Тип:";
    enTexts["lbl_type"] = "Type:";

    ukTexts["lbl_name"] = "Назва:";
    enTexts["lbl_name"] = "Name:";

    ukTexts["lbl_amount"] = "Кількість:";
    enTexts["lbl_amount"] = "Amount:";

    ukTexts["lbl_date"] = "Термін придатності:";
    enTexts["lbl_date"] = "Expiration Date:";

    ukTexts["btn_add"] = "Додати";
    enTexts["btn_add"] = "Add";

    ukTexts["btn_cancel"] = "Скасувати";
    enTexts["btn_cancel"] = "Cancel";

    ukTexts["dlg_add_rec_title"] = "Додати рецепт";
    enTexts["dlg_add_rec_title"] = "Add Recipe";

    ukTexts["lbl_title"] = "Назва рецепта:";
    enTexts["lbl_title"] = "Recipe Title:";

    ukTexts["lbl_prep_time"] = "Час приготування (хв):";
    enTexts["lbl_prep_time"] = "Prep Time (mins):";

    ukTexts["lbl_calories"] = "Калорії (ккал):";
    enTexts["lbl_calories"] = "Calories (kcal):";

    ukTexts["lbl_instructions"] = "Інструкції:";
    enTexts["lbl_instructions"] = "Instructions:";

    ukTexts["lbl_req_ingredients"] = "Необхідні інгредієнти:";
    enTexts["lbl_req_ingredients"] = "Required Ingredients:";

    ukTexts["btn_add_req"] = "➕ Додати інгредієнт";
    enTexts["btn_add_req"] = "➕ Add Requirement";

    ukTexts["btn_remove_req"] = "❌ Видалити вибране";
    enTexts["btn_remove_req"] = "❌ Remove Selected";

    ukTexts["btn_save"] = "💾 Зберегти рецепт";
    enTexts["btn_save"] = "💾 Save Recipe";
}

TranslationManager& TranslationManager::instance() {
    static TranslationManager manager;
    return manager;
}

void TranslationManager::setLanguage(Language lang) {
    currentLang = lang;
}

Language TranslationManager::getCurrentLanguage() const {
    return currentLang;
}

QString TranslationManager::tr(const QString &key) const {
    if (currentLang == Language::Ukrainian) {
        return ukTexts.value(key, key);
    } else {
        return enTexts.value(key, key);
    }
}