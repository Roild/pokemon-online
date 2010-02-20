#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QtCore>
#include <QTextEdit>

#include <QtScript>


class Server;
class ChallengeInfo;

class ScriptEngine : public QObject
{
    Q_OBJECT
public:
    ScriptEngine(Server *s);

    /* Events */
    bool beforeChatMessage(int src, const QString &message);
    void afterChatMessage(int src, const QString &message);
    void serverStartUp();
    void beforeLogOut(int src);
    void afterLogOut(int src);
    bool beforeLogIn(int src);
    void afterLogIn(int src);
    bool beforeChallengeIssued(int src, int dest, const ChallengeInfo &desc);
    void afterChallengeIssued(int src, int dest, const ChallengeInfo &desc);
    void beforeBattleStarted(int src, int dest, const ChallengeInfo &desc);
    void afterBattleStarted(int winner, int loser, const ChallengeInfo &desc);
    void beforeBattleEnded(int winner, int loser, int desc);
    void afterBattleEnded(int winner, int loser, int desc);

    /* Functions called in scripts */
    Q_INVOKABLE void sendAll(const QString &mess);
    Q_INVOKABLE void kick(int id);
    /* Prevents the event from happening.
       For exemple, if called in 'beforeChatMessage', the message won't appear.
       If called in 'beforeChallengeIssued', the challenge won't be issued.
       */
    Q_INVOKABLE void stopEvent();
    Q_INVOKABLE void sendMessage(int id, const QString &mess);
    /* Print on the server. Useful for debug purposes */
    Q_INVOKABLE void print(QScriptContext *context, QScriptEngine *engine);
    Q_INVOKABLE void changeAuth(int id, int auth);
    /* Save vals using the QSettings (persistent vals, that stay after the shutdown of the server */
    Q_INVOKABLE void saveVal(const QString &key, const QVariant &val);
    Q_INVOKABLE void removeVal(const QString &key);
    /* Calls the underlying OS for a command */
    Q_INVOKABLE void system(const QString &command);
    Q_INVOKABLE void clearChat();
    /* Evaluates the script given in parameter */
    Q_INVOKABLE QScriptValue eval(const QString &script);

    Q_INVOKABLE QScriptValue name(int id);
    Q_INVOKABLE QScriptValue id(const QString& name);
    Q_INVOKABLE QScriptValue auth(int id);
    Q_INVOKABLE QScriptValue ip(int id); 
    Q_INVOKABLE QScriptValue getVal(const QString &key);
    Q_INVOKABLE int numPlayers();
    Q_INVOKABLE bool loggedIn(int id);

    /* Set and unset player arrays:
       Those are arrays that, once a player logs off, have their content cleared
       for that offset. Useful as "session variables". */
    Q_INVOKABLE void setPA(const QString &name);
    Q_INVOKABLE void unsetPA(const QString &name);

    static QScriptValue nativePrint(QScriptContext *context, QScriptEngine *engine);
private slots:
    void changeScript(const QString &script);
private:
    Server *myserver;
    QScriptEngine myengine;
    QScriptValue myscript;
    bool stopevent;
    QSet<QString> playerArrays;

    void evaluate(const QScriptValue &expr);
    void printLine(const QString &s);
};

class ScriptWindow : public QWidget
{
    Q_OBJECT
public:
    ScriptWindow();
signals:
    void scriptChanged(const QString &script);
public slots:
    void okPressed();
private:
    QTextEdit *myedit;
};

#endif // SCRIPTENGINE_H
