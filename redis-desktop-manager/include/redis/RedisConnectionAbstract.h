#pragma once

#include "RedisConnectionConfig.h"
#include "RedisException.h"
#include "Response.h"
#include <QtCore>
#include <QMap>
#include <QList>

class TestRedisConnection;

class RedisConnectionAbstract : public QObject
{
	Q_OBJECT

	friend class TestRedisConnection;
public:

	RedisConnectionConfig config;	

	RedisConnectionAbstract(const RedisConnectionConfig & c); 		
	virtual ~RedisConnectionAbstract() {};

	/** Interface for all connection classes **/

	virtual bool connect() = 0;
	virtual QString getLastError() = 0;
	virtual QVariant execute(QString) = 0;
	virtual bool isConnected();
	virtual void runCommand(const QString &cmd) = 0;
	virtual Response getLastResponse();	
	virtual void sendResponse();

	/** 
	 * Get list of databases with keys counters 
	 * @emit databesesLoaded
	 **/
	typedef QMap <QString, int> RedisDatabases;
	void getDatabases();

	/** 
	 * Select db 
	 * see  http://redis.io/commands/select 
	 */
	void selectDb(int dbIndex);

	/** 
	 * Get keys list from db
	 * see http://redis.io/commands/keys
	 *  @emit keysLoaded
	 **/	
	void getKeys(QString pattern = "*");	

signals:
	void responseResived(Response &);
	void databesesLoaded(RedisConnectionAbstract::RedisDatabases);
	void keysLoaded(QStringList&);

protected:
	bool connected;
	QTimer executionTimer;
	Response resp;
	bool commandRunning;
	bool keysLoadingRunning;
	QString runningCommand;

protected slots:
	void executionTimeout();
};

