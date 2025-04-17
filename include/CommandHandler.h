#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <utility>

class CommandHandler {
public:
    CommandHandler() = default;
    CommandHandler(QString op, QString user, QString room)
        : m_operation(std::move(op)),
          m_userName(std::move(user)),
          m_roomName(std::move(room)) {}

    QByteArray ToJson() const {
        return QJsonDocument({
            {"operation", m_operation},
            {"userName",  m_userName},
            {"roomName",  m_roomName}
        }).toJson();
    }

    bool FromJson(const QJsonObject& obj) {
        if(!obj.contains("operation") ||
           !obj.contains("userName") ||
           !obj.contains("roomName")) return false;

        m_operation = obj["operation"].toString();
        m_userName  = obj["userName"].toString();
        m_roomName  = obj["roomName"].toString();
        return true;
    }

    static bool TryParse(const QString& message, CommandHandler& cmd) {
        const auto json = ExtractJsonObject(message);
        return !json.isEmpty() && cmd.FromJson(json);
    }

    static QString FormatJoinMessage(const QString& user) {
        return QStringLiteral("[%1]<%2> Join to room")
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"), user);
    }

    QString operation() const { return m_operation; }
    QString userName()  const { return m_userName; }
    QString roomName()  const { return m_roomName; }

private:
    QString m_operation;
    QString m_userName;
    QString m_roomName;

    static QJsonObject ExtractJsonObject(const QString& msg) {
        const int start = msg.indexOf('{');
        const int end = msg.lastIndexOf('}');

        if(start < 0 || end <= start) return {};

        const auto doc = QJsonDocument::fromJson(
            msg.mid(start, end - start + 1).toUtf8()
        );

        return doc.isObject() ? doc.object() : QJsonObject();
    }
};

#endif // COMMANDHANDLER_H