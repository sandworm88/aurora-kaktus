/*
  Copyright (C) 2014 Michal Kosciesza <michal@mkiol.net>

  This file is part of Kaktus.

  Kaktus is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Kaktus is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Kaktus.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "settings.h"

#include "downloadmanager.h"
#include "databasemanager.h"
#include "cacheserver.h"
#include "netvibesfetcher.h"
#include "simplecrypt.h"
#include "../key.h"

Settings* Settings::inst = 0;

Settings::Settings(QObject *parent) : QObject(parent), settings()
{
    // Reset Last Update date if not Signed in
    if (!getSignedIn())
        setNetvibesLastUpdateDate(0);
}

Settings* Settings::instance()
{
    if (Settings::inst == NULL) {
        Settings::inst = new Settings();
    }

    return Settings::inst;
}

void Settings::setShowStarredTab(bool value)
{
    if (getShowStarredTab() != value) {
        settings.setValue("showstarredtab", value);
        emit showStarredTabChanged();
    }
}

bool Settings::getShowStarredTab()
{
    return settings.value("showstarredtab", true).toBool();
}

void Settings::setShowOnlyUnread(bool value)
{
    if (getShowOnlyUnread() != value) {
        settings.setValue("showonlyunread", value);
        emit showOnlyUnreadChanged();
    }
}

bool Settings::getShowOnlyUnread()
{
    return settings.value("showonlyunread", false).toBool();
}

void Settings::setOfflineMode(bool value)
{
    if (getOfflineMode() != value) {
        settings.setValue("offlinemode", value);
        emit offlineModeChanged();
    }
}

bool Settings::getOfflineMode()
{
    return settings.value("offlinemode", false).toBool();
}

void Settings::setAutoMarkAsRead(bool value)
{
    settings.setValue("automarkasread", value);
}

bool Settings::getAutoMarkAsRead()
{
    return settings.value("automarkasread", true).toBool();
}


void Settings::setShowTabIcons(bool value)
{
    if (getShowTabIcons() != value) {
        settings.setValue("showtabicons", value);
        emit showTabIconsChanged();
    }
}

bool Settings::getShowTabIcons()
{
    return settings.value("showtabicons", true).toBool();
}

void Settings::setSignedIn(bool value)
{
    if (getSignedIn() != value) {
        settings.setValue("signedin", value);
        emit signedInChanged();
    }
}

bool Settings::getSignedIn()
{
    return settings.value("signedin", false).toBool();
}

void Settings::setAutoDownloadOnUpdate(bool value)
{
    settings.setValue("autodownloadonupdate", value);
}

bool Settings::getAutoDownloadOnUpdate()
{
    return settings.value("autodownloadonupdate", true).toBool();
}

void Settings::setNetvibesUsername(const QString &value)
{
    settings.setValue("username", value);
}

QString Settings::getNetvibesUsername()
{
    return settings.value("username", "").toString();
}

void Settings::setNetvibesPassword(const QString &value)
{
    SimpleCrypt crypto(KEY);
    QString encryptedPassword = crypto.encryptToString(value);
    if (!crypto.lastError() == SimpleCrypt::ErrorNoError) {
        emit error(512);
    }
    settings.setValue("password", encryptedPassword);
}

QString Settings::getNetvibesPassword()
{
    SimpleCrypt crypto(KEY);
    QString plainPassword = crypto.decryptToString(settings.value("password", "").toString());
    if (!crypto.lastError() == SimpleCrypt::ErrorNoError) {
        emit error(511);
        return "";
    }
    return plainPassword;
}

void Settings::setDashboardInUse(const QString &value)
{
    if (getDashboardInUse() != value) {
        if (getDashboardInUse() == "") {
            settings.setValue("dafaultdashboard", value);
        } else {
            settings.setValue("dafaultdashboard", value);
            emit dashboardInUseChanged();
        }
    }
}

QString Settings::getDashboardInUse()
{
    return settings.value("dafaultdashboard", "").toString();
}

void Settings::setNetvibesLastUpdateDate(int value)
{
    settings.setValue("lastupdatedate", value);
}

int Settings::getNetvibesLastUpdateDate()
{
    return settings.value("lastupdatedate", 0).toInt();
}

QString Settings::getSettingsDir()
{
    QString value = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).path();

    if (!QDir(value).exists()) {
        if (!QDir::root().mkpath(value)) {
            qWarning() << "Unable to create settings dir!";
            emit error(501);
        }
    }

    return value;
}

void Settings::setDmConnections(int value)
{
    settings.setValue("connections", value);
}

int Settings::getDmConnections()
{
    return settings.value("connections", 20).toInt();
}

void Settings::setDmTimeOut(int value)
{
    settings.setValue("timeout", value);
}

int Settings::getDmTimeOut()
{
    return settings.value("timeout", 20000).toInt();
}

void Settings::setDmMaxSize(int value)
{
    settings.setValue("maxsize", value);
}

int Settings::getDmMaxSize()
{
    return settings.value("maxsize", 1000000).toInt();
}

QString Settings::getDmCacheDir()
{
    QString value = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation))
            .filePath("cached_files");

    if (!QDir(value).exists()) {
        if (!QDir::root().mkpath(value)) {
            qWarning() << "Unable to create cache dir!";
            emit error(502);
        }
    }

    return value;
}

void Settings::setDmUserAgent(const QString &value)
{
    settings.setValue("useragent", value);
}

QString Settings::getDmUserAgent()
{
    QString value = "Mozilla/5.0 (Linux; Android 4.2.1; Nexus 4 Build/JOP40D) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.166 Mobile Safari/535.19";
    return settings.value("useragent", value).toString();
}

QString Settings::getCsTheme()
{
    return settings.value("theme", "black").toString();
}

void Settings::setCsTheme(const QString &value)
{
    settings.setValue("theme", value);
}
