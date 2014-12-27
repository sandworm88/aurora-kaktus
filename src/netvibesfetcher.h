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

#ifndef NETVIBESFETCHER_H
#define NETVIBESFETCHER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QNetworkConfigurationManager>
#include <QThread>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QJsonObject>
#else
#include <QVariantMap>
#endif

#include "databasemanager.h"
#include "downloadmanager.h"

class NetvibesFetcher : public QThread
{
    Q_OBJECT
    Q_ENUMS(BusyType)
    Q_PROPERTY (bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY (BusyType busyType READ readBusyType NOTIFY busyChanged)

public:

    enum BusyType {
        Unknown = 0,
        Initiating = 1,
        Updating = 2,
        CheckingCredentials = 3,
        InitiatingWaiting = 11,
        UpdatingWaiting = 21,
        CheckingCredentialsWaiting = 31
    };

    explicit NetvibesFetcher(QObject *parent = 0);

    Q_INVOKABLE bool init();
    Q_INVOKABLE bool update();
    Q_INVOKABLE bool checkCredentials();
    Q_INVOKABLE void cancel();

    BusyType readBusyType();
    bool isBusy();

protected:
    void run();

signals:
    void quit();
    void busyChanged();
    void progress(int current, int total);
    void networkNotAccessible();
    void uploading();
    void checkingCredentials();
    void addDownload(DatabaseManager::CacheItem item);

    /*
    200 - Fether is busy
    400 - Email/password is not defined
    401 - SignIn failed
    402 - SignIn user/password do no match
    500 - Network error
    501 - SignIn resposne is null
    502 - Internal error
    600 - Error while parsing XML
    601 - Unknown XML response
     */
    void credentialsValid();
    void errorCheckingCredentials(int code);
    void error(int code);
    void canceled();
    void ready();

public slots:
    void finishedSignIn();
    void finishedSignInOnlyCheck();
    void finishedDashboards();
    void finishedDashboards2();
    void finishedTabs();
    void finishedTabs2();
    void finishedFeeds();
    void finishedFeeds2();
    void finishedFeedsUpdate();
    void finishedFeedsUpdate2();
    void finishedFeedsReadlater();
    void finishedFeedsReadlater2();
    void finishedSet();

    void readyRead();
    void networkError(QNetworkReply::NetworkError);
    void networkAccessibleChanged (QNetworkAccessManager::NetworkAccessibility accessible);
    bool delayedUpdate(bool state);

private:

    enum Job { StoreDashboards, StoreTabs, StoreFeeds,
               StoreFeedsInfo, StoreFeedsUpdate, StoreFeedsReadlater
             };

    static const int feedsAtOnce = 5;
    static const int limitFeeds = 50;
    static const int limitFeedsUpdate = 50;
    static const int limitFeedsReadlater = 50;
    static const int feedsUpdateAtOnce = 10;

    QNetworkAccessManager _manager;
    QNetworkReply* _currentReply;
    QByteArray _data;

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QJsonObject _jsonObj;
#else
    QVariantMap _jsonObj;
#endif

    bool _busy;
    BusyType _busyType;
    QList<QString> _dashboardList;
    QList<QString> _tabList;
    QList<DatabaseManager::StreamModuleTab> _streamList;
    QList<DatabaseManager::StreamModuleTab> _streamUpdateList;
    QList<DatabaseManager::Action> actionsList;
    int _total;
    QByteArray _cookie;
    QNetworkConfigurationManager ncm;
    Job currentJob;
    int publishedBeforeDate;

    bool parse();
    QString hash(const QString &url);
    void storeTabs();
    int storeFeeds();
    void storeDashboards();
    void signIn();
    void fetchDashboards();
    void fetchTabs();
    void fetchFeeds();
    void fetchFeedsUpdate();
    void fetchFeedsReadlater();
    void uploadActions();
    void set();
    void cleanNewFeeds();
    void cleanRemovedFeeds();
    void taskEnd();
    void downloadFeeds();
    void setBusy(bool busy, BusyType type = Unknown);
    void startJob(Job job);
    bool checkError();
    void removeAction();
};

#endif // NETVIBESFETCHER_H
