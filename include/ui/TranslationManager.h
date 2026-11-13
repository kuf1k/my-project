#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QString>
#include <QMap>


enum class Language {
    Ukrainian,
    English
};

class TranslationManager {
    Language currentLang = Language::Ukrainian;
    QMap<QString, QString> ukTexts;
    QMap<QString, QString> enTexts;

    QMap<QString, QString> ingToUk;
    QMap<QString, QString> unitToUk;
    QMap<QString, QString> recipeTitleToUk;
    QMap<QString, QString> recipeInstrToUk;

    TranslationManager();

public:
    static TranslationManager& instance();

    void setLanguage(Language lang);
    Language getCurrentLanguage() const;

    QString tr(const QString &key) const;

    QString translateIngredient(const QString &enName) const;
    QString translateUnit(const QString &enUnit) const;
    QString translateRecipeTitle(const QString &enTitle) const;
    QString translateInstructions(const QString &enTitle, const QString &rawInstructions) const;
};

#endif //TRANSLATIONMANAGER_H