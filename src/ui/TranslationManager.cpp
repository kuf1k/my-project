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

    ukTexts["btn_add_recipe"] = "➕ Додати власний рецепт";
    enTexts["btn_add_recipe"] = "➕ Add Custom Recipe";

    ukTexts["btn_remove_req"] = "❌ Видалити вибране";
    enTexts["btn_remove_req"] = "❌ Remove Selected";

    ukTexts["btn_save"] = "💾 Зберегти рецепт";
    enTexts["btn_save"] = "💾 Save Recipe";

    ingToUk["eggs"] = "Яйця";
    ingToUk["apples"] = "Яблука";
    ingToUk["apple"] = "Яблука";
    ingToUk["bananas"] = "Банани";
    ingToUk["banana"] = "Банани";
    ingToUk["tomatoes"] = "Помідори";
    ingToUk["potatoes"] = "Картопля";
    ingToUk["onions"] = "Цибуля";
    ingToUk["onion"] = "Цибуля";
    ingToUk["carrots"] = "Морква";
    ingToUk["carrot"] = "Морква";
    ingToUk["lemons"] = "Лимони";
    ingToUk["lemon"] = "Лимон";
    ingToUk["oranges"] = "Апельсини";
    ingToUk["cucumbers"] = "Огірки";
    ingToUk["cucumber"] = "Огірки";
    ingToUk["garlic"] = "Часник";
    ingToUk["bread"] = "Хліб";
    ingToUk["avocado"] = "Авокадо";
    ingToUk["bell pepper"] = "Болгарський перець";

    ingToUk["milk"] = "Молоко";
    ingToUk["water"] = "Вода";
    ingToUk["olive oil"] = "Оливкова олія";
    ingToUk["sunflower oil"] = "Соняшникова олія";
    ingToUk["vegetable oil"] = "Рослинна олія";
    ingToUk["oil"] = "Олія";
    ingToUk["cream"] = "Вершки";
    ingToUk["soy sauce"] = "Соєвий соус";
    ingToUk["vinegar"] = "Оцет";
    ingToUk["juice"] = "Сік";
    ingToUk["orange juice"] = "Апельсиновий сік";
    ingToUk["honey"] = "Мед";
    ingToUk["kefir"] = "Кефір";
    ingToUk["yogurt"] = "Йогурт";

    ingToUk["flour"] = "Борошно";
    ingToUk["sugar"] = "Цукор";
    ingToUk["salt"] = "Сіль";
    ingToUk["butter"] = "Вершкове масло";
    ingToUk["cheese"] = "Сир";
    ingToUk["feta cheese"] = "Сир Фета";
    ingToUk["rice"] = "Рис";
    ingToUk["pasta"] = "Макарони";
    ingToUk["buckwheat"] = "Гречка";
    ingToUk["oatmeal"] = "Вівсянка";
    ingToUk["chicken breast"] = "Куряче філе";
    ingToUk["chicken"] = "Курка";
    ingToUk["beef"] = "Яловичина";
    ingToUk["pork"] = "Свинина";
    ingToUk["fish"] = "Риба";
    ingToUk["salmon"] = "Лосось";
    ingToUk["cottage cheese"] = "Кисломолочний сир";
    ingToUk["sour cream"] = "Сметана";
    ingToUk["chocolate"] = "Шоколад";
    ingToUk["cocoa"] = "Какао";
    ingToUk["yeast"] = "Дріжджі";
    ingToUk["pepper"] = "Перець";
    ingToUk["mushrooms"] = "Печериці / Гриби";
    ingToUk["pepperoni"] = "Пепероні";

    unitToUk["g"] = "г";
    unitToUk["ml"] = "мл";
    unitToUk["pcs"] = "шт.";

    recipeTitleToUk["Pancakes"] = "Млинці";
    recipeInstrToUk["Pancakes"] = "1) У мисці збийте 2 яйця з 500 мл молока; 2) Поступово просійте 200 г борошна, постійно збиваючи, щоб не було грудочок; 3) Додайте дрібку солі та цукор за бажанням; 4) Розігрійте сковороду на середньому вогні, злегка змастіть олією та налийте ополоник тіста; 5) Смажте по 1–2 хвилини з кожного боку до золотистої скоринки.";

    recipeTitleToUk["Omelette"] = "Омлет";
    recipeInstrToUk["Omelette"] = "1) Розбийте 3 яйця в миску та влийте 100 мл молока; 2) Посоліть і поперчіть, після чого енергійно збивайте протягом 1 хвилини; 3) Розтопіть 15 г вершкового масла на антипригарній сковороді на середньому вогні; 4) Вилийте яєчну суміш на сковороду; 5) Готуйте під кришкою на слабкому вогні 5–7 хвилин до пишності.";

    recipeTitleToUk["Lemon Tea"] = "Лимонний чай";
    recipeInstrToUk["Lemon Tea"] = "1) Закип'ятіть 300 мл води; 2) Покладіть 1 чайний пакетик (або заварку) у чашку та залийте окропом; 3) Дайте настоятися 3–4 хвилини; 4) Вичавіть 0,5 шт. лимона в чай та розмішайте в ньому 15 г меду.";

    recipeTitleToUk["Scrambled Eggs with Tomatoes"] = "Яєчня з помідорами";
    recipeInstrToUk["Scrambled Eggs with Tomatoes"] = "1) Наріжте 1 помідор дрібними кубиками; 2) Обсмажте помідори на сковороді з 10 мл оливкової олії протягом 3 хвилин до м'якості; 3) Збийте 3 яйця в мисці та вилийте поверх помідорів; 4) Обережно перемішуйте лопаткою на середньо-слабкому вогні до ніжної кремової консистенції.";

    recipeTitleToUk["Mashed Potatoes"] = "Картопляне пюре";
    recipeInstrToUk["Mashed Potatoes"] = "1) Очистіть 500 г картоплі та наріжте однаковими кубиками; 2) Покладіть у каструлю з підсоленою водою та варіть 15–20 хвилин до м'якості; 3) Злийте воду; 4) Підігрійте 100 мл молока та розтопіть 30 г вершкового масла; 5) Ретельно розімніть картоплю, додаючи молоко та масло до однорідності.";

    recipeTitleToUk["Greek Salad"] = "Грецький салат";
    recipeInstrToUk["Greek Salad"] = "1) Наріжте 2 помідори, 1 огірок та 0,5 червоної цибулини великими шматочками; 2) Наріжте кубиками 150 г сиру Фета; 3) З'єднайте овочі в мисці, зверху викладіть фету; 4) Полийте 20 мл оливкової олії та посипте сушеним орегано.";

    recipeTitleToUk["Oatmeal with Honey and Apple"] = "Вівсянка з медом та яблуком";
    recipeInstrToUk["Oatmeal with Honey and Apple"] = "1) Доведіть 250 мл молока та 100 мл води до кипіння в невеликій каструлі; 2) Додайте 80 г вівсянки та дрібку солі; 3) Варіть на слабкому вогні, постійно помішуючи, 5 хвилин; 4) Наріжте 1 яблуко кубиками; 5) Подавайте кашу з кубиками яблука та 20 г меду.";

    recipeTitleToUk["Garlic Pasta"] = "Часникова паста";
    recipeInstrToUk["Garlic Pasta"] = "1) Відваріть 200 г макаронів у підсоленій воді згідно з інструкцією на упаковці; 2) Тонко наріжте 3 зубчики часнику; 3) Розігрійте 30 мл оливкової олії на сковороді та злегка обсмажте часник до золотистого кольору; 4) Злийте воду з пасти, залишивши 50 мл відвару; 5) Перемішайте пасту з часниковою олією, додавши воду з-під макаронів для шовковистого соусу.";

    recipeTitleToUk["Chicken Soup with Rice"] = "Курячий суп із рисом";
    recipeInstrToUk["Chicken Soup with Rice"] = "1) Наріжте 300 г курячого філе невеликими шматочками; 2) Варіть курку в 1000 мл води 15 хвилин, знімаючи піну; 3) Додайте 70 г рису, нарізану кубиками 1 моркву та 1 картоплину; 4) Варіть на повільному вогні ще 20 хвилин до готовності овочів.";

    recipeTitleToUk["Avocado Toast with Poached Egg"] = "Тост з авокадо та яйцем пашот";
    recipeInstrToUk["Avocado Toast with Poached Egg"] = "1) Підсушіть 2 скибочки хліба в тостері або на сковороді до золотистої скоринки; 2) Розімніть 1 авокадо в мисці з сіллю та лимонним соком; 3) Закип'ятіть воду з оцтом, зробіть воронку та зваріть 1 яйце пашот протягом 3 хвилин; 4) Намажте авокадо на тости та викладіть зверху яйце пашот.";

    recipeTitleToUk["Rice with Vegetables"] = "Рис з овочами";
    recipeInstrToUk["Rice with Vegetables"] = "1) Промийте 150 г рису та варіть у 300 мл підсоленої води 15 хвилин; 2) Наріжте кубиками 1 моркву та 1 болгарський перець; 3) Розігрійте 15 мл рослинної олії на сковороді та обсмажте овочі 7 хвилин; 4) Додайте відварений рис на сковороду та обсмажте все разом ще 3 хвилини.";

    recipeTitleToUk["Banana Milkshake"] = "Банановий молочний коктейль";
    recipeInstrToUk["Banana Milkshake"] = "1) Очистіть 2 стиглих банани та наріжте кружальцями; 2) Покладіть шматочки банана в чашу блендера; 3) Влийте 400 мл холодного молока та додайте 10 г меду; 4) Збивайте на високій швидкості 60 секунд до однорідності.";

    recipeTitleToUk["French Toast"] = "Французькі грінки";
    recipeInstrToUk["French Toast"] = "1) У мисці збийте 2 яйця з 50 мл молока та 15 г цукру; 2) Занурте 3 скибочки хліба в яєчну суміш з обох боків; 3) Розтопіть 20 г вершкового масла на сковороді на середньому вогні; 4) Обсмажуйте грінки по 2–3 хвилини з кожного боку до золотистої скоринки.";

    recipeTitleToUk["Fried Mushrooms with Onions"] = "Смажені печериці з цибулею";
    recipeInstrToUk["Fried Mushrooms with Onions"] = "1) Наріжте пластинками 300 г печериць та подрібніть 1 цибулину; 2) Розігрійте 20 мл рослинної олії на сковороді; 3) Додайте цибулю та пасеруйте 3 хвилини до прозорості; 4) Додайте гриби, посоліть та смажте 12–15 хвилин.";

    recipeTitleToUk["Tomato Basil Soup"] = "Томатний суп із базиліком";
    recipeInstrToUk["Tomato Basil Soup"] = "1) Наріжте 4 помідори та 1 цибулину; 2) Розігрійте 15 мл оливкової олії в каструлі та обсмажте цибулю 4 хвилини; 3) Додайте помідори та влийте 400 мл води, тушкуйте 20 хвилин; 4) Збийте суп блендером до стану пюре, приправте сіллю та базиліком.";

    recipeTitleToUk["Pan-Seared Chicken Breast"] = "Смажене куряче філе";
    recipeInstrToUk["Pan-Seared Chicken Breast"] = "1) Обсушіть паперовим рушником 350 г курячого філе та натріть сіллю й часником; 2) Розігрійте 15 мл оливкової олії на сковороді на середньо-сильному вогні; 3) Викладіть філе та смажте 6–7 хвилин; 4) Переверніть і готуйте ще 5–6 хвилин до повної готовності.";

    recipeTitleToUk["Cottage Cheese Pancakes (Syrnyky)"] = "Сирники";
    recipeInstrToUk["Cottage Cheese Pancakes (Syrnyky)"] = "1) Розімніть 300 г кисломолочного сиру з 1 яйцем, 30 г цукру та дрібкою солі; 2) Вмішайте 50 г борошна до утворення м'якого тіста; 3) Сформуйте невеликі круглі сирники; 4) Смажте на 20 г вершкового масла на середньо-слабкому вогні по 4 хвилини з кожного боку.";

    recipeTitleToUk["Fresh Cucumber Salad"] = "Салат зі свіжих огірків";
    recipeInstrToUk["Fresh Cucumber Salad"] = "1) Тонко наріжте 3 свіжих огірки; 2) Змішайте в мисці 100 мл сметани з сіллю, перцем та кропом; 3) Заправте нарізані огірки сметанним соусом і подавайте до столу.";

    recipeTitleToUk["Baked Apples with Honey and Cinnamon"] = "Запечені яблука з медом та корицею";
    recipeInstrToUk["Baked Apples with Honey and Cinnamon"] = "1) Розігрійте духовку до 180°C; 2) Помийте 3 яблука та обережно виріжте серцевину; 3) Наповніть заглиблення 30 г меду та посипте корицею; 4) Запікайте 20 хвилин до м'якості.";

    recipeTitleToUk["Cheesy Toast"] = "Гарячі сирні тости";
    recipeInstrToUk["Cheesy Toast"] = "1) Наріжте 100 г сиру товстими скибочками; 2) Викладіть сир на 2 скибочки хліба; 3) Змастіть зовнішні сторони хліба 10 г вершкового масла; 4) Підсмажте на сковороді по 3 хвилини з кожного боку, поки сир не розплавиться.";

    recipeTitleToUk["Homemade Pepperoni Pizza"] = "Домашня піца Пепероні";
    recipeInstrToUk["Homemade Pepperoni Pizza"] = "1) Змішайте 200 г борошна, 120 мл води та дрібку солі для тіста; 2) Рівномірно розкачайте тісто на деку; 3) Змастіть основу 50 г томатного соусу; 4) Щедро посипте 150 г сиру та викладіть 80 г скибочок пепероні; 5) Випікайте в духовці при 220°C 12–15 хвилин до золотистої скоринки.";
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

QString TranslationManager::translateIngredient(const QString &enName) const {
    if (currentLang == Language::English) {
        return enName;
    }
    return ingToUk.value(enName.trimmed().toLower(), enName);
}

QString TranslationManager::translateUnit(const QString &enUnit) const {
    if (currentLang == Language::English) {
        return enUnit;
    }
    return unitToUk.value(enUnit.trimmed().toLower(), enUnit);
}

QString TranslationManager::translateRecipeTitle(const QString &enTitle) const {
    if (currentLang == Language::English) {
        return enTitle;
    }
    return recipeTitleToUk.value(enTitle.trimmed(), enTitle);
}

QString TranslationManager::translateInstructions(const QString &enTitle, const QString &rawInstructions) const {
    if (currentLang == Language::English) {
        return rawInstructions;
    }
    return recipeInstrToUk.value(enTitle.trimmed(), rawInstructions);
}