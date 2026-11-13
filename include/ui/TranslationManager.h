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

    TranslationManager();

public:
    static TranslationManager& instance();

    void setLanguage(Language lang);
    Language getCurrentLanguage() const;

    QString tr(const QString &key) const;
};

#endif //TRANSLATIONMANAGER_H